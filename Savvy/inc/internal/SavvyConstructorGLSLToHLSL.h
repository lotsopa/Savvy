#ifndef SAVVY_CONSTRUCTOR_GLSL_TO_HLSL_H
#define SAVVY_CONSTRUCTOR_GLSL_TO_HLSL_H

#include "SavvyConstructor.h"

namespace Savvy
{
	namespace Internal
	{
		class FuncConverterGLSLToHLSL;
		class ConstructorGLSLToHLSL : public Constructor
		{
		public:

			ConstructorGLSLToHLSL();
			virtual ~ConstructorGLSLToHLSL();

			virtual void Reset();
			virtual ResultCode Initialize(Key& a_InputEntry, Key& a_OutputEntry, uint32 a_Inputlang, uint32 a_ShaderType, Database* a_Database);

			virtual ResultCode Construct(std::ostream& a_OutputStream);
		private:
			Key m_CurrFunction;
			ResultCode ConstructUniformBuffersHLSL(std::ostream& a_OutputStream);
			ResultCode ConstructSamplersHLSL(std::ostream& a_OutputStream);
			ResultCode ConstructInputVariablesHLSL(std::ostream& a_OutputStream);
			ResultCode ConstructOutputVariablesHLSL(std::ostream& a_OutputStream);
			ResultCode ConstructStructsHLSL(std::ostream& a_OutputStream);
			ResultCode ConstructGlobalVarsHLSL(std::ostream& a_OutputStream);
			ResultCode ConstructFunctionsHLSL(std::ostream& a_OutputStream);
			ResultCode ConstructInnerFunctionHLSL(const Key& a_FuncName, std::ostream& a_OutputStream);
			ResultCode ConstructMainFunctionHLSL(std::ostream& a_OutputStream);
			ResultCode ConstructGenericFunctionHLSL(const Key& a_FuncName, std::ostream& a_OutputStream);
			ResultCode ConstructDefines(std::ostream& a_OutputStream);
			ResultCode ConstructGenericBuffersHLSL(std::ostream& a_OutputStream);

			const bool IsEndOfLine(const Key& a_Key, const std::string& a_CurrLine);
			Savvy::ResultCode HandleMatrixMultiplication(std::string& currentLine, Database::WordMultiMap::iterator& it1, bool& a_Handled);
			bool IsVarVector(std::string a_VarName);
			bool IsVarMatrix(std::string a_VarName);
			const bool IsMatrixDowncast(std::string& a_CurrLine, const Key& a_FuncName, Database::WordMultiMap::iterator& a_Iter);
			const Key& GetFragmentShaderReturnVariableName();
			void TidyLineFormatting(std::string& a_Line);
			ResultCode HandleNoEquivalentFunc(Database::WordMultiMap::iterator& it1, std::string& currentLine);
			bool HLSLEquivalentExists(const Key& a_Name);
			const Key& GetHLSLKeyword(const Key& a_Name);
			bool HandleBuiltInVariablesOut(const Key& a_Name, std::ostream& a_OutputStream);
			bool HandleBuiltInVariablesIn(const Key& a_Name, std::ostream& a_OutputStream);
			ResultCode ConstructVertexOrFragment(std::ostream& a_OutputStream);
			ResultCode HandleExtraArgsInputs(std::ostream& a_OutputStream, Key& a_CurrFunc);
			ResultCode HandleExtraArgsOutputs(std::ostream& a_OutputStream, Key& a_CurrFunc);
			FuncConverterGLSLToHLSL* m_FuncConverter;
			Database::KeyList m_FuncToAddInputArg;
			Database::KeyList m_FuncToAddOutputArg;
			bool m_ClipDistHandled;
			bool m_CullDistHandled;
		};
	}
}

#endif // !SAVVY_CONSTRUCTOR_GLSL_TO_HLSL_H