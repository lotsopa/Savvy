#include "SavvyPCH.h"
#include "SavvyBlob.h"
#include "SavvyKey.h"
#include "SavvyShaderConverter.h"
#include "SavvyScanner.h"
#include "SavvyParser.h"
#include "SavvyDatabase.h"
#include "SavvyConstructor.h"
#include "SavvyException.h"

Savvy::ShaderConverter::ShaderConverter()
{

}

Savvy::ShaderConverter::~ShaderConverter()
{

	ScannerMap::iterator scannerIt;

	for (scannerIt = m_Scanners.begin(); scannerIt != m_Scanners.end(); ++scannerIt)
	{
		if (scannerIt->second != NULL)
			delete scannerIt->second;
	}

	ParserMap::iterator parserIt;

	for (parserIt = m_Parsers.begin(); parserIt != m_Parsers.end(); ++parserIt)
	{
		if (parserIt->second != NULL)
			delete parserIt->second;
	}

	ConstructorMap::iterator it;

	for (it = m_ConstructorMap.begin(); it != m_ConstructorMap.end(); ++it)
	{
		if (it->second != NULL)
		{
			delete it->second;
		}
	}

	DatabaseMap::iterator dbIt;

	for (dbIt = m_Databases.begin(); dbIt != m_Databases.end(); ++dbIt)
	{
		if (dbIt->second != NULL)
		{
			delete dbIt->second;
		}
	}
}

Savvy::ResultCode Savvy::ShaderConverter::ConvertShaderFromFileToBlob(FileBlobConvertOptions& a_Options)
{
#ifndef SAVVY_NO_EXCEPTIONS
	try
	{
#endif
		m_LastError.clear();

		if (!ShaderTypeRegistered(a_Options.ShaderType))
		{
			m_LastError = "Shader type not registered.";
#ifndef SAVVY_NO_EXCEPTIONS
			throw Exception(m_LastError);
#endif
			return SAVVY_INVALID_SHADER_TYPE;
		}

		if (!InputShaderLangRegistered(a_Options.InputLang))
		{
			m_LastError = "Shader language not registered.";
#ifndef SAVVY_NO_EXCEPTIONS
			throw Exception(m_LastError);
#endif
			return SAVVY_INVALID_SHADER_LANG;
		}
		else
		{
			// Check if we need to create the language's scanner, database and parser
			bool scannerExists = ElementInMapExists(m_Scanners, a_Options.InputLang);

			if (!scannerExists)
			{
				m_Scanners.insert(std::make_pair(a_Options.InputLang, m_ScannerClassMap[a_Options.InputLang]()));
			}

			bool parserExists = ElementInMapExists(m_Parsers, a_Options.InputLang);

			if (!parserExists)
			{
				m_Parsers.insert(std::make_pair(a_Options.InputLang, m_ParserClassMap[a_Options.InputLang]()));
			}

			bool databaseExists = ElementInMapExists(m_Databases, a_Options.InputLang);

			if (!databaseExists)
			{
				m_Databases.insert(std::make_pair(a_Options.InputLang, m_DatabaseClassMap[a_Options.InputLang]()));
			}
		}

		if (a_Options.InputPath == NULL)
		{
			m_LastError.assign("The input file path is NULL.");
#ifndef SAVVY_NO_EXCEPTIONS
			throw Exception(m_LastError);
#endif
			return SAVVY_INVALID_ARG;
		}

		if (a_Options.InputLang == INVALID_SHADER_LANG)
		{
			m_LastError.assign("The Input shader type is not assigned.");
#ifndef SAVVY_NO_EXCEPTIONS
			throw Exception(m_LastError);
#endif
			return SAVVY_INVALID_ARG;
		}

		if (a_Options.OutputLang == INVALID_SHADER_LANG)
		{
			m_LastError.assign("The Output shader type is not assigned.");
#ifndef SAVVY_NO_EXCEPTIONS
			throw Exception(m_LastError);
#endif
			return SAVVY_INVALID_ARG;
		}

		if (a_Options.OutputBlob == NULL)
		{
			m_LastError.assign("The Output Blob is NULL.");
#ifndef SAVVY_NO_EXCEPTIONS
			throw Exception(m_LastError);
#endif
			return SAVVY_INVALID_ARG;
		}

		if (a_Options.InputLang == a_Options.OutputLang)
		{
			std::ifstream is(a_Options.InputPath);
			if (!is.is_open())
			{
				m_LastError.assign("Error reading file");
#ifndef SAVVY_NO_EXCEPTIONS
				throw Exception(m_LastError);
#endif
				return SAVVY_INVALID_ARG;
			}
			std::string fileStr(static_cast<std::stringstream const&>(std::stringstream() << is.rdbuf()).str());
			is.close();

			// Create a blob with the loaded file in memory
			a_Options.OutputBlob->SetData(&fileStr[0], fileStr.size());
			return ResultCode::SAVVY_OK;
		}

		// Create an input stream from a file path
		std::ifstream inputStream;
		inputStream.open(a_Options.InputPath, std::ios::in);

		if (!inputStream.is_open())
		{
			// Failed to open file
			m_LastError.assign("The input file path ");
			m_LastError.append("is invalid.");
#ifndef SAVVY_NO_EXCEPTIONS
			throw Exception(m_LastError);
#endif
			return ResultCode::SAVVY_INVALID_FILE_PATH;
		}

		// Switch input stream in the lexical scanner
		m_Scanners[a_Options.InputLang]->SwitchInputStream(&inputStream);

		// Initialize Parser
		m_Parsers[a_Options.InputLang]->Initialize(m_Databases[a_Options.InputLang]);

		// Parse
		uint32 token = m_Scanners[a_Options.InputLang]->GetNextToken();
		ResultCode parseResult = SAVVY_OK;
		while (token != SAVVY_EOF)
		{
			if (token == SAVVY_UNKNOWN_TOKEN)
			{
				uint32 linenum = m_Scanners[a_Options.InputLang]->GetLineNumber();
				m_LastError = "Unrecognized token at line " + std::to_string(linenum) + ".";
#ifndef SAVVY_NO_EXCEPTIONS
				throw Exception(m_LastError);
#endif
				return ResultCode::SAVVY_UNRECOGNIZED_TOKEN;
			}
			else if (token == SAVVY_INCLUDE)
			{
				// Parse all includes
				ResultCode resCode = ParseIncludes(a_Options);

				if (resCode != SAVVY_OK)
				{
#ifndef SAVVY_NO_EXCEPTIONS
					throw Exception(m_LastError);
#endif
					return resCode;
				}

				// Switch input stream in the lexical scanner back to the main file
				m_Scanners[a_Options.InputLang]->SwitchInputStream(&inputStream);
				token = m_Scanners[a_Options.InputLang]->GetNextToken();
				continue;
			}
			std::string tokenName = m_Scanners[a_Options.InputLang]->GetLastTokenName();
			parseResult = m_Parsers[a_Options.InputLang]->ParseToken(token, tokenName);

			if (parseResult != SAVVY_OK)
			{
				std::string err;
				err.assign("Syntax error while parsing the token: " + tokenName + ".");
				err.append(m_Parsers[a_Options.InputLang]->GetLastError());
				m_LastError.assign(err.begin(), err.end());
				m_Parsers[a_Options.InputLang]->Reset();
				m_Databases[a_Options.InputLang]->Reset();
#ifndef SAVVY_NO_EXCEPTIONS
				throw Exception(m_LastError);
#endif
				return parseResult;
			}

			token = m_Scanners[a_Options.InputLang]->GetNextToken();
		}

		inputStream.close();

		// Determine the output stream
		std::ostringstream outputStream;

		std::string entryPoint;
		if (a_Options.InputEntryPoint == NULL)
		{
			entryPoint = "main";
		}
		else
		{
			entryPoint = a_Options.InputEntryPoint;
		}

		std::string entryPointOut;
		if (a_Options.OutputEntryPoint == NULL)
		{
			entryPointOut = "main";
		}
		else
		{
			entryPointOut = a_Options.OutputEntryPoint;
		}

		// Output some comments to let the user know this file has been generated
		outputStream << "/*" << std::endl;
		outputStream << "Shader generated by Savvy - The Smart Shader Cross Compiler." << std::endl;
		outputStream << "Author: Apostol Dadachev" << std::endl;
		outputStream << "Version: " << std::to_string(SAVVY_VERSION_MAJOR) << "." << std::to_string(SAVVY_VERSION_MINOR) << " "SAVVY_VERSION_EXTRA << std::endl;
		outputStream << "*/" << std::endl;
		outputStream << std::endl;

		ResultCode result = ConstructTranslatedShader(outputStream, entryPoint, entryPointOut, a_Options.OutputLang, a_Options.InputLang, a_Options.ShaderType);

		if (result != SAVVY_OK)
		{
			std::string err;
			err = "Error encountered while constructing output shader.";
			err.append(m_Parsers[a_Options.InputLang]->GetLastError());
			m_LastError.assign(err.begin(), err.end());
#ifndef SAVVY_NO_EXCEPTIONS
			throw Exception(m_LastError);
#endif
			return result;
		}

		m_Parsers[a_Options.InputLang]->Reset();
		m_Databases[a_Options.InputLang]->Reset();
		m_ParsedIncludes.clear();

		std::string& OutputString = outputStream.str();
		a_Options.OutputBlob->SetData(OutputString.c_str(), OutputString.size());
		outputStream.str(std::string());

#ifndef SAVVY_NO_EXCEPTIONS
	}
	catch (Exception& e)
	{
		std::cerr << "Savvy exception caught." << std::endl;
		std::cerr << e.what() << std::endl;
	}
	catch (std::exception& e)
	{
		std::cerr << "General C++ exception caught." << std::endl;
		std::cerr << e.what() << std::endl;
	}
#endif
	return ResultCode::SAVVY_OK;
}

