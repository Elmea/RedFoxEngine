#include "ObjParser.hpp"

#define MATHS_HPP_IMPLEMENTATION
#include "engine_math.hpp"

#define MEMORY_IMPLEMENTATION
#include "MyMemory.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_STDIO
#include <STB_Image/stb_image.h>

/*
    Function that uses intrinsics to read 32 bytes by 32 bytes
    until it finds the newline character  - '\n' or the end of string ('\0')
    and advances the str pointer to that character.
*/
static char *get_next_line(MyString buffer, char *str)
{
    /* If we are on the newline character we try to find the next one. */
    if (*str == '\n' || *str == '\r')
        str = str + 1;
    /* We first load 32 bytes of newline characters into the Carriage
     * 256 bits register. */
    __m256i Carriage = _mm256_set1_epi8('\n'); 
    __m256i Zero = _mm256_set1_epi8(0);

    /* If the string is smaller than 32 bytes, we can't use the intrinsics,
        and load 32 bytes at a time. That's why we go byte by byte.       */
    while (buffer.size - (str - (char *)buffer.data) >= 32)
    {
        /* We then load 32 bytes from string into the Batch 256 bits register*/
        __m256i Batch = _mm256_loadu_si256((__m256i *)str); 

        /* We then check if there are any newline characters in the first
         * string by comparing 32 bytes at a time. The result */
        __m256i TestC = _mm256_cmpeq_epi8(Batch, Carriage);
        __m256i TestZ = _mm256_cmpeq_epi8(Batch, Zero);
        /* We check if either the '\n' character or '\0' character were found*/
        __m256i Test = _mm256_or_si256(TestC, TestZ); 

        /* We store the results of the check into a int,
            transforming the mask from 256 bits, into a 1bit mask */
        s32 Check = _mm256_movemask_epi8(Test);
        if (Check)
        {
            /* The _tzcnt_u32 func counts 
             * the numbers of zeros inside the parameter.
             * In our case it's going to count 
             * how many characters different than '\n' there are
             */

            s32 Advance = _tzcnt_u32(Check);
            str += Advance;
            if (*str == '\r')
                str++;
            return (str);
        }
        str += 32;
    }

    if (buffer.size - (str - (char *)buffer.data) < 32)
    {
        while (*str != '\n' && *str != '\0')
            str++;
    }
    if (*str == '\r')
        str++;
    return (str);
}

/*
    Function that uses intrinsics to read 16 bytes by 16 bytes
    until it finds a character different than space(' ') or tab('\t')
    and advances the str pointer to that character.

    It has the exact same logic as the func get_next_line above,
    but instead of looking for a character, it looks for the lack of a character.
    */

static char *get_next_word(char *str)
{
    __m256i Space = _mm256_set1_epi8(' ');
    __m256i Tab = _mm256_set1_epi8('\t');
    __m256i NewLine = _mm256_set1_epi8('\n');

    char first = 0;
    while (1)
    {
        __m256i Batch = _mm256_loadu_si256((__m256i *)str);
        __m256i TestS = _mm256_cmpeq_epi8(Batch, Space);
        __m256i TestT = _mm256_cmpeq_epi8(Batch, Tab);
        __m256i TestN = _mm256_cmpeq_epi8(Batch, NewLine);
        __m256i Test = _mm256_or_si256(TestS, TestT);
        Test = _mm256_or_si256(Test, TestN);

        u32 Check = _mm256_movemask_epi8(Test);
        if (first)
            Check = ~Check;
        if (Check)
        {
            s32 Advance = _tzcnt_u32(Check);
            str += Advance;
            if (first)
                break;
            first = 1;
        }
        if (!Check)
            str += 32;
    }
    return (str);
}

static s32 _digit_value(char c)
{
    s32 ri = c;
    s32 v = 16;

    if (c >= '0' && c <= '9')
        v = ri - '0';
    else if (c >= 'a' && c <= 'z')
        v = ri - 'a' + 10;
    else if (c >= 'A' && c <= 'Z')
        v = ri - 'A' + 10;
    return v;
}

static s32 parse_i32(char *str)
{
    const __m256i zeroCharacter = _mm256_set1_epi8('0');

    s32 value = 0;
    char *s = str;

    char neg = 0;
    switch (s[0])
    {
    case '-': {
        neg = 1;
        s++;
        break;
    }
    case '+':
        s++;
    }
    const s32 base = 10;
    s32 i = 0;

    __m256i Batch = _mm256_loadu_si256((__m256i *)s);
    __m256i digitValue = _mm256_sub_epi8(Batch, zeroCharacter);

    s = (char *)&digitValue;
    while (i < 9)
    {

        s32 v = s[i];
        if (v >= base || v < 0)
        {
            break;
        }
        value *= base;
        value += v;
        i++;
    }

    if (neg)
    {
        value = -value;
    }
    return value;
}

