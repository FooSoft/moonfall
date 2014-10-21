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

#pragma once

class FileManifest
{
public:
    static bool                             SaveManifest(const char* filename, const std::vector<std::string>& filenames);
    bool                                    LoadManifest(const char* filename);
    void                                    ClearManifest();

    boost::shared_ptr<const Buffer>         LoadFile(const char* filename);
    bool                                    SaveFile(const char* filename, const Buffer& buffer);

    void                                    FlushFile(const char* filename);
    void                                    FlushFiles();

private:
    boost::shared_ptr<const Buffer>         LoadFileInternal(const char* filename, bool* cached = NULL);
    boost::shared_ptr<const Buffer>         LoadFileExternal(const char* filename, bool* cached = NULL);

    enum
    {
        PACKAGE_TOKEN   = 0x55534544,
        PACKAGE_VERSION = 0x00000000
    };

    struct PackageHeader
    {
        PackageHeader() :
                token(0), version(0), flags(0), count(0) { }

        bool Deserialize(Deserializer* data)
        {
            return
                data->Read(&token) &&
                data->Read(&version) &&
                data->Read(&flags) &&
                data->Read(&count);
        }

        void Serialize(Serializer* data)
        {
            data->Write(token);
            data->Write(version);
            data->Write(flags);
            data->Write(count);
        }

        int token;
        int version;
        int flags;
        int count;
    };

    struct PackageFileHeader
    {
        PackageFileHeader() :
                offset(0), sizeCompressed(0), sizeUncompressed(0) { }

        bool Deserialize(Deserializer* data)
        {
            return
                data->ReadString(&filename) &&
                data->Read(&offset) &&
                data->Read(&sizeCompressed) &&
                data->Read(&sizeUncompressed);
        }

        void Serialize(Serializer* data)
        {
            data->WriteString(filename);
            data->Write(offset);
            data->Write(sizeCompressed);
            data->Write(sizeUncompressed);
        }

        std::string filename;
        int         offset;
        int         sizeCompressed;
        int         sizeUncompressed;
    };

    struct PackageFileEntry
    {
        PackageFileEntry(const PackageFileHeader& header, const std::string& filename) :
                header(header), filename(filename) { }

        PackageFileHeader   header;
        std::string         filename;
    };

    std::map<std::string, PackageFileEntry> m_manifest;
    RefCount<const Buffer>                  m_manager;
};
