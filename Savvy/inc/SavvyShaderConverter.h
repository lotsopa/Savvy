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
#ifndef SAVVY_SHADER_CONVERTER_H
#define SAVVY_SHADER_CONVERTER_H

#include "SavvyConvertOptions.h"

namespace Savvy
{
	class Blob;
	class ScannerGLSL;
	class Parser;
	class Database;
	class Constructor;
	class Scanner;

	/*
	The Shader Converter class is the main access point for the user of the tool.
	It provides an interface for converting shaders from file or memory.
	The newly converted shader is written either to disk or returned as a blob to the caller.
	*/
	class ShaderConverter
	{
	public:
		ShaderConverter();
		~ShaderConverter();

		/*
		Converts a shader file from a specified path to a blob.
		*/
		ResultCode ConvertShaderFromFileToBlob(FileBlobConvertOptions& a_Options);

		/*
		Converts a shader file from a file to another file.
		*/
		ResultCode ConvertShaderFromFileToFile(FileConvertOptions& a_Options);

		/*
		Converts a shader from a blob to another blob
		*/
		ResultCode ConvertShaderFromBlobToBlob(BlobConvertOptions& a_Options);

		/*
		Registers a new custom shader type to the system.
		If a custom shader type is not registered and a conversion si called with it, the call will fail.
		*/
		ResultCode RegisterCustomShaderType(uint32 a_ID);

		/*
		Converts a shader from memory to file.
		*/
		ResultCode ConvertShaderFromBlobToFile(BlobFileConvertOptions& a_Options);

		/*
		Registers a new custom shader language for output purposes only.
		If the ID of this shader type is used as an input, the conversion will fail.
		*/
		template<typename ConstructorClass>
		ResultCode RegisterCustomOutputShaderLang(uint32 a_ID, const wchar* a_DefaultExtension)
		{
			if (OutputShaderLangRegistered(a_ID))
			{
				m_LastError = "Output shader language already registered.";
				return SAVVY_SHADER_LANG_ALREADY_EXISTS;
			}
			m_ConstructorClassMap[a_ID] = &CreateConstructorClassInstance <ConstructorClass>;
			m_OutputLanguageList.push_back(a_ID);
			m_Extensions[a_ID] = a_DefaultExtension;
			return SAVVY_OK;
		}

		/*
		Registers a new custom shader language for input purposes only.
		If the ID of this shader type is used as an output, the conversion will fail.
		*/
		template<typename ScannerClass, typename ParserClass, typename DatabaseClass>
		ResultCode RegisterCustomInputShaderLang(uint32 a_ID)
		{
			if (InputShaderLangRegistered(a_ID))
			{
				m_LastError = "Input shader language already registered.";
				return SAVVY_SHADER_LANG_ALREADY_EXISTS;
			}
			m_ScannerClassMap[a_ID] = &CreateScannerClassInstance <ScannerClass>;
			m_ParserClassMap[a_ID] = &CreateParserClassInstance <ParserClass>;
			m_DatabaseClassMap[a_ID] = &CreateDatabaseClassInstance <DatabaseClass>;
			m_InputLanguageList.push_back(a_ID);
			return SAVVY_OK;
		}

		/*
		Registers a new custom shader language for both input and output purposes.
		*/
		template<typename ScannerClass, typename ParserClass, typename DatabaseClass, typename ConstructorClass>
		ResultCode RegisterCustomShaderLang(uint32 a_ID, const wchar* a_DefaultExtension)
		{
			if (InputShaderLangRegistered(a_ID))
			{
				m_LastError = "Input shader language already registered.";
				return SAVVY_SHADER_LANG_ALREADY_EXISTS;
			}

			if (OutputShaderLangRegistered(a_ID))
			{
				m_LastError = "Output shader language already registered.";
				return SAVVY_SHADER_LANG_ALREADY_EXISTS;
			}

			m_ScannerClassMap[a_ID] = &CreateScannerClassInstance <ScannerClass>;
			m_ParserClassMap[a_ID] = &CreateParserClassInstance <ParserClass>;
			m_ConstructorClassMap[a_ID] = &CreateConstructorClassInstance <ConstructorClass>;
			m_DatabaseClassMap[a_ID] = &CreateDatabaseClassInstance <DatabaseClass>;
			m_InputLanguageList.push_back(a_ID);
			m_OutputLanguageList.push_back(a_ID);
			m_Extensions[a_ID] = a_DefaultExtension;
			return SAVVY_OK;
		}

		/*
		Returns a string with a verbose explanation of the last encountered error.
		If there is no error the returned string is empty. The string is never NULL.
		*/
		const char8* GetLastError();

	private:
		std::string m_LastError;
		typedef std::map<uint32, Parser*> ParserMap;
		typedef std::map<uint32, Scanner*> ScannerMap;
		typedef std::map<uint32, Database*> DatabaseMap;
		typedef std::map<uint32, std::wstring> ExtensionMap;
		typedef std::vector<uint32> ShaderTypeList;
		typedef std::vector<uint32> LangList;
		typedef std::map<uint32, Constructor*> ConstructorMap;
		typedef std::map<uint32, Scanner*(*)()> ScannerClassMap;
		typedef std::map<uint32, Parser*(*)()> ParserClassMap;
		typedef std::map<uint32, Constructor*(*)()> ConstructorClassMap;
		typedef std::map<uint32, Database*(*)()> DatabaseClassMap;
		ConstructorMap m_ConstructorMap;
		ParserMap m_Parsers;
		ScannerMap m_Scanners;
		DatabaseMap m_Databases;
		ExtensionMap m_Extensions;
		typedef std::vector<std::wstring> WideStringList;
		WideStringList m_ParsedIncludes;
		ShaderTypeList m_ShaderTypes;
		LangList m_InputLanguageList;
		LangList m_OutputLanguageList;
		ScannerClassMap m_ScannerClassMap;
		ParserClassMap m_ParserClassMap;
		ConstructorClassMap m_ConstructorClassMap;
		DatabaseClassMap m_DatabaseClassMap;

		const bool ShaderTypeRegistered(uint32 a_ID);
		const bool OutputShaderLangRegistered(uint32 a_ID);
		const bool InputShaderLangRegistered(uint32 a_ID);

		ResultCode ParseIncludes(FileConvertOptions& a_Options);
		ResultCode ParseIncludes(BlobConvertOptions& a_Options);
		ResultCode ParseIncludes(BlobFileConvertOptions& a_Options);
		ResultCode ParseIncludes(FileBlobConvertOptions& a_Options);

		void ResetConverters();

		template<typename T>
		const bool ElementInMapExists(T& a_Map, uint32 a_ID)
		{
			uint32 count = a_Map.count(a_ID);

			if (count > 0)
				return true;

			return false;
		}

		ResultCode ConstructTranslatedShader(std::ostream& a_OutputStream, std::string a_InputEntryPoint, std::string a_OutputEntryPoint, uint32 a_OutputLang, uint32 a_InputLang, uint32 a_ShaderType);

		// Static functions for creating the registered classes
		template<typename T> static Savvy::Scanner* CreateScannerClassInstance()
		{
			return new T;
		}

		template<typename T> static Savvy::Parser* CreateParserClassInstance()
		{
			return new T;
		}

		template<typename T> static Savvy::Constructor* CreateConstructorClassInstance()
		{
			return new T;
		}

		template<typename T> static Savvy::Database* CreateDatabaseClassInstance()
		{
			return new T;
		}
	};
}
#endif // !SAVVY_SHADER_CONVERTER_H
