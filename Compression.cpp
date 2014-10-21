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
#include "Compression.h"
#include <zlib.h>

namespace Zlib
{
    bool Compress(const Buffer& bufferUncomp, Buffer* bufferComp)
    {
        while (true)
        {
            uLongf size = bufferComp->GetSize();
            const int result = compress(
                                   static_cast<Bytef*>(bufferComp->Get()),
                                   &size,
                                   static_cast<const Bytef*>(bufferUncomp.Get()),
                                   bufferUncomp.GetSize()
                               );

            if (result == Z_OK)
            {
                // trim buffer to data size
                bufferComp->RemoveFromBack(NULL, bufferComp->GetSize() - size);
                return true;
            }

            if (result == Z_BUF_ERROR)
            {
                bufferComp->Allocate(size * 2);
            }
            else
            {
                return false;
            }
        }
    }

    bool Decompress(const Buffer& bufferComp, Buffer* bufferUncomp)
    {
        while (true)
        {
            uLongf size = bufferUncomp->GetSize();
            const int result = uncompress(
                                   static_cast<Bytef*>(bufferUncomp->Get()),
                                   &size,
                                   static_cast<const Bytef*>(bufferComp.Get()),
                                   bufferComp.GetSize()
                               );

            if (result == Z_OK)
            {
                // trim buffer to data size
                bufferUncomp->RemoveFromBack(NULL, bufferUncomp->GetSize() - size);
                return true;
            }

            if (result == Z_BUF_ERROR)
            {
                bufferUncomp->Allocate(size * 2);
            }
            else
            {
                return false;
            }
        }
    }
}
