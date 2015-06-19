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
#ifndef SAVVY_CONSTRUCTOR_HLSL_TO_GLSL_H
#define SAVVY_CONSTRUCTOR_HLSL_TO_GLSL_H
#include "SavvyConstructor.h"

namespace Savvy
{
	namespace Internal
	{
		// Forward Declarations
		class FuncConverterHLSLToGLSL;
		class TemplateConverterHLSLToGLSL;
		class ConstructorHLSLToGLSL : public Constructor
		{
		public:

			/*
			Default constructor
			*/
			ConstructorHLSLToGLSL();
			
			/*
			Destructor
			*/
			virtual ~ConstructorHLSLToGLSL();

			/*
			The function which resets the Constructor to its initial state.
			*/
			virtual void Reset();
			
			/*
			The function which initializes the Constructor options.
			It MUST be called before the Construct() function for proper behaviour.
			*/
			virtual ResultCode Initialize(Key& a_InputEntry, Key& a_OutputEntry, uint32 a_Inputlang, uint32 a_ShaderType, Database* a_Database);

			/*
			The function which constructs the new shader to the specified output stream.
			*/
			virtual ResultCode Construct(std::ostream& a_OutputStream);

		protected:
			virtual ResultCode ConstructInputsGLSL(std::ostream& a_OutputStream);
			virtual ResultCode ConstructOutputsGLSL(std::ostream& a_OutputStream);
			virtual ResultCode ConstructUniformBuffersGLSL(std::ostream& a_OutputStream);
			virtual ResultCode ConstructGenericBuffersGLSL(std::ostream& a_OutputStream);
			virtual ResultCode ConstructSamplersGLSL(std::ostream& a_OutputStream);

			Key m_InputStructName;
			Key m_OutputStructName;
			Key m_CurrFunction;
			void TidyLineFormatting(std::string& a_Line);
			ResultCode ConstructStructsGLSL(std::ostream& a_OutputStream);
			ResultCode ConstructGlobalVarsGLSL(std::ostream& a_OutputStream);
			ResultCode ConstructFunctionsGLSL(std::ostream& a_OutputStream);
			ResultCode ConstructMainInnerFunctionGLSL(std::ostream& a_OutputStream);
			ResultCode ConstructGenericInnerFunctionGLSL(std::ostream& a_OutputStream, Key& a_FuncName);

			bool GLSLEquivalentExists(const Key& a_Name);
			const Key& GetGLSLKeyword(const Key& a_Name);
			const bool HandleBuiltInSemantic(Key& a_Name, Key& a_VarName, bool a_Rename = false);
			const bool IsEndOfLine(const Key& a_Key, const std::string& a_CurrLine);
			ResultCode RemoveAutomaticVars(Database::WordMultiMap::iterator& it1);
			ResultCode HandleNoEquivalentFunc(Database::WordMultiMap::iterator& it1, std::string& currentLine, Key& a_InputStruct, Key& a_OutputStruct);
			ResultCode HandleMatrixMultiplication(Database::WordMultiMap::iterator& it1, std::string& currentLine);
			ResultCode ConstructVertexOrFragment(std::ostream& a_OutputStream);
			ResultCode RemoveExtraFuncArgs(Key& a_InputStructName, Key& a_OutputStructName);
			ResultCode RetrieveOutputStructName(Key& a_Name);
			FuncConverterHLSLToGLSL* m_FuncConverter;
			TemplateConverterHLSLToGLSL* m_TemplateConverter;
		};
	}
}

#endif // !SAVVY_CONSTRUCTOR_HLSL_TO_GLSL_H