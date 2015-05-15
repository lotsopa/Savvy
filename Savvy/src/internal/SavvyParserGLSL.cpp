#include "SavvyPCH.h"
#include "SavvyDatabaseGLSL.h"
#include "SavvyParserGLSL.h"

#define STATE_FUNC_MAP_INSERT(a, b) m_StateFuncMap.insert(std::make_pair(a, std::bind(&b, this, std::placeholders::_1)))

Savvy::Internal::ParserGLSL::ParserGLSL() : m_State(BASE), m_OldState(BASE), m_CurrVarType(GLOBAL_VAR), m_ScopeState(GLOBAL),
m_ConditionalDepth(0), m_LastLayoutIndex(0), m_AddedLocalVar(false)
{
	// Bind state machine functions to appropriate tokens
	STATE_FUNC_MAP_INSERT(SAVVY_EOF, Savvy::Internal::ParserGLSL::Parse_EOF);
	STATE_FUNC_MAP_INSERT(SAVVY_IDENTIFIER, Savvy::Internal::ParserGLSL::Parse_ID);
	STATE_FUNC_MAP_INSERT(SAVVY_INTEGER_VALUE, Savvy::Internal::ParserGLSL::Parse_IntValue);
	STATE_FUNC_MAP_INSERT(SAVVY_FLOAT_VALUE, Savvy::Internal::ParserGLSL::Parse_FloatValue);
	STATE_FUNC_MAP_INSERT(SAVVY_BOOL_VALUE, Savvy::Internal::ParserGLSL::Parse_BoolValue);
	STATE_FUNC_MAP_INSERT(SAVVY_OPEN_BRACKET, Savvy::Internal::ParserGLSL::Parse_OpenBracket);
	STATE_FUNC_MAP_INSERT(SAVVY_CLOSE_BRACKET, Savvy::Internal::ParserGLSL::Parse_CloseBracket);
	STATE_FUNC_MAP_INSERT(SAVVY_OPEN_SQUARE_BRACKET, Savvy::Internal::ParserGLSL::Parse_OpenSquareBracket);
	STATE_FUNC_MAP_INSERT(SAVVY_CLOSE_SQUARE_BRACKET, Savvy::Internal::ParserGLSL::Parse_CloseSquareBracket);
	STATE_FUNC_MAP_INSERT(SAVVY_OPEN_BRACE, Savvy::Internal::ParserGLSL::Parse_OpenBrace);
	STATE_FUNC_MAP_INSERT(SAVVY_CLOSE_BRACE, Savvy::Internal::ParserGLSL::Parse_CloseBrace);
	STATE_FUNC_MAP_INSERT(SAVVY_DATA_TYPE, Savvy::Internal::ParserGLSL::Parse_DataType);
	STATE_FUNC_MAP_INSERT(SAVVY_OPERATOR, Savvy::Internal::ParserGLSL::Parse_Operator);
	STATE_FUNC_MAP_INSERT(SAVVY_INSTRUCTION_END, Savvy::Internal::ParserGLSL::Parse_InstructionEnd);
	STATE_FUNC_MAP_INSERT(SAVVY_RETURN_KEYWORD, Savvy::Internal::ParserGLSL::Parse_Return);
	STATE_FUNC_MAP_INSERT(SAVVY_IF_STATEMENT, Savvy::Internal::ParserGLSL::Parse_If);
	STATE_FUNC_MAP_INSERT(SAVVY_ELSE_STATEMENT, Savvy::Internal::ParserGLSL::Parse_Else);
	STATE_FUNC_MAP_INSERT(SAVVY_SWITCH_STATEMENT, Savvy::Internal::ParserGLSL::Parse_Switch);
	STATE_FUNC_MAP_INSERT(SAVVY_CASE_STATEMENT, Savvy::Internal::ParserGLSL::Parse_Case);
	STATE_FUNC_MAP_INSERT(SAVVY_BREAK_STATEMENT, Savvy::Internal::ParserGLSL::Parse_Break);
	STATE_FUNC_MAP_INSERT(SAVVY_FOR_STATEMENT, Savvy::Internal::ParserGLSL::Parse_For);
	STATE_FUNC_MAP_INSERT(SAVVY_DO_STATEMENT, Savvy::Internal::ParserGLSL::Parse_Do);
	STATE_FUNC_MAP_INSERT(SAVVY_WHILE_STATEMENT, Savvy::Internal::ParserGLSL::Parse_While);
	STATE_FUNC_MAP_INSERT(SAVVY_CONTINUE_STATEMENT, Savvy::Internal::ParserGLSL::Parse_Continue);
	STATE_FUNC_MAP_INSERT(SAVVY_BUILT_IN_FUNCTION, Savvy::Internal::ParserGLSL::Parse_BuiltInFunc);
	STATE_FUNC_MAP_INSERT(SAVVY_LANGUAGE_SPECIFIC_KEYWORD, Savvy::Internal::ParserGLSL::Parse_LangSpecific);
	STATE_FUNC_MAP_INSERT(SAVVY_PREPROCESSOR, Savvy::Internal::ParserGLSL::Parse_Preprocessor);
	STATE_FUNC_MAP_INSERT(SAVVY_UNKNOWN_TOKEN, Savvy::Internal::ParserGLSL::Parse_Unknown);
	STATE_FUNC_MAP_INSERT(SAVVY_NEW_LINE, Savvy::Internal::ParserGLSL::Parse_NewLine);

}

Savvy::Internal::ParserGLSL::~ParserGLSL()
{

}

Savvy::ResultCode Savvy::Internal::ParserGLSL::ParseToken(uint32 a_Token, std::string& a_TokenName)
{
	m_LastError.clear();
	ResultCode res = m_StateFuncMap.at(a_Token)(a_TokenName);

	//Debug
#if SAVVY_DEBUG == 1
	std::cout << a_TokenName << std::endl;
#endif
	return res;
}

void Savvy::Internal::ParserGLSL::Reset()
{
	m_State = BASE;
	m_OldState = BASE;
	m_ScopeState = GLOBAL;
	m_CurrVarType = GLOBAL_VAR;
	m_LastDataType.Clear();
	m_LastBufferName.Clear();
	m_LastIdentifier.Clear();
	m_LastFunctionName.Clear();
	m_LastStructName.Clear();
	m_LastIdentifierBeforeBracketOpen.Clear();
	m_LastFuncVarList.clear();
	m_LastTypeQualifierList.clear();
	m_LastGlobalVarName.Clear();
	m_InputOrOutput.Clear();
	m_ConditionalDepth = 0;
	m_LastLayoutIndex = 0;
	m_AddedLocalVar = false;
}

void Savvy::Internal::ParserGLSL::AddGlobalInstruction(Key& a_Instruction, bool a_Translate, Key& a_VarName)
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

void Savvy::Internal::ParserGLSL::AddLocalInstruction(Key& a_Instruction, bool a_Translate, Key& a_FuncName)
{
	if (m_ScopeState != GLOBAL && m_ScopeState != LAYOUT_LOCATION_DECL)
	{
		m_Database->AddLocalInstruction(a_Instruction, a_Translate, a_FuncName);
	}
}

Savvy::ResultCode Savvy::Internal::ParserGLSL::Parse_EOF(std::string& a_TokenName)
{
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ParserGLSL::Parse_ID(std::string& a_TokenName)
{
	if (m_State == DATA_TYPE_DECLARATION || m_State == LANGUAGE_SPECIFIC_DECLARATION || m_State == BASE ||
		m_State == IDENTIFIER_USE || m_State == IDENTIFIER_DECLARATION)
	{
		if (m_State == LANGUAGE_SPECIFIC_DECLARATION)
		{
			// Uniform buffer declaration - save the name
			if (m_CurrVarType == UNIFORM_VAR)
			{
				m_LastBufferName = a_TokenName;

				if (m_ScopeState == LAYOUT_LOCATION_DECL)
				{
					m_Database->AddUniformBuffer(m_LastBufferName, m_LastLayoutIndex);
				}
				else
				{
					m_Database->AddUniformBuffer(m_LastBufferName);
				}
			}
			else if (m_CurrVarType == ORIGINAL_SEMANTIC)
			{
				m_Database->AddSemantic(m_InputOrOutput, m_LastIdentifier, Key(a_TokenName));
			}
			else if (m_CurrVarType == GENERIC_BUFFER_VAR)
			{
				m_LastBufferName = a_TokenName;
				if (m_ScopeState == LAYOUT_LOCATION_DECL)
				{
					m_Database->AddGenericBuffer(m_LastBufferName, m_LastLayoutIndex);
				}
				else
				{
					m_Database->AddGenericBuffer(m_LastBufferName);
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
				}

				ResultCode res;
				res = m_Database->AddLocalVariable(Key(a_TokenName), m_LastTypeQualifierList, m_LastFunctionName);
				m_AddedLocalVar = true;
				if (res != SAVVY_OK)
				{
					m_LastError = "Could not add local variable " + a_TokenName + " inside function " + m_LastFunctionName.GetString() + ". Local variable already exists.";
					return res;
				}
			}

			if (m_ScopeState == GLOBAL)
			{
				if (m_LastIdentifierBeforeBracketOpen.GetString() != "")
				{
					m_LastFuncVariable.m_Name = a_TokenName;
					m_LastFuncVarList.push_back(m_LastFuncVariable);
				}
				// Check if it's a user defined data type
				if (m_Database->StructExists(a_TokenName))
				{
					m_LastTypeQualifierList.push_back(a_TokenName);
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

			if (m_CurrVarType == ORIGINAL_SEMANTIC)
			{
				m_CurrVarType = GLOBAL_VAR;
				m_State = BASE;
			}
			else
			{
				m_State = IDENTIFIER_DECLARATION;
			}
		}
	}
	else if (m_State == OPERATION)
	{
		// Check if it's a user defined data type
		if (m_Database->StructExists(a_TokenName))
		{
			if (m_LastIdentifierBeforeBracketOpen.GetString() != "")
			{
				m_LastTypeQualifierList.push_back(a_TokenName);
				m_LastFuncVariable.m_QualifierList = m_LastTypeQualifierList;
			}
			m_LastDataType = a_TokenName;
			m_State = DATA_TYPE_DECLARATION;
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
					res = m_Database->AddLocalVariable(Key(a_TokenName), m_LastTypeQualifierList, m_LastFunctionName);
					m_AddedLocalVar = true;
					if (res != SAVVY_OK)
					{
						m_LastError = "Could not add local variable " + a_TokenName + " inside function " + m_LastFunctionName.GetString() + ". Local variable already exists.";
						return res;
					}
				}
			}
			
			m_State = IDENTIFIER_USE;
		}
	}
	else
	{
		m_LastError = "Could not parse identifier " + a_TokenName + ". Possible syntax error.";
		return SAVVY_SHADER_SYNTAX_ERROR;
	}

	AddLocalInstruction(Key(a_TokenName), false, m_LastFunctionName);
	AddGlobalInstruction(Key(a_TokenName), false, m_LastGlobalVarName);
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ParserGLSL::Parse_DataType(std::string& a_TokenName)
{
	if (m_State == BASE || m_State == LANGUAGE_SPECIFIC_DECLARATION || m_State == OPERATION || m_State == DATA_TYPE_DECLARATION || m_State == IDENTIFIER_USE)
	{
		m_State = DATA_TYPE_DECLARATION;
		m_LastDataType = a_TokenName;

		m_LastTypeQualifierList.push_back(m_LastDataType);
		if (m_ScopeState == GLOBAL || m_ScopeState == LAYOUT_LOCATION_DECL)
		{
			if (m_LastIdentifierBeforeBracketOpen.GetString() != "")
			{
				m_LastFuncVariable.m_QualifierList = m_LastTypeQualifierList;
			}
		}

		AddGlobalInstruction(Key(a_TokenName), true, m_LastGlobalVarName);
	}
	else
	{
		m_LastError = "Could not parse data type " + a_TokenName + ". Possible syntax error.";
		return SAVVY_SHADER_SYNTAX_ERROR;
	}

	AddLocalInstruction(Key(a_TokenName), true, m_LastFunctionName);
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ParserGLSL::Parse_Operator(std::string& a_TokenName)
{
	if (m_State == OPERATION || m_State == IDENTIFIER_DECLARATION || m_State == VALUE_DECLARATION || 
		m_State == LANGUAGE_SPECIFIC_DECLARATION || m_State == IDENTIFIER_USE || m_State == DATA_TYPE_DECLARATION)
	{
		m_State = OPERATION;
	}
	else if (m_State == BASE)
	{
		if (m_OldState == IN_PLACE_DECLARATION)
		{
			m_State = OPERATION;
		}

		if (a_TokenName == ".")
		{
			m_State = OPERATION;
		}
	}
	else
	{
		m_LastError = "Could not parse operator " + a_TokenName + ". Possible syntax error.";
		return SAVVY_SHADER_SYNTAX_ERROR;
	}

	AddLocalInstruction(Key(a_TokenName), false, m_LastFunctionName);
	if (m_ScopeState == LAYOUT_LOCATION_DECL)
	{
		if (a_TokenName != "=" && a_TokenName != ",")
		{
			m_LastError = "Could not parse operator " + a_TokenName + ". Possible syntax error.";
			return SAVVY_SHADER_SYNTAX_ERROR;
		}
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
		}
	}

	if (m_CurrVarType == GLOBAL_VAR || m_CurrVarType == GLOBAL_VAR_INIT)
	{
		if (m_ScopeState == GLOBAL)
		{
			if (!m_LastTypeQualifierList.empty() && m_LastIdentifierBeforeBracketOpen.IsEmpty())
			{
				m_Database->AddGlobalVariable(m_LastIdentifier, m_LastTypeQualifierList);
				m_Database->AddLocalInstruction(Key(a_TokenName), false, m_LastIdentifier);
				m_LastGlobalVarName = m_LastIdentifier;
				m_CurrVarType = GLOBAL_VAR_INIT;
			}
		}
	}

	if (a_TokenName == "=")
	{
		if (m_CurrVarType == UNIFORM_VAR)
		{
			m_Database->AddUniformVariable(m_LastIdentifier, m_LastTypeQualifierList, m_LastBufferName);
		}
	}
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ParserGLSL::Parse_IntValue(std::string& a_TokenName)
{
	if (m_State != LANGUAGE_SPECIFIC_DECLARATION && m_State != BASE)
	{
		m_State = VALUE_DECLARATION;
	}

	AddLocalInstruction(Key(a_TokenName), false, m_LastFunctionName);
	if (m_ScopeState == LAYOUT_LOCATION_DECL)
	{
		m_LastLayoutIndex = std::stoi(a_TokenName);
	}

	AddGlobalInstruction(Key(a_TokenName), false, m_LastGlobalVarName);
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ParserGLSL::Parse_FloatValue(std::string& a_TokenName)
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

	AddLocalInstruction(Key(a_TokenName), false, m_LastFunctionName);
	if (m_ScopeState == LAYOUT_LOCATION_DECL)
	{
		m_LastLayoutIndex = std::stoi(a_TokenName);
	}

	AddGlobalInstruction(Key(a_TokenName), false, m_LastGlobalVarName);
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ParserGLSL::Parse_BoolValue(std::string& a_TokenName)
{
	if (m_State != LANGUAGE_SPECIFIC_DECLARATION && m_State != BASE)
	{
		m_State = VALUE_DECLARATION;
	}

	AddLocalInstruction(Key(a_TokenName), false, m_LastFunctionName);
	AddGlobalInstruction(Key(a_TokenName), false, m_LastGlobalVarName);
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ParserGLSL::Parse_BuiltInFunc(std::string& a_TokenName)
{
	// Handle cases when a variable has the same name as a built in function
	if (m_State == DATA_TYPE_DECLARATION && m_ScopeState != GLOBAL)
	{
		m_LastIdentifier = a_TokenName;
		m_Database->AddLocalVariable(m_LastIdentifier, m_LastTypeQualifierList, m_LastFunctionName);
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
				res = m_Database->AddLocalVariable(Key(a_TokenName), m_LastTypeQualifierList, m_LastFunctionName);
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

Savvy::ResultCode Savvy::Internal::ParserGLSL::Parse_LangSpecific(std::string& a_TokenName)
{
	if (m_State == BASE || m_State == LANGUAGE_SPECIFIC_DECLARATION || m_State == OPERATION || m_State == DATA_TYPE_DECLARATION)
	{
		m_State = LANGUAGE_SPECIFIC_DECLARATION;

		HandleBuiltInVariable(Key(a_TokenName));

		if (a_TokenName == "in")
		{
			if (m_ScopeState == GLOBAL || m_ScopeState == LAYOUT_LOCATION_DECL)
			{
				if (m_LastIdentifierBeforeBracketOpen.GetString() != "")
				{
					m_State = DATA_TYPE_DECLARATION;
					m_LastDataType = a_TokenName;
					m_LastTypeQualifierList.push_back(m_LastDataType);
					m_LastFuncVariable.m_QualifierList = m_LastTypeQualifierList;
				}
				else
				{
					m_CurrVarType = INPUT_VAR;
					m_InputOrOutput.SetString("in");
				}
			}
		}
		else if (a_TokenName == "out")
		{
			if (m_ScopeState == GLOBAL || m_ScopeState == LAYOUT_LOCATION_DECL)
			{
				if (m_LastIdentifierBeforeBracketOpen.GetString() != "")
				{
					m_State = DATA_TYPE_DECLARATION;
					m_LastDataType = a_TokenName;
					m_LastTypeQualifierList.push_back(m_LastDataType);
					m_LastFuncVariable.m_QualifierList = m_LastTypeQualifierList;
				}
				else
				{
					m_CurrVarType = OUTPUT_VAR;
					m_InputOrOutput.SetString("out");
				}
			}
		}
		else if (a_TokenName == "inout")
		{
			if (m_ScopeState == GLOBAL || m_ScopeState == LAYOUT_LOCATION_DECL)
			{
				if (m_LastIdentifierBeforeBracketOpen.GetString() != "")
				{
					m_State = DATA_TYPE_DECLARATION;
					m_LastDataType = a_TokenName;
					m_LastTypeQualifierList.push_back(m_LastDataType);
					m_LastFuncVariable.m_QualifierList = m_LastTypeQualifierList;
				}
			}
		}
		else if (a_TokenName == "uniform")
		{
			m_CurrVarType = UNIFORM_VAR;
		}
		else if (a_TokenName == "buffer" && m_ScopeState == GLOBAL)
		{
			m_CurrVarType = GENERIC_BUFFER_VAR;
		}
		else if (a_TokenName == "location" || a_TokenName == "binding")
		{
			m_ScopeState = LAYOUT_LOCATION_DECL;
		}
		else if (a_TokenName == SAVVY_ORIGINAL_SEMANTIC_DECL)
		{
			m_CurrVarType = ORIGINAL_SEMANTIC;
		}
	}
	else
	{
		m_LastError = "Could not parse language specific keyword " + a_TokenName + ". Possible syntax error.";
		return SAVVY_SHADER_SYNTAX_ERROR;
	}

	AddLocalInstruction(Key(a_TokenName), false, m_LastFunctionName);
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ParserGLSL::Parse_InstructionEnd(std::string& a_TokenName)
{
	if (m_State == BASE || m_State == IDENTIFIER_DECLARATION || m_State == IDENTIFIER_USE || 
		m_State == VALUE_DECLARATION || m_State == LANGUAGE_SPECIFIC_DECLARATION || m_State == OPERATION)
	{
		if (m_State == IDENTIFIER_DECLARATION && m_ScopeState != IN_FUNCTION)
		{
			ResultCode addVarResult = SAVVY_OK;
			switch (m_CurrVarType)
			{
			case INPUT_VAR:
				if (m_ScopeState == LAYOUT_LOCATION_DECL)
				{
					addVarResult = m_Database->AddInputVariable(m_LastIdentifier, m_LastTypeQualifierList, m_LastLayoutIndex);
					m_ScopeState = GLOBAL;
				}
				else
				{
					addVarResult = m_Database->AddInputVariable(m_LastIdentifier, m_LastTypeQualifierList);
				}
				break;
			case OUTPUT_VAR:
				if (m_ScopeState == LAYOUT_LOCATION_DECL)
				{
					addVarResult = m_Database->AddOutputVariable(m_LastIdentifier, m_LastTypeQualifierList, m_LastLayoutIndex);
					m_ScopeState = GLOBAL;
				}
				else
				{
					addVarResult = m_Database->AddOutputVariable(m_LastIdentifier, m_LastTypeQualifierList);
				}
				break;
			case STRUCT_VAR_INIT:
			case STRUCT_VAR:
			{
				addVarResult = m_Database->AddStructVariable(m_LastStructName, m_LastIdentifier, m_LastTypeQualifierList);
				m_ScopeState = GLOBAL;
				m_CurrVarType = STRUCT_VAR;
			}
				break;
			case UNIFORM_VAR_INIT:
			case UNIFORM_VAR:

				if (m_LastDataType.Contains("sampler"))
				{
					if (m_ScopeState == LAYOUT_LOCATION_DECL)
					{
						addVarResult = m_Database->AddSampler(m_LastIdentifier, m_LastDataType, m_LastLayoutIndex);
						m_ScopeState = GLOBAL;
					}
					else
					{
						addVarResult = m_Database->AddSampler(m_LastIdentifier, m_LastDataType);
						m_ScopeState = GLOBAL;
					}
				}
				else
				{
					if (m_ScopeState == GLOBAL || m_ScopeState == LAYOUT_LOCATION_DECL)
						addVarResult = m_Database->AddUniformVariable(m_LastIdentifier, m_LastTypeQualifierList, m_LastBufferName);
				}

				break;

			case GENERIC_BUFFER_VAR:

				if (m_ScopeState == GLOBAL || m_ScopeState == LAYOUT_LOCATION_DECL)
					addVarResult = m_Database->AddGenericBufferVariable(m_LastIdentifier, m_LastTypeQualifierList, m_LastBufferName);

				break;

			case GLOBAL_VAR:
			{
				// Global variable
				if (m_ScopeState == GLOBAL)
				{
					if (!m_LastTypeQualifierList.empty())
					{
						m_Database->AddGlobalVariable(m_LastIdentifier, m_LastTypeQualifierList);
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
		m_OldState = BASE;

		if (m_LastBufferName.IsEmpty() && m_LastStructName.IsEmpty())
			m_CurrVarType = GLOBAL_VAR;

		m_AddedLocalVar = false;
		if (m_CurrVarType == GLOBAL_VAR_INIT)
		{
			m_CurrVarType = GLOBAL_VAR;
			m_LastGlobalVarName.Clear();
		}
	}
	else
	{
		m_LastError = "Could not parse semicolon. Possible syntax error.";
		return SAVVY_SHADER_SYNTAX_ERROR;
	}

	AddLocalInstruction(Key(a_TokenName), false, m_LastFunctionName);
	m_LastTypeQualifierList.clear();
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ParserGLSL::Parse_OpenBracket(std::string& a_TokenName)
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

	AddLocalInstruction(Key(a_TokenName), false, m_LastFunctionName);
	AddGlobalInstruction(Key(a_TokenName), false, m_LastGlobalVarName);

	if (!m_LastIdentifierBeforeBracketOpen.IsEmpty())
	{
		m_LastTypeQualifierList.clear();
		m_LastFuncVarList.clear();
	}
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ParserGLSL::Parse_CloseBracket(std::string& a_TokenName)
{
	if (m_State == IDENTIFIER_DECLARATION || m_State == BASE)
	{
		if (m_ScopeState == GLOBAL)
		{
			// Function
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

	AddLocalInstruction(Key(a_TokenName), false, m_LastFunctionName);
	AddGlobalInstruction(Key(a_TokenName), false, m_LastGlobalVarName);
	m_State = BASE;
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ParserGLSL::Parse_OpenBrace(std::string& a_TokenName)
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

Savvy::ResultCode Savvy::Internal::ParserGLSL::Parse_CloseBrace(std::string& a_TokenName)
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
		if (m_State == IN_FUNCTION)
			m_LastFunctionName.Clear();

		m_ScopeState = GLOBAL;
		m_LastBufferName.Clear();
	}

	m_State = BASE;
	m_LastTypeQualifierList.clear();
	
	if (m_ScopeState == GLOBAL)
		m_CurrVarType = GLOBAL_VAR;
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ParserGLSL::Parse_OpenSquareBracket(std::string& a_TokenName)
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

Savvy::ResultCode Savvy::Internal::ParserGLSL::Parse_CloseSquareBracket(std::string& a_TokenName)
{
	if (m_CurrVarType == STRUCT_VAR_INIT)
	{
		m_Database->AddLocalInstruction(Key(a_TokenName), false, m_LastStructName + "." + m_LastIdentifier);
	}
	else if (m_CurrVarType == UNIFORM_VAR_INIT)
	{
		m_Database->AddLocalInstruction(Key(a_TokenName), false, m_LastBufferName + "." + m_LastIdentifier);
	}

	AddLocalInstruction(Key(a_TokenName), false, m_LastFunctionName);
	AddGlobalInstruction(Key(a_TokenName), false, m_LastGlobalVarName);
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ParserGLSL::Parse_Do(std::string& a_TokenName)
{
	if (m_ScopeState == IN_FUNCTION || m_ScopeState == CONDITIONAL)
	{
		m_Database->AddLocalInstruction(Key(a_TokenName), false, m_LastFunctionName);
	}
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ParserGLSL::Parse_Return(std::string& a_TokenName)
{
	if (m_ScopeState == IN_FUNCTION || m_ScopeState == CONDITIONAL)
	{
		m_Database->AddLocalInstruction(Key(a_TokenName), false, m_LastFunctionName);
	}
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ParserGLSL::Parse_While(std::string& a_TokenName)
{
	if (m_ScopeState == IN_FUNCTION || m_ScopeState == CONDITIONAL)
	{
		m_Database->AddLocalInstruction(Key(a_TokenName), false, m_LastFunctionName);
	}
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ParserGLSL::Parse_If(std::string& a_TokenName)
{
	if (m_ScopeState == IN_FUNCTION || m_ScopeState == CONDITIONAL)
	{
		m_Database->AddLocalInstruction(Key(a_TokenName), false, m_LastFunctionName);
	}
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ParserGLSL::Parse_Else(std::string& a_TokenName)
{
	if (m_ScopeState == IN_FUNCTION || m_ScopeState == CONDITIONAL)
	{
		m_Database->AddLocalInstruction(Key(a_TokenName), false, m_LastFunctionName);
	}
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ParserGLSL::Parse_For(std::string& a_TokenName)
{
	if (m_ScopeState == IN_FUNCTION || m_ScopeState == CONDITIONAL)
	{
		m_Database->AddLocalInstruction(Key(a_TokenName), false, m_LastFunctionName);
	}
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ParserGLSL::Parse_Switch(std::string& a_TokenName)
{
	if (m_ScopeState == IN_FUNCTION || m_ScopeState == CONDITIONAL)
	{
		m_Database->AddLocalInstruction(Key(a_TokenName), false, m_LastFunctionName);
	}
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ParserGLSL::Parse_Continue(std::string& a_TokenName)
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

Savvy::ResultCode Savvy::Internal::ParserGLSL::Parse_Case(std::string& a_TokenName)
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

Savvy::ResultCode Savvy::Internal::ParserGLSL::Parse_Break(std::string& a_TokenName)
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

Savvy::ResultCode Savvy::Internal::ParserGLSL::Parse_Unknown(std::string& a_TokenName)
{
	m_LastError = "Could not parse token. Unrecognized token name.";
	return SAVVY_UNRECOGNIZED_TOKEN;
}

void Savvy::Internal::ParserGLSL::HandleBuiltInVariable(Key& a_TokenName)
{
	if (a_TokenName == "gl_Position")
	{
		Database::KeyList list;
		list.push_back("vec4");
		m_Database->AddOutputVariable(Key("gl_Position"), list);
	}
	else if (a_TokenName == "gl_PointSize")
	{
		Database::KeyList list;
		list.push_back("float");
		m_Database->AddOutputVariable(Key("gl_PointSize"), list);
	}
	else if (a_TokenName == "gl_FragColor")
	{
		Database::KeyList list;
		list.push_back("vec4");
		m_Database->AddOutputVariable(Key("gl_FragColor"), list);
	}
	else if (a_TokenName == "gl_FragCoord")
	{
		Database::KeyList list;
		list.push_back("vec4");
		m_Database->AddInputVariable(Key("gl_FragCoord"), list);
	}
	else if (a_TokenName == "gl_FrontFacing")
	{
		Database::KeyList list;
		list.push_back("bool");
		m_Database->AddInputVariable(Key("gl_FrontFacing"), list);
	}
	else if (a_TokenName == "gl_PointCoord")
	{
		Database::KeyList list;
		list.push_back("vec2");
		m_Database->AddInputVariable(Key("gl_PointCoord"), list);
	}
	else if (a_TokenName == "gl_FragDepth")
	{
		Database::KeyList list;
		list.push_back("float");
		m_Database->AddOutputVariable(Key("gl_FragDepth"), list);
	}
	else if (a_TokenName.Contains("gl_ClipDistance") || a_TokenName.Contains("gl_CullDistance"))
	{
		Database::KeyList list;
		list.push_back("float");
		m_Database->AddOutputVariable(a_TokenName, list);
	}
}

Savvy::ResultCode Savvy::Internal::ParserGLSL::Parse_Preprocessor(std::string& a_TokenName)
{
	AddLocalInstruction(Key(a_TokenName + "\n\t"), false, m_LastFunctionName);
	Key tokenKey = a_TokenName;
	if (tokenKey.Contains("define") || tokenKey.Contains("error"))
	{
		m_Database->AddDefine(tokenKey);
	}
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ParserGLSL::Parse_NewLine(std::string& a_TokenName)
{
	return SAVVY_OK;
}