static f64 parse_f64(char *str, s32 size)
{
    f64 value = 0;
    char *s = str;

    s32 i = 0;

    f64 sign = 1;
    switch (s[i])
    {
    case '-': {
        i += 1;
        sign = -1;
    }
    break;
    case '+':
        i += 1;
        break;
    }

    for (; i < size; i += 1)
    {
        char c = s[i];

        s32 v = _digit_value(c);
        if (v >= 10)
        {
            break;
        }
        value *= 10;
        value += (f64)v;
    }

    if (i < size && s[i] == '.')
    {
        f64 pow10 = 10;
        i += 1;

        for (; i < size; i += 1)
        {
            char c = s[i];

            s32 v = _digit_value(c);
            if (v >= 10)
            {
                break;
            }
            value += (f64)v / pow10;
            pow10 *= 10;
        }
    }

    char frac = 0;
    f64 scale = 1;

    if (i < size && (s[i] == 'e' || s[i] == 'E'))
    {
        i += 1;

        if (i < size)
        {
            switch (s[i])
            {
            case '-': {
                i += 1;
                sign = -1;
                frac = 1;
            }
            break;
            case '+':
                i += 1;
                break;
            }

            u32 exp = 0;
            for (; i < size; i += 1)
            {
                u32 d = _digit_value(s[i]);
                if (d >= 10)
                {
                    break;
                }
                exp = exp * 10 + d;
            }
            if (exp > 308)
            {
                exp = 308;
            }

            while (exp >= 50)
            {
                scale *= 1e50;
                exp -= 50;
            }
            while (exp >= 8)
            {
                scale *= 1e8;
                exp -= 8;
            }
            while (exp > 0)
            {
                scale *= 10;
                exp -= 1;
            }
        }
    }
    if (frac)
    {
        value = sign * (value / scale);
    }
    else
    {
        value = sign * (value * scale);
    }
    return (value);
}

static vec3 getvec3(char *__restrict *str)
{
    vec3 result;

    result.x = (f32)parse_f64(*str, 32);
    *str = get_next_word(*str);
    result.y = (f32)parse_f64(*str, 32);
    *str = get_next_word(*str);
    result.z = (f32)parse_f64(*str, 32);
    return (result);
}

static vec2 getvec2(char *__restrict *str)
{
    vec2 result;

    result.x = (f32)parse_f64(*str, 32);
    *str = get_next_word(*str);
    result.y = (f32)parse_f64(*str, 32);
    return (result);
}

static void initImage(ObjImageData *result, Memory *memory)
{
    fileResource file = FileResourceInit((char *)result->path.data, memory);
    LoadFile(&file, memory);

    // We hash the image file, so we can check if we already uncompressed it before
    // and load the uncompressed version, bypassing the stbi load.
    meow_u128 Hash = MeowHash(MeowDefaultSeed, file.size, file.data);
    result->hash64 = MeowU64From(Hash, 0);
    _mkdir("Temp");
    char tempBuffer[128];
    snprintf(tempBuffer, 128, "Temp/%02zX.img", result->hash64);

    fileResource imageFile = FileResourceInit(tempBuffer, memory);
    LoadFile(&imageFile, memory);
    if (imageFile.handle != INVALID_HANDLE_VALUE)
    {
        ObjImageData temp;
        temp = *((ObjImageData *)imageFile.data);
        if (result->hash64 == temp.hash64)
        {
            result->width = temp.width;
            result->height = temp.height;
            result->nrChannels = temp.nrChannels;
            result->data = (unsigned char *)((size_t)imageFile.data + sizeof(ObjImageData));
        }
        else
        {
            __debugbreak();
            //			FatalError("Invalid hashed image file");
        }

        CloseHandle(imageFile.handle);
    }
    else
    {
        stbi_set_flip_vertically_on_load(1);
        result->data = stbi_load_from_memory((u8 *)file.data, file.size, &result->width, &result->height,
                                             &result->nrChannels, STBI_rgb_alpha);

        HANDLE File = CreateFileA(tempBuffer, GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, CREATE_NEW,
                                  FILE_ATTRIBUTE_NORMAL, NULL);
        DWORD bytesWritten = 0;
        WriteFile(File, result, sizeof(ObjImageData), &bytesWritten, NULL);
        WriteFile(File, result->data, result->width * result->height * STBI_rgb_alpha, &bytesWritten, NULL);
#ifdef DEBUG
        DWORD LastError = GetLastError();

        CloseHandle(File);

        LastError = GetLastError();
#endif
    }
}