Savvy::ResultCode Savvy::ShaderConverter::ConvertShaderFromFileToFile(FileConvertOptions& a_Options)
{
#ifndef SAVVY_NO_EXCEPTIONS
	try
	{
#endif
	m_LastError.clear();

	if (!ShaderTypeRegistered(a_Options.ShaderType))
	{
		m_LastError = "Shader type not registered.";
#ifndef SAVVY_NO_EXCEPTIONS
		throw Exception(m_LastError);
#endif
		return SAVVY_INVALID_SHADER_TYPE;
	}

	if (!InputShaderLangRegistered(a_Options.InputLang))
	{
		m_LastError = "Shader language not registered.";
#ifndef SAVVY_NO_EXCEPTIONS
		throw Exception(m_LastError);
#endif
		return SAVVY_INVALID_SHADER_LANG;
	}
	else
	{
		// Check if we need to create the language's scanner and parser
		bool scannerExists = ElementInMapExists(m_Scanners, a_Options.InputLang);

		if (!scannerExists)
		{
			m_Scanners.insert(std::make_pair(a_Options.InputLang, m_ScannerClassMap[a_Options.InputLang]()));
		}

		bool parserExists = ElementInMapExists(m_Parsers, a_Options.InputLang);

		if (!parserExists)
		{
			m_Parsers.insert(std::make_pair(a_Options.InputLang, m_ParserClassMap[a_Options.InputLang]()));
		}

		bool databaseExists = ElementInMapExists(m_Databases, a_Options.InputLang);

		if (!databaseExists)
		{
			m_Databases.insert(std::make_pair(a_Options.InputLang, m_DatabaseClassMap[a_Options.InputLang]()));
		}
	}

	if (a_Options.InputPath == NULL)
	{
		m_LastError.assign("The input file path is NULL.");
#ifndef SAVVY_NO_EXCEPTIONS
		throw Exception(m_LastError);
#endif
		return ResultCode::SAVVY_INVALID_ARG;
	}

	if (a_Options.InputLang == INVALID_SHADER_LANG)
	{
		m_LastError.assign("The Input shader type is not assigned.");
#ifndef SAVVY_NO_EXCEPTIONS
		throw Exception(m_LastError);
#endif
		return ResultCode::SAVVY_INVALID_ARG;
	}

	if (a_Options.OutputLang == INVALID_SHADER_LANG)
	{
		m_LastError.assign("The Output shader type is not assigned.");
#ifndef SAVVY_NO_EXCEPTIONS
		throw Exception(m_LastError);
#endif
		return ResultCode::SAVVY_INVALID_ARG;
	}

	if (a_Options.InputLang == a_Options.OutputLang)
	{
		// Output the same file
		std::ifstream stream1(a_Options.InputPath);

		std::wstring outputPath;
		if (a_Options.OutputPath == NULL)
		{
			outputPath = a_Options.InputPath;
			outputPath.append(m_Extensions[a_Options.OutputLang]);
		}
		else
		{
			outputPath = a_Options.OutputPath;
		}

		std::ofstream stream2(outputPath);

		if (!stream1.is_open() || !stream2.is_open())
		{
			m_LastError.assign("Could not open file streams.");
#ifndef SAVVY_NO_EXCEPTIONS
			throw Exception(m_LastError);
#endif
			return ResultCode::SAVVY_LOGICAL_ERROR;
		}
		stream2 << stream1.rdbuf();
		return ResultCode::SAVVY_OK;
	}

	// Create an input stream from a file path
	std::ifstream inputStream;
	inputStream.open(a_Options.InputPath, std::ios::in);

	if (!inputStream.is_open())
	{
		// Failed to open file
		m_LastError.assign("The input file path ");
		m_LastError.append("is invalid.");
#ifndef SAVVY_NO_EXCEPTIONS
		throw Exception(m_LastError);
#endif
		return ResultCode::SAVVY_INVALID_FILE_PATH;
	}

	// Switch input stream in the lexical scanner
	m_Scanners[a_Options.InputLang]->SwitchInputStream(&inputStream);

	// Initialize Parser
	m_Parsers[a_Options.InputLang]->Initialize(m_Databases[a_Options.InputLang]);

	// Parse
	uint32 token = m_Scanners[a_Options.InputLang]->GetNextToken();
	ResultCode parseResult;
	while (token != SAVVY_EOF)
	{
		if (token == SAVVY_UNKNOWN_TOKEN)
		{
			uint32 linenum = m_Scanners[a_Options.InputLang]->GetLineNumber();
			m_LastError = "Unrecognized token at line " + std::to_string(linenum) + ".";
#ifndef SAVVY_NO_EXCEPTIONS
			throw Exception(m_LastError);
#endif
			return ResultCode::SAVVY_UNRECOGNIZED_TOKEN;
		}
		else if (token == SAVVY_INCLUDE)
		{
			// Parse all includes
			ResultCode resCode = ParseIncludes(a_Options);

			if (resCode != SAVVY_OK)
			{
#ifndef SAVVY_NO_EXCEPTIONS
				throw Exception(m_LastError);
#endif
				return resCode;
			}

			// Switch input stream in the lexical scanner back to the main file
			m_Scanners[a_Options.InputLang]->SwitchInputStream(&inputStream);
			token = m_Scanners[a_Options.InputLang]->GetNextToken();
			continue;
		}
		std::string tokenName = m_Scanners[a_Options.InputLang]->GetLastTokenName();
		parseResult = m_Parsers[a_Options.InputLang]->ParseToken(token, tokenName);

		if (parseResult != SAVVY_OK)
		{
			std::string err;
			err.assign("Syntax error while parsing the token: " + tokenName + ".");
			err.append(m_Parsers[a_Options.InputLang]->GetLastError());
			m_LastError.assign(err.begin(), err.end());
			m_Parsers[a_Options.InputLang]->Reset();
			m_Databases[a_Options.InputLang]->Reset();
#ifndef SAVVY_NO_EXCEPTIONS
			throw Exception(m_LastError);
#endif
			return parseResult;
		}

		token = m_Scanners[a_Options.InputLang]->GetNextToken();
	}

	inputStream.close();

	// Determine the output stream
	std::ofstream outputStream;
	if (a_Options.OutputPath == NULL)
	{
		std::wstring path = a_Options.InputPath;
		path.append(m_Extensions[a_Options.OutputLang]);
		outputStream.open(path.c_str(), std::ios::out | std::ios::trunc);
	}
	else
	{
		outputStream.open(a_Options.OutputPath, std::ios::out | std::ios::trunc);
	}

	if (!outputStream.is_open())
	{
		m_LastError.assign("Could not open output file stream.");
#ifndef SAVVY_NO_EXCEPTIONS
		throw Exception(m_LastError);
#endif
		ResultCode::SAVVY_INVALID_FILE_PATH;
	}

	std::string entryPoint;
	if (a_Options.InputEntryPoint == NULL)
	{
		entryPoint = "main";
	}
	else
	{
		entryPoint = a_Options.InputEntryPoint;
	}

	std::string entryPointOut;
	if (a_Options.OutputEntryPoint == NULL)
	{
		entryPointOut = "main";
	}
	else
	{
		entryPointOut = a_Options.OutputEntryPoint;
	}

	// Output some comments to let the user know this file has been generated
	outputStream << "/*" << std::endl;
	outputStream << "Shader generated by Savvy - The Smart Shader Cross Compiler." << std::endl;
	outputStream << "Author: Apostol Dadachev" << std::endl;
	outputStream << "Version: " << std::to_string(SAVVY_VERSION_MAJOR) << "." << std::to_string(SAVVY_VERSION_MINOR) << " "SAVVY_VERSION_EXTRA << std::endl;
	outputStream << "*/" << std::endl;
	outputStream << std::endl;

	ResultCode result = ConstructTranslatedShader(outputStream, entryPoint, entryPointOut, a_Options.OutputLang, a_Options.InputLang, a_Options.ShaderType);
	outputStream.close();

	if (result != SAVVY_OK)
	{
		std::string err;
		err = "Error encountered while constructing output shader.";
		err.append(m_Parsers[a_Options.InputLang]->GetLastError());
		m_LastError.assign(err.begin(), err.end());
#ifndef SAVVY_NO_EXCEPTIONS
		throw Exception(m_LastError);
#endif
		return result;
	}

	m_Parsers[a_Options.InputLang]->Reset();
	m_Databases[a_Options.InputLang]->Reset();
	m_ParsedIncludes.clear();
#ifndef SAVVY_NO_EXCEPTIONS
	}
	catch (Exception& e)
	{
		std::cerr << "Savvy exception caught." << std::endl;
		std::cerr << e.what() << std::endl;
	}
	catch (std::exception& e)
	{
		std::cerr << "General C++ exception caught." << std::endl;
		std::cerr << e.what() << std::endl;
	}
#endif
	return ResultCode::SAVVY_OK;
}

