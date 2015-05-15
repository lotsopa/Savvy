#ifndef SAVVY_FUNC_CONVERTER_GLSL_TO_HLSL_H
#define SAVVY_FUNC_CONVERTER_GLSL_TO_HLSL_H

#include "SavvyFuncConverter.h"

namespace Savvy
{
	namespace Internal
	{
		class FuncConverterGLSLToHLSL: public FuncConverter
		{
		public:
			FuncConverterGLSLToHLSL();
			virtual ~FuncConverterGLSLToHLSL();
			virtual ResultCode ConvertFunction(Database::WordMultiMap::iterator& it1, std::string& currentLine, Key& a_FuncName, uint32 a_Shader, Key& a_InputStructName = Key(), Key& a_OutputStructName = Key());
			virtual ResultCode GetFuncArgumentList(Database::WordMultiMap::iterator& it1, StringList& a_OutputList, std::string& currentLine);
			virtual void Reset();
			ResultCode GetArgumentVars(Database::WordMultiMap::iterator& it1, StringList& a_Variables);
		private:
			typedef std::map<Key, std::function<ResultCode(Database::WordMultiMap::iterator&, std::string&)>> FuncConvertMap;
			FuncConvertMap m_FuncConvertMap;

			ResultCode Convert_Acosh_Asinh_Atanh(Database::WordMultiMap::iterator& it1, std::string& currentLine);
			ResultCode Convert_Atomic_Operations(Database::WordMultiMap::iterator& it1, std::string& currentLine);
			ResultCode Convert_Atomic_CompSwap(Database::WordMultiMap::iterator& it1, std::string& currentLine);
			ResultCode Convert_Unsupported(Database::WordMultiMap::iterator& it1, std::string& currentLine);
			ResultCode Convert_Atan(Database::WordMultiMap::iterator& it1, std::string& currentLine);
			ResultCode Convert_BitfieldExtract(Database::WordMultiMap::iterator& it1, std::string& currentLine);
			ResultCode Convert_BitfieldInsert(Database::WordMultiMap::iterator& it1, std::string& currentLine);
			ResultCode Convert_Vec_Comparisons(Database::WordMultiMap::iterator& it1, std::string& currentLine);
			ResultCode Convert_Mod(Database::WordMultiMap::iterator& it1, std::string& currentLine);
			ResultCode Convert_MatrixCompMult(Database::WordMultiMap::iterator& it1, std::string& currentLine);
			ResultCode Convert_Not(Database::WordMultiMap::iterator& it1, std::string& currentLine);
			ResultCode ConvertTexturing(Database::WordMultiMap::iterator& it1, std::string& currentLine);
			ResultCode ConvertVectors(Database::WordMultiMap::iterator& it1, std::string& currentLine);
			ResultCode Convert_TextureSize(Database::WordMultiMap::iterator& it1, std::string& currentLine);
			ResultCode HandleMatrixMultiplication(std::string& currentLine, Database::WordMultiMap::iterator& it1, bool& a_Handled);
			bool IsVarVector(std::string a_VarName);
			bool IsVarMatrix(std::string a_VarName);
			const bool IsMatrixDowncast(std::string& a_CurrLine, const Key& a_FuncName, Database::WordMultiMap::iterator& a_Iter);
			uint32 m_AtomicCtr;
			uint32 m_BitfieldExtractCtr;
			uint32 m_BitfieldInsertCtr;
			uint32 m_TextureSizeCtr;
		};
	}
}

#endif