#ifndef SAVVY_FUNC_CONVERTER_HLSL_TO_GLSL_H
#define SAVVY_FUNC_CONVERTER_HLSL_TO_GLSL_H

#include "SavvyFuncConverter.h"

namespace Savvy
{
	namespace Internal
	{
		class FuncConverterHLSLToGLSL : public FuncConverter
		{
		public:
			FuncConverterHLSLToGLSL();
			virtual ~FuncConverterHLSLToGLSL();
			virtual ResultCode ConvertFunction(Database::WordMultiMap::iterator& it1, std::string& currentLine, Key& a_FuncName, uint32 a_Shader, Key& a_InputStructName = Key(), Key& a_OutputStructName = Key());
			virtual ResultCode GetFuncArgumentList(Database::WordMultiMap::iterator& it1, StringList& a_OutputList);
			virtual void Reset();
		private:
			typedef std::map<Key, std::function<ResultCode(Database::WordMultiMap::iterator&, std::string&)>> FuncConvertMap;
			FuncConvertMap m_FuncConvertMap;

			ResultCode HandleNoEquivalentFunc(Database::WordMultiMap::iterator& it1, std::string& currentLine);
			ResultCode HandleMatrixMultiplication(Database::WordMultiMap::iterator& it1, std::string& currentLine);
			ResultCode Convert_Interlocked_CompSwap(Database::WordMultiMap::iterator& it1, std::string& currentLine);
			ResultCode Convert_Interlocked_Operations(Database::WordMultiMap::iterator& it1, std::string& currentLine);
			ResultCode Convert_Saturate(Database::WordMultiMap::iterator& it1, std::string& currentLine);
			ResultCode Convert_Atan2(Database::WordMultiMap::iterator& it1, std::string& currentLine);
			ResultCode Convert_Unsupported(Database::WordMultiMap::iterator& it1, std::string& currentLine);
			ResultCode Convert_Fmod(Database::WordMultiMap::iterator& it1, std::string& currentLine);
			ResultCode Convert_Log10(Database::WordMultiMap::iterator& it1, std::string& currentLine);
			ResultCode Convert_Mad(Database::WordMultiMap::iterator& it1, std::string& currentLine);
			ResultCode Convert_Noise(Database::WordMultiMap::iterator& it1, std::string& currentLine);
			ResultCode Convert_Texturing(Database::WordMultiMap::iterator& it1, std::string& currentLine);
			ResultCode Convert_TexGather(Database::WordMultiMap::iterator& it1, std::string& currentLine);
		protected:
			const bool IsBuiltInGLSLVariable(Key& a_Name);
		};
	}
}

#endif