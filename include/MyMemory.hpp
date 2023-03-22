/* date = November 3rd 2022 1:31 pm */

#ifndef MY_MEMORY_H
#define MY_MEMORY_H

#define _AMD64_
#define WIN32_LEAN_AND_MEAN
#include "engine_math.hpp"
#include <memoryapi.h>
#include <windows.h>

typedef struct
{
    u32 size;
    u32 capacity;
    const char *data;
} MyString;

typedef struct ArenaAllocator
{
    void *data;
    size_t virtualSize;
    size_t usedSize;
    size_t totalCapacity;
} Memory;

typedef struct
{
    HANDLE handle;
    MyString path;
    union {
        MyString file;
        struct
        {
            u32 size;
            u32 capacity;
            s8 *data;
        };
    };
} fileResource;

/*
    Initizlize an arena allocator with the maximum possible memory size
    it's going to use. This function is usually preceded by calls to MyMalloc();
*/

Memory InitVirtualMemory(size_t size);
/*
    Takes an arena allocator __memory__, and a __size__, and returns
    a pointer with requested memory of the size __size__
*/

void *MyMalloc(Memory *memory, size_t size);
/*
    Takes an arena allocator and increases the requested physical memory
    by AmountIncrease.
*/
void IncreaseTotalCapacity(Memory *memory, size_t AmountIncrease);

/*
    Takes a filepath and an arena allocator. It returns a MyString
    with the contents of the file read. TODO Handle error cases
*/
MyString OpenAndReadEntireFile(const char *filePath, Memory *memory);
void MyFree(Memory *mem, size_t size);
void DeInitMemory(Memory *memory);

fileResource FileResourceInit(const char *fileName, Memory *m);
fileResource *LoadFile(fileResource *result, Memory *memory);
MyString initStringChar(const char *str, u64 n, Memory *memory);
int StringsAreEqual_C(MyString a, const char *str, const char *delimiter);
int StringsAreEqual(MyString a, MyString b);
char *my_strncpy_s(char *dest, u64 dest_size, const char *src, u64 src_size);
u32 my_strnlen(char *src, u64 n);

#define KiloByte 1024LL
#define MegaByte 1024LL * KiloByte
#define GigaByte 1024LL * MegaByte
#define TeraByte 1024LL * GigaByte

#endif // MY_MEMORY_H

#ifdef MEMORY_IMPLEMENTATION

static void IncreaseTotalCapacity(Memory *memory, size_t AmountIncrease)
{
#ifdef MEMORY_DEBUG_BUILD
    if (memory->totalCapacity + AmountIncrease > memory->virtualSize)
        __debugbreak();
#endif
    VirtualAlloc((void *)((u8 *)memory->data + memory->usedSize),
        AmountIncrease, MEM_COMMIT, PAGE_READWRITE);
    memory->totalCapacity += AmountIncrease;
}

static int StringsAreEqual(MyString a, MyString b)
{
    if (a.size == b.size)
    {
        int i = 0;
        while (a.data[i] == b.data[i] && i < a.size)
            i++;
        if (i == a.size)
            return (1);
    }
    return (0);
}

static int StringsAreEqual_C(MyString a, 
                             const char *str, const char *delimiter)
{
    int i = 0;

    while (a.data[i] == str[i] && i < a.size)
        i++;
    if (i == a.size)
    {
        if (delimiter)
        {
            int j = 0;
            if (!delimiter[j])
            {
                if (str[i] == '\0')
                    return (1);
            }
            while (delimiter[j])
            {
                if (str[i] == '\0' || delimiter[j] == str[i])
                    return (1);
                j++;
            }
            return (0);
        }
        return (1);
    }
    return (0);
}

static Memory InitVirtualMemory(size_t size)
{
    Memory memory = {0};

    memory.virtualSize = size;
    memory.data = VirtualAlloc(NULL, size, MEM_RESERVE, PAGE_READWRITE);
    memory.usedSize = 0;
    memory.totalCapacity = 0;
#ifdef MEMORY_DEBUG_BUILD
    if (memory.data == NULL)
        __debugbreak();
#endif
    return (memory);
}

static void *MyMalloc(Memory *memory, size_t size)
{
#ifdef MEMORY_DEBUG_BUILD
    if (memory->usedSize + size >= memory->totalCapacity)
        __debugbreak();
#else
    if (memory->usedSize + size >= memory->totalCapacity)
        IncreaseTotalCapacity(memory, memory->totalCapacity);
#endif
    VirtualAlloc((void *)((size_t)memory->data + memory->usedSize), size,
        MEM_COMMIT, PAGE_READWRITE);
    size_t memoryAlgebra = (size_t)memory->data + memory->usedSize;
    memory->usedSize += size + (64 - size % 64);
    return ((void *)memoryAlgebra);
}