static ObjMaterial initMaterial()
{
    ObjMaterial result = {};

    result.ambient = {{0.2f, 0.2f, 0.2f}};
    result.diffuse = {{0.8f, 0.8f, 0.8f}};
    result.specular = {{0}};
    result.emissive = {{0}};
    result.Shininess = 25.f;
    result.Opaqueness = 1;
    return (result);
}

typedef struct ParameterImageThread
{
    ObjImageData *result;
    u64 count;
    Memory *memory;
} ParameterImageThread;

static void initImageThread(void *parameter)
{
    ParameterImageThread *p = (ParameterImageThread *)parameter;
    for (int i = 0; i < (int)p->count; i++)
        initImage(&p->result[i], p->memory);
}

/*
    Takes the path to .obj file, the file contents inside a MyString struct, a pointer to the images array,
    and the arena allocators for image, and meshes, also a temporary that can get discarded imediately.

    The separate arenas are here to allow multithreaded loading, and freeing of memory at
    different steps after the function LoadModel returns.

    The meshes have a material index that coresponds to a material inside materials array
    The materials have an index to the image inside the images array

    Return - the function returns the array of materials
*/
ObjMaterials ParseMTL(const char *objPath, MyString objBuffer, ObjImages *ImagesOut,
    Memory *imageMem, Memory *meshMem, Memory *tempMem)
{
    ObjMaterials result = {};
    char mtlLibFilePath[255] = {};

    char *mtlLib = nullptr;
    int pathLen = 0;
    for (pathLen = 0; objPath[pathLen]; pathLen++)
        ;
    for (int i = 0; i + 6 < (int)objBuffer.size; i++)
    {
        if (StringsAreEqual({6, 6, &objBuffer.data[i]}, {6, 6, "mtllib"}))
        {
            mtlLib = (char *)&objBuffer.data[i + 7];
            break;
        }
    }
    if (mtlLib)
        my_strncpy_s(mtlLibFilePath, 255, objPath, pathLen);

    int i = 0;
    while (mtlLibFilePath[i] != '\0')
        i++;
    while ((!(mtlLibFilePath[i] == '/' || mtlLibFilePath[i] == '\\')) && i >= 0)
        i--;
    i++;
    s64 j = 0;
    if (mtlLib != NULL)
    {
        while (mtlLib[j] != '\n' && mtlLib[j] != '\t' && mtlLib[j] != '\r')
        {
            mtlLibFilePath[i] = mtlLib[j++];
            i++;
        }
        mtlLibFilePath[i] = '\0';
    }
    if (mtlLibFilePath[0] == '\0')
    {
        return result;
    }
    fileResource mtlLibData = FileResourceInit(mtlLibFilePath, tempMem);
    LoadFile(&mtlLibData, tempMem);

    if (mtlLibData.handle)
        CloseHandle(mtlLibData.handle);

    char *current = (char *)mtlLibData.data;
    char *endOfLine = (char *)mtlLibData.data;

    i = -1;

    while (*endOfLine != '\0')
    {
        current = endOfLine;
        while (*current == '#')
            current = get_next_line(mtlLibData.file, current) + 1;
        endOfLine = get_next_word(current);
        if (StringsAreEqual_C({7, 7, "newmtl "}, current, NULL))
            result.count++;
    }
    result.material = (ObjMaterial *)MyMalloc(meshMem, sizeof(ObjMaterial) * result.count);
    ImagesOut->data = (ObjImageData *)MyMalloc(imageMem, sizeof(ObjImageData) * result.count);
    ParameterImageThread *para = (ParameterImageThread *)MyMalloc(imageMem, sizeof(ParameterImageThread));

    current = (char *)mtlLibData.data;
    endOfLine = (char *)mtlLibData.data;

    while (*endOfLine != '\0')
    {
        current = endOfLine;
        while (*current == '#')
            current = get_next_line(mtlLibData.file, current) + 1;
        endOfLine = get_next_word(current);
        if (StringsAreEqual_C({7, 7, "newmtl "}, current, NULL))
        {
            i++;
            result.material[i] = initMaterial();

            int j = 0;

            while (current[j + 7] != '\n' && current[j + 7] != '\r' && current[j + 7] != '\0')
                j++;
            result.material[i].name = initStringChar(&current[7], j, meshMem);
            endOfLine = get_next_line(mtlLibData.file, current) + 1;
        }
        if (StringsAreEqual_C({7, 7, "map_Kd "}, current, NULL))
        {
            result.material[i].hasTexture = 1;
            int j = 0;

            while (current[j + 7] != '\n' && current[j + 7] != '\r' && current[j + 7] != '\0')
                j++;
            ImagesOut->data[ImagesOut->count].path = initStringChar(&current[7], j, meshMem);
            u8 found = 0;
            for (int k = 0; k < (int)ImagesOut->count; k++)
            {
                if (k != i && StringsAreEqual(ImagesOut->data[ImagesOut->count].path, ImagesOut->data[k].path))
                {
                    result.material[i].diffuseMap = result.material[k].diffuseMap;
                    found = 1;
                    break;
                }
            }
            if (!found)
            {
                result.material[i].diffuseMap.index0 = ImagesOut->count;
                ImagesOut->count++;
            }
            endOfLine = get_next_line(mtlLibData.file, current) + 1;
        }
        else if (StringsAreEqual_C({3, 3, "Kd "}, current, NULL))
        {
            current = get_next_word(current);
            result.material[i].diffuse = getvec3(&current);
            endOfLine = get_next_line(mtlLibData.file, current) + 1;
        }
        else if (StringsAreEqual_C({3, 3, "Ks "}, current, NULL))
        {
            current = get_next_word(current);
            result.material[i].specular = getvec3(&current);
            endOfLine = get_next_line(mtlLibData.file, current) + 1;
        }
        else if (StringsAreEqual_C({3, 3, "Ke "}, current, NULL))
        {
            current = get_next_word(current);
            result.material[i].emissive = getvec3(&current);
            endOfLine = get_next_line(mtlLibData.file, current) + 1;
        }
        else if (StringsAreEqual_C({2, 2, "d "}, current, NULL))
        {
            current = get_next_word(current);
            result.material[i].Opaqueness = (f32)parse_f64(current, 32);
            endOfLine = get_next_line(mtlLibData.file, current) + 1;
        }
    }
    para[0] = {ImagesOut->data, ImagesOut->count, imageMem};
    ImagesOut->thread =
        CreateThread(NULL,                                    //[in, optional]  LPSECURITY_ATTRIBUTES   lpThreadAttributes,
                     sizeof(ParameterImageThread),            //[in]            SIZE_T                  dwStackSize,
                     (LPTHREAD_START_ROUTINE)initImageThread, //[in]            LPTHREAD_START_ROUTINE  lpStartAddress,
                     &para[0],                                //[in, optional]  __drv_aliasesMem LPVOID lpParameter,
                     0,                                       //[in]            DWORD                   dwCreationFlags,
                     NULL);                                   //[out, optional] LPDWORD                 lpThreadId

    // for (int i = 0; i < (int)ImagesOut->count;i++)
    //     initImage(&ImagesOut->data[i], imageMem);
    return result;
}

