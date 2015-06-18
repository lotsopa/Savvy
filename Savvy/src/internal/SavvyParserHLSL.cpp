#include "SavvyPCH.h"
#include "SavvyParserHLSL.h"
#include "SavvyDatabaseHLSL.h"
#include "SavvyConstructor.h"
#include "SavvyFuncConverter.h"

#define STATE_FUNC_MAP_INSERT(a, b) m_StateFuncMap.insert(std::make_pair(a, std::bind(&b, this, std::placeholders::_1)))

Savvy::Internal::ParserHLSL::ParserHLSL() : m_State(BASE), m_OldState(BASE), m_CurrVarType(GLOBAL_VAR), m_ScopeState(GLOBAL),
m_ConditionalDepth(0), m_AddedLocalVar(false)
{
	// Bind state machine functions to appropriate tokens
	STATE_FUNC_MAP_INSERT(SAVVY_EOF, Savvy::Internal::ParserHLSL::Parse_EOF);
	STATE_FUNC_MAP_INSERT(SAVVY_IDENTIFIER, Savvy::Internal::ParserHLSL::Parse_ID);
	STATE_FUNC_MAP_INSERT(SAVVY_INTEGER_VALUE, Savvy::Internal::ParserHLSL::Parse_IntValue);
	STATE_FUNC_MAP_INSERT(SAVVY_FLOAT_VALUE, Savvy::Internal::ParserHLSL::Parse_FloatValue);
	STATE_FUNC_MAP_INSERT(SAVVY_BOOL_VALUE, Savvy::Internal::ParserHLSL::Parse_BoolValue);
	STATE_FUNC_MAP_INSERT(SAVVY_OPEN_BRACKET, Savvy::Internal::ParserHLSL::Parse_OpenBracket);
	STATE_FUNC_MAP_INSERT(SAVVY_CLOSE_BRACKET, Savvy::Internal::ParserHLSL::Parse_CloseBracket);
	STATE_FUNC_MAP_INSERT(SAVVY_OPEN_SQUARE_BRACKET, Savvy::Internal::ParserHLSL::Parse_OpenSquareBracket);
	STATE_FUNC_MAP_INSERT(SAVVY_CLOSE_SQUARE_BRACKET, Savvy::Internal::ParserHLSL::Parse_CloseSquareBracket);
	STATE_FUNC_MAP_INSERT(SAVVY_OPEN_BRACE, Savvy::Internal::ParserHLSL::Parse_OpenBrace);
	STATE_FUNC_MAP_INSERT(SAVVY_CLOSE_BRACE, Savvy::Internal::ParserHLSL::Parse_CloseBrace);
	STATE_FUNC_MAP_INSERT(SAVVY_DATA_TYPE, Savvy::Internal::ParserHLSL::Parse_DataType);
	STATE_FUNC_MAP_INSERT(SAVVY_OPERATOR, Savvy::Internal::ParserHLSL::Parse_Operator);
	STATE_FUNC_MAP_INSERT(SAVVY_INSTRUCTION_END, Savvy::Internal::ParserHLSL::Parse_InstructionEnd);
	STATE_FUNC_MAP_INSERT(SAVVY_RETURN_KEYWORD, Savvy::Internal::ParserHLSL::Parse_Return);
	STATE_FUNC_MAP_INSERT(SAVVY_IF_STATEMENT, Savvy::Internal::ParserHLSL::Parse_If);
	STATE_FUNC_MAP_INSERT(SAVVY_ELSE_STATEMENT, Savvy::Internal::ParserHLSL::Parse_Else);
	STATE_FUNC_MAP_INSERT(SAVVY_SWITCH_STATEMENT, Savvy::Internal::ParserHLSL::Parse_Switch);
	STATE_FUNC_MAP_INSERT(SAVVY_CASE_STATEMENT, Savvy::Internal::ParserHLSL::Parse_Case);
	STATE_FUNC_MAP_INSERT(SAVVY_BREAK_STATEMENT, Savvy::Internal::ParserHLSL::Parse_Break);
	STATE_FUNC_MAP_INSERT(SAVVY_FOR_STATEMENT, Savvy::Internal::ParserHLSL::Parse_For);
	STATE_FUNC_MAP_INSERT(SAVVY_DO_STATEMENT, Savvy::Internal::ParserHLSL::Parse_Do);
	STATE_FUNC_MAP_INSERT(SAVVY_WHILE_STATEMENT, Savvy::Internal::ParserHLSL::Parse_While);
	STATE_FUNC_MAP_INSERT(SAVVY_CONTINUE_STATEMENT, Savvy::Internal::ParserHLSL::Parse_Continue);
	STATE_FUNC_MAP_INSERT(SAVVY_BUILT_IN_FUNCTION, Savvy::Internal::ParserHLSL::Parse_BuiltInFunc);
	STATE_FUNC_MAP_INSERT(SAVVY_LANGUAGE_SPECIFIC_KEYWORD, Savvy::Internal::ParserHLSL::Parse_LangSpecific);
	STATE_FUNC_MAP_INSERT(SAVVY_PREPROCESSOR, Savvy::Internal::ParserHLSL::Parse_Preprocessor);
	STATE_FUNC_MAP_INSERT(SAVVY_NEW_LINE, Savvy::Internal::ParserHLSL::Parse_NewLine);
	STATE_FUNC_MAP_INSERT(SAVVY_UNKNOWN_TOKEN, Savvy::Internal::ParserHLSL::Parse_Unknown);
}

Savvy::Internal::ParserHLSL::~ParserHLSL()
{

}