static void MyFree(Memory *m, size_t size)
{
    size_t memoryAlgebra = (size_t)m->data + m->usedSize - size;
    memset((void *)memoryAlgebra, 0, size);
    m->usedSize -= size;
}

static void DeInitMemory(Memory *memory)
{
    VirtualFree(memory->data, 0, MEM_RELEASE);
    Memory result = {NULL, 0, 0, 0};
    *memory = result;
}

static MyString initString(u64 n, Memory *memory)
{
    MyString result = {0};
    result.size = 0;
    result.capacity = n;
    result.data = (const char *)MyMalloc(memory, n);
    return (result);
}

static MyString initStringChar(const char *str, u64 n, Memory *memory)
{
    MyString result = {0};

    int i = 0;
    while (str[i] && i < n)
        i++;
    result.size = i;
    result.capacity = n;
    result.data = (const char *)MyMalloc(memory, n);

    i = 0;
    char *temp = (char *)result.data;

    while (i < result.size)
        temp[i] = str[i++];
    return (result);
}

static MyString OpenAndReadEntireFile(const char *filePath, Memory *memory)
{
    MyString result = {0};

    HANDLE File = CreateFileA(filePath, GENERIC_READ,
        FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL, NULL);

    if (File)
    {
        u64 fileSize;
        GetFileSizeEx(File, (LARGE_INTEGER *)&fileSize);
        result = initString(fileSize, memory);
        ReadFile(File, (void *)result.data, fileSize,
            (DWORD *)&result.size, NULL);
        if (fileSize != result.size)
            __debugbreak();
        char *temp = (char *)result.data;
        temp[result.size] = '\0';
        CloseHandle(File);
    }
    return (result);
}

#if 1

#if 0
static
int strncmp(const char *s1, const char *s2, u64 n)
{
    u64 i = 0;
    while (i < n && s1[i] == s2[i] && s1[i])
        i++;
    return (s1[i] - s2[i]);
}
#endif

#pragma function(strcmp)
static int strcmp(const char *s1, const char *s2)
{
    u64 i = 0;
    while (s1[i] == s2[i] && s1[i])
        i++;
    return (s1[i] - s2[i]);
}

static u32 my_strlen(char *src)
{
    int i = 0;
    while (src[i])
        i++;
    return (i);
}

static u32 my_strnlen(char *src, u64 n)
{
    int i = 0;
    while (src[i] && i < n)
        i++;
    return (i);
}

static char *my_strncpy_s(char *dest, u64 dest_size, const char *src,
    u64 src_size)
{
    int i = 0;
    while (i < dest_size && i < src_size)
    {
        dest[i] = src[i];
        i++;
    }
    while (i < dest_size && i < src_size)
    {
        dest[i] = '\0';
        i++;
    }
    return (dest);
}

static char *my_strcpy_s(char *dest, u64 n, char *src)
{
    int i = 0;
    while (i < n && src[i])
    {
        dest[i] = src[i];
        i++;
    }
    memset(&dest[i], '\0', n - i);
    return (dest);
}
#endif

static fileResource FileResourceInit(const char *fileName, Memory *m)
{
    fileResource result;

    result.path = initStringChar(fileName, 255, m);
    memset(&result.file, 0, sizeof(MyString));
    result.handle = 0;
    return (result);
}

static fileResource *LoadFile(fileResource *result, Memory *memory)
{
    result->handle = CreateFileA((LPCSTR)result->path.data, // PATH
                                 GENERIC_READ,              // Desired access
                                 0,                         // Share Mode
                                 NULL,                      // Security Attributes
                                 OPEN_EXISTING,             // Creation Disposition
                                 FILE_ATTRIBUTE_NORMAL,     // Flags and attributes
                                 NULL);                     // Template file

    if (result->handle != INVALID_HANDLE_VALUE)
    {
        result->size = GetFileSize(result->handle, NULL);
        result->data = (s8 *)MyMalloc(memory, result->size);

        ReadFile(result->handle, // file handle
                 result->data,   // buffer
                 result->size,   // number of bytes to read
                 NULL,           // number of bytes read
                 NULL);          // lpoverlapped
        CloseHandle(result->handle);
        result->handle = 0;
    }
    return (result);
}

#undef MEMORY_IMPLEMENTATION
#endif // MEMORY_IMPLEMENTATION
