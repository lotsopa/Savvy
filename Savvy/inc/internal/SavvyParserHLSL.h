#ifndef SAVVY_PARSER_HLSL_H
#define SAVVY_PARSER_HLSL_H
#include "SavvyParser.h"

namespace Savvy
{
	class Constructor;
	namespace Internal
	{
		class DatabaseHLSL;
		class ParserHLSL : public Parser
		{
		public:
			ParserHLSL();
			virtual ~ParserHLSL();

			/*
			Parses a GLSL token.
			*/
			virtual ResultCode ParseToken(uint32 a_Token, std::string& a_TokenName);

			/*
			Resets the parser state.
			*/
			virtual void Reset();

		private:

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
				LANGUAGE_SPECIFIC_DECLARATION,
				SEMANTIC_DECLARATION,
				TEMPLATE_DECLARATION
			};

			enum ScopeState
			{
				GLOBAL = 0,
				IN_FUNCTION,
				CONDITIONAL
			};

			enum VariableType
			{
				STRUCT_VAR = 0,
				STRUCT_VAR_INIT,
				TEXTURE_VAR,
				GLOBAL_VAR,
				GLOBAL_VAR_INIT,
				UNIFORM_VAR,
				UNIFORM_VAR_INIT,
				GENERIC_BUFFER_VAR
			};

			uint32 m_ConditionalDepth;
			State m_State;
			State m_OldState;
			ScopeState m_ScopeState;
			Database::Variable m_LastFuncVariable;
			Key m_LastDataType;
			Key m_LastIdentifier;
			Key m_LastBufferName;
			Key m_LastStructName;
			Key m_LastFunctionName;
			Key m_LastSemantic;
			Key m_LastGlobalVarName;
			Key m_LastIdentifierBeforeBracketOpen;
			Key m_LastDataTypeBeforeBracketOpen;
			Key m_LastLanguageSpecificWord;
			Key m_EntryPoint;
			Key m_InputStructName;
			Key m_OutputStructName;
			State m_LastStateBeforeTemplateOpen;
			bool m_AddedLocalVar;
			VariableType m_CurrVarType;
			Database::VarList m_LastFuncVarList;
			Database::KeyList m_LastTypeQualifierList;
			Database::KeyList m_LastTemplateTypeList;

			void AddLocalInstruction(Key& a_Instruction, bool a_Translate, Key& a_FuncName);
			void AddGlobalInstruction(Key& a_Instruction, bool a_Translate, Key& a_VarName);
			void HandleBuiltInSemantic(Key& a_TokenName);

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

#endif // !SAVVY_PARSER_HLSL_H