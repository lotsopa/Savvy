#include "Savvy.h"

Savvy::ResultCode RegisterDefaultConverters(Savvy::ShaderConverter* converter)
{
	Savvy::ResultCode res;
	res = converter->RegisterCustomShaderLang<Savvy::Internal::ScannerGLSL, Savvy::Internal::ParserGLSL, Savvy::Internal::DatabaseGLSL, Savvy::Internal::ConstructorHLSLToGLSL>(Savvy::GLSL_4_5, L".glsl");

	if (res != Savvy::ResultCode::SAVVY_OK)
		std::cerr << converter->GetLastError() << std::endl;

	converter->RegisterCustomShaderLang<Savvy::Internal::ScannerGLSL, Savvy::Internal::ParserGLSL, Savvy::Internal::DatabaseGLSL, Savvy::Internal::ConstructorHLSLToGLSLNoLoc>(Savvy::GLSL_NO_LOC, L".glsl");

	if (res != Savvy::ResultCode::SAVVY_OK)
		std::cerr << converter->GetLastError() << std::endl;

	converter->RegisterCustomShaderLang<Savvy::Internal::ScannerGLSL, Savvy::Internal::ParserGLSL, Savvy::Internal::DatabaseGLSL, Savvy::Internal::ConstructorHLSLToGLSLNoUBO>(Savvy::GLSL_NO_UBO, L".glsl");

	if (res != Savvy::ResultCode::SAVVY_OK)
		std::cerr << converter->GetLastError() << std::endl;

	converter->RegisterCustomShaderLang<Savvy::Internal::ScannerGLSL, Savvy::Internal::ParserGLSL, Savvy::Internal::DatabaseGLSL, Savvy::Internal::ConstructorHLSLToGLSLNoUBONoLoc>(Savvy::GLSL_NO_UBO_NO_LOC, L".glsl");

	if (res != Savvy::ResultCode::SAVVY_OK)
		std::cerr << converter->GetLastError() << std::endl;

	converter->RegisterCustomShaderLang<Savvy::Internal::ScannerHLSL, Savvy::Internal::ParserHLSL, Savvy::Internal::DatabaseHLSL, Savvy::Internal::ConstructorGLSLToHLSL>(Savvy::HLSL_5_0, L".hlsl");

	return res;
}

Savvy::ResultCode RegisterDefaultShaderTypes(Savvy::ShaderConverter* converter)
{
	Savvy::ResultCode res;

	res = converter->RegisterCustomShaderType(Savvy::INVALID_SHADER_LANG);

	if (res != Savvy::ResultCode::SAVVY_OK)
		std::cerr << converter->GetLastError() << std::endl;

	res = converter->RegisterCustomShaderType(Savvy::FRAGMENT_SHADER);

	if (res != Savvy::ResultCode::SAVVY_OK)
		std::cerr << converter->GetLastError() << std::endl;

	res = converter->RegisterCustomShaderType(Savvy::VERTEX_SHADER);

	if (res != Savvy::ResultCode::SAVVY_OK)
		std::cerr << converter->GetLastError() << std::endl;

	return res;
}

unsigned int GetUserNumber()
{
	unsigned int choice = 0;
	std::string input = "";
	while (true)
	{
		std::cout << "Please enter a valid number: ";
		std::getline(std::cin, input);

		// This code converts from string to number safely.
		std::stringstream myStream(input);
		if (myStream >> choice)
		{
			break;
		}

		std::cout << "Invalid number, please try again" << std::endl;
	}

	return choice;
}

std::string GetUserString()
{
	std::string input = "";
	std::cout << "Please enter a valid string of characters: ";
	std::getline(std::cin, input);
	return input;
}

std::wstring GetUserWideString()
{
	std::wstring input;
	std::wcout << "Please enter a valid string of characters: ";
	std::getline(std::wcin, input);
	return input;
}


