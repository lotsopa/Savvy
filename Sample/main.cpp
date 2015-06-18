#include "Savvy.h"
#include "CustomConstructor.h"
#include "CustomParser.h"
#include "CustomScanner.h"
#include "CustomDatabase.h"

enum CustomLang
{
	CUSTOM_LANG = Savvy::SAVVY_CUSTOM_USER_LANG
};

Savvy::ResultCode BlobToBlob(Savvy::ShaderConverter* converter)
{
	Savvy::ResultCode res;
	// Load file in memory
	std::ifstream is("../Converted/HLSL/ProceduralTerrain.frag.hlsl");
	if (!is.is_open())
	{
		std::cout << "Error reading file" << std::endl;
	}
	std::string fileStr(static_cast<std::stringstream const&>(std::stringstream() << is.rdbuf()).str());
	is.close();

	// Create a blob with the loaded file in memory
	Savvy::Blob inputBlob(&fileStr[0], fileStr.size());
	Savvy::Blob outputBlob;
	Savvy::BlobConvertOptions options;
	options.InputBlob = &inputBlob;
	options.OutputBlob = &outputBlob;
	options.InputLang = Savvy::HLSL_5_0;
	options.OutputLang = Savvy::GLSL_4_5;
	options.ShaderType = Savvy::FRAGMENT_SHADER;

	res = converter->ConvertShaderFromBlobToBlob(options);

	// Output the converted blob to file to verify its integrity
	std::ofstream str("BlobToBlobTest.txt");
	std::string mystring(options.OutputBlob->GetRawDataPtr(), options.OutputBlob->GetDataSize());
	str << mystring;
	str.close();
	return res;
}

Savvy::ResultCode FileToFile(Savvy::ShaderConverter* converter)
{
	Savvy::ResultCode res;
	Savvy::FileConvertOptions options;
	options.InputPath = L"output.txt";
	options.OutputPath = L"bla.txt";
	options.InputLang = Savvy::HLSL_5_0;
	options.OutputLang = Savvy::GLSL_4_5;
	options.InputEntryPoint = "PS_main";
	options.ShaderType = Savvy::FRAGMENT_SHADER;

	res = converter->ConvertShaderFromFileToFile(options);

	return res;
}

Savvy::ResultCode BlobToFile(Savvy::ShaderConverter* converter)
{
	Savvy::ResultCode res;
	// Load file in memory
	std::ifstream is("../Converted/HLSL/ProceduralTerrain.frag.hlsl");
	if (!is.is_open())
	{
		std::cout << "Error reading file" << std::endl;
	}
	std::string fileStr(static_cast<std::stringstream const&>(std::stringstream() << is.rdbuf()).str());
	is.close();

	// Create a blob with the loaded file in memory
	Savvy::Blob inputBlob(&fileStr[0], fileStr.size());
	Savvy::BlobFileConvertOptions options;
	options.InputBlob = &inputBlob;
	options.OutputPath = L"BlobToFileTest.txt";
	options.InputLang = Savvy::HLSL_5_0;
	options.OutputLang = Savvy::GLSL_4_5;
	options.ShaderType = Savvy::FRAGMENT_SHADER;

	res = converter->ConvertShaderFromBlobToFile(options);

	return res;
}

Savvy::ResultCode FileToBlob(Savvy::ShaderConverter* converter)
{
	Savvy::ResultCode res;

	// Create a blob with the loaded file in memory
	Savvy::FileBlobConvertOptions options;
	Savvy::Blob outputBlob;
	options.InputPath = L"../Converted/HLSL/ProceduralTerrain.frag.hlsl";
	options.OutputBlob = &outputBlob;
	options.InputLang = Savvy::HLSL_5_0;
	options.OutputLang = Savvy::GLSL_4_5;
	options.ShaderType = Savvy::FRAGMENT_SHADER;

	res = converter->ConvertShaderFromFileToBlob(options);

	// Output the converted blob to file to verify its integrity
	std::ofstream str("FileToBlobTest.txt");
	std::string mystring(options.OutputBlob->GetRawDataPtr(), options.OutputBlob->GetDataSize());
	str << mystring;
	str.close();

	return res;
}
/*
The function will produce an empty file, because the Custom Constructor class is a stub.
The function is provided simply to give the user an idea of the custom shader language capabilities of the library.
*/
Savvy::ResultCode FileToFileCustomOutput(Savvy::ShaderConverter* converter)
{
	Savvy::ResultCode res;

	res = converter->RegisterCustomOutputShaderLang<CustomConstructor>(CUSTOM_LANG, L".custom");

	if (res != Savvy::SAVVY_OK)
		return res;

	Savvy::FileConvertOptions options;
	options.InputPath = L"../Converted/HLSL/ProceduralTerrain.frag.hlsl";
	options.OutputPath = L"../Converted/ProceduralTerrain.frag.custom";
	options.InputLang = Savvy::HLSL_5_0;
	options.OutputLang = CUSTOM_LANG;
	options.ShaderType = Savvy::FRAGMENT_SHADER;

	res = converter->ConvertShaderFromFileToFile(options);

	return res;
}

/*
This conversion returns an error when trying to construct the shader.
This is simply because our custom scanner and parser are just stubs
and haven't populated the database. Empty database = nothing to construct, therefore an error.
The function is provided simply to give the user an idea of the custom shader language capabilities of the library.
*/
Savvy::ResultCode FileToFileCustomInput(Savvy::ShaderConverter* converter)
{
	Savvy::ResultCode res;

	res = converter->RegisterCustomInputShaderLang<CustomScanner, CustomParser, CustomDatabase>(CUSTOM_LANG);

	if (res != Savvy::SAVVY_OK)
		return res;

	Savvy::FileConvertOptions options;
	options.InputPath = L"../Converted/ProceduralTerrain.frag.custom";
	options.OutputPath = L"../Converted/HLSL/ProceduralTerrain.frag.hlsl";
	options.InputLang = CUSTOM_LANG;
	options.OutputLang = Savvy::HLSL_5_0;
	options.ShaderType = Savvy::FRAGMENT_SHADER;

	res = converter->ConvertShaderFromFileToFile(options);

	return res;
}

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

char* t[2];
int main(int argc, char** argv)
{
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

	// mcpp test
	//t[0] = argv[0];
	//t[1] = "-C";
	//int mcppRes = mcpp_lib_main(2, t, "ForwardRendering.hlsl", "output.txt");
	// File to File example
	res = FileToFile(converter);

	// Blob to Blob example
	//res = BlobToBlob(converter);

	// Blob to File example
	//res = BlobToFile(converter);

	// File to Blob Example
	//res = FileToBlob(converter);

	// Custom Output example
	//res = FileToFileCustomOutput(converter);

	// Custom input example
	//res = FileToFileCustomInput(converter);


	if (res != Savvy::ResultCode::SAVVY_OK)
		std::cerr << converter->GetLastError() << std::endl;

	getchar();
	delete converter;
	return 0;
}