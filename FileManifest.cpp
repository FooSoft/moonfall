/*
    Moonfall  Copyright (C) 2008  Alex Yatskov

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Pch.h"
#include "FileManifest.h"

bool FileManifest::LoadManifest(const char* filename)
{
    boost::shared_ptr<const Buffer> buffer = LoadFileExternal(filename);
    if (!buffer)
    {
        TRACE_ERROR(boost::format("Unable to load manifest %s") % filename);
        return false;
    }

    Deserializer data(buffer.get());

    PackageHeader packageHeader;
    if (!packageHeader.Deserialize(&data) || packageHeader.token != PACKAGE_TOKEN || packageHeader.version != PACKAGE_VERSION)
    {
        TRACE_ERROR(boost::format("Invalid package header data (token = %x, version = %d) in manifest %s") % packageHeader.token % packageHeader.version % filename);
        return false;
    }

    for (int i = 0; i < packageHeader.count; ++i)
    {
        PackageFileHeader packageFileHeader;
        if (packageFileHeader.Deserialize(&data))
        {
            if (m_manifest.find(packageFileHeader.filename) == m_manifest.end())
            {
                m_manifest.insert(std::make_pair(packageFileHeader.filename, PackageFileEntry(packageFileHeader, filename)));
            }
            else
            {
                TRACE_WARNING(boost::format("Duplicate file %s in manifest %s") % packageFileHeader.filename % filename);
            }

            if (!data.SetOffset(packageFileHeader.sizeCompressed, true))
            {
                TRACE_ERROR(boost::format("Inconsistant file header data for %s in manifest %s") % packageFileHeader.filename % filename);
                return false;
            }
        }
    }

    return true;
}

bool FileManifest::SaveManifest(const char* filename, const std::vector<std::string>& filenames)
{
    Buffer buffer;
    Serializer data(&buffer);

    PackageHeader packageHeader;
    packageHeader.token = PACKAGE_TOKEN;
    packageHeader.version = PACKAGE_VERSION;
    packageHeader.flags = 0;
    packageHeader.count = filenames.size();
    packageHeader.Serialize(&data);

    for (std::vector<std::string>::const_iterator iter = filenames.begin(); iter != filenames.end(); ++iter)
    {
        FILE* fp = fopen(iter->c_str(), "rb");
        if (fp == NULL)
        {
            TRACE_ERROR(boost::format("Cannot package file %s into manifest %s") % iter->c_str() % filename);
            return false;
        }

        fseek(fp, 0, SEEK_END);
        const size_t size = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        Buffer contentsUncompressed(size);
        Buffer contentsCompressed(size);
        fread(contentsUncompressed.Get(), 1, contentsUncompressed.GetSize(), fp);
        fclose(fp);

        if (!Zlib::Compress(contentsUncompressed, &contentsCompressed))
        {
            TRACE_ERROR(boost::format("Cannot compress file %s into manifest %s") % iter->c_str() % filename);
            return false;
        }

        PackageFileHeader packageFileHeader;
        packageFileHeader.filename = *iter;
        packageFileHeader.sizeUncompressed = contentsUncompressed.GetSize();
        packageFileHeader.sizeCompressed = contentsCompressed.GetSize();
        packageFileHeader.offset =
            data.GetOffset() +
            packageFileHeader.filename.size() + 1 +
            sizeof(packageFileHeader.offset) +
            sizeof(packageFileHeader.sizeCompressed) +
            sizeof(packageFileHeader.sizeUncompressed);
        packageFileHeader.Serialize(&data);

        data.Write(contentsCompressed.Get(), contentsCompressed.GetSize());
    }

    FILE* fp = fopen(filename, "wb");
    if (fp == NULL)
    {
        TRACE_ERROR(boost::format("Cannot save manifest file %s") % filename);
        return false;
    }

    fwrite(buffer.Get(), 1, buffer.GetSize(), fp);
    fclose(fp);
    return true;
}

void FileManifest::ClearManifest()
{
    m_manifest.clear();
}

bool FileManifest::SaveFile(const char* filename, const Buffer& buffer)
{
    FILE* fp = fopen(filename, "wb");
    if (fp == NULL)
    {
        TRACE_ERROR(boost::format("Cannot save external file %s") % filename);
        return false;
    }

    fwrite(buffer.Get(), 1, buffer.GetSize(), fp);
    fclose(fp);

    FlushFile(filename);

    TRACE_INFO(boost::format("Saved external file %s") % filename);
    return true;
}

void FileManifest::FlushFile(const char* filename)
{
    m_manager.RemoveClient(filename);
}

void FileManifest::FlushFiles()
{
    m_manager.ClearClients();
    TRACE_INFO("Flushed cached files");
}

boost::shared_ptr<const Buffer> FileManifest::LoadFile(const char* filename)
{
    boost::shared_ptr<const Buffer> buffer;
    bool cached = false;

    if (buffer = LoadFileExternal(filename, &cached))
    {
        if (!cached)
        {
            TRACE_INFO(boost::format("Loaded external file %s") % filename);
        }
    }
    else if (buffer = LoadFileInternal(filename, &cached))
    {
        if (!cached)
        {
            TRACE_INFO(boost::format("Loaded internal file %s") % filename);
        }
    }
    else
    {
        TRACE_ERROR(boost::format("Cannot load file %s") % filename);
    }

    return buffer;
}

boost::shared_ptr<const Buffer> FileManifest::LoadFileInternal(const char* filename, bool* cached)
{
    boost::shared_ptr<const Buffer> bufferCached = m_manager.FindClient(filename);
    if (cached != NULL)
    {
        *cached = bufferCached;
    }
    if (bufferCached)
    {
        return bufferCached;
    }

    std::map<std::string, PackageFileEntry>::iterator iter = m_manifest.find(filename);
    if (iter == m_manifest.end())
    {
        return boost::shared_ptr<const Buffer>();
    }

    FILE* fp = fopen(iter->second.filename.c_str(), "rb");
    if (fp == NULL)
    {
        return boost::shared_ptr<const Buffer>();
    }

    const int sizeCompressed = iter->second.header.sizeCompressed;
    const int sizeUncompressed = iter->second.header.sizeUncompressed;
    const int offset = iter->second.header.offset;

    Buffer bufferCompressed(sizeCompressed);
    boost::shared_ptr<Buffer> bufferUncompressed = boost::shared_ptr<Buffer>(new Buffer(sizeUncompressed));

    fseek(fp, offset, SEEK_SET);
    fread(bufferCompressed.Get(), 1, bufferCompressed.GetSize(), fp);
    fclose(fp);

    if (!Zlib::Decompress(bufferCompressed, bufferUncompressed.get()))
    {
        TRACE_ERROR(boost::format("Cannot inflate file %s") % filename);
        return boost::shared_ptr<const Buffer>();
    }

    m_manager.AddClient(filename, bufferUncompressed);
    return bufferUncompressed;
}

boost::shared_ptr<const Buffer> FileManifest::LoadFileExternal(const char* filename, bool* cached)
{
    ADJUST_FILENAME(filename);

    boost::shared_ptr<const Buffer> buffer = m_manager.FindClient(filename);
    if (cached != NULL)
    {
        *cached = buffer;
    }
    if (buffer)
    {
        return buffer;
    }

    FILE* fp = fopen(filename, "rb");
    if (fp != NULL)
    {
        fseek(fp, 0, SEEK_END);
        size_t size = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        boost::shared_ptr<Buffer> temp(new Buffer(size));
        fread(temp->Get(), 1, temp->GetSize(), fp);
        fclose(fp);

        buffer = temp;
        m_manager.AddClient(filename, buffer);
    }

    return buffer;
}