void HandleInput(Savvy::FileConvertOptions& a_Options)
{
	static std::string inputEntryString;
	static std::string outputEntryString;
	static std::wstring inputPathString;
	static std::wstring outputPathString;

	// Input language
	std::cout << "--------------------------------------------------------------------" << std::endl;
	std::cout << "Please choose the shader language of the input shader:" << std::endl;
	std::cout << "1. GLSL 4.5" << std::endl;
	std::cout << "2. HLSL 5.0" << std::endl;
	std::cout << "3. GLSL 4.5 Without Uniform Buffer Objects" << std::endl;
	std::cout << "4. GLSL 4.5 Without explicit layout locations" << std::endl;
	std::cout << "5. GLSL Without explicit layout locations and Uniform Buffer Objects" << std::endl;
	std::cout << "--------------------------------------------------------------------" << std::endl;
	std::cout << std::endl;
	unsigned int choice = 0;
	do 
	{
		choice = GetUserNumber();

	} while (choice < 1 || choice > 5);

	switch (choice)
	{
	case 1:
		a_Options.InputLang = Savvy::GLSL_4_5;
		std::cout << "Input language chosen: GLSL 4.5.";
		break;
	case 2:
		a_Options.InputLang = Savvy::HLSL_5_0;
		std::cout << "Input language chosen: HLSL 5.0.";
		break;
	case 3:
		a_Options.InputLang = Savvy::GLSL_NO_UBO;
		std::cout << "Input language chosen: GLSL 4.5 Without Uniform Buffer Objects.";
		break;
	case 4:
		a_Options.InputLang = Savvy::GLSL_NO_LOC;
		std::cout << "Input language chosen: GLSL 4.5 Without explicit layout locations.";
		break;
	case 5:
		a_Options.InputLang = Savvy::GLSL_NO_UBO_NO_LOC;
		std::cout << "Input language chosen: GLSL Without explicit layout locations and Uniform Buffer Objects.";
		break;
	}
	std::cout << std::endl;
	std::cout << std::endl;

	// Output language
	std::cout << "--------------------------------------------------------------------" << std::endl;
	std::cout << "Please choose the shader language of the output shader:" << std::endl;
	std::cout << "1. GLSL 4.5" << std::endl;
	std::cout << "2. HLSL 5.0" << std::endl;
	std::cout << "3. GLSL 4.5 Without Uniform Buffer Objects" << std::endl;
	std::cout << "4. GLSL 4.5 Without explicit layout locations" << std::endl;
	std::cout << "5. GLSL Without explicit layout locations and Uniform Buffer Objects" << std::endl;
	std::cout << "--------------------------------------------------------------------" << std::endl;
	std::cout << std::endl;
	choice = 0;
	do
	{
		choice = GetUserNumber();

	} while (choice < 1 || choice > 5);

	switch (choice)
	{
	case 1:
		a_Options.OutputLang = Savvy::GLSL_4_5;
		std::cout << "Output language chosen: GLSL 4.5.";
		break;
	case 2:
		a_Options.OutputLang = Savvy::HLSL_5_0;
		std::cout << "Output language chosen: HLSL 5.0.";
		break;
	case 3:
		a_Options.OutputLang = Savvy::GLSL_NO_UBO;
		std::cout << "Output language chosen: GLSL 4.5 Without Uniform Buffer Objects.";
		break;
	case 4:
		a_Options.OutputLang = Savvy::GLSL_NO_LOC;
		std::cout << "Output language chosen: GLSL 4.5 Without explicit layout locations.";
		break;
	case 5:
		a_Options.OutputLang = Savvy::GLSL_NO_UBO_NO_LOC;
		std::cout << "Output language chosen: GLSL Without explicit layout locations and Uniform Buffer Objects.";
		break;
	}
	std::cout << std::endl;
	std::cout << std::endl;

	// Shader type
	std::cout << "--------------------------------------------------------------------" << std::endl;
	std::cout << "Please choose the shader type: " << std::endl;
	std::cout << "1. Vertex Shader" << std::endl;
	std::cout << "2. Fragment\\Pixel Shader" << std::endl;
	std::cout << "--------------------------------------------------------------------" << std::endl;
	std::cout << std::endl;
	choice = 0;
	do
	{
		choice = GetUserNumber();

	} while (choice < 1 || choice > 2);

	switch (choice)
	{
	case 1:
		a_Options.ShaderType = Savvy::VERTEX_SHADER;
		std::cout << "Shader type chosen: Vertex Shader.";
		break;
	case 2:
		a_Options.ShaderType = Savvy::FRAGMENT_SHADER;
		std::cout << "Shader type chosen: Fragment\\Pixel Shader.";
		break;
	}
	std::cout << std::endl;
	std::cout << std::endl;

	// Input shader Entry point
	std::cout << "--------------------------------------------------------------------" << std::endl;
	std::cout << "Please specify the input shader entry point. " << std::endl;
	std::cout << "--------------------------------------------------------------------" << std::endl;
	inputEntryString = GetUserString();
	a_Options.InputEntryPoint = inputEntryString.c_str();
	std::cout << std::endl;
	std::cout << std::endl;

	// Output shader Entry point
	std::cout << "--------------------------------------------------------------------" << std::endl;
	std::cout << "Please specify the output shader entry point. " << std::endl;
	std::cout << "--------------------------------------------------------------------" << std::endl;
	inputEntryString = GetUserString();
	a_Options.OutputEntryPoint = inputEntryString.c_str();
	std::cout << std::endl;
	std::cout << std::endl;

	// Input shader path
	std::cout << "--------------------------------------------------------------------" << std::endl;
	std::cout << "Please specify the input shader file path. " << std::endl;
	std::cout << "--------------------------------------------------------------------" << std::endl;
	inputPathString = GetUserWideString();
	a_Options.InputPath = inputPathString.c_str();
	std::cout << std::endl;
	std::cout << std::endl;

	// Output shader path
	std::cout << "--------------------------------------------------------------------" << std::endl;
	std::cout << "Please specify the output shader file path. " << std::endl;
	std::cout << "--------------------------------------------------------------------" << std::endl;
	outputPathString = GetUserWideString();
	a_Options.OutputPath = outputPathString.c_str();
	std::cout << std::endl;
	std::cout << std::endl;
}

int main(int argc, char** argv)
{
	// Initialize the converter
	Savvy::ShaderConverter* converter = new Savvy::ShaderConverter();
	Savvy::ResultCode res;

	// Register the default supported shader types
	res = RegisterDefaultShaderTypes(converter);

	if (res != Savvy::ResultCode::SAVVY_OK)
		std::cerr << converter->GetLastError() << std::endl;

	// Register the built-in shader scanners, parsers and databases.
	res = RegisterDefaultConverters(converter);

	if (res != Savvy::ResultCode::SAVVY_OK)
		std::cerr << converter->GetLastError() << std::endl;

	// Get input from user
	Savvy::FileConvertOptions convertOptions;
	HandleInput(convertOptions);
	
	// Start conversion
	std::cout << "--------------------------------------------------------------------" << std::endl;
	std::cout << "Conversion in progress. Please wait..." << std::endl;
	std::cout << "--------------------------------------------------------------------" << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
	res = converter->ConvertShaderFromFileToFile(convertOptions);

	if (res != Savvy::ResultCode::SAVVY_OK)
		std::cerr << converter->GetLastError() << std::endl;

	std::cout << "Press any key to exit." << std::endl;
	getchar();
	delete converter;
	return 0;
}