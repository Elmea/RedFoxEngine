#include "ObjParser.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_STDIO
#include <STB_Image/stb_image.h>

#include <meow_hash_x64_aesni.h>
#ifndef MEMORY_IMPLEMENTATION
#define MEMORY_IMPLEMENTATION
#endif
#define MATHS_HPP_IMPLEMENTATION
#include "engine_math.h"

#include "MyMemory.h"

/*
Function that uses intrinsics to read 32 bytes by 32 bytes
until it finds the newline character  - '\n' or the end of string ('\0')
and advances the str pointer to that character.
*/

char *
get_next_line(MyString buffer, char *str)
{
	if (*str == '\n' || *str == '\r')     /* If we are on the newline character we try to find the next one. */
		str = str + 1;
	__m256i Carriage  = _mm256_set1_epi8('\n');     /* We first load 32 bytes of newline characters into the Carriage 256 bits register. */
	//	__m256i CarriageR = _mm256_set1_epi8('\r');
	__m256i Zero      = _mm256_set1_epi8(0);

	/* If the string is smaller than 32 bytes, we can't use the intrinsics,
	and load 32 bytes at a time. That's why we go byte by byte.       */
	while (buffer.size - (str - (char *)buffer.data) >= 32)
	{
		__m256i Batch = _mm256_loadu_si256((__m256i *)str);  /* We then load 32 bytes from string into the Batch 256 bits register */

		/* We then check if there are any newline characters in the first string
		by comparing 32 bytes at a time. The result*/
		__m256i TestC = _mm256_cmpeq_epi8(Batch, Carriage);
		__m256i TestZ = _mm256_cmpeq_epi8(Batch, Zero);

		__m256i Test  = _mm256_or_si256(TestC, TestZ);      /* We check if either the '\n' character or '\0' character were found*/

		/* We store the results of the check into a int,
		transforming the mask from 256 bits, into a 1bit mask*/
		s32 Check = _mm256_movemask_epi8(Test);
		if(Check)
		{
			/* The _tzcnt_u32 counts the numbers of zeros inside the parameter.
			In our case it's going to count how many characters different than '\n' there are*/

			s32 Advance = _tzcnt_u32(Check);
			str += Advance;
			if (*str == '\r')
				str++;
			return (str);
		}
		str += 32;
	}

	if (buffer.size - (str - (char *)buffer.data) < 32){
		while (*str != '\n' && *str != '\0')
			str++;
	}
	if (*str == '\r')
		str++;
	return (str);
}

char
is_whitespace(char c)
{
	if (c == ' ' || c == '\t')
		return (1);
	return (0);
}

/*
Function that uses intrinsics to read 16 bytes by 16 bytes
until it finds a character different than space(' ') or tab('\t')
and advances the str pointer to that character.

It has the exact same logic as the get_next_line above,
but instead of looking for a character, it looks for the lack of a character.
*/