static void Countvector2(char *__restrict current, s64 *__restrict Count, vec2 **__restrict v, Memory *__restrict temp,
                  u32 *__restrict size)
{
    if (*Count >= *size)
    {
        MyMalloc(temp, sizeof(vec2) * *size);
        *size = *size * 2;
    }
    current = get_next_word(current);
    (*v)[*Count] = getvec2(&current);
    *Count = *Count + 1;
}

static void Countvector3(char *__restrict current, s64 *__restrict Count, vec3 **__restrict v, Memory *__restrict temp,
                  u32 *__restrict size)
{
    if (*Count >= *size)
    {
        MyMalloc(temp, sizeof(vec3) * *size);
        *size = *size * 2;
    }
    current = get_next_word(current);
    (*v)[*Count] = getvec3(&current);
    *Count = *Count + 1;
}

typedef struct VertexKey
{
    ObjVertexIndex key;
    u32 index;
    VertexKey *next;
} VertexKey;

/*
    Takes the model it will output, and the path to the obj file

    Returns a value less than 0 if an error occurs
                              0 if it succeds (not all error states are handled)
*/
int ParseModel(ObjModel *result, const char *path)
{
    if (result == nullptr)
        return (-3);
    *result = {};
    ObjParsingObject tempParser = {};

    result->meshMem = InitVirtualMemory(1ULL * GigaByte);
    result->imageMem = InitVirtualMemory(1ULL * GigaByte);
    Memory temporary = InitVirtualMemory(1ULL * GigaByte);

    fileResource objFile = FileResourceInit(path, &temporary);
    LoadFile(&objFile, &temporary);

    if (objFile.handle == INVALID_HANDLE_VALUE)
        return -1;

    if (objFile.data == NULL)
        return -2;

    result->materials = ParseMTL(path, objFile.file, &result->images, &result->imageMem, &result->meshMem, &temporary);

    u64 start = __rdtsc();
    u64 end;
    LARGE_INTEGER StartingTime, EndingTime, ElapsedMicroseconds;
    LARGE_INTEGER Frequency;

    QueryPerformanceFrequency(&Frequency);
    QueryPerformanceCounter(&StartingTime);

    char OutputStringDebug[254];
    Memory tempPosition = InitVirtualMemory(1LL * GigaByte);
    Memory tempNormal = InitVirtualMemory(1LL * GigaByte);
    Memory tempTexture = InitVirtualMemory(1LL * GigaByte);
    Memory tempVertexIndexMem = InitVirtualMemory(1LL * GigaByte);
    Memory tempVertexHashMem = InitVirtualMemory(1LL * GigaByte);

    result->vertexMem = InitVirtualMemory(1ULL * GigaByte);
    result->indexMem = InitVirtualMemory(1ULL * GigaByte);

    u32 vertexSize = 1024 * 4;
    u32 posSize = 1024 * 4;
    u32 normalSize = 1024 * 4;
    u32 uvSize = 1024 * 4;
    u32 vertexIndicesSize = 1024 * 4;

    tempParser.position = (vec3 *)MyMalloc(&tempPosition, sizeof(vec3) * posSize);
    tempParser.normal = (vec3 *)MyMalloc(&tempNormal, sizeof(vec3) * normalSize);
    tempParser.textureUV = (vec2 *)MyMalloc(&tempTexture, sizeof(vec2) * uvSize);
    tempParser.vertexIndices =
        (ObjVertexIndex *)MyMalloc(&tempVertexIndexMem, sizeof(ObjVertexIndex) * vertexIndicesSize);

    result->meshes = (ObjMesh *)MyMalloc(&result->meshMem, sizeof(ObjMesh));

    __m256i MaterialChar = _mm256_set1_epi8('m');
    __m256i VertexIndexBegin = _mm256_set1_epi8('f');
    __m256i VertexBegin = _mm256_set1_epi8('v');

    char *current = (char *)objFile.data;
    char *endOfLine = (char *)objFile.data;

    while (objFile.size - (current - (char *)objFile.data) >= 32)
    {
        current = endOfLine;

        while (*current == '#')
            current = get_next_line(objFile.file, current) + 1;

        __m256i Batch = _mm256_loadu_si256((__m256i *)current);
        __m256i TestM = _mm256_cmpeq_epi8(Batch, MaterialChar);
        __m256i TestF = _mm256_cmpeq_epi8(Batch, VertexIndexBegin);
        __m256i TestV = _mm256_cmpeq_epi8(Batch, VertexBegin);

        int Check = 0;
        if ((Check = _mm256_movemask_epi8(TestV)))
        {
            char *tempStr = current;
            tempStr += _tzcnt_u32(Check) + 1;
            char parsed = 0;
            switch (*tempStr)
            {
            case ' ':
                Countvector3(tempStr, &tempParser.PosCount, &tempParser.position, &tempPosition, &posSize);
                parsed = 1;
                break;
            case 't':
                Countvector2(tempStr, &tempParser.TextureUVCount, &tempParser.textureUV, &tempTexture, &uvSize);
                parsed = 1;
                break;
            case 'n':
                Countvector3(tempStr, &tempParser.NormalCount, &tempParser.normal, &tempNormal, &normalSize);
                parsed = 1;
                break;
            }
            if (parsed)
                current = tempStr;
        }
        else if ((Check = _mm256_movemask_epi8(TestM)))
        {
            int advance = 1;
            char *temp = current;
            int strCount = 0;
            while (strCount < 32 && Check && advance)
            {
                bool found = false;
                advance = _tzcnt_u32(Check);
                strCount += advance;
                // TODO count meshes from g and o, materials from mtl file
                if (StringsAreEqual_C({6, 6, "usemtl"}, temp + advance - 3, NULL))
                {
                    MyMalloc(&result->meshMem, sizeof(ObjMesh));
                    if (result->meshCount >= 1)
                    {
                        result->meshes[result->meshCount].indexStart = result->indexCount;
                        result->meshes[result->meshCount - 1].indexCount =
                            result->indexCount - result->meshes[result->meshCount - 1].indexStart;
                    }

                    result->meshes[result->meshCount].materialIndex = -1;
                    int j = 0;
                    for (j = 0; j < (int)result->materials.count; j++)
                    {
                        if (StringsAreEqual_C(result->materials.material[j].name, temp + advance + 4, " \t\n\r"))
                        {
                            result->meshes[result->meshCount].materialIndex = j;
                            break;
                        }
                    }

                    if (result->meshes[result->meshCount].materialIndex == (u32)-1)
                        __debugbreak(); // FatalError("Material unassigned");
                    result->meshCount++;
                    found = true;
                }
                Check = Check << advance;
                temp += advance;
                if (found)
                    current = temp;
            }
        }
        else if ((Check = _mm256_movemask_epi8(TestF)))
        {
            char *temp = current;
            int Advance = 1;
            Advance = _tzcnt_u32(Check);
            temp += Advance;
            Check = Check << Advance;
            int i = 0;
            int j = 0;
            while (*current != 's' && temp[i] != '\n' && temp[i] != '\r')
            {
                if (result->indexCount >= vertexIndicesSize)
                {
                    MyMalloc(&tempVertexIndexMem, sizeof(ObjVertexIndex) * vertexIndicesSize);
                    vertexIndicesSize *= 2;
                }
                int tempIndex = parse_i32(&temp[i + 1]) - 1;
                switch (temp[i])
                {
                case ' ':
                case '/': {
                    if (tempIndex != -1)
                    {
#ifdef DEBUG
                        if (tempIndex > tempParser.PosCount && j % 3 == 0)
                            __debugbreak();
                        if (tempIndex > tempParser.NormalCount && j % 3 == 2)
                            __debugbreak();
                        if (tempIndex > tempParser.TextureUVCount && j % 3 == 1)
                            __debugbreak();
#endif
                        switch (j % 3)
                        {
                        case 0:
                            tempParser.vertexIndices[result->indexCount].positionIndex = tempIndex;
                            break;
                        case 1:
                            tempParser.vertexIndices[result->indexCount].textureIndex = tempIndex;
                            break;
                        case 2:
                            tempParser.vertexIndices[result->indexCount++].normalIndex = tempIndex;
                            break;
                        }
                        j++;
                    }
                }
                break;
                }
                i++;
            }
        }

        endOfLine = get_next_line(objFile.file, current) + 1;
    }
    result->indices = (u32 *)MyMalloc(&result->indexMem, result->indexCount * sizeof(u32));

    if (result->materials.count == 0)
    {
        result->meshCount = 1;
        result->materials.count = 1;
        result->materials.material =
            (ObjMaterial *)MyMalloc(&result->meshMem, sizeof(ObjMaterials) + sizeof(ObjMaterial));
        *result->materials.material = initMaterial();
    }

    result->meshes[result->meshCount - 1].indexCount =
        result->indexCount - result->meshes[result->meshCount - 1].indexStart;
    if (result->meshes[result->meshCount - 1].materialIndex == (u32)-1)
    {
        __debugbreak();
    }

    // Custom hash map with custom hash function to deduplicate vertices
#if 0
    result->vertices = (ObjVertex *)MyMalloc(&result->vertexMem, result->indexCount * sizeof(ObjVertex));
    result->vertexCount = result->indexCount;
    for (int i = 0; i < (int)result->indexCount; i++)
    {
        result->indices[i] = i;
        ObjVertex tempVrtx = {tempParser.position[tempParser.vertexIndices[i].positionIndex],
                              tempParser.normal[tempParser.vertexIndices[i].normalIndex],
                              tempParser.textureUV[tempParser.vertexIndices[i].textureIndex]};

        result->vertices[i] = tempVrtx;
    }
#else
    result->vertices = (ObjVertex *)MyMalloc(&result->vertexMem, result->indexCount * sizeof(ObjVertex));
    VertexKey *hash = (VertexKey *)MyMalloc(&tempVertexHashMem, sizeof(VertexKey) * result->indexCount);
    VertexKey *tempNext = (VertexKey *)MyMalloc(&tempVertexHashMem, sizeof(VertexKey) * result->indexCount);
    u64 listCount = 0;

    int deduplicates = 0;
    ObjVertexIndex zero = {};
    u32 tempCount = 0;
    u32 tempIndex = 0;

    ObjVertexIndex *vertexIndices = tempParser.vertexIndices;
    for (int i = 0; i < (int)result->indexCount; i++)
    {
        // TODO(V. Caraulan): Write a better hash function
        u64 hashSlot = (int)(vertexIndices[i].positionIndex + vertexIndices[i].normalIndex * 2 +
                             vertexIndices[i].textureIndex * 3) %
                       (result->indexCount);
        VertexKey *key = hash + hashSlot;

        tempIndex = tempCount;
        if (tempCount > (u32)vertexSize)
        {
            MyMalloc(&result->vertexMem, sizeof(ObjVertex) * vertexSize);
            vertexSize *= 2;
        }
        if (memcmp(&key->key, &zero, sizeof(ObjVertexIndex)) == 0)
        {
            ObjVertex tempVrtx = {tempParser.position[tempParser.vertexIndices[i].positionIndex],
                                  tempParser.normal[tempParser.vertexIndices[i].normalIndex],
                                  tempParser.textureUV[tempParser.vertexIndices[i].textureIndex]};
            key->key = tempParser.vertexIndices[i];
            result->vertices[tempCount] = tempVrtx;
            key->index = tempCount;
            tempCount++;
        }
        else
        {
            VertexKey *current = key;
            while (current->next != nullptr)
            {
                if (memcmp(&current->key, &tempParser.vertexIndices[i], sizeof(ObjVertexIndex)) == 0)
                {
                    tempIndex = current->index;
                    deduplicates++;
                    break;
                }
                current = current->next;
            }
            if (current->next == nullptr &&
                memcmp(&current->key, &tempParser.vertexIndices[i], sizeof(ObjVertexIndex)) != 0)
            {
                ObjVertex tempVrtx = {tempParser.position[tempParser.vertexIndices[i].positionIndex],
                                      tempParser.normal[tempParser.vertexIndices[i].normalIndex],
                                      tempParser.textureUV[tempParser.vertexIndices[i].textureIndex]};
                current->next = tempNext + listCount;
                current->next->key = tempParser.vertexIndices[i];
                current->next->index = tempCount;
                listCount++;
                result->vertices[tempCount] = tempVrtx;
                tempIndex = tempCount;
                tempCount++;
            }
            else if (memcmp(&current->key, &tempParser.vertexIndices[i], sizeof(ObjVertexIndex)) == 0)
            {
                deduplicates++;
                tempIndex = current->index;
            }
        }
        result->indices[i] = tempIndex;
    }
    result->vertexCount = tempCount;

   snprintf(OutputStringDebug, 254, "%d deduplicates %lld hash collisions\n", deduplicates, listCount);
   OutputDebugStringA(OutputStringDebug);
#endif

    DeInitMemory(&tempPosition);
    DeInitMemory(&tempNormal);
    DeInitMemory(&tempTexture);
    DeInitMemory(&temporary);
    DeInitMemory(&tempVertexIndexMem);
    DeInitMemory(&tempVertexHashMem);

    end = __rdtsc();
    QueryPerformanceCounter(&EndingTime);
    ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;

    ElapsedMicroseconds.QuadPart *= 1000000;
    ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;

    snprintf(OutputStringDebug, 254, "%lld cy, %lld us, %lld ms %s\n", end - start, ElapsedMicroseconds.QuadPart,
             ElapsedMicroseconds.QuadPart / 1000, path);
    OutputDebugStringA(OutputStringDebug);

    return 0;
}