Savvy::ResultCode Savvy::ShaderConverter::ConvertShaderFromBlobToBlob(BlobConvertOptions& a_Options)
{
#ifndef SAVVY_NO_EXCEPTIONS
	try
	{
#endif
	m_LastError.clear();

	if (!ShaderTypeRegistered(a_Options.ShaderType))
	{
		m_LastError = "Shader type not registered.";
#ifndef SAVVY_NO_EXCEPTIONS
		throw Exception(m_LastError);
#endif
		return SAVVY_INVALID_SHADER_TYPE;
	}

	if (!InputShaderLangRegistered(a_Options.InputLang))
	{
		m_LastError = "Shader language not registered.";
#ifndef SAVVY_NO_EXCEPTIONS
		throw Exception(m_LastError);
#endif
		return SAVVY_INVALID_SHADER_LANG;
	}
	else
	{
		// Check if we need to create the language's scanner and parser
		bool scannerExists = ElementInMapExists(m_Scanners, a_Options.InputLang);

		if (!scannerExists)
		{
			m_Scanners.insert(std::make_pair(a_Options.InputLang, m_ScannerClassMap[a_Options.InputLang]()));
		}

		bool parserExists = ElementInMapExists(m_Parsers, a_Options.InputLang);

		if (!parserExists)
		{
			m_Parsers.insert(std::make_pair(a_Options.InputLang, m_ParserClassMap[a_Options.InputLang]()));
		}

		bool databaseExists = ElementInMapExists(m_Databases, a_Options.InputLang);

		if (!databaseExists)
		{
			m_Databases.insert(std::make_pair(a_Options.InputLang, m_DatabaseClassMap[a_Options.InputLang]()));
		}
	}

	if (a_Options.InputBlob == NULL)
	{
		m_LastError.assign("Input Blob is NULL.");
#ifndef SAVVY_NO_EXCEPTIONS
		throw Exception(m_LastError);
#endif
		return SAVVY_INVALID_ARG;
	}

	if (a_Options.OutputBlob == NULL)
	{
		m_LastError.assign("Output Blob is NULL.");
#ifndef SAVVY_NO_EXCEPTIONS
		throw Exception(m_LastError);
#endif
		return SAVVY_INVALID_ARG;
	}

	if (a_Options.InputBlob->GetRawDataPtr() == NULL)
	{
		m_LastError.assign("Output Blob Data is NULL.");
#ifndef SAVVY_NO_EXCEPTIONS
		throw Exception(m_LastError);
#endif
		return SAVVY_INVALID_ARG;
	}

	if (a_Options.InputBlob->GetDataSize() == NULL)
	{
		m_LastError.assign("Output Blob Data Size is NULL.");
#ifndef SAVVY_NO_EXCEPTIONS
		throw Exception(m_LastError);
#endif
		return SAVVY_INVALID_ARG;
	}

	if (a_Options.InputLang == INVALID_SHADER_LANG)
	{
		m_LastError.assign("The Input shader type is not assigned.");
#ifndef SAVVY_NO_EXCEPTIONS
		throw Exception(m_LastError);
#endif
		return SAVVY_INVALID_ARG;
	}

	if (a_Options.OutputLang == INVALID_SHADER_LANG)
	{
		m_LastError.assign("The Output shader type is not assigned.");
#ifndef SAVVY_NO_EXCEPTIONS
		throw Exception(m_LastError);
#endif
		return SAVVY_INVALID_ARG;
	}

	if (a_Options.InputLang == a_Options.OutputLang)
	{
		a_Options.OutputBlob->SetData(*a_Options.InputBlob);
		return SAVVY_OK;
	}

	std::string inputString(a_Options.InputBlob->GetRawDataPtr(), a_Options.InputBlob->GetDataSize());
	std::istringstream inputStream;
	inputStream.str(inputString);
	inputString.clear();
	if (!inputStream)
	{
		m_LastError.assign("Could not open input stream.");
#ifndef SAVVY_NO_EXCEPTIONS
		throw Exception(m_LastError);
#endif
		return SAVVY_UNKNOWN_ERROR;
	}

	// Switch input stream in the lexical scanner
	m_Scanners[a_Options.InputLang]->SwitchInputStream(&inputStream);

	// Initialize Parser
	m_Parsers[a_Options.InputLang]->Initialize(m_Databases[a_Options.InputLang]);

	// Parse
	uint32 token = m_Scanners[a_Options.InputLang]->GetNextToken();
	ResultCode parseResult = SAVVY_OK;
	while (token != SAVVY_EOF)
	{
		if (token == SAVVY_UNKNOWN_TOKEN)
		{
			uint32 linenum = m_Scanners[a_Options.InputLang]->GetLineNumber();
			m_LastError = "Unrecognized token at line " + std::to_string(linenum) + ".";
#ifndef SAVVY_NO_EXCEPTIONS
			throw Exception(m_LastError);
#endif
			return ResultCode::SAVVY_UNRECOGNIZED_TOKEN;
		}
		else if (token == SAVVY_INCLUDE)
		{
			// Parse all includes
			ResultCode resCode = ParseIncludes(a_Options);

			if (resCode != SAVVY_OK)
				return resCode;

			// Switch input stream in the lexical scanner back to the main file
			m_Scanners[a_Options.InputLang]->SwitchInputStream(&inputStream);
			token = m_Scanners[a_Options.InputLang]->GetNextToken();
			continue;
		}
		std::string tokenName = m_Scanners[a_Options.InputLang]->GetLastTokenName();
		parseResult = m_Parsers[a_Options.InputLang]->ParseToken(token, tokenName);

		if (parseResult != SAVVY_OK)
		{
			std::string err;
			err.assign("Syntax error while parsing the token: " + tokenName + ".");
			err.append(m_Parsers[a_Options.InputLang]->GetLastError());
			m_LastError.assign(err.begin(), err.end());
			m_Parsers[a_Options.InputLang]->Reset();
			m_Databases[a_Options.InputLang]->Reset();
#ifndef SAVVY_NO_EXCEPTIONS
			throw Exception(m_LastError);
#endif
			return parseResult;
		}

		token = m_Scanners[a_Options.InputLang]->GetNextToken();
	}

	// Construct
	inputStream.str(std::string());
	std::ostringstream outputStream;

	std::string entryPoint;
	if (a_Options.InputEntryPoint == NULL)
	{
		entryPoint = "main";
	}
	else
	{
		entryPoint = a_Options.InputEntryPoint;
	}

	std::string entryPointOut;
	if (a_Options.OutputEntryPoint == NULL)
	{
		entryPointOut = "main";
	}
	else
	{
		entryPointOut = a_Options.OutputEntryPoint;
	}

	// Output some comments to let the user know this file has been generated
	outputStream << "/*" << std::endl;
	outputStream << "Shader generated by Savvy - The Smart Shader Cross Compiler." << std::endl;
	outputStream << "Author: Apostol Dadachev" << std::endl;
	outputStream << "Version: " << std::to_string(SAVVY_VERSION_MAJOR) << "." << std::to_string(SAVVY_VERSION_MINOR) << " "SAVVY_VERSION_EXTRA << std::endl;
	outputStream << "*/" << std::endl;
	outputStream << std::endl;

	ResultCode result = ConstructTranslatedShader(outputStream, entryPoint, entryPointOut, a_Options.OutputLang, a_Options.InputLang, a_Options.ShaderType);

	if (result != SAVVY_OK)
	{
		std::string err;
		err = "Error encountered while constructing output shader.";
		err.append(m_Parsers[a_Options.InputLang]->GetLastError());
		m_LastError.assign(err.begin(), err.end());
#ifndef SAVVY_NO_EXCEPTIONS
		throw Exception(m_LastError);
#endif
		return result;
	}

	m_Parsers[a_Options.InputLang]->Reset();
	m_Databases[a_Options.InputLang]->Reset();
	m_ParsedIncludes.clear();

	std::string& OutputString = outputStream.str();
	a_Options.OutputBlob->SetData(OutputString.c_str(), OutputString.size());
	outputStream.str(std::string());
#ifndef SAVVY_NO_EXCEPTIONS
	}
	catch (Exception& e)
	{
		std::cerr << "Savvy exception caught." << std::endl;
		std::cerr << e.what() << std::endl;
	}
	catch (std::exception& e)
	{
		std::cerr << "General C++ exception caught." << std::endl;
		std::cerr << e.what() << std::endl;
	}
#endif
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::ShaderConverter::ConvertShaderFromBlobToFile(BlobFileConvertOptions& a_Options)
{
#ifndef SAVVY_NO_EXCEPTIONS
	try
	{
#endif
	m_LastError.clear();

	if (!ShaderTypeRegistered(a_Options.ShaderType))
	{
		m_LastError = "Shader type not registered.";
#ifndef SAVVY_NO_EXCEPTIONS
		throw Exception(m_LastError);
#endif
		return SAVVY_INVALID_SHADER_TYPE;
	}

	if (!InputShaderLangRegistered(a_Options.InputLang))
	{
		m_LastError = "Shader language not registered.";
#ifndef SAVVY_NO_EXCEPTIONS
		throw Exception(m_LastError);
#endif
		return SAVVY_INVALID_SHADER_LANG;
	}
	else
	{
		// Check if we need to create the language's scanner and parser
		bool scannerExists = ElementInMapExists(m_Scanners, a_Options.InputLang);

		if (!scannerExists)
		{
			m_Scanners.insert(std::make_pair(a_Options.InputLang, m_ScannerClassMap[a_Options.InputLang]()));
		}

		bool parserExists = ElementInMapExists(m_Parsers, a_Options.InputLang);

		if (!parserExists)
		{
			m_Parsers.insert(std::make_pair(a_Options.InputLang, m_ParserClassMap[a_Options.InputLang]()));
		}

		bool databaseExists = ElementInMapExists(m_Databases, a_Options.InputLang);

		if (!databaseExists)
		{
			m_Databases.insert(std::make_pair(a_Options.InputLang, m_DatabaseClassMap[a_Options.InputLang]()));
		}
	}

	if (a_Options.InputBlob == NULL)
	{
		m_LastError.assign("Input Blob is NULL.");
#ifndef SAVVY_NO_EXCEPTIONS
		throw Exception(m_LastError);
#endif
		return SAVVY_INVALID_ARG;
	}

	if (a_Options.OutputPath == NULL)
	{
		m_LastError.assign("Output File is NULL.");
#ifndef SAVVY_NO_EXCEPTIONS
		throw Exception(m_LastError);
#endif
		return SAVVY_INVALID_ARG;
	}

	if (a_Options.InputBlob->GetRawDataPtr() == NULL)
	{
		m_LastError.assign("Output Blob Data is NULL.");
#ifndef SAVVY_NO_EXCEPTIONS
		throw Exception(m_LastError);
#endif
		return SAVVY_INVALID_ARG;
	}

	if (a_Options.InputBlob->GetDataSize() == NULL)
	{
		m_LastError.assign("Output Blob Data Size is NULL.");
#ifndef SAVVY_NO_EXCEPTIONS
		throw Exception(m_LastError);
#endif
		return SAVVY_INVALID_ARG;
	}

	if (a_Options.InputLang == INVALID_SHADER_LANG)
	{
		m_LastError.assign("The Input shader type is not assigned.");
#ifndef SAVVY_NO_EXCEPTIONS
		throw Exception(m_LastError);
#endif
		return SAVVY_INVALID_ARG;
	}

	if (a_Options.OutputLang == INVALID_SHADER_LANG)
	{
		m_LastError.assign("The Output shader type is not assigned.");
#ifndef SAVVY_NO_EXCEPTIONS
		throw Exception(m_LastError);
#endif
		return SAVVY_INVALID_ARG;
	}

	if (a_Options.InputLang == a_Options.OutputLang)
	{
		std::ofstream out(a_Options.OutputPath);

		if (!out.is_open())
		{
			m_LastError.assign("Could not open output path.");
#ifndef SAVVY_NO_EXCEPTIONS
			throw Exception(m_LastError);
#endif
			return SAVVY_INVALID_ARG;
		}
		std::string outString(a_Options.InputBlob->GetRawDataPtr(), a_Options.InputBlob->GetDataSize());
		out << outString;
		out.close();
		outString.clear();
		return SAVVY_OK;
	}

	std::string inputString(a_Options.InputBlob->GetRawDataPtr(), a_Options.InputBlob->GetDataSize());
	std::istringstream inputStream;
	inputStream.str(inputString);
	inputString.clear();
	if (!inputStream)
	{
		m_LastError.assign("Could not open input stream.");
#ifndef SAVVY_NO_EXCEPTIONS
		throw Exception(m_LastError);
#endif
		return SAVVY_UNKNOWN_ERROR;
	}

	// Switch input stream in the lexical scanner
	m_Scanners[a_Options.InputLang]->SwitchInputStream(&inputStream);

	// Initialize Parser
	m_Parsers[a_Options.InputLang]->Initialize(m_Databases[a_Options.InputLang]);

	// Parse
	uint32 token = m_Scanners[a_Options.InputLang]->GetNextToken();
	ResultCode parseResult = SAVVY_OK;
	while (token != SAVVY_EOF)
	{
		if (token == SAVVY_UNKNOWN_TOKEN)
		{
			uint32 linenum = m_Scanners[a_Options.InputLang]->GetLineNumber();
			m_LastError = "Unrecognized token at line " + std::to_string(linenum) + ".";
#ifndef SAVVY_NO_EXCEPTIONS
			throw Exception(m_LastError);
#endif
			return ResultCode::SAVVY_UNRECOGNIZED_TOKEN;
		}
		else if (token == SAVVY_INCLUDE)
		{
			// Parse all includes
			ResultCode resCode = ParseIncludes(a_Options);

			if (resCode != SAVVY_OK)
				return resCode;

			// Switch input stream in the lexical scanner back to the main file
			m_Scanners[a_Options.InputLang]->SwitchInputStream(&inputStream);
			token = m_Scanners[a_Options.InputLang]->GetNextToken();
			continue;
		}
		std::string tokenName = m_Scanners[a_Options.InputLang]->GetLastTokenName();
		parseResult = m_Parsers[a_Options.InputLang]->ParseToken(token, tokenName);

		if (parseResult != SAVVY_OK)
		{
			std::string err;
			err.assign("Syntax error while parsing the token: " + tokenName + ".");
			err.append(m_Parsers[a_Options.InputLang]->GetLastError());
			m_LastError.assign(err.begin(), err.end());
			m_Parsers[a_Options.InputLang]->Reset();
			m_Databases[a_Options.InputLang]->Reset();
			return parseResult;
		}

		token = m_Scanners[a_Options.InputLang]->GetNextToken();
	}

	// Construct
	inputStream.str(std::string());

	// Determine the output stream
	std::ofstream outputStream;
	outputStream.open(a_Options.OutputPath, std::ios::out | std::ios::trunc);

	if (!outputStream.is_open())
	{
		m_LastError.assign("Could not open output file stream.");
#ifndef SAVVY_NO_EXCEPTIONS
		throw Exception(m_LastError);
#endif
		ResultCode::SAVVY_INVALID_FILE_PATH;
	}

	std::string entryPoint;
	if (a_Options.InputEntryPoint == NULL)
	{
		entryPoint = "main";
	}
	else
	{
		entryPoint = a_Options.InputEntryPoint;
	}

	std::string entryPointOut;
	if (a_Options.OutputEntryPoint == NULL)
	{
		entryPointOut = "main";
	}
	else
	{
		entryPointOut = a_Options.OutputEntryPoint;
	}

	// Output some comments to let the user know this file has been generated
	outputStream << "/*" << std::endl;
	outputStream << "Shader generated by Savvy - The Smart Shader Cross Compiler." << std::endl;
	outputStream << "Author: Apostol Dadachev" << std::endl;
	outputStream << "Version: " << std::to_string(SAVVY_VERSION_MAJOR) << "." << std::to_string(SAVVY_VERSION_MINOR) << " "SAVVY_VERSION_EXTRA << std::endl;
	outputStream << "*/" << std::endl;
	outputStream << std::endl;

	ResultCode result = ConstructTranslatedShader(outputStream, entryPoint, entryPointOut, a_Options.OutputLang, a_Options.InputLang, a_Options.ShaderType);

	if (result != SAVVY_OK)
	{
		std::string err;
		err = "Error encountered while constructing output shader.";
		err.append(m_Parsers[a_Options.InputLang]->GetLastError());
		m_LastError.assign(err.begin(), err.end());
#ifndef SAVVY_NO_EXCEPTIONS
		throw Exception(m_LastError);
#endif
		return result;
	}

	m_Parsers[a_Options.InputLang]->Reset();
	m_Databases[a_Options.InputLang]->Reset();
	m_ParsedIncludes.clear();
#ifndef SAVVY_NO_EXCEPTIONS
	}
	catch (Exception& e)
	{
		std::cerr << "Savvy exception caught." << std::endl;
		std::cerr << e.what() << std::endl;
	}
	catch (std::exception& e)
	{
		std::cerr << "General C++ exception caught." << std::endl;
		std::cerr << e.what() << std::endl;
	}
#endif
	return SAVVY_OK;
}

const Savvy::char8* Savvy::ShaderConverter::GetLastError()
{
	return m_LastError.c_str();
}

Savvy::ResultCode Savvy::ShaderConverter::ParseIncludes(FileConvertOptions& a_Options)
{
	static uint32 recursionDepth = 0;
	// Get the string
	uint32 token = m_Scanners[a_Options.InputLang]->GetNextToken();

	// Check if it's legit
	if (token != SAVVY_STRING)
	{
		uint32 linenum = m_Scanners[a_Options.InputLang]->GetLineNumber();
		m_LastError = "Could not read include file string at line " + std::to_string(linenum) + ".";
		return SAVVY_SHADER_SYNTAX_ERROR;
	}

	std::string tempname = m_Scanners[a_Options.InputLang]->GetLastTokenName();
	std::wstring includeName;
	includeName.assign(tempname.begin(), tempname.end());

	// Format it correctly
	includeName = includeName.substr(1, includeName.size() - 2);

	// Check if we haven't already parsed this one
	WideStringList::iterator it;
	for (it = m_ParsedIncludes.begin(); it != m_ParsedIncludes.end(); ++it)
	{
		if ((*it) == includeName)
			return SAVVY_OK;
	}
	m_ParsedIncludes.push_back(includeName);

	// Create an input stream from a file path
	std::ifstream inputStream;
	inputStream.open(includeName, std::ios::in);

	if (!inputStream.is_open())
	{
		// Try by merging the path of the current shader
		std::wstring path(a_Options.InputPath);
		uint32 lastDir = path.rfind(L"\\");

		if (lastDir != std::wstring::npos)
		{
			std::wstring newPath = path.substr(0, lastDir + 1) + includeName;
			inputStream.open(newPath, std::ios::in);

			if (!inputStream.is_open())
			{
				// Failed to open file
				m_LastError.assign("The input file path");
				m_LastError.append(" is invalid.");
				return SAVVY_INVALID_FILE_PATH;
			}
		}
		else
		{
			// Failed to open file
			m_LastError.assign("The input file path");
			m_LastError.append(" is invalid.");
			return SAVVY_INVALID_FILE_PATH;
		}
	}

	// Switch input stream in the lexical scanner
	m_Scanners[a_Options.InputLang]->SwitchInputStream(&inputStream);

	// Parse
	token = m_Scanners[a_Options.InputLang]->GetNextToken();
	ResultCode parseResult;
	while (token != SAVVY_EOF)
	{
		if (token == SAVVY_UNKNOWN_TOKEN)
		{
			uint32 linenum = m_Scanners[a_Options.InputLang]->GetLineNumber();
			m_LastError.assign("Error while parsing include file.");
			m_LastError.append("Unrecognized token at line " + std::to_string(linenum) + ".");
			return ResultCode::SAVVY_UNRECOGNIZED_TOKEN;
		}
		else if (token == SAVVY_INCLUDE)
		{
			if (recursionDepth > SAVVY_MAX_INCLUDE_RECURSION_DEPTH)
			{
				m_LastError.assign("Error while parsing include file.");
				m_LastError.append("Maximum recursion depth exceeded.");
				return SAVVY_MAX_RECURSION_DEPTH_EXCEEDED;
			}

			// Parse all includes
			ResultCode resCode = ParseIncludes(a_Options);

			if (resCode != SAVVY_OK)
				return resCode;

			// Switch input stream in the lexical scanner back to the main file
			m_Scanners[a_Options.InputLang]->SwitchInputStream(&inputStream);
			token = m_Scanners[a_Options.InputLang]->GetNextToken();
			continue;
		}
		std::string tokenName = m_Scanners[a_Options.InputLang]->GetLastTokenName();
		parseResult = m_Parsers[a_Options.InputLang]->ParseToken(token, tokenName);

		if (parseResult != SAVVY_OK)
		{
			std::string err;
			err.assign("Syntax error while parsing the token: " + tokenName + ".");
			err.append(m_Parsers[a_Options.InputLang]->GetLastError());
			m_LastError.assign(err.begin(), err.end());
			m_Parsers[a_Options.InputLang]->Reset();
			m_Databases[a_Options.InputLang]->Reset();
			return parseResult;
		}

		token = m_Scanners[a_Options.InputLang]->GetNextToken();
	}

	inputStream.close();
	recursionDepth++;
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::ShaderConverter::ParseIncludes(BlobConvertOptions& a_Options)
{
	static uint32 recursionDepth = 0;
	// Get the string
	uint32 token = m_Scanners[a_Options.InputLang]->GetNextToken();

	// Check if it's legit
	if (token != SAVVY_STRING)
	{
		uint32 linenum = m_Scanners[a_Options.InputLang]->GetLineNumber();
		m_LastError = "Could not read include file string at line " + std::to_string(linenum) + ".";
		return SAVVY_SHADER_SYNTAX_ERROR;
	}

	std::string tempname = m_Scanners[a_Options.InputLang]->GetLastTokenName();
	std::wstring includeName;
	includeName.assign(tempname.begin(), tempname.end());

	// Format it correctly
	includeName = includeName.substr(1, includeName.size() - 2);

	// Check if we haven't already parsed this one
	WideStringList::iterator it;
	for (it = m_ParsedIncludes.begin(); it != m_ParsedIncludes.end(); ++it)
	{
		if ((*it) == includeName)
			return SAVVY_OK;
	}
	m_ParsedIncludes.push_back(includeName);

	// Create an input stream from a file path
	std::ifstream inputStream;
	inputStream.open(includeName, std::ios::in);

	if (!inputStream.is_open())
	{
		// Failed to open file
		m_LastError.assign("Failed to Open Include File inside shader");
		return SAVVY_INVALID_FILE_PATH;
	}

	// Switch input stream in the lexical scanner
	m_Scanners[a_Options.InputLang]->SwitchInputStream(&inputStream);

	// Parse
	token = m_Scanners[a_Options.InputLang]->GetNextToken();
	ResultCode parseResult;
	while (token != SAVVY_EOF)
	{
		if (token == SAVVY_UNKNOWN_TOKEN)
		{
			uint32 linenum = m_Scanners[a_Options.InputLang]->GetLineNumber();
			m_LastError.assign("Error while parsing include file.");
			m_LastError.append("Unrecognized token at line " + std::to_string(linenum) + ".");
			return ResultCode::SAVVY_UNRECOGNIZED_TOKEN;
		}
		else if (token == SAVVY_INCLUDE)
		{
			if (recursionDepth > SAVVY_MAX_INCLUDE_RECURSION_DEPTH)
			{
				m_LastError.assign("Error while parsing include file.");
				m_LastError.append("Maximum recursion depth exceeded.");
				return SAVVY_MAX_RECURSION_DEPTH_EXCEEDED;
			}

			// Parse all includes
			ResultCode resCode = ParseIncludes(a_Options);

			if (resCode != SAVVY_OK)
				return resCode;

			// Switch input stream in the lexical scanner back to the main file
			m_Scanners[a_Options.InputLang]->SwitchInputStream(&inputStream);
			token = m_Scanners[a_Options.InputLang]->GetNextToken();
			continue;
		}
		std::string tokenName = m_Scanners[a_Options.InputLang]->GetLastTokenName();
		parseResult = m_Parsers[a_Options.InputLang]->ParseToken(token, tokenName);

		if (parseResult != SAVVY_OK)
		{
			std::string err;
			err.assign("Syntax error while parsing the token: " + tokenName + ".");
			m_LastError.assign(err.begin(), err.end());
			m_Parsers[a_Options.InputLang]->Reset();
			m_Databases[a_Options.InputLang]->Reset();
			return parseResult;
		}

		token = m_Scanners[a_Options.InputLang]->GetNextToken();
	}

	inputStream.close();
	recursionDepth++;
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::ShaderConverter::ParseIncludes(BlobFileConvertOptions& a_Options)
{
	static uint32 recursionDepth = 0;
	// Get the string
	uint32 token = m_Scanners[a_Options.InputLang]->GetNextToken();

	// Check if it's legit
	if (token != SAVVY_STRING)
	{
		uint32 linenum = m_Scanners[a_Options.InputLang]->GetLineNumber();
		m_LastError = "Could not read include file string at line " + std::to_string(linenum) + ".";
		return SAVVY_SHADER_SYNTAX_ERROR;
	}

	std::string tempname = m_Scanners[a_Options.InputLang]->GetLastTokenName();
	std::wstring includeName;
	includeName.assign(tempname.begin(), tempname.end());

	// Format it correctly
	includeName = includeName.substr(1, includeName.size() - 2);

	// Check if we haven't already parsed this one
	WideStringList::iterator it;
	for (it = m_ParsedIncludes.begin(); it != m_ParsedIncludes.end(); ++it)
	{
		if ((*it) == includeName)
			return SAVVY_OK;
	}
	m_ParsedIncludes.push_back(includeName);

	// Create an input stream from a file path
	std::ifstream inputStream;
	inputStream.open(includeName, std::ios::in);

	if (!inputStream.is_open())
	{
		// Failed to open file
		m_LastError.assign("Failed to Open Include File inside shader");
		return SAVVY_INVALID_FILE_PATH;
	}

	// Switch input stream in the lexical scanner
	m_Scanners[a_Options.InputLang]->SwitchInputStream(&inputStream);

	// Parse
	token = m_Scanners[a_Options.InputLang]->GetNextToken();
	ResultCode parseResult;
	while (token != SAVVY_EOF)
	{
		if (token == SAVVY_UNKNOWN_TOKEN)
		{
			uint32 linenum = m_Scanners[a_Options.InputLang]->GetLineNumber();
			m_LastError.assign("Error while parsing include file.");
			m_LastError.append("Unrecognized token at line " + std::to_string(linenum) + ".");
			return ResultCode::SAVVY_UNRECOGNIZED_TOKEN;
		}
		else if (token == SAVVY_INCLUDE)
		{
			if (recursionDepth > SAVVY_MAX_INCLUDE_RECURSION_DEPTH)
			{
				m_LastError.assign("Error while parsing include file.");
				m_LastError.append("Maximum recursion depth exceeded.");
				return SAVVY_MAX_RECURSION_DEPTH_EXCEEDED;
			}

			// Parse all includes
			ResultCode resCode = ParseIncludes(a_Options);

			if (resCode != SAVVY_OK)
				return resCode;

			// Switch input stream in the lexical scanner back to the main file
			m_Scanners[a_Options.InputLang]->SwitchInputStream(&inputStream);
			token = m_Scanners[a_Options.InputLang]->GetNextToken();
			continue;
		}
		std::string tokenName = m_Scanners[a_Options.InputLang]->GetLastTokenName();
		parseResult = m_Parsers[a_Options.InputLang]->ParseToken(token, tokenName);

		if (parseResult != SAVVY_OK)
		{
			std::string err;
			err.assign("Syntax error while parsing the token: " + tokenName + ".");
			m_LastError.assign(err.begin(), err.end());
			m_Parsers[a_Options.InputLang]->Reset();
			m_Databases[a_Options.InputLang]->Reset();
			return parseResult;
		}

		token = m_Scanners[a_Options.InputLang]->GetNextToken();
	}

	inputStream.close();
	recursionDepth++;
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::ShaderConverter::ParseIncludes(FileBlobConvertOptions& a_Options)
{
	static uint32 recursionDepth = 0;
	// Get the string
	uint32 token = m_Scanners[a_Options.InputLang]->GetNextToken();

	// Check if it's legit
	if (token != SAVVY_STRING)
	{
		uint32 linenum = m_Scanners[a_Options.InputLang]->GetLineNumber();
		m_LastError = "Could not read include file string at line " + std::to_string(linenum) + ".";
		return SAVVY_SHADER_SYNTAX_ERROR;
	}

	std::string tempname = m_Scanners[a_Options.InputLang]->GetLastTokenName();
	std::wstring includeName;
	includeName.assign(tempname.begin(), tempname.end());

	// Format it correctly
	includeName = includeName.substr(1, includeName.size() - 2);

	// Check if we haven't already parsed this one
	WideStringList::iterator it;
	for (it = m_ParsedIncludes.begin(); it != m_ParsedIncludes.end(); ++it)
	{
		if ((*it) == includeName)
			return SAVVY_OK;
	}
	m_ParsedIncludes.push_back(includeName);

	// Create an input stream from a file path
	std::ifstream inputStream;
	inputStream.open(includeName, std::ios::in);

	if (!inputStream.is_open())
	{
		// Try by merging the path of the current shader
		std::wstring path(a_Options.InputPath);
		uint32 lastDir = path.rfind(L"\\");

		if (lastDir != std::wstring::npos)
		{
			std::wstring newPath = path.substr(0, lastDir + 1) + includeName;
			inputStream.open(newPath, std::ios::in);

			if (!inputStream.is_open())
			{
				// Failed to open file
				m_LastError.assign("The input file path");
				m_LastError.append(" is invalid.");
				return SAVVY_INVALID_FILE_PATH;
			}
		}
		else
		{
			// Failed to open file
			m_LastError.assign("The input file path");
			m_LastError.append(" is invalid.");
			return SAVVY_INVALID_FILE_PATH;
		}
	}

	// Switch input stream in the lexical scanner
	m_Scanners[a_Options.InputLang]->SwitchInputStream(&inputStream);

	// Parse
	token = m_Scanners[a_Options.InputLang]->GetNextToken();
	ResultCode parseResult;
	while (token != SAVVY_EOF)
	{
		if (token == SAVVY_UNKNOWN_TOKEN)
		{
			uint32 linenum = m_Scanners[a_Options.InputLang]->GetLineNumber();
			m_LastError.assign("Error while parsing include file.");
			m_LastError.append("Unrecognized token at line " + std::to_string(linenum) + ".");
			return ResultCode::SAVVY_UNRECOGNIZED_TOKEN;
		}
		else if (token == SAVVY_INCLUDE)
		{
			if (recursionDepth > SAVVY_MAX_INCLUDE_RECURSION_DEPTH)
			{
				m_LastError.assign("Error while parsing include file.");
				m_LastError.append("Maximum recursion depth exceeded.");
				return SAVVY_MAX_RECURSION_DEPTH_EXCEEDED;
			}

			// Parse all includes
			ResultCode resCode = ParseIncludes(a_Options);

			if (resCode != SAVVY_OK)
				return resCode;

			// Switch input stream in the lexical scanner back to the main file
			m_Scanners[a_Options.InputLang]->SwitchInputStream(&inputStream);
			token = m_Scanners[a_Options.InputLang]->GetNextToken();
			continue;
		}
		std::string tokenName = m_Scanners[a_Options.InputLang]->GetLastTokenName();
		parseResult = m_Parsers[a_Options.InputLang]->ParseToken(token, tokenName);

		if (parseResult != SAVVY_OK)
		{
			std::string err;
			err.assign("Syntax error while parsing the token: " + tokenName + ".");
			m_LastError.assign(err.begin(), err.end());
			m_Parsers[a_Options.InputLang]->Reset();
			m_Databases[a_Options.InputLang]->Reset();
			return parseResult;
		}

		token = m_Scanners[a_Options.InputLang]->GetNextToken();
	}

	inputStream.close();
	recursionDepth++;
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::ShaderConverter::RegisterCustomShaderType(uint32 a_ID)
{
	if (ShaderTypeRegistered(a_ID))
		return SAVVY_SHADER_TYPE_ALREADY_EXISTS;

	m_ShaderTypes.push_back(a_ID);

	return SAVVY_OK;
}

const bool Savvy::ShaderConverter::ShaderTypeRegistered(uint32 a_ID)
{
	uint32 size = m_ShaderTypes.size();
	for (uint32 i = 0; i < size; i++)
	{
		if (m_ShaderTypes[i] == a_ID)
		{
			return true;
		}
	}

	return false;
}

Savvy::ResultCode Savvy::ShaderConverter::ConstructTranslatedShader(std::ostream& a_OutputStream, std::string a_InputEntryPoint, std::string a_OutputEntryPoint, uint32 a_OutputLang, uint32 a_InputLang, uint32 a_ShaderType)
{
	m_LastError.clear();
	Database* database = m_Databases[a_InputLang];
	ResultCode res;

	if (!OutputShaderLangRegistered(a_OutputLang))
	{
		m_LastError = "Shader language not registered as output language.";
		return SAVVY_INVALID_SHADER_LANG;
	}
	else
	{
		// Check if we need to create the language's constructor
		bool constructorExists = ElementInMapExists(m_ConstructorMap, a_OutputLang);

		if (!constructorExists)
		{
			m_ConstructorMap.insert(std::make_pair(a_OutputLang, m_ConstructorClassMap[a_OutputLang]()));
		}
	}

	if (m_ConstructorMap.count(a_OutputLang) <= 0)
	{
		m_LastError = "No proper constructor available for the specified output shader language.";
		return SAVVY_LOGICAL_ERROR;
	}

	res = m_ConstructorMap[a_OutputLang]->Initialize(Key(a_InputEntryPoint), Key(a_OutputEntryPoint), a_InputLang, a_ShaderType, database);

	if (res != SAVVY_OK)
	{
		m_LastError = "Could not initialize output type constructor.";
		return res;
	}

	res = m_ConstructorMap[a_OutputLang]->Construct(a_OutputStream);

	if (res != SAVVY_OK)
	{
		std::string err;
		err = "Error while constructing shader.";
		err.append(m_ConstructorMap[a_OutputLang]->GetLastError());
		m_LastError.assign(err.begin(), err.end());
	}

	m_ConstructorMap[a_OutputLang]->Reset();

	return res;

}

const bool Savvy::ShaderConverter::OutputShaderLangRegistered(uint32 a_ID)
{
	uint32 size = m_OutputLanguageList.size();
	for (uint32 i = 0; i < size; i++)
	{
		if (m_OutputLanguageList[i] == a_ID)
		{
			return true;
		}
	}

	return false;
}

const bool Savvy::ShaderConverter::InputShaderLangRegistered(uint32 a_ID)
{
	uint32 size = m_InputLanguageList.size();
	for (uint32 i = 0; i < size; i++)
	{
		if (m_InputLanguageList[i] == a_ID)
		{
			return true;
		}
	}

	return false;
}