Savvy::ResultCode Savvy::Internal::ParserHLSL::ParseToken(uint32 a_Token, std::string& a_TokenName)
{
	m_LastError.clear();
	ResultCode res = m_StateFuncMap.at(a_Token)(a_TokenName);

	//Debug
#if SAVVY_DEBUG == 1
	std::cout << a_TokenName << std::endl;
#endif
	return res;
}

void Savvy::Internal::ParserHLSL::Reset()
{
	m_State = BASE;
	m_OldState = BASE;
	m_ScopeState = GLOBAL;
	m_CurrVarType = GLOBAL_VAR;
	m_LastStateBeforeTemplateOpen = BASE;
	m_LastDataType.Clear();
	m_LastBufferName.Clear();
	m_LastIdentifier.Clear();
	m_LastFunctionName.Clear();
	m_LastStructName.Clear();
	m_LastIdentifierBeforeBracketOpen.Clear();
	m_LastLanguageSpecificWord.Clear();
	m_LastFuncVarList.clear();
	m_LastSemantic.Clear();
	m_LastTypeQualifierList.clear();
	m_LastGlobalVarName.Clear();
	m_EntryPoint.Clear();
	m_ConditionalDepth = 0;
	m_LastTemplateTypeList.clear();
	m_AddedLocalVar = false;
}

void Savvy::Internal::ParserHLSL::AddLocalInstruction(Key& a_Instruction, bool a_Translate, Key& a_FuncName)
{
	if (m_ScopeState != GLOBAL)
	{
		m_Database->AddLocalInstruction(a_Instruction, a_Translate, a_FuncName);
	}
}

void Savvy::Internal::ParserHLSL::AddGlobalInstruction(Key& a_Instruction, bool a_Translate, Key& a_VarName)
{
	if (m_CurrVarType == GLOBAL_VAR_INIT)
	{
		if (m_ScopeState == GLOBAL)
		{
			if (!m_LastTypeQualifierList.empty() && m_LastIdentifierBeforeBracketOpen.IsEmpty())
			{
				m_Database->AddLocalInstruction(a_Instruction, a_Translate, a_VarName);
			}
		}
	}
}