void DeInitGraphicsObj(ObjModel *obj)
{
    if (obj->vertexMem.data)
        DeInitMemory(&obj->vertexMem);
    if (obj->indexMem.data)
        DeInitMemory(&obj->indexMem);
    if (obj->imageMem.data)
        DeInitMemory(&obj->imageMem);
}

void DeInitObj(ObjModel *obj)
{
    if (obj->vertexMem.data)
        DeInitMemory(&obj->vertexMem);
    if (obj->indexMem.data)
        DeInitMemory(&obj->indexMem);
    if (obj->imageMem.data)
        DeInitMemory(&obj->imageMem);
    if (obj->meshMem.data)
        DeInitMemory(&obj->meshMem);
}

static ObjModel CreateCube(Memory *memory)
{
    ObjModel result = {};

    ObjMesh *tmp = (ObjMesh *)MyMalloc(memory, sizeof(ObjMesh));

    result.meshes = tmp;
    *tmp = {};
    result.meshCount = 1;

    vec3 positions[] = {
        {{0.50000f, -0.50000f, -0.50000f}},  {{0.50000f, -0.50000f, 0.50000f}},  {{-0.50000f, -0.50000f, 0.50000f}},
        {{-0.50000f, -0.50000f, -0.50000f}}, {{0.50000f, 0.50000f, -0.5000f}},   {{0.50000f, 0.50000f, 0.50000f}},
        {{-0.50000f, 0.50000f, 0.50000f}},   {{-0.50000f, 0.50000f, -0.50000f}},
    };
    vec3 normals[] = {
        {{0.00000f, -1.00000f, 0.00000f}}, {{0.00000f, 1.00000f, 0.00000f}},    {{1.00000f, 0.00000f, 0.00000f}},
        {{-0.00000f, 0.00000f, 1.00000f}}, {{-1.00000f, -0.00000f, -0.00000f}}, {{0.00000f, 0.00000f, -1.00000f}},
    };
    vec2 uv[] = {
        {{1.00000f, 0.333333f}},  {{1.00000f, 0.666667f}},
        {{0.666667f, 0.666667f}}, {{0.666667f, 0.333333f}},
        {{0.666667f, 0.00000f}},  {{0.00000f, 0.333333f}},
        {{0.00000f, 0.00000f}},   {{0.333333f, 0.00000f}},
        {{0.333333f, 1.00000f}},  {{0.00000f, 1.00000f}},
        {{0.00000f, 0.666667f}},  {{0.333333f, 0.333333f}},
        {{0.333333f, 0.666667f}}, {{1, 0}},
    };

    ObjVertex vertices[] = {
        {positions[1], normals[0], uv[0]},  {positions[2], normals[0], uv[1]},
        {positions[3], normals[0], uv[2]}, //  0  1  2
        {positions[7], normals[1], uv[0]},  {positions[6], normals[1], uv[3]},
        {positions[5], normals[1], uv[4]}, //  3  4  5
        {positions[4], normals[2], uv[5]},  {positions[5], normals[2], uv[6]},
        {positions[1], normals[2], uv[7]}, //  6  7  8
        {positions[5], normals[3], uv[7]},  {positions[6], normals[3], uv[4]},
        {positions[2], normals[3], uv[3]}, //  9 10 11
        {positions[2], normals[4], uv[8]},  {positions[6], normals[4], uv[9]},
        {positions[7], normals[4], uv[10]}, // 12 13 14
        {positions[0], normals[5], uv[11]}, {positions[3], normals[5], uv[12]},
        {positions[7], normals[5], uv[10]}, // 15 16 17
        {positions[0], normals[0], uv[3]},  // 18  0  2
        {positions[4], normals[1], uv[13]}, // 19  3  5
        {positions[0], normals[2], uv[11]}, // 20  6  8
        {positions[1], normals[3], uv[11]}, // 21  9 11
        {positions[3], normals[4], uv[12]}, // 22 12 14
        {positions[4], normals[5], uv[5]},  // 23 15 17
    };

    result.vertexCount = sizeof(vertices) / sizeof(ObjVertex);
    result.vertices = (ObjVertex *)MyMalloc(memory, sizeof(ObjVertex) * result.vertexCount);

    for (int i = 0; i < (int)result.vertexCount; i++)
        result.vertices[i] = vertices[i];

    int triangleIndices[] = {0,  1, 2, 3,  4, 5, 6,  7, 8, 9,  10, 11, 12, 13, 14, 15, 16, 17,
                             18, 0, 2, 19, 3, 5, 20, 6, 8, 21, 9,  11, 22, 12, 14, 23, 15, 17};
    tmp->indexCount = result.indexCount = sizeof(triangleIndices) / sizeof(int);
    result.indices = (u32 *)MyMalloc(memory, sizeof(u32) * result.indexCount);

    for (int i = 0; i < (int)tmp->indexCount; i++)
        result.indices[i] = triangleIndices[i];
    // tmp->material.ambient = (vec3){1, 1, 1};
    // tmp->material.diffuse = (vec3){1, 1, 1};
    // tmp->material.Opaqueness = 1.f;

    return (result);
}

