/*
The MIT License (MIT)

Copyright (c) 2015 Apostol Dadachev

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#ifndef SAVVY_CONVERT_OPTIONS_H
#define SAVVY_CONVERT_OPTIONS_H

#include "SavvyDefines.h"

namespace Savvy
{
	class Blob;

	/*
	The options for file to file conversion
	*/
	struct FileConvertOptions
	{
		/*
		Default Constructor
		*/
		FileConvertOptions() :
			InputPath(NULL),
			OutputPath(NULL),
			InputLang(INVALID_SHADER_LANG),
			OutputLang(INVALID_SHADER_LANG),
			InputEntryPoint(NULL),
			OutputEntryPoint(NULL),
			ShaderType(INVALID_SHADER_TYPE)

		{

		}

		/*
		Default Destructor
		*/
		~FileConvertOptions()
		{
		}

		/*
		The path of the input shader.
		If not filled in, an error is returned by the conversion function.
		*/
		const wchar* InputPath;

		/*
		The path of the output shader.
		If not filled in, the path is the same as the input path.
		*/
		const wchar* OutputPath;

		/*
		The shader input language.
		If not filled in, an error is returned by the conversion function.
		*/
		uint32 InputLang;

		/*
		The shader output language.
		If not filled in, an error is returned by the conversion function.
		*/
		uint32 OutputLang;

		/*
		The input shader entry point function name.
		May not have any effect for certain shader types.
		*/
		const char8* InputEntryPoint;

		/*
		The output shader entry point function name.
		If not filled in, "main" is assumed.
		May not have any effect for certain shader types.
		*/
		const char8* OutputEntryPoint;

		/*
		The type of shader we are converting.
		If not filled in, an error is returned by the conversion function.
		*/
		uint32 ShaderType;

	};

	/*
	The options for blob to blob conversion
	*/
	struct BlobConvertOptions
	{
		/*
		Default Constructor
		*/
		BlobConvertOptions() :
			InputBlob(NULL),
			OutputBlob(NULL),
			InputLang(INVALID_SHADER_LANG),
			OutputLang(INVALID_SHADER_LANG),
			InputEntryPoint(NULL),
			OutputEntryPoint(NULL),
			ShaderType(INVALID_SHADER_TYPE)

		{

		}

		/*
		Default Destructor
		*/
		~BlobConvertOptions()
		{
		}

		/*
		A pointer to the blob of the input shader.
		If not filled in or NULL, an error is returned by the conversion function.
		*/
		Blob* InputBlob;

		/*
		An opaque pointer to the blob of the output shader.
		If not filled in or NULL, an error is returned by the conversion function.
		*/
		Blob* OutputBlob;

		/*
		The shader input language.
		If not filled in, an error is returned by the conversion function.
		*/
		uint32 InputLang;

		/*
		The shader output language.
		If not filled in, an error is returned by the conversion function.
		*/
		uint32 OutputLang;

		/*
		The input shader entry point function name.
		May not have any effect for certain shader types.
		*/
		const char8* InputEntryPoint;

		/*
		The output shader entry point function name.
		If not filled in, "main" is assumed.
		May not have any effect for certain shader types.
		*/
		const char8* OutputEntryPoint;

		/*
		The type of shader we are converting.
		If not filled in, an error is returned by the conversion function.
		*/
		uint32 ShaderType;
	};

	/*
	The options for file to blob conversion
	*/
	struct FileBlobConvertOptions
	{
		/*
		Default Constructor
		*/
		FileBlobConvertOptions() :
			InputPath(NULL),
			OutputBlob(NULL),
			InputLang(INVALID_SHADER_LANG),
			OutputLang(INVALID_SHADER_LANG),
			InputEntryPoint(NULL),
			OutputEntryPoint(NULL),
			ShaderType(INVALID_SHADER_TYPE)

		{

		}

		/*
		Default Destructor
		*/
		~FileBlobConvertOptions()
		{
		}

		/*
		The path of the input shader.
		If not filled in, an error is returned by the conversion function.
		*/
		const wchar* InputPath;

		/*
		An opaque pointer to the blob of the output shader.
		If not filled in or NULL, an error is returned by the conversion function.
		*/
		Blob* OutputBlob;

		/*
		The shader input language.
		If not filled in, an error is returned by the conversion function.
		*/
		uint32 InputLang;

		/*
		The shader output language.
		If not filled in, an error is returned by the conversion function.
		*/
		uint32 OutputLang;

		/*
		The input shader entry point function name.
		May not have any effect for certain shader types.
		*/
		const char8* InputEntryPoint;

		/*
		The output shader entry point function name.
		If not filled in, "main" is assumed.
		May not have any effect for certain shader types.
		*/
		const char8* OutputEntryPoint;

		/*
		The type of shader we are converting.
		If not filled in, an error is returned by the conversion function.
		*/
		uint32 ShaderType;

	};

	/*
	The options for blob to file conversion
	*/
	struct BlobFileConvertOptions
	{
		/*
		Default Constructor
		*/
		BlobFileConvertOptions() :
			InputBlob(NULL),
			OutputPath(NULL),
			InputLang(INVALID_SHADER_LANG),
			OutputLang(INVALID_SHADER_LANG),
			InputEntryPoint(NULL),
			OutputEntryPoint(NULL),
			ShaderType(INVALID_SHADER_TYPE)

		{

		}

		/*
		Default Destructor
		*/
		~BlobFileConvertOptions()
		{
		}

		/*
		A pointer to the blob of the input shader.
		If not filled in or NULL, an error is returned by the conversion function.
		*/
		Blob* InputBlob;

		/*
		The path of the output shader.
		If not filled in, the path is the same as the input path.
		*/
		const wchar* OutputPath;

		/*
		The shader input language.
		If not filled in, an error is returned by the conversion function.
		*/
		uint32 InputLang;

		/*
		The shader output language.
		If not filled in, an error is returned by the conversion function.
		*/
		uint32 OutputLang;

		/*
		The input shader entry point function name.
		If not filled in, "main" is assumed.
		May not have any effect for certain shader types.
		*/
		const char8* InputEntryPoint;

		/*
		The output shader entry point function name.
		If not filled in, "main" is assumed.
		May not have any effect for certain shader types.
		*/
		const char8* OutputEntryPoint;

		/*
		The type of shader we are converting.
		If not filled in, an error is returned by the conversion function.
		*/
		uint32 ShaderType;

	};
}

#endif