Savvy::ResultCode Savvy::Internal::ParserHLSL::Parse_EOF(std::string& a_TokenName)
{
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ParserHLSL::Parse_ID(std::string& a_TokenName)
{
	if (m_State == DATA_TYPE_DECLARATION || m_State == LANGUAGE_SPECIFIC_DECLARATION || m_State == BASE ||
		m_State == IDENTIFIER_DECLARATION || m_State == IDENTIFIER_USE)
	{
		if (m_State == LANGUAGE_SPECIFIC_DECLARATION)
		{
			// Uniform buffer declaration - save the name
			if (m_CurrVarType == UNIFORM_VAR || m_CurrVarType == TEXTURE_VAR || m_CurrVarType == GENERIC_BUFFER_VAR)
			{
				if (m_LastLanguageSpecificWord == "register")
				{
					if (m_CurrVarType == UNIFORM_VAR)
					{
						// A buffer index specifier
						uint32 bidx = a_TokenName.find("b");
						if (bidx == 0 && a_TokenName.size() < 4 && FuncConverter::isNumber(a_TokenName.substr(bidx + 1)))
						{
							uint32 currIdx = std::stoi(a_TokenName.substr(bidx + 1));
							m_Database->AddUniformBuffer(m_LastBufferName, currIdx);
						}
						else
						{
							m_LastError = "Could not parse identifier " + a_TokenName + ". Possible syntax error.";
							return SAVVY_SHADER_SYNTAX_ERROR;
						}
					}
					else if (m_CurrVarType == GENERIC_BUFFER_VAR)
					{
						// A buffer index specifier
						uint32 bidx = 0;
						if (bidx == 0 && FuncConverter::isNumber(a_TokenName.substr(bidx + 1)))
						{
							uint32 currIdx = std::stoi(a_TokenName.substr(bidx + 1));
							m_Database->AddGenericBuffer(m_LastIdentifier, currIdx);

							// Add the variable it will hold
							ResultCode addRes = m_Database->AddGenericBufferVariable(m_LastIdentifier, m_LastTemplateTypeList, m_LastIdentifier);

							if (addRes != SAVVY_OK)
							{
								m_LastError = "Could not parse identifier " + a_TokenName + ". Possible syntax error.";
								return SAVVY_SHADER_SYNTAX_ERROR;
							}
								
						}
						else
						{
							m_LastError = "Could not parse identifier " + a_TokenName + ". Possible syntax error.";
							return SAVVY_SHADER_SYNTAX_ERROR;
						}
					}
					else if (m_CurrVarType == TEXTURE_VAR)
					{
						// A buffer index specifier
						uint32 bidx = a_TokenName.find("t");
						if (bidx == 0 && a_TokenName.size() < 4 && FuncConverter::isNumber(a_TokenName.substr(bidx + 1)))
						{
							uint32 currIdx = std::stoi(a_TokenName.substr(bidx + 1));

							if (!m_Database->SamplerExists(m_LastIdentifier))
							{
								m_Database->AddSampler(m_LastIdentifier, m_LastDataType, currIdx, m_LastTemplateTypeList);
							}
							m_CurrVarType = GLOBAL_VAR;
						}
						else if (m_ScopeState == GLOBAL && m_CurrVarType == GLOBAL_VAR_INIT)
						{
							Database::WordMultiMap& instructionMap = m_Database->GetInstructionMap();
							Database::WordMultiMap::iterator it1 = instructionMap.upper_bound(m_LastIdentifier);

							it1--;
							if (it1->second.m_Name == ",")
								instructionMap.erase(it1);

							m_LastIdentifier = a_TokenName;
							m_Database->AddGlobalVariable(m_LastIdentifier, m_LastTypeQualifierList);
							m_State = IDENTIFIER_DECLARATION;
						}
						else
						{
							m_State = IDENTIFIER_USE;
						}
					}

				}
				else
				{
					m_LastBufferName = a_TokenName;
				}
			}
		}
		else if (m_State == DATA_TYPE_DECLARATION)
		{
			if (m_LastFunctionName.GetString() != "" && m_OldState != IN_PLACE_DECLARATION && !m_Database->FunctionExists(a_TokenName) &&
				!m_Database->LocalVariableExists(m_LastFunctionName, a_TokenName))
			{
				// Check if it's a user defined data type
				if (m_Database->StructExists(a_TokenName))
				{
					m_LastDataType = a_TokenName;
					m_LastTypeQualifierList.push_back(a_TokenName);
				}

				ResultCode res;
				res = m_Database->AddLocalVariable(Key(a_TokenName), m_LastTypeQualifierList, m_LastFunctionName, m_LastTemplateTypeList);
				m_AddedLocalVar = true;
				if (res != SAVVY_OK)
				{
					m_LastError = "Could not add local variable " + a_TokenName + " inside function " + m_LastFunctionName.GetString() + ". Local variable already exists.";
					return res;
				}
			}

			if (m_ScopeState == GLOBAL)
			{
				if (!m_LastIdentifierBeforeBracketOpen.IsEmpty() && !m_Database->StructExists(a_TokenName))
				{
					m_LastFuncVariable.m_Name = a_TokenName;
					m_LastFuncVarList.push_back(m_LastFuncVariable);
				}
			}
		}

		// Check if it's a user defined data type
		if (m_Database->StructExists(a_TokenName))
		{
			m_LastTypeQualifierList.push_back(a_TokenName);
			if (m_LastIdentifierBeforeBracketOpen.GetString() != "")
			{
				m_LastFuncVariable.m_QualifierList = m_LastTypeQualifierList;
			}
			m_LastDataType = a_TokenName;
			m_State = DATA_TYPE_DECLARATION;
		}
		else
		{
			if (m_CurrVarType != STRUCT_VAR_INIT && m_CurrVarType != UNIFORM_VAR_INIT)
				m_LastIdentifier = a_TokenName;
			else
			{
				if (m_CurrVarType == STRUCT_VAR_INIT)
				{
					m_Database->AddLocalInstruction(Key(a_TokenName), false, m_LastStructName + "." + m_LastIdentifier);
				}
				else if (m_CurrVarType == UNIFORM_VAR_INIT)
				{
					m_Database->AddLocalInstruction(Key(a_TokenName), false, m_LastBufferName + "." + m_LastIdentifier);
				}
			}

			m_State = IDENTIFIER_DECLARATION;
		}
	}
	else if (m_State == OPERATION)
	{
		// Handle adding local variables of the same type with a comma
		Database::WordMultiMap& instructionMap = m_Database->GetInstructionMap();
		Database::WordMultiMap::iterator it1 = instructionMap.upper_bound(m_LastFunctionName);
		it1--;
		if (it1->second.m_Name == "," && m_OldState != IN_PLACE_DECLARATION && !m_Database->FunctionExists(a_TokenName) &&
			!m_Database->LocalVariableExists(m_LastFunctionName, a_TokenName))
		{
			if (m_AddedLocalVar)
			{
				ResultCode res;
				res = m_Database->AddLocalVariable(Key(a_TokenName), m_LastTypeQualifierList, m_LastFunctionName, m_LastTemplateTypeList);
				m_AddedLocalVar = true;
				if (res != SAVVY_OK)
				{
					m_LastError = "Could not add local variable " + a_TokenName + " inside function " + m_LastFunctionName.GetString() + ". Local variable already exists.";
					return res;
				}
			}
		}
		else
			m_AddedLocalVar = false;

		m_State = IDENTIFIER_USE;
	}
	else if (m_State == SEMANTIC_DECLARATION)
	{
		// Save it
		m_LastSemantic = a_TokenName;
		m_Database->AddSemantic(m_LastStructName, m_LastIdentifier, m_LastSemantic);
	}
	else if (m_State == BASE)
	{
		// Check if it's a user defined data type
		if (m_Database->StructExists(a_TokenName))
		{
			m_LastDataType = a_TokenName;
			m_LastTypeQualifierList.push_back(a_TokenName);
		}
	}
	else if (m_State == TEMPLATE_DECLARATION)
	{
		m_LastTemplateTypeList.push_back(a_TokenName);
	}
	else
	{
		m_LastError = "Could not parse identifier " + a_TokenName + ". Possible syntax error.";
		return SAVVY_SHADER_SYNTAX_ERROR;
	}

	// Add extra instructions on function argument declaration
	if (m_CurrVarType == GLOBAL_VAR || m_CurrVarType == GLOBAL_VAR_INIT)
	{
		if (m_ScopeState == GLOBAL && m_State == OPERATION)
		{
			if (!m_LastIdentifierBeforeBracketOpen.IsEmpty())
			{
				m_Database->AddLocalInstruction(Key(a_TokenName), false, m_LastIdentifierBeforeBracketOpen + "." + m_LastIdentifier);
			}
		}
	}

	AddLocalInstruction(Key(a_TokenName), false, m_LastFunctionName);
	AddGlobalInstruction(Key(a_TokenName), false, m_LastGlobalVarName);
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ParserHLSL::Parse_DataType(std::string& a_TokenName)
{
	if (m_State == BASE || m_State == LANGUAGE_SPECIFIC_DECLARATION ||
		m_State == OPERATION || m_State == DATA_TYPE_DECLARATION || m_State == IDENTIFIER_DECLARATION || m_State == IDENTIFIER_USE)
	{
		m_State = DATA_TYPE_DECLARATION;
		m_LastDataType = a_TokenName;
		m_LastTypeQualifierList.push_back(m_LastDataType);

		if (m_LastDataType.Contains("Texture"))
		{
			m_CurrVarType = TEXTURE_VAR;
		}

		if (m_ScopeState == GLOBAL)
		{
			if (m_LastIdentifierBeforeBracketOpen.GetString() != "")
			{
				m_LastFuncVariable.m_QualifierList = m_LastTypeQualifierList;
			}
		}
	}
	else if (m_State == TEMPLATE_DECLARATION)
	{
		m_LastTemplateTypeList.push_back(a_TokenName);
	}
	else
	{
		m_LastError = "Could not parse data type " + a_TokenName + ". Possible syntax error.";
		return SAVVY_SHADER_SYNTAX_ERROR;
	}

	// Add extra instructions on function argument declaration
	if (m_CurrVarType == GLOBAL_VAR || m_CurrVarType == GLOBAL_VAR_INIT)
	{
		if (m_ScopeState == GLOBAL && m_State == OPERATION)
		{
			if (!m_LastIdentifierBeforeBracketOpen.IsEmpty())
			{
				m_Database->AddLocalInstruction(Key(a_TokenName), true, m_LastIdentifierBeforeBracketOpen + "." + m_LastIdentifier);
			}
		}
	}

	AddLocalInstruction(Key(a_TokenName), true, m_LastFunctionName);
	AddGlobalInstruction(Key(a_TokenName), true, m_LastGlobalVarName);
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ParserHLSL::Parse_Operator(std::string& a_TokenName)
{
	if (m_State == OPERATION || m_State == IDENTIFIER_DECLARATION || m_State == VALUE_DECLARATION || m_State == LANGUAGE_SPECIFIC_DECLARATION 
		|| m_State == IDENTIFIER_USE || m_State == DATA_TYPE_DECLARATION || m_State == TEMPLATE_DECLARATION)
	{
		if (m_ScopeState != GLOBAL)
		{
			m_State = OPERATION;
		}
		else
		{
			if (a_TokenName == ":" && m_CurrVarType == STRUCT_VAR)
				m_State = SEMANTIC_DECLARATION;
		}

		if (a_TokenName == "<")
		{
			if (m_State == DATA_TYPE_DECLARATION || m_State == LANGUAGE_SPECIFIC_DECLARATION)
			{
				m_LastStateBeforeTemplateOpen = m_State;
				m_State = TEMPLATE_DECLARATION;
				m_LastTemplateTypeList.clear();
			}
		}
		else if (a_TokenName == ">")
		{
			if (!m_LastTemplateTypeList.empty())
			{
				m_State = m_LastStateBeforeTemplateOpen;
			}
		}
	}
	else if (m_State == BASE)
	{
		if (m_OldState == IN_PLACE_DECLARATION)
		{
			m_State = OPERATION;
			m_OldState = BASE;
		}
	}
	else
	{
		m_LastError = "Could not parse operator " + a_TokenName + ". Possible syntax error.";
		return SAVVY_SHADER_SYNTAX_ERROR;
	}

	if (m_ScopeState != GLOBAL && !m_AddedLocalVar)
	{
		m_LastTypeQualifierList.clear();
	}
	else
	{
		if (!m_LastIdentifierBeforeBracketOpen.IsEmpty())
		{
			m_LastTypeQualifierList.clear();
			m_State = BASE;
		}
	}

	if (m_CurrVarType == GLOBAL_VAR || m_CurrVarType == GLOBAL_VAR_INIT)
	{
		if (m_ScopeState == GLOBAL)
		{
			if (!m_LastTypeQualifierList.empty() && m_LastIdentifierBeforeBracketOpen.IsEmpty())
			{
				m_Database->AddGlobalVariable(m_LastIdentifier, m_LastTypeQualifierList, m_LastTemplateTypeList);
				m_LastGlobalVarName = m_LastIdentifier;
				m_CurrVarType = GLOBAL_VAR_INIT;
			}
			else if (!m_LastIdentifierBeforeBracketOpen.IsEmpty() && a_TokenName != ",")
			{
				// Add extra instructions on function argument declaration
				m_Database->AddLocalInstruction(Key(a_TokenName), false, m_LastIdentifierBeforeBracketOpen + "." + m_LastIdentifier);
			}
		}
	}

	AddLocalInstruction(Key(a_TokenName), false, m_LastFunctionName);
	AddGlobalInstruction(Key(a_TokenName), false, m_LastGlobalVarName);
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ParserHLSL::Parse_IntValue(std::string& a_TokenName)
{
	if (m_State != LANGUAGE_SPECIFIC_DECLARATION && m_State != BASE)
	{
		m_State = VALUE_DECLARATION;
	}

	if (m_CurrVarType == STRUCT_VAR_INIT)
	{
		m_Database->AddLocalInstruction(Key(a_TokenName), false, m_LastStructName + "." + m_LastIdentifier);
	}
	else if (m_CurrVarType == UNIFORM_VAR_INIT)
	{
		m_Database->AddLocalInstruction(Key(a_TokenName), false, m_LastBufferName + "." + m_LastIdentifier);
	}

	// Add extra instructions on function argument declaration
	if (m_CurrVarType == GLOBAL_VAR || m_CurrVarType == GLOBAL_VAR_INIT)
	{
		if (m_ScopeState == GLOBAL)
		{
			if (!m_LastIdentifierBeforeBracketOpen.IsEmpty())
			{
				m_Database->AddLocalInstruction(Key(a_TokenName), false, m_LastIdentifierBeforeBracketOpen + "." + m_LastIdentifier);
			}
		}
	}

	AddLocalInstruction(Key(a_TokenName), false, m_LastFunctionName);
	AddGlobalInstruction(Key(a_TokenName), false, m_LastGlobalVarName);
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ParserHLSL::Parse_FloatValue(std::string& a_TokenName)
{
	if (m_State != LANGUAGE_SPECIFIC_DECLARATION && m_State != BASE)
	{
		m_State = VALUE_DECLARATION;
	}

	// Float values should be saved without 'f'
	if (a_TokenName.back() == 'f')
	{
		a_TokenName.erase(a_TokenName.size() - 1);
	}

	// Add extra instructions on function argument declaration
	if (m_CurrVarType == GLOBAL_VAR || m_CurrVarType == GLOBAL_VAR_INIT)
	{
		if (m_ScopeState == GLOBAL)
		{
			if (!m_LastIdentifierBeforeBracketOpen.IsEmpty())
			{
				m_Database->AddLocalInstruction(Key(a_TokenName), false, m_LastIdentifierBeforeBracketOpen + "." + m_LastIdentifier);
			}
		}
	}

	AddLocalInstruction(Key(a_TokenName), false, m_LastFunctionName);
	AddGlobalInstruction(Key(a_TokenName), false, m_LastGlobalVarName);
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ParserHLSL::Parse_BoolValue(std::string& a_TokenName)
{
	if (m_State != LANGUAGE_SPECIFIC_DECLARATION && m_State != BASE)
	{
		m_State = VALUE_DECLARATION;
	}

	AddLocalInstruction(Key(a_TokenName), false, m_LastFunctionName);
	AddGlobalInstruction(Key(a_TokenName), false, m_LastGlobalVarName);
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ParserHLSL::Parse_BuiltInFunc(std::string& a_TokenName)
{
	// Handle cases when a variable has the same name as a built in function
	if (m_State == DATA_TYPE_DECLARATION && m_ScopeState != GLOBAL)
	{
		m_LastIdentifier = a_TokenName;
		m_Database->AddLocalVariable(m_LastIdentifier, m_LastTypeQualifierList, m_LastFunctionName, m_LastTemplateTypeList);
		m_State = IDENTIFIER_DECLARATION;
	}
	else if (m_State == OPERATION && m_ScopeState != GLOBAL)
	{
		// Handle adding local variables of the same type with a comma
		Database::WordMultiMap& instructionMap = m_Database->GetInstructionMap();
		Database::WordMultiMap::iterator it1 = instructionMap.upper_bound(m_LastFunctionName);
		it1--;
		if (it1->second.m_Name == "," && m_OldState != IN_PLACE_DECLARATION && !m_Database->FunctionExists(a_TokenName) &&
			!m_Database->LocalVariableExists(m_LastFunctionName, a_TokenName))
		{
			if (m_AddedLocalVar)
			{
				ResultCode res;
				res = m_Database->AddLocalVariable(Key(a_TokenName), m_LastTypeQualifierList, m_LastFunctionName, m_LastTemplateTypeList);
				m_AddedLocalVar = true;
				m_State = IDENTIFIER_DECLARATION;
				if (res != SAVVY_OK)
				{
					m_LastError = "Could not add local variable " + a_TokenName + " inside function " + m_LastFunctionName.GetString() + ". Local variable already exists.";
					return res;
				}
			}
		}
	}
	AddLocalInstruction(Key(a_TokenName), true, m_LastFunctionName);
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ParserHLSL::Parse_LangSpecific(std::string& a_TokenName)
{
	if (m_State == BASE || m_State == LANGUAGE_SPECIFIC_DECLARATION ||
		m_State == IDENTIFIER_DECLARATION || m_State == SEMANTIC_DECLARATION || m_State == OPERATION || m_State == DATA_TYPE_DECLARATION)
	{
		if (a_TokenName == "cbuffer")
		{
			m_CurrVarType = UNIFORM_VAR;
		}
		else if (a_TokenName == "Buffer")
		{
			m_CurrVarType = GENERIC_BUFFER_VAR;
		}
		else if (a_TokenName == "StructuredBuffer")
		{
			m_CurrVarType = GENERIC_BUFFER_VAR;
		}

		if (m_LastFunctionName.GetString() != "" && m_OldState != IN_PLACE_DECLARATION && !m_Database->FunctionExists(a_TokenName) &&
			!m_Database->LocalVariableExists(m_LastFunctionName, a_TokenName))
		{
			// Check if it's a user defined data type
			if (m_Database->StructExists(a_TokenName))
			{
				m_LastDataType = a_TokenName;
				m_LastTypeQualifierList.push_back(a_TokenName);
			}

			ResultCode res;
			res = m_Database->AddLocalVariable(Key(a_TokenName), m_LastTypeQualifierList, m_LastFunctionName, m_LastTemplateTypeList);
			m_AddedLocalVar = true;
			if (res != SAVVY_OK)
			{
				m_LastError = "Could not add local variable " + a_TokenName + " inside function " + m_LastFunctionName.GetString() + ". Local variable already exists.";
				return res;
			}
		}

		// Handle built-in variables
		HandleBuiltInSemantic(Key(a_TokenName));

		m_LastLanguageSpecificWord = a_TokenName;
		m_State = LANGUAGE_SPECIFIC_DECLARATION;
	}
	else
	{
		m_LastError = "Could not parse language specific keyword " + a_TokenName + ". Possible syntax error.";
		return SAVVY_SHADER_SYNTAX_ERROR;
	}

	AddLocalInstruction(Key(a_TokenName), true, m_LastFunctionName);
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ParserHLSL::Parse_InstructionEnd(std::string& a_TokenName)
{
	if (m_State == BASE || m_State == IDENTIFIER_DECLARATION || m_State == DATA_TYPE_DECLARATION ||
		m_State == IDENTIFIER_USE || m_State == VALUE_DECLARATION || m_State == SEMANTIC_DECLARATION || m_State == OPERATION)
	{
		if (m_State == IDENTIFIER_DECLARATION || m_State == SEMANTIC_DECLARATION || m_State == VALUE_DECLARATION)
		{
			ResultCode addVarResult = SAVVY_OK;
			switch (m_CurrVarType)
			{
			case GENERIC_BUFFER_VAR:
				addVarResult = m_Database->AddGenericBufferVariable(m_LastIdentifier, m_LastTemplateTypeList, m_LastIdentifier);
				m_CurrVarType = GLOBAL_VAR;
				break;

			case UNIFORM_VAR_INIT:
			case UNIFORM_VAR:
				addVarResult = m_Database->AddUniformVariable(m_LastIdentifier, m_LastTypeQualifierList, m_LastBufferName, m_LastTemplateTypeList);
				m_ScopeState = GLOBAL;
				m_CurrVarType = UNIFORM_VAR;
				break;

			case STRUCT_VAR_INIT:
			case STRUCT_VAR:

				if (m_ScopeState == GLOBAL)
				{
					addVarResult = m_Database->AddStructVariable(m_LastStructName, m_LastIdentifier, m_LastTypeQualifierList, m_LastTemplateTypeList);
				}
				m_CurrVarType = STRUCT_VAR;
				break;

			case TEXTURE_VAR:
				if (m_ScopeState == GLOBAL)
				{
					if (!m_Database->SamplerExists(m_LastIdentifier))
					{
						addVarResult = m_Database->AddSampler(m_LastIdentifier, m_LastDataType, m_LastTemplateTypeList);
					}
					m_CurrVarType = GLOBAL_VAR;
				}
				break;

			case GLOBAL_VAR:
			{
				// Global variable
				if (m_ScopeState == GLOBAL)
				{
					if (!m_LastTypeQualifierList.empty())
					{
						addVarResult = m_Database->AddGlobalVariable(m_LastIdentifier, m_LastTypeQualifierList, m_LastTemplateTypeList);
					}
					m_CurrVarType = GLOBAL_VAR;
				}
			}
				break;
			default:
				
				break;
			}

			if (addVarResult != SAVVY_OK)
			{
				m_LastError = "Could not add variable " + m_LastIdentifier.GetString() + " to database. Variable already exists";
				return addVarResult;
			}
		}
		m_State = BASE;
	}
	else if (m_State == LANGUAGE_SPECIFIC_DECLARATION)
	{

	}
	else
	{
		m_LastError = "Could not parse semicolon. Possible syntax error.";
		return SAVVY_SHADER_SYNTAX_ERROR;
	}
	m_LastTypeQualifierList.clear();
	if (m_CurrVarType == GLOBAL_VAR_INIT)
	{
		m_CurrVarType = GLOBAL_VAR;
		m_LastGlobalVarName.Clear();
	}
	m_AddedLocalVar = false;
	m_LastTemplateTypeList.clear();
	AddLocalInstruction(Key(a_TokenName), false, m_LastFunctionName);
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ParserHLSL::Parse_OpenBracket(std::string& a_TokenName)
{
	if (m_State == DATA_TYPE_DECLARATION)
	{
		m_OldState = IN_PLACE_DECLARATION;
	}
	else if (m_State == IDENTIFIER_DECLARATION)
	{
		// Must be a function
		m_LastIdentifierBeforeBracketOpen = m_LastIdentifier;
		m_LastDataTypeBeforeBracketOpen = m_LastDataType;
		m_State = BASE;
	}

	if (!m_LastIdentifierBeforeBracketOpen.IsEmpty())
	{
		m_LastTypeQualifierList.clear();
		m_LastFuncVarList.clear();
	}

	AddLocalInstruction(Key(a_TokenName), false, m_LastFunctionName);
	AddGlobalInstruction(Key(a_TokenName), false, m_LastGlobalVarName);
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ParserHLSL::Parse_CloseBracket(std::string& a_TokenName)
{
	if (m_State == IDENTIFIER_DECLARATION || m_State == BASE)
	{
		if (m_ScopeState == GLOBAL)
		{
			// Function
			if (!m_LastIdentifierBeforeBracketOpen.IsEmpty())
			{
				m_LastFunctionName = m_LastIdentifierBeforeBracketOpen;
				if (m_Database->StructExists(m_LastDataTypeBeforeBracketOpen))
				{
					m_Database->AddFunction(m_LastFunctionName, m_LastDataTypeBeforeBracketOpen, m_LastFuncVarList, false);
				}
				else
				{
					m_Database->AddFunction(m_LastFunctionName, m_LastDataTypeBeforeBracketOpen, m_LastFuncVarList);
				}
				m_LastFuncVarList.clear();
				m_LastIdentifierBeforeBracketOpen.Clear();
			}
		}
	}
	m_State = BASE;
	AddLocalInstruction(Key(a_TokenName), false, m_LastFunctionName);
	AddGlobalInstruction(Key(a_TokenName), false, m_LastGlobalVarName);
	m_LastTypeQualifierList.clear();
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ParserHLSL::Parse_OpenBrace(std::string& a_TokenName)
{
	m_State = BASE;
	AddLocalInstruction(Key(a_TokenName), false, m_LastFunctionName);
	AddGlobalInstruction(Key(a_TokenName), false, m_LastGlobalVarName);

	if (m_CurrVarType != GLOBAL_VAR_INIT)
	{
		m_LastTypeQualifierList.clear();
	}

	if (m_LastFunctionName.GetString() != "")
	{
		if (m_ScopeState == GLOBAL)
		{
			m_ConditionalDepth = 0;
			m_ScopeState = IN_FUNCTION;
		}
		else
		{
			m_ConditionalDepth++;
			m_ScopeState = CONDITIONAL;
		}
	}
	else
	{
		if (m_LastDataType == "struct")
		{
			m_CurrVarType = STRUCT_VAR;
			m_LastStructName = m_LastIdentifier;
			m_Database->AddStruct(m_LastIdentifier);
		}
	}
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ParserHLSL::Parse_CloseBrace(std::string& a_TokenName)
{
	AddGlobalInstruction(Key(a_TokenName), false, m_LastGlobalVarName);
	if (m_ScopeState == CONDITIONAL)
	{
		m_Database->AddLocalInstruction(Key(a_TokenName), false, m_LastFunctionName);
		m_ConditionalDepth--;

		if (m_ConditionalDepth == 0)
		{
			m_ScopeState = IN_FUNCTION;
		}
	}
	else
	{
		m_ScopeState = GLOBAL;
		m_LastFunctionName.Clear();
		m_LastBufferName.Clear();
		m_LastStructName.Clear();
	}
	m_LastTypeQualifierList.clear();
	if (m_ScopeState == GLOBAL)
		m_CurrVarType = GLOBAL_VAR;

	m_State = BASE;
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ParserHLSL::Parse_OpenSquareBracket(std::string& a_TokenName)
{
	if (m_CurrVarType == GLOBAL_VAR || m_CurrVarType == GLOBAL_VAR_INIT)
	{
		if (m_ScopeState == GLOBAL)
		{
			if (!m_LastTypeQualifierList.empty() && m_LastIdentifierBeforeBracketOpen.IsEmpty())
			{
				m_Database->AddGlobalVariable(m_LastIdentifier, m_LastTypeQualifierList);
				m_LastGlobalVarName = m_LastIdentifier;
				m_CurrVarType = GLOBAL_VAR_INIT;
			}
			else if (!m_LastIdentifierBeforeBracketOpen.IsEmpty())
			{
				// Add extra instructions on function argument declaration
				m_Database->AddLocalInstruction(Key(a_TokenName), false, m_LastIdentifierBeforeBracketOpen + "." + m_LastIdentifier);
			}
		}
	}
	else if (m_CurrVarType == STRUCT_VAR)
	{
		m_Database->AddLocalInstruction(Key(a_TokenName), false, m_LastStructName + "." + m_LastIdentifier);
		m_CurrVarType = STRUCT_VAR_INIT;
	}
	else if (m_CurrVarType == UNIFORM_VAR)
	{
		m_Database->AddLocalInstruction(Key(a_TokenName), false, m_LastBufferName + "." + m_LastIdentifier);
		m_CurrVarType = UNIFORM_VAR_INIT;
	}
	AddLocalInstruction(Key(a_TokenName), false, m_LastFunctionName);
	AddGlobalInstruction(Key(a_TokenName), false, m_LastGlobalVarName);
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ParserHLSL::Parse_CloseSquareBracket(std::string& a_TokenName)
{
	if (m_CurrVarType == STRUCT_VAR_INIT)
	{
		m_Database->AddLocalInstruction(Key(a_TokenName), false, m_LastStructName + "." + m_LastIdentifier);
	}
	else if (m_CurrVarType == UNIFORM_VAR_INIT)
	{
		m_Database->AddLocalInstruction(Key(a_TokenName), false, m_LastBufferName + "." + m_LastIdentifier);
	}

	// Add extra instructions on function argument declaration
	if (m_CurrVarType == GLOBAL_VAR || m_CurrVarType == GLOBAL_VAR_INIT)
	{
		if (m_ScopeState == GLOBAL)
		{
			if (!m_LastIdentifierBeforeBracketOpen.IsEmpty())
			{
				m_Database->AddLocalInstruction(Key(a_TokenName), false, m_LastIdentifierBeforeBracketOpen + "." + m_LastIdentifier);
			}
		}
	}
	AddLocalInstruction(Key(a_TokenName), false, m_LastFunctionName);
	AddGlobalInstruction(Key(a_TokenName), false, m_LastGlobalVarName);
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ParserHLSL::Parse_While(std::string& a_TokenName)
{
	if (m_ScopeState == IN_FUNCTION || m_ScopeState == CONDITIONAL)
	{
		m_Database->AddLocalInstruction(Key(a_TokenName), false, m_LastFunctionName);
	}
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ParserHLSL::Parse_Do(std::string& a_TokenName)
{
	if (m_ScopeState == IN_FUNCTION || m_ScopeState == CONDITIONAL)
	{
		m_Database->AddLocalInstruction(Key(a_TokenName), false, m_LastFunctionName);
	}
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ParserHLSL::Parse_Return(std::string& a_TokenName)
{
	if (m_ScopeState == IN_FUNCTION || m_ScopeState == CONDITIONAL)
	{
		m_Database->AddLocalInstruction(Key(a_TokenName), false, m_LastFunctionName);
	}
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ParserHLSL::Parse_If(std::string& a_TokenName)
{
	if (m_ScopeState == IN_FUNCTION || m_ScopeState == CONDITIONAL)
	{
		m_Database->AddLocalInstruction(Key(a_TokenName), false, m_LastFunctionName);
	}
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ParserHLSL::Parse_Else(std::string& a_TokenName)
{
	if (m_ScopeState == IN_FUNCTION || m_ScopeState == CONDITIONAL)
	{
		m_Database->AddLocalInstruction(Key(a_TokenName), false, m_LastFunctionName);
	}
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ParserHLSL::Parse_For(std::string& a_TokenName)
{
	if (m_ScopeState == IN_FUNCTION || m_ScopeState == CONDITIONAL)
	{
		m_Database->AddLocalInstruction(Key(a_TokenName), false, m_LastFunctionName);
	}
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ParserHLSL::Parse_Switch(std::string& a_TokenName)
{
	if (m_ScopeState == IN_FUNCTION || m_ScopeState == CONDITIONAL)
	{
		m_Database->AddLocalInstruction(Key(a_TokenName), false, m_LastFunctionName);
	}
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ParserHLSL::Parse_Continue(std::string& a_TokenName)
{
	if (m_ScopeState == CONDITIONAL)
	{
		m_Database->AddLocalInstruction(Key(a_TokenName), false, m_LastFunctionName);
	}
	else
	{
		m_LastError = "Could not parse continue. Possible syntax error.";
		return SAVVY_SHADER_SYNTAX_ERROR;
	}
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ParserHLSL::Parse_Case(std::string& a_TokenName)
{
	if (m_ScopeState == CONDITIONAL)
	{
		m_Database->AddLocalInstruction(Key(a_TokenName), false, m_LastFunctionName);
	}
	else
	{
		m_LastError = "Could not parse case. Possible syntax error.";
		return SAVVY_SHADER_SYNTAX_ERROR;
	}
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ParserHLSL::Parse_Break(std::string& a_TokenName)
{
	if (m_ScopeState == CONDITIONAL)
	{
		m_Database->AddLocalInstruction(Key(a_TokenName), false, m_LastFunctionName);
	}
	else
	{
		m_LastError = "Could not parse break. Possible syntax error.";
		return SAVVY_SHADER_SYNTAX_ERROR;
	}
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ParserHLSL::Parse_Unknown(std::string& a_TokenName)
{
	m_LastError = "Could not parse token. Unrecognized token name.";
	return SAVVY_UNRECOGNIZED_TOKEN;
}

void Savvy::Internal::ParserHLSL::HandleBuiltInSemantic(Key& a_TokenName)
{
	if (a_TokenName == "SV_Position" || a_TokenName == "SV_POSITION")
	{
		Database::KeyList list;
		list.push_back("float4");
		m_Database->AddStructVariable(m_LastStructName, m_LastIdentifier, list);
		m_LastSemantic = a_TokenName;
		m_Database->AddSemantic(m_LastStructName, m_LastIdentifier, m_LastSemantic);
	}
	else if (a_TokenName == "psize" || a_TokenName == "PSIZE")
	{
		Database::KeyList list;
		list.push_back("float");
		m_Database->AddStructVariable(m_LastStructName, m_LastIdentifier, list);
		m_LastSemantic = a_TokenName;
		m_Database->AddSemantic(m_LastStructName, m_LastIdentifier, m_LastSemantic);
	}
	else if (a_TokenName == "SV_Target" || a_TokenName == "SV_TARGET")
	{
		Database::KeyList list;
		list.push_back("float4");
		m_Database->AddStructVariable(m_LastStructName, m_LastIdentifier, list);
		m_LastSemantic = a_TokenName;
		m_Database->AddSemantic(m_LastStructName, m_LastIdentifier, m_LastSemantic);
	}
	else if (a_TokenName == "SV_Target" || a_TokenName == "SV_TARGET")
	{
		Database::KeyList list;
		list.push_back("float4");
		m_Database->AddStructVariable(m_LastStructName, m_LastIdentifier, list);
		m_LastSemantic = a_TokenName;
		m_Database->AddSemantic(m_LastStructName, m_LastIdentifier, m_LastSemantic);
	}
	else if (a_TokenName == "SV_IsFrontFace" || a_TokenName == "SV_ISFONTFACE")
	{
		Database::KeyList list;
		list.push_back("bool");
		m_Database->AddStructVariable(m_LastStructName, m_LastIdentifier, list);
		m_LastSemantic = a_TokenName;
		m_Database->AddSemantic(m_LastStructName, m_LastIdentifier, m_LastSemantic);
	}
	else if (a_TokenName == "SV_Depth" || a_TokenName == "SV_DEPTH")
	{
		Database::KeyList list;
		list.push_back("float");
		m_Database->AddStructVariable(m_LastStructName, m_LastIdentifier, list);
		m_LastSemantic = a_TokenName;
		m_Database->AddSemantic(m_LastStructName, m_LastIdentifier, m_LastSemantic);
	}
	else if (a_TokenName.Contains("SV_CullDistance") || a_TokenName.Contains("SV_CULLDISTANCE"))
	{
		Database::KeyList list;
		list.push_back("float");
		m_Database->AddStructVariable(m_LastStructName, m_LastIdentifier, list);
		m_LastSemantic = a_TokenName;
		m_Database->AddSemantic(m_LastStructName, m_LastIdentifier, m_LastSemantic);
	}
	else if (a_TokenName.Contains("SV_ClipDistance") || a_TokenName.Contains("SV_CLIPDISTANCE"))
	{
		Database::KeyList list;
		list.push_back("float");
		m_Database->AddStructVariable(m_LastStructName, m_LastIdentifier, list);
		m_LastSemantic = a_TokenName;
		m_Database->AddSemantic(m_LastStructName, m_LastIdentifier, m_LastSemantic);
	}
}

Savvy::ResultCode Savvy::Internal::ParserHLSL::Parse_Preprocessor(std::string& a_TokenName)
{
	AddLocalInstruction(Key(a_TokenName + "\n\t"), false, m_LastFunctionName);

	Key tokenKey = a_TokenName;
	if (tokenKey.Contains("define") || tokenKey.Contains("error"))
	{
		m_Database->AddDefine(tokenKey);
	}
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ParserHLSL::Parse_NewLine(std::string& a_TokenName)
{
	return SAVVY_OK;
}
