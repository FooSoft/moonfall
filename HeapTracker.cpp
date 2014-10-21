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
#include "HeapTracker.h"
#undef new

namespace
{
    struct BlockNode
    {
        Heap::Block block;
        BlockNode*  previous;
        BlockNode*  next;
    };

    BlockNode*   s_head = NULL;
    int          s_count = 0;
    bool         s_track = false;

    void AddBlock(const void* address, size_t size, const char* filename, int line)
    {
        BlockNode* node = static_cast<BlockNode*>(malloc(sizeof(BlockNode)));

        strcpy(node->block.filename, filename);
        node->block.address = address;
        node->block.size = size;
        node->block.line = line;
        node->previous = NULL;
        node->next = s_head;

        if (s_head != NULL)
        {
            s_head->previous = node;
        }

        s_head = node;
        ++s_count;
    }

    void RemoveBlock(const void* address)
    {
        for (BlockNode* node = s_head; node != NULL; node = node->next)
        {
            if (node->block.address != address)
            {
                continue;
            }

            if (node->previous != NULL)
            {
                node->previous->next = node->next;
            }

            if (node->next != NULL)
            {
                node->next->previous = node->previous;
            }

            if (node == s_head)
            {
                s_head = node->next;
            }

            free(node);
            --s_count;
            break;
        }

    }

    void ClearBlocks()
    {
        BlockNode* current = s_head;

        while (current != NULL)
        {
            BlockNode* next = current->next;
            free(current);
            current = next;
        }

        s_head = NULL;
        s_count = 0;
    }
}

void* operator new(size_t size, const char* filename, int line)
{
    void* ptr = malloc(size);
    if (s_track)
    {
        AddBlock(ptr, size, filename, line);
    }
    return ptr;
}

void* operator new[](size_t size, const char* filename, int line)
{
    void* ptr = malloc(size);
    if (s_track)
    {
        AddBlock(ptr, size, filename, line);
    }
    return ptr;
}

void operator delete(void* ptr, const char* filename, int line)
{
    if (s_track)
    {
        RemoveBlock(ptr);
    }
    free(ptr);
}

void operator delete[](void* ptr, const char* filename, int line)
{
    if (s_track)
    {
        RemoveBlock(ptr);
    }
    free(ptr);
}

namespace Heap
{
    bool HasAllocs()
    {
        return s_count > 0;
    }

    void DumpAllocs(std::vector<Block>* blocks)
    {
        for (BlockNode* node = s_head; node != NULL; node = node->next)
        {
            blocks->push_back(node->block);
        }
    }

    void TrackBegin()
    {
        TrackEnd();
        s_track = true;
    }

    void TrackEnd()
    {
        ClearBlocks();
        s_track = false;
    }
}