char *
get_next_word(char *str)
{
	__m256i Space   = _mm256_set1_epi8(' ');
	__m256i Tab     = _mm256_set1_epi8('\t');
	__m256i NewLine = _mm256_set1_epi8('\n');

	char first = 0;
	while (1)
	{
		__m256i Batch = _mm256_loadu_si256((__m256i *)str);
		__m256i TestS = _mm256_cmpeq_epi8(Batch, Space);
		__m256i TestT = _mm256_cmpeq_epi8(Batch, Tab);
		__m256i TestN = _mm256_cmpeq_epi8(Batch, NewLine);
		__m256i Test  = _mm256_or_si256(TestS, TestT);
		Test          = _mm256_or_si256(Test, TestN);

		u32 Check = _mm256_movemask_epi8(Test);
		if (first)
			Check = ~Check;
		if(Check)
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

s32
_digit_value (char c) {
	s32 ri = c;
	s32 v = 16;

	if (c >= '0' && c <= '9')
		v = ri - '0';
	else if (c >= 'a' && c <= 'z')
		v = ri-'a'+ 10;
	else if (c >= 'A' && c <= 'Z')
		v = ri-'A'+ 10;
	return v;
}

s32
parse_i64(char *str, s32 size) {
	s32 value = 0;
	char *s = str;

	char neg = 0;
	if (size > 1) {
		switch (s[0]) {
			case '-': {neg = 1; s++; break;}
			case '+': s++;
		}
	}
	s32 base = 10;
	s32 i = 0;

	while  (i < size) {

		s32 v = (s32)_digit_value(s[i]);
		if (v >= base) {
			break;
		}
		value *= base;
		value += v;
		i++;
	}

	if (neg) {
		value = -value;
	}
	return value;
}

f64
parse_f64(char *str, s32 size){
	f64 value = 0;
	char *s = str;

	s32 i = 0;

	f64 sign = 1;
	switch (s[i]) {
		case '-': { i += 1; sign = -1;}  break;
		case '+': i += 1; break;
	}

	for (; i < size; i += 1) {
		char c = s[i];

		s32 v = _digit_value(c);
		if (v >= 10) {
			break;
		}
		value *= 10;
		value += (f64)v;
	}

	if (i < size && s[i] == '.') {
		f64 pow10 = 10;
		i += 1;

		for (; i < size; i += 1) {
			char c = s[i];

			s32 v = _digit_value(c);
			if (v >= 10) {
				break;
			}
			value += (f64)v/pow10;
			pow10 *= 10;
		}
	}

	char frac = 0;
	f64 scale = 1;

	if (i < size && (s[i] == 'e' || s[i] == 'E')) {
		i += 1;

		if (i < size) {
			switch (s[i]) {
				case '-': { i += 1; sign = -1;frac = 1;}  break;
				case '+': i += 1; break;
			}

			u32 exp = 0;
			for (; i < size; i += 1) {
				u32 d  = _digit_value(s[i]);
				if (d >= 10) {
					break;
				}
				exp = exp * 10 + d;
			}
			if (exp > 308) { exp = 308; }

			while (exp >= 50) {scale *= 1e50; exp -= 50; }
			while (exp >=  8) { scale *=  1e8; exp -=  8; }
			while (exp >   0) { scale *=   10; exp -=  1; }
		}
	}
	if (frac) {
		value = sign * (value/scale);
	} else {
		value = sign * (value*scale);
	}
	return (value);
}

vec3
getvec3(char **str)
{
	vec3 result;

	result.x = (f32)parse_f64(*str, 32);
	*str = get_next_word(*str);
	result.y = (f32)parse_f64(*str, 32);
	*str = get_next_word(*str);
	result.z = (f32)parse_f64(*str, 32);
	return (result);
}

vec2
getvec2(char **str)
{
	vec2 result;

	result.x = (f32)parse_f64(*str, 32);
	*str = get_next_word(*str);
	result.y = (f32)parse_f64(*str, 32);
	return (result);
}

void FatalError(const char* message, char *file, int line)
{
	MessageBoxA(NULL, message, "Error", MB_ICONEXCLAMATION);
	ExitProcess(0);
}

void
initImage(ImageData *result, Memory *memory)
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
	if (imageFile.handle != INVALID_HANDLE_VALUE) {
		ImageData temp;
		temp = *((ImageData *)imageFile.data);
		if (result->hash64 == temp.hash64) {
			result->width  = temp.width;
			result->height = temp.height;
			result->nrChannels = temp.nrChannels;
			result->imageSize = imageFile.size;
			result->data = (unsigned char *)((size_t)imageFile.data + sizeof(ImageData));
		}
		else {
			FatalError("Invalid hashed image file", __FILE__, __LINE__);
		}

		CloseHandle(imageFile.handle);
	} else {
		stbi_set_flip_vertically_on_load(1);
		result->data = stbi_load_from_memory(file.data, file.size, &result->width, &result->height, &result->nrChannels, STBI_rgb_alpha);

		HANDLE File = CreateFileA(tempBuffer,
								  GENERIC_WRITE,
								  FILE_SHARE_WRITE | FILE_SHARE_READ,
								  NULL,
								  CREATE_NEW,
								  FILE_ATTRIBUTE_NORMAL,
								  NULL);
		DWORD bytesWritten = 0;
		WriteFile(File, result, sizeof(ImageData), &bytesWritten, NULL);
		WriteFile(File, result->data, result->width * result->height * result->nrChannels, &bytesWritten, NULL);
		DWORD LastError = GetLastError();

		CloseHandle(File);
	}
}

Material
initMaterial()
{
	Material result = {0};

	result.ambient    = {0.2f, 0.2f, 0.2f};
	result.diffuse    = {0.8f, 0.8f, 0.8f};
	result.specular   = {0};
	result.emissive   = {0};
	result.Shininess  = 25.f;
	result.Opaqueness = 1;
	return (result);
}


typedef struct ParameterImageThread{
	ImageData *result;
	u64 	   count;
	Memory *memory;
} ParameterImageThread;

void initImageThread(void *parameter)
{
	ParameterImageThread*p = (ParameterImageThread *)parameter;
	for (int i = 0; i < p->count; i++)
	{
		initImage(&p->result[i], p->memory);
	}
}

ObjMaterials
ParseMTL(MyString buffer, ObjImages *ImagesOut, Memory *imageMem, Memory *meshMem)
{
	ObjMaterials result = { 0 };

	char *current = (char *)buffer.data;
	char *endOfLine = (char *)buffer.data;
	int i = -1;

	// TODO(V. Caraulan): Return default material in case of empty buffer

	while (*endOfLine != '\0')
	{
		current = endOfLine;
		while (*current == '#')
			current = get_next_line(buffer, current) + 1;
		endOfLine = get_next_word(current);
		MyString temp = { 7, 7, (u8 *)"newmtl " };
		if (StringsAreEqual_C(temp, current, NULL))
			result.count++;
	}
	result.material = (Material *)MyMalloc(meshMem, sizeof(Material) * result.count);
	ImagesOut->data = (ImageData *)MyMalloc(imageMem, sizeof(ImageData) * result.count);
	ParameterImageThread *para = (ParameterImageThread *)MyMalloc(imageMem, sizeof(ParameterImageThread));
	current = (char *)buffer.data;
	endOfLine = (char *)buffer.data;
	while (*endOfLine != '\0')
	{
		current = endOfLine;
		while (*current == '#')
			current = get_next_line(buffer, current) + 1;
		endOfLine = get_next_word(current);
		if (StringsAreEqual_C({ 7, 7, (u8 *)"newmtl " }, current, NULL))
		{
			i++;
			result.material[i] = initMaterial();

			int j = 0;

			while (current[j + 7] != '\n' &&
				   current[j + 7] != '\r' &&
				   current[j + 7] != '\0')
				j++;
			result.material[i].name = initStringChar(&current[7], j, meshMem);
			endOfLine = get_next_line(buffer, current) + 1;
		}
		if (StringsAreEqual_C({ 7, 7, (u8 *)"map_Kd " }, current, NULL))
		{
			result.material[i].hasTexture = 1;
			int j = 0;

			while (current[j + 7] != '\n' &&
				   current[j + 7] != '\r' &&
				   current[j + 7] != '\0')
				j++;
			ImagesOut->data[ImagesOut->count].path = initStringChar(&current[7], j, meshMem);
			u8 found = 0;
			for (int k = 0; k < ImagesOut->count; k++)
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
			//TODO else remove the path allocation from ImagesOut
			endOfLine = get_next_line(buffer, current) + 1;
		}
		else if (StringsAreEqual_C({ 3, 3, (u8 *)"Kd " }, current, NULL))
		{
			current = get_next_word(current);
			result.material[i].diffuse = getvec3(&current);
			endOfLine = get_next_line(buffer, current) + 1;
		}
		else if (StringsAreEqual_C({ 3, 3, (u8 *)"Ks " }, current, NULL))
		{
			current = get_next_word(current);
			result.material[i].specular = getvec3(&current);
			endOfLine = get_next_line(buffer, current) + 1;
		}
		else if (StringsAreEqual_C( { 3, 3, (u8 *)"Ke " }, current, NULL))
		{
			current = get_next_word(current);
			result.material[i].emissive = getvec3(&current);
			endOfLine = get_next_line(buffer, current) + 1;
		}
		else if (StringsAreEqual_C({ 2, 2, (u8 *)"d " }, current, NULL))
		{
			current = get_next_word(current);
			result.material[i].Opaqueness = (f32)parse_f64(current, 32);
			endOfLine = get_next_line(buffer, current) + 1;
		}
	}
	para[0] = {ImagesOut->data,ImagesOut->count, imageMem};
	ImagesOut->thread = CreateThread(NULL,                                    //[in, optional]  LPSECURITY_ATTRIBUTES   lpThreadAttributes,
									 sizeof(ParameterImageThread),            //[in]            SIZE_T                  dwStackSize,
									 (LPTHREAD_START_ROUTINE)initImageThread, //[in]            LPTHREAD_START_ROUTINE  lpStartAddress,
									 &para[0],                 //[in, optional]  __drv_aliasesMem LPVOID lpParameter,
									 0,                                       //[in]            DWORD                   dwCreationFlags,
									 NULL);                                   //[out, optional] LPDWORD                 lpThreadId
	return result;
}


void
Countvector2(char * __restrict current, s64 * __restrict Count, vec2 **  __restrict v, Memory *  __restrict temp, int * __restrict size)
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

void
Countvector3(char * __restrict current, s64 * __restrict Count, vec3 ** __restrict v, Memory * __restrict temp, int * __restrict size)
{
	vec3 zero = {};
	if (*Count >= *size)
	{
		MyMalloc(temp, sizeof(vec3) * *size);
		*size = *size * 2;
	}
	current = get_next_word(current);
	(*v)[*Count] = getvec3(&current);
	//if (memcmp(&((*v)[*Count]), &zero, sizeof(vec3)) == 0)
	//__debugbreak();
	*Count = *Count + 1;
}


//typedef DWORD (__stdcall *LPTHREAD_START_ROUTINE) ([in] LPVOID lpThreadParameter);

typedef struct VertexKey
{
	ObjVertexIndex	key;
	u32			   index;
	VertexKey		*next;
} VertexKey;

int
ParseModel(ObjModel *result, char *path)
{
	ObjParsingObject tempParser = {};

	result->meshMem = InitVirtualMemory(1ULL * GigaByte);
	result->vertexMem = InitVirtualMemory(1ULL * GigaByte);
	result->indexMem = InitVirtualMemory(1ULL * GigaByte);
	result->imageMem = InitVirtualMemory(1ULL * GigaByte);

	Memory temporary = InitVirtualMemory(1ULL * GigaByte);

	fileResource objFile = FileResourceInit(path, &temporary);
	LoadFile(&objFile, &temporary);

	// TODO(V. Caraulan): deinit memory
	if (objFile.handle == INVALID_HANDLE_VALUE)
		return -1;

	if (objFile.data == NULL)
		return -1;

	char *mtlLib;

	u64 start = __rdtsc();
	u64 end;
	LARGE_INTEGER StartingTime, EndingTime, ElapsedMicroseconds;
	LARGE_INTEGER Frequency;

	QueryPerformanceFrequency(&Frequency);
	QueryPerformanceCounter(&StartingTime);

	char OutputStringDebug[254];
	Memory tempVertexMem  = InitVirtualMemory(1LL * GigaByte);

	Memory tempVertexHashMem  = InitVirtualMemory(1LL * GigaByte);

	Memory tempPosition= InitVirtualMemory(1LL * GigaByte);
	Memory tempNormal  = InitVirtualMemory(1LL * GigaByte);
	Memory tempTexture = InitVirtualMemory(1LL * GigaByte);

	int posSize    = 3;
	int normalSize = 3;
	int uvSize     = 3;
	int indexSize  = 3;
	int vertexSize = 3;

	result->vertices = (ObjVertex*)MyMalloc(&result->vertexMem, sizeof(ObjVertex) * vertexSize);
	result->indices  = (u32 *)MyMalloc(&result->indexMem , sizeof(u32) * indexSize);
	tempParser.verticesIndices = (ObjVertexIndex*)MyMalloc(&tempVertexMem, sizeof(ObjVertexIndex) * indexSize);
	tempParser.position        = (vec3*)MyMalloc(&tempPosition, sizeof(vec3) * posSize);
	tempParser.normal          = (vec3*)MyMalloc(&tempNormal,  sizeof(vec3) * normalSize);
	tempParser.textureUV       = (vec2*)MyMalloc(&tempTexture, sizeof(vec2) * uvSize);

	//	struct { ObjVertexIndex key; u32 value; } *hash = NULL;
	//	hmdefault(hash, -1);
	int deduplicates = 0;

	char ended = 0;
	char parsed = 0;
	{
		mtlLib = NULL;
		__m256i MaterialChar     = _mm256_set1_epi8('m');
		__m256i VertexIndexBegin = _mm256_set1_epi8('f');
		__m256i VertexBegin      = _mm256_set1_epi8('v');

		size_t currentRead = 0;

		char *current   = (char *)objFile.data;
		char *endOfLine = (char *)objFile.data;
		while (objFile.size - (current - (char *)objFile.data) >= 32)
		{
			current = endOfLine;

			while (*current == '#')
				current = get_next_line(objFile.file, current) + 1;

			__m256i Batch       = _mm256_loadu_si256((__m256i *)current);
			__m256i TestM       = _mm256_cmpeq_epi8(Batch, MaterialChar);
			__m256i TestF       = _mm256_cmpeq_epi8(Batch, VertexIndexBegin);
			__m256i TestV       = _mm256_cmpeq_epi8(Batch, VertexBegin);

			int Check  = 0;
			int Count  = 0;
			char *temp = current;

			if (Check = _mm256_movemask_epi8(TestV)) {
				char *tempStr = current;
				tempStr += _tzcnt_u32(Check) + 1;
				char tempParsed = parsed;
				parsed = 0;
				switch (*tempStr)
				{
					case ' ': Countvector3(tempStr, &tempParser.PosCount, &tempParser.position, &tempPosition, &posSize);parsed = ' '; break;
					case 'n': Countvector3(tempStr, &tempParser.NormalCount, &tempParser.normal, &tempNormal, &normalSize);parsed = 'n'; break;
					case 't': Countvector2(tempStr, &tempParser.TextureUVCount, &tempParser.textureUV, &tempTexture, &uvSize);parsed = 't'; break;
				}
				if (parsed)
				{
					if (ended != parsed) // TODO(V. Caraulan): DELETE ???
						ended = parsed;
					current = tempStr;
				}
				else
					parsed = tempParsed;
			}

			if (Check = _mm256_movemask_epi8(TestM)) {
				int Advance = 1;
				temp = current;
				Count = 0;
				while (Count < 32 && Check && Advance)
				{
					Advance = _tzcnt_u32(Check);
					Count += Advance;
					if (StringsAreEqual_C({6, 6, (u8 *)"mtllib"}, temp + Advance, NULL))
						mtlLib = temp + Advance + 7;
					else if (StringsAreEqual_C({6, 6, (u8 *)"usemtl"}, temp + Advance - 3, NULL))
						result->materials.count++; // TODO(V. Caraulan): remove this
					else
						break;
					Check = Check << Advance;
					temp += Advance;
				}
			}

			if (Check = _mm256_movemask_epi8(TestF)) {
				temp = current;
				int Advance = 1;
				Count = 0;
				int j = 0;
				while (Count < 32 && Check && Advance)
				{
					Advance = _tzcnt_u32(Check);
					Count += Advance;
					temp += Advance;
					if (temp[0] != 'f' && temp[1] != ' ')
					{
						break;
						__debugbreak();
					}
					else
					{
						parsed = 'f';
						if (ended != parsed)
						{
							ended = parsed;
						}
					}
					for (s64 i = 0; temp[i] != '\n' && temp[i + 1] != '\r' && temp[i + 1] != '\n'; i++)
					{
						if (temp[i] == ' ' || temp[i] == '/')
						{
							if (result->IndexCount >= indexSize)
							{
								MyMalloc(&result->indexMem , sizeof(u32) * indexSize);
								MyMalloc(&tempVertexMem, sizeof(ObjVertexIndex) * indexSize);
								indexSize *= 2;
							}
							s64 TempIndex = parse_i64(&temp[i + 1], 32);
							if(TempIndex == 0)
							{
								//__debugbreak();
								break;
							}
							else if (TempIndex - 1 < 0)
								__debugbreak();
							if (j % 3 == 0)
								tempParser.verticesIndices[result->IndexCount].positionIndex = TempIndex - 1;
							else if (j % 3 == 1)
								tempParser.verticesIndices[result->IndexCount].textureIndex = TempIndex - 1;
							else if (j % 3 == 2)
							{
								tempParser.verticesIndices[result->IndexCount].normalIndex = TempIndex - 1;
								result->indices[result->IndexCount] = result->IndexCount;
								result->IndexCount++;
							}
							j++;
						}
					}
					Check = Check << Advance;
				}
			}
			endOfLine = get_next_line(objFile.file, current) + 1;
		}

		if (result->materials.count == 0)
			result->materials.count = 1;
		result->meshes = (ObjMesh   *)MyMalloc(&result->meshMem, result->materials.count * sizeof(ObjMesh));
		// TODO(V. Caraulan): Overcounting meshes
		result->meshCount = result->materials.count;
	}

	VertexKey *hash = (VertexKey*)MyMalloc(&tempVertexHashMem, sizeof(VertexKey) * result->IndexCount);
	VertexKey *tempNext = (VertexKey*)MyMalloc(&tempVertexHashMem, sizeof(VertexKey) * result->IndexCount);
	u64 listCount = 0;

	ObjVertexIndex zero = {};
	u32 tempCount = 0;
	u32 tempIndex = 0;

	for (int i = 0; i < result->IndexCount; i++)
	{
		meow_u128 Hash = MeowHash(MeowDefaultSeed, sizeof(ObjVertexIndex), &tempParser.verticesIndices[i]);
		u64 hashSlot = MeowU64From(Hash, 0) % result->IndexCount;
		VertexKey *key = hash + hashSlot;

		tempIndex = tempCount;
		if (tempCount > vertexSize)
		{
			MyMalloc(&result->vertexMem, sizeof(ObjVertex) * vertexSize);
			vertexSize *= 2;
		}
		if (memcmp(&key->key, &zero, sizeof(ObjVertexIndex)) == 0)
		{
			key->key = tempParser.verticesIndices[i];
			ObjVertex tempVrtx = {
				tempParser.position [tempParser.verticesIndices[i].positionIndex],
				tempParser.normal   [tempParser.verticesIndices[i].  normalIndex],
				tempParser.textureUV[tempParser.verticesIndices[i]. textureIndex]
			};
			result->vertices[tempCount] = tempVrtx;
			key->index = tempCount;
			tempCount++;
		}
		else
		{
			VertexKey *current = key;
			while (current->next != nullptr)
			{
				if (memcmp(&current->key, &tempParser.verticesIndices[i], sizeof(ObjVertexIndex)) == 0)
				{
					tempIndex = current->index;
					break;
				}
				current = current->next;
			}
			if (current->next == nullptr && memcmp(&current->key, &tempParser.verticesIndices[i], sizeof(ObjVertexIndex)) != 0)
			{
				current->next = tempNext + listCount;
				current->next->key = tempParser.verticesIndices[i];
				current->next->index = tempCount;
				listCount++;

				ObjVertex tempVrtx = {
					tempParser.position [tempParser.verticesIndices[i].positionIndex],
					tempParser.normal   [tempParser.verticesIndices[i].  normalIndex],
					tempParser.textureUV[tempParser.verticesIndices[i]. textureIndex]
				};
				result->vertices[tempCount] = tempVrtx;
				tempIndex = tempCount;
				tempCount++;
			}
			else if(memcmp(&current->key, &tempParser.verticesIndices[i], sizeof(ObjVertexIndex)) == 0)
			{
				tempIndex = current->index;
			}
		}
		result->indices[i] = tempIndex;
	}
	result->vertexCount = tempCount;

	if (objFile.data == NULL)
		return {0};

	if (mtlLib == NULL)
		result->materials.count = 1;
	char MtlLibFilePath[255] = {0};

	int pathLen = 0;
	for (pathLen = 0; path[pathLen]; pathLen++)
		;

	if (mtlLib)
		my_strncpy_s(MtlLibFilePath, 255, path, pathLen);


	s64 i = 0;
	while (MtlLibFilePath[i] != '\0')
		i++;
	while ((!(MtlLibFilePath[i] == '/' || MtlLibFilePath[i] == '\\')) && i >= 0)
		i--;
	i++;
	s64 j = 0;
	if (mtlLib != NULL)
	{
		while (mtlLib[j] != '\n' && mtlLib[j] != '\t' && mtlLib[j] != '\r')
		{
			MtlLibFilePath[i] = mtlLib[j++];
			i++;
		}
		MtlLibFilePath[i] = '\0';
	}

	fileResource mtlLibData = FileResourceInit(MtlLibFilePath, &temporary);
	LoadFile(&mtlLibData, &temporary);

	if (mtlLibData.handle)
		CloseHandle(mtlLibData.handle);
	if (mtlLibData.file.data)
		result->materials = ParseMTL(mtlLibData.file, &result->images, &result->imageMem, &result->meshMem);

	char *current = (char *)objFile.data;
	char *endOfLine = (char *)objFile.data;

	s64 meshIndex      = 0;
	s64 vertexIndex = 0;
	while (*endOfLine != '\0') {
		if (StringsAreEqual_C({6, 6, (u8 *)"usemtl"}, current, NULL)) {
			if (meshIndex >= 1) {
				result->meshes[meshIndex].indexStart = vertexIndex;
				result->meshes[meshIndex - 1].indexCount  = vertexIndex - result->meshes[meshIndex - 1].indexStart;
			}

			result->meshes[meshIndex].materialIndex = -1;
			int j = 0;
			for (j = 0; j < result->materials.count; j++) {
				if (StringsAreEqual_C(result->materials.material[j].name, &current[7], " \t\n\r"))
					result->meshes[meshIndex].materialIndex = j;
			}

			if (result->meshes[meshIndex].materialIndex == -1)
				FatalError("Material unassigned", __FILE__, __LINE__);
			meshIndex++;
			current = endOfLine;
		}
		else if (*current == 'f' && current[1] == ' ') {
			vertexIndex += 3;
			endOfLine = get_next_line(objFile.file, current);
		}
		endOfLine = get_next_line(objFile.file, current);
		current = endOfLine + 1;
	}

	QueryPerformanceCounter(&EndingTime);
	ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
	ElapsedMicroseconds.QuadPart *= 1000000;
	ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;
	end = __rdtsc();

	snprintf(OutputStringDebug, 254, "%lld cy, %lld us, %lld ms %s\n", end - start, ElapsedMicroseconds.QuadPart, ElapsedMicroseconds.QuadPart / 1000, path);
	OutputDebugStringA(OutputStringDebug);

	if (meshIndex == 0)
	{
		meshIndex++;
		result->materials.material  = (Material *)MyMalloc(&result->meshMem, sizeof(ObjMaterials) + sizeof(Material));
		*result->materials.material = initMaterial();
		result->materials.count     = 1;

		result->meshes[meshIndex - 1].materialIndex = 0;
	}

	result->meshes[meshIndex - 1].indexCount  = vertexIndex - result->meshes[meshIndex - 1].indexStart;
	if (result->meshes[meshIndex - 1].materialIndex == -1)
	{
		__debugbreak();
	}

	snprintf(OutputStringDebug, 254, "duplicates %d\n", deduplicates / 3);
	OutputDebugStringA(OutputStringDebug);

	DeInitMemory(&tempVertexHashMem);
	DeInitMemory(&tempVertexMem);
	DeInitMemory(&tempPosition);
	DeInitMemory(&tempNormal);
	DeInitMemory(&tempTexture);
	DeInitMemory(&temporary);

	end = __rdtsc();
	QueryPerformanceCounter(&EndingTime);

	ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
	ElapsedMicroseconds.QuadPart *= 1000000;
	ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;

	snprintf(OutputStringDebug, 254, "%lld cy, %lld us, %lld ms %s\n", end - start, ElapsedMicroseconds.QuadPart, ElapsedMicroseconds.QuadPart / 1000, path);
	OutputDebugStringA(OutputStringDebug);
	return 0;
}

void DeInitObj(ObjModel *obj)
{
	if (obj->meshMem.virtualSize)
		DeInitMemory(&obj->meshMem);
	if (obj->imageMem.virtualSize)
		DeInitMemory(&obj->imageMem);
	if (obj->vertexMem.virtualSize)
		DeInitMemory(&obj->vertexMem);
	if (obj->indexMem.virtualSize)
		DeInitMemory(&obj->indexMem);
}

void DeInitGraphicsObj(ObjModel *obj)
{
	if (obj->imageMem.virtualSize)
		DeInitMemory(&obj->imageMem);
	if (obj->vertexMem.virtualSize)
		DeInitMemory(&obj->vertexMem);
	if (obj->indexMem.virtualSize)
		DeInitMemory(&obj->indexMem);
}

ObjModel
CreateCube()
{
	ObjParsingObject tempParser = {0};
	ObjModel result = {0};

	result.meshMem   = InitVirtualMemory(4 * KiloByte);
	result.vertexMem = InitVirtualMemory(4 * KiloByte);
	result.indexMem  = InitVirtualMemory(4 * KiloByte);

	ObjMesh *tmp = (ObjMesh *)MyMalloc(&result.meshMem, sizeof(ObjMesh));

	result.meshes = tmp;
	result.meshCount = 1;

	vec3 positions[] = {
		{0.50000f, -0.50000f, -0.50000f},
		{0.50000f, -0.50000f, 0.50000f},
		{-0.50000f, -0.50000f, 0.50000f},
		{-0.50000f, -0.50000f, -0.50000f},
		{0.50000f, 0.50000f, -0.5000f},
		{0.50000f, 0.50000f, 0.50000f},
		{-0.50000f, 0.50000f, 0.50000f},
		{-0.50000f, 0.50000f, -0.50000f},
	};
	vec3 normals[] ={
		{0.00000f, -1.00000f, 0.00000f},
		{0.00000f, 1.00000f, 0.00000f},
		{1.00000f, 0.00000f, 0.00000f},
		{-0.00000f, 0.00000f, 1.00000f},
		{-1.00000f, -0.00000f, -0.00000f},
		{0.00000f, 0.00000f, -1.00000f},
	};
	vec2 uv[] = {
		{1.00000f, 0.333333f},
		{1.00000f, 0.666667f},
		{0.666667f, 0.666667f},
		{0.666667f, 0.333333f},
		{0.666667f, 0.00000f},
		{0.00000f, 0.333333f},
		{0.00000f, 0.00000f},
		{0.333333f, 0.00000f},
		{0.333333f, 1.00000f},
		{0.00000f, 1.00000f},
		{0.00000f, 0.666667f},
		{0.333333f, 0.333333f},
		{0.333333f, 0.666667f},
		{1        , 0},
	};

	ObjVertex vertices[] = {
		{positions[1], normals[0], uv[0]}, {positions[2], normals[0], uv[1]}, {positions[3], normals[0], uv[2]},   //  0  1  2
		{positions[7], normals[1], uv[0]}, {positions[6], normals[1], uv[3]}, {positions[5], normals[1], uv[4]},   //  3  4  5
		{positions[4], normals[2], uv[5]}, {positions[5], normals[2], uv[6]}, {positions[1], normals[2], uv[7]},   //  6  7  8
		{positions[5], normals[3], uv[7]}, {positions[6], normals[3], uv[4]}, {positions[2], normals[3], uv[3]},   //  9 10 11
		{positions[2], normals[4], uv[8]}, {positions[6], normals[4], uv[9]},  {positions[7], normals[4], uv[10]}, // 12 13 14
		{positions[0], normals[5], uv[11]},{positions[3], normals[5], uv[12]}, {positions[7], normals[5], uv[10]}, // 15 16 17
		{positions[0], normals[0], uv[3]},                                                                         // 18  0  2
		{positions[4], normals[1], uv[13]},                                                                        // 19  3  5
		{positions[0], normals[2], uv[11]},                                                                        // 20  6  8
		{positions[1], normals[3], uv[11]},                                                                        // 21  9 11
		{positions[3], normals[4], uv[12]},                                                                        // 22 12 14
		{positions[4], normals[5], uv[5]},                                                                         // 23 15 17
	};

	result.vertexCount = sizeof(vertices) / sizeof(ObjVertex);
	result.vertices = (ObjVertex *)MyMalloc(&result.vertexMem, sizeof(ObjVertex) * result.vertexCount);

	for (int i = 0; i < (int)result.vertexCount; i++)
		result.vertices[i] = vertices[i];

	int triangleIndices[] =
	{
		0, 1, 2,
		3, 4, 5,
		6, 7, 8,
		9, 10, 11,
		12, 13, 14,
		15, 16, 17,
		18, 0, 2,
		19, 3, 5,
		20, 6, 8,
		21, 9, 11,
		22, 12, 14,
		23, 15, 17
	};
	tmp->indexCount = sizeof(triangleIndices) / sizeof(int);
	tmp->indexStart = 0;
	result.IndexCount = tmp->indexCount;
	result.indices = (u32 *)MyMalloc(&result.indexMem, sizeof(u32) * tmp->indexCount);

	for (int i = 0; i < (int)tmp->indexCount; i++)
		result.indices[i] = triangleIndices[i];

	// TODO(V. Caraulan): create material
	//	tmp->material->ambient = {1, 1, 1};
	//	tmp->material->diffuse = {1, 1, 1};
	//	tmp->material->Opaqueness = 1.f;

	return (result);
}


ObjModel
CreateSphere(int latitudeCount, int longitudeCount, Memory *memory)
{
	ObjModel result = {0};

	ObjMesh *mesh = (ObjMesh *)MyMalloc(memory, sizeof(ObjMesh));

	result.meshes    = mesh;
	result.meshCount = 1;

	mesh->indexCount =  6 * longitudeCount   * (latitudeCount - 1);
	mesh->indexStart = 0;
	result.vertexCount  = (longitudeCount + 1) * (latitudeCount + 1);
	result.IndexCount = mesh->indexCount;

	result.indices  = (u32 *)MyMalloc(memory, sizeof(u32)    * mesh->indexCount);
	result.vertices = (ObjVertex *)MyMalloc(memory, sizeof(ObjVertex) * result.vertexCount);

	float longitudeStep = M_PI * 2 / longitudeCount;
	float latitudeStep  = M_PI     / latitudeCount;

	int v = 0;
	for (int i = 0; i <= latitudeCount; ++i)
	{
		for (int j = 0; j <= longitudeCount; ++j, v++)
		{
			result.vertices[v].position = {
				cosf(longitudeStep * j) * sinf(i * latitudeStep),
				cosf(i * latitudeStep - M_PI)                     ,
				sinf(longitudeStep * j) * sinf(i * latitudeStep)
			};

			result.vertices[v].normal = -result.vertices[v].position;
			NormalizeV3(result.vertices[v].normal);
			result.vertices[v].textureUV = {(f32)j / longitudeCount, (f32)i / latitudeCount};
		}
	}


	int i = 0;
	v = longitudeCount + 1;
	for (int lon = 0; lon < longitudeCount; lon++, v++)
	{
		result.indices[i++] = lon;
		result.indices[i++] = v;
		result.indices[i++] = v + 1;
	}

	v = longitudeCount + 1;
	for (int lat = 1; lat < latitudeCount - 1; lat++, v++)
	{
		for (int lon = 0; lon < longitudeCount; lon++, v++)
		{
			result.indices[i++] = v;
			result.indices[i++] = v + longitudeCount + 1;
			result.indices[i++] = v + 1;

			result.indices[i++] = v + 1;
			result.indices[i++] = v + longitudeCount + 1;
			result.indices[i++] = v + longitudeCount + 2;
		}
	}

	for (int lon = 0; lon < longitudeCount; lon++, v++)
	{
		result.indices[i++] = v;
		result.indices[i++] = v + longitudeCount + 1;
		result.indices[i++] = v + 1;
	}

	// TODO(V. Caraulan): create material
	//	mesh->material->ambient    = {1, 1, 1};
	//	mesh->material->diffuse    = {1, 1, 1};
	//	mesh->material->Opaqueness = 1.f;

	return result;
}