#if 0
static ObjModel CreateSphere(int latitudeCount, int longitudeCount, ArenaAllocator *memory)
{
    ObjModel result = {};

    ObjMesh *mesh = (ObjMesh *)MyMalloc(memory, sizeof(ObjMesh));

    result.meshes = mesh;
    result.meshCount = 1;

    u32 indexCount = 6 * longitudeCount * (latitudeCount - 1);
    u32 vertexCount = (longitudeCount + 1) * (latitudeCount + 1);
    mesh->indexCount = indexCount;

    u32 *indices = (u32 *)MyMalloc(memory, sizeof(u32) * mesh->indexCount);
    ObjVertex *vertices = (ObjVertex *)MyMalloc(memory, sizeof(ObjVertex) * vertexCount);

    float longitudeStep = M_PI * 2 / longitudeCount;
    float latitudeStep = M_PI / latitudeCount;

    int v = 0;
    for (int i = 0; i <= latitudeCount; ++i)
    {
        for (int j = 0; j <= longitudeCount; ++j, v++)
        {
            vertices[v].position = {{cosf(longitudeStep * j) * sinf(i * latitudeStep), cosf(i * latitudeStep - M_PI),
                                     sinf(longitudeStep * j) * sinf(i * latitudeStep)}};

            vec3 zero = {};
            vertices[v].normal = zero - vertices[v].position;
            NormalizeV3(vertices[v].normal);
            vertices[v].textureUV = {{(f32)j / longitudeCount, (f32)i / latitudeCount}};
        }
    }

    int i = 0;
    v = longitudeCount + 1;
    for (int lon = 0; lon < longitudeCount; lon++, v++)
    {
        indices[i++] = lon;
        indices[i++] = v;
        indices[i++] = v + 1;
    }

    v = longitudeCount + 1;
    for (int lat = 1; lat < latitudeCount - 1; lat++, v++)
    {
        for (int lon = 0; lon < longitudeCount; lon++, v++)
        {
            indices[i++] = v;
            indices[i++] = v + longitudeCount + 1;
            indices[i++] = v + 1;

            indices[i++] = v + 1;
            indices[i++] = v + longitudeCount + 1;
            indices[i++] = v + longitudeCount + 2;
        }
    }

    for (int lon = 0; lon < longitudeCount; lon++, v++)
    {
        indices[i++] = v;
        indices[i++] = v + longitudeCount + 1;
        indices[i++] = v + 1;
    }
    // mesh->material.ambient    = (vec3){1, 1, 1};
    // mesh->material.diffuse    = (vec3){1, 1, 1};
    // mesh->material.Opaqueness = 1.f;
    return result;
}
#endif