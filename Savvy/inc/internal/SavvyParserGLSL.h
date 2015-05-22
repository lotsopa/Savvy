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
#ifndef SAVVY_PARSER_GLSL_H
#define SAVVY_PARSER_GLSL_H

#include "SavvyParser.h"

namespace Savvy
{
	class Constructor;
	namespace Internal
	{
		class DatabaseGLSL;
		class ParserGLSL : public Parser
		{
		public:
			ParserGLSL();
			virtual ~ParserGLSL();

			/*
			Parses a GLSL token.
			*/
			virtual ResultCode ParseToken(uint32 a_Token, std::string& a_TokenName);

			/*
			Resets the parser state.
			*/
			virtual void Reset();

		private:

			void AddLocalInstruction(Key& a_Instruction, bool a_Translate, Key& a_FuncName);
			void AddGlobalInstruction(Key& a_Instruction, bool a_Translate, Key& a_VarName);
			void HandleBuiltInVariable(Key& a_TokenName);

			enum State
			{
				BASE = 0,
				DATA_TYPE_DECLARATION,
				IDENTIFIER_DECLARATION,
				IDENTIFIER_USE,
				IN_PLACE_DECLARATION,
				VALUE_DECLARATION,
				FUNCTION_DECLARATION,
				OPERATION,
				FUNCTION_CALL,
				LANGUAGE_SPECIFIC_DECLARATION
			};

			enum ScopeState
			{
				GLOBAL = 0,
				LAYOUT_LOCATION_DECL,
				IN_FUNCTION,
				CONDITIONAL
			};

			enum VariableType
			{
				GLOBAL_VAR = 0,
				GLOBAL_VAR_INIT,
				INPUT_VAR,
				OUTPUT_VAR,
				STRUCT_VAR,
				STRUCT_VAR_INIT,
				UNIFORM_VAR,
				UNIFORM_VAR_INIT,
				GENERIC_BUFFER_VAR,
				ORIGINAL_SEMANTIC
			};

			uint32 m_ConditionalDepth;
			uint32 m_LastLayoutIndex;
			State m_State;
			State m_OldState;
			bool m_AddedLocalVar;
			ScopeState m_ScopeState;
			Database::Variable m_LastFuncVariable;
			Database::KeyList m_LastTypeQualifierList;
			Key m_LastDataType;
			Key m_LastIdentifier;
			Key m_LastBufferName;
			Key m_LastFunctionName;
			Key m_LastIdentifierBeforeBracketOpen;
			Key m_LastDataTypeBeforeBracketOpen;
			Key m_LastStructName;
			Key m_LastGlobalVarName;
			Key m_InputOrOutput;
			VariableType m_CurrVarType;
			Database::VarList m_LastFuncVarList;

		protected:
			typedef std::map<uint32, std::function<ResultCode(std::string&)>> StateFuncMap;
			StateFuncMap m_StateFuncMap;

			// State machine functions
			ResultCode Parse_EOF(std::string& a_TokenName);
			ResultCode Parse_ID(std::string& a_TokenName);
			ResultCode Parse_DataType(std::string& a_TokenName);
			ResultCode Parse_Operator(std::string& a_TokenName);
			ResultCode Parse_IntValue(std::string& a_TokenName);
			ResultCode Parse_FloatValue(std::string& a_TokenName);
			ResultCode Parse_BoolValue(std::string& a_TokenName);
			ResultCode Parse_BuiltInFunc(std::string& a_TokenName);
			ResultCode Parse_LangSpecific(std::string& a_TokenName);
			ResultCode Parse_InstructionEnd(std::string& a_TokenName);
			ResultCode Parse_OpenBracket(std::string& a_TokenName);
			ResultCode Parse_CloseBracket(std::string& a_TokenName);
			ResultCode Parse_OpenBrace(std::string& a_TokenName);
			ResultCode Parse_CloseBrace(std::string& a_TokenName);
			ResultCode Parse_OpenSquareBracket(std::string& a_TokenName);
			ResultCode Parse_CloseSquareBracket(std::string& a_TokenName);
			ResultCode Parse_While(std::string& a_TokenName);
			ResultCode Parse_Do(std::string& a_TokenName);
			ResultCode Parse_Return(std::string& a_TokenName);
			ResultCode Parse_If(std::string& a_TokenName);
			ResultCode Parse_Else(std::string& a_TokenName);
			ResultCode Parse_For(std::string& a_TokenName);
			ResultCode Parse_Switch(std::string& a_TokenName);
			ResultCode Parse_Continue(std::string& a_TokenName);
			ResultCode Parse_Case(std::string& a_TokenName);
			ResultCode Parse_Break(std::string& a_TokenName);
			ResultCode Parse_Unknown(std::string& a_TokenName);
			ResultCode Parse_Preprocessor(std::string& a_TokenName);
			ResultCode Parse_NewLine(std::string& a_TokenName);
		};
	}
}

#endif // !SAVVY_PARSER_GLSL_H