#include "SavvyPCH.h"
#include "SavvyConstructorGLSLToHLSL.h"
#include "SavvyDatabaseGLSL.h"
#include "SavvyFuncConverter.h"
#include "SavvyFuncConverterGLSLToHLSL.h"

Savvy::Internal::ConstructorGLSLToHLSL::ConstructorGLSLToHLSL() : Constructor(), m_CullDistHandled(false), m_ClipDistHandled(false)
{
	m_FuncConverter = new FuncConverterGLSLToHLSL();
}

Savvy::Internal::ConstructorGLSLToHLSL::~ConstructorGLSLToHLSL()
{
	if (m_FuncConverter != NULL)
		delete m_FuncConverter;
}

void Savvy::Internal::ConstructorGLSLToHLSL::Reset()
{
	Constructor::Reset();
	m_CurrFunction.Clear();
	m_FuncToAddInputArg.clear();
	m_FuncToAddOutputArg.clear();
	m_CullDistHandled = false;
	m_ClipDistHandled = false;
	m_FuncConverter->Reset();
}

Savvy::ResultCode Savvy::Internal::ConstructorGLSLToHLSL::Initialize(Key& a_InputEntry, Key& a_OutputEntry, uint32 a_Inputlang, uint32 a_ShaderType, Database* a_Database)
{
	ResultCode res = Constructor::Initialize(a_InputEntry, a_OutputEntry, a_Inputlang, a_ShaderType, a_Database);
	if (res != SAVVY_OK)
		return res;
	res = m_FuncConverter->Initialize(m_Database);

	return res;
}

Savvy::ResultCode Savvy::Internal::ConstructorGLSLToHLSL::Construct(std::ostream& a_OutputStream)
{
	ResultCode res;
	switch (m_CurrShader)
	{
	case VERTEX_SHADER:
	case FRAGMENT_SHADER:
		res = ConstructVertexOrFragment(a_OutputStream);
		break;

	default:
		m_LastError = "Shader type not implemented in Constructor";
		res = SAVVY_UNSUPPORTED_SHADER_TYPE;
		break;
	}

	return res;
}

Savvy::ResultCode Savvy::Internal::ConstructorGLSLToHLSL::ConstructUniformBuffersHLSL(std::ostream& a_OutputStream)
{
	Database::KeyList& uniformBuffers = m_Database->GetUniformBufferList();
	Database::IndexMap& uniformIndices = m_Database->GetUniformBufferIndexMap();

	if (!uniformBuffers.empty())
	{
		Database::KeyVarMultiMap& uniformMap = m_Database->GetUniformMap();
		Database::KeyVarMultiMap::iterator uniformIt1;
		Database::KeyVarMultiMap::iterator uniformIt2;
		Database::IndexMap::iterator uniformIndicesIt;

		for (uniformIndicesIt = uniformIndices.begin(); uniformIndicesIt != uniformIndices.end(); ++uniformIndicesIt)
		{
			Key& key = uniformIndicesIt->second;

			if (key.GetString() == "")
			{
				a_OutputStream << "cbuffer " << "ConstantBuffer" + std::to_string(uniformIndicesIt->first) << " : " << "register(b" << std::to_string(uniformIndicesIt->first) << ")" << std::endl;
			}
			else
			{
				a_OutputStream << "cbuffer " << key.GetString() << " : " << "register(b" << std::to_string(uniformIndicesIt->first) << ")" << std::endl;
			}

			a_OutputStream << "{" << std::endl;

			// Loop through all the variables
			uniformIt1 = uniformMap.lower_bound(key);
			uniformIt2 = uniformMap.upper_bound(key);

			while (uniformIt1 != uniformIt2)
			{
				// Output
				a_OutputStream << "\t";
				Database::KeyList& list = uniformIt1->second.m_QualifierList;

				uint32 size = list.size();

				for (uint32 i = 0; i < size; i++)
				{
					if (HLSLEquivalentExists(list[i]))
					{
						const Key& varType = GetHLSLKeyword(list[i]);
						a_OutputStream << varType.GetString() << " ";
					}
					else
					{
						if (m_Database->StructExists(list[i]))
						{
							a_OutputStream << list[i].GetString() << " ";
						}
						else
						{
							return SAVVY_LOGICAL_ERROR;
						}
					}
				}
				
				a_OutputStream << uniformIt1->second.m_Name.GetString() << ";";
				a_OutputStream << std::endl;
				++uniformIt1;
			}
			a_OutputStream << "};" << std::endl;
			a_OutputStream << std::endl;
		}
	}

	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ConstructorGLSLToHLSL::ConstructSamplersHLSL(std::ostream& a_OutputStream)
{
	Database::KeyVarMap& samplerMap = m_Database->GetSamplerList();
	Database::IndexMap& indexMap = m_Database->GetSamplerIndexMap();

	if (!samplerMap.empty())
	{
		Database::IndexMap::iterator it;

		for (it = indexMap.begin(); it != indexMap.end(); ++it)
		{
			const Key& name = m_Database->GetSamplerWithIndex(it->first);

			for (uint32 i = 0; i < samplerMap[name].m_QualifierList.size(); i++)
			{
				const Key& type = samplerMap[name].m_QualifierList[i];

				if (HLSLEquivalentExists(type))
				{
					const Key& convertedKey = GetHLSLKeyword(type);
					a_OutputStream << convertedKey.GetString() << " ";
				}
				else
				{
					return SAVVY_LOGICAL_ERROR;
				}
			}
			a_OutputStream << name.GetString() << " : ";
			a_OutputStream << "register(t" + std::to_string(it->first) + ");";
			a_OutputStream << std::endl;
			// Create a sampler object
			a_OutputStream << "sampler " << name.GetString() + "Sampler : register(s" + std::to_string(it->first) + ");" << std::endl;
		}
		a_OutputStream << std::endl;
	}
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ConstructorGLSLToHLSL::ConstructInputVariablesHLSL(std::ostream& a_OutputStream)
{
	// Construct Input buffer
	Database::KeyVarMap& inputVarMap = m_Database->GetInputMap();
	Database::IndexMap& inputIndexMap = m_Database->GetInputVariablesIndexMap();
	if (!inputVarMap.empty())
	{
		a_OutputStream << "struct "SAVVY_INPUT_STRUCT_TYPE << std::endl;
		a_OutputStream << "{" << std::endl;

		Database::IndexMap::iterator it;
		for (it = inputIndexMap.begin(); it != inputIndexMap.end(); ++it)
		{
			const Key& varName = m_Database->GetInputVariableWithIndex(it->first);

			// Special cases
			bool handled = HandleBuiltInVariablesIn(varName, a_OutputStream);

			if (!handled)
			{
				a_OutputStream << "\t";
				Database::KeyList& list = inputVarMap[varName].m_QualifierList;
				uint32 sizeType = list.size();

				for (uint32 j = 0; j < sizeType; j++)
				{
					const Key& varKey = list[j];

					if (HLSLEquivalentExists(varKey))
					{
						const Key& varType = GetHLSLKeyword(varKey);
						if (varType.GetString() != "")
							a_OutputStream << varType.GetString() << " ";
					}
					else
					{
						if (m_Database->StructExists(list[j]))
						{
							a_OutputStream << list[j].GetString() << " ";
						}
						else
						{
							return SAVVY_LOGICAL_ERROR;
						}
					}
				}

				a_OutputStream << varName.GetString();

				std::string semantic;

				if (m_Database->SemanticExists(Key("in"), varName))
				{
					semantic = m_Database->GetSemantic(Key("in"), varName).GetString();
				}
				else
				{
					// By default, the semantic for each variable is its index with a "LOCATION_" prefix
					semantic = "LOCATION_" + std::to_string(it->first);
				}
				a_OutputStream << " : " << semantic << ";" << std::endl;
			}
		}
			
		a_OutputStream << "};" << std::endl;
		a_OutputStream << std::endl;
	}
	else
	{
		return SAVVY_LOGICAL_ERROR;
	}

	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ConstructorGLSLToHLSL::ConstructOutputVariablesHLSL(std::ostream& a_OutputStream)
{
	Database::KeyVarMap& outputVarMap = m_Database->GetOutputMap();
	Database::IndexMap& outputIndexMap = m_Database->GetOutputVariablesIndexMap();
	// determine if it's a fragment shader

	if (m_CurrShader == FRAGMENT_SHADER)
		return SAVVY_OK;

	if (!outputVarMap.empty())
	{
		a_OutputStream << "struct "SAVVY_OUTPUT_STRUCT_TYPE << std::endl;
		a_OutputStream << "{" << std::endl;

		uint32 size = outputIndexMap.size();
		Database::IndexMap::iterator it;
		for (it = outputIndexMap.begin(); it != outputIndexMap.end(); ++it)
		{
			const Key& varName = m_Database->GetOutputVariableWithIndex(it->first);

			// Special cases
			bool handled = HandleBuiltInVariablesOut(varName, a_OutputStream);

			if (!handled)
			{
				a_OutputStream << "\t";
				Database::KeyList& list = outputVarMap[varName].m_QualifierList;
				uint32 sizeType = list.size();
				for (uint32 j = 0; j < sizeType; j++)
				{
					const Key& varKey = list[j];
					if (HLSLEquivalentExists(varKey))
					{
						const Key& varType = GetHLSLKeyword(varKey);
						if (varType.GetString() != "")
							a_OutputStream << varType.GetString() << " ";
					}
					else
					{
						if (m_Database->StructExists(list[j]))
						{
							a_OutputStream << list[j].GetString() << " ";
						}
						else
						{
							return SAVVY_LOGICAL_ERROR;
						}
					}
				}
				a_OutputStream << varName.GetString();

				std::string semantic;

				if (m_Database->SemanticExists(Key("out"),varName))
				{
					semantic = m_Database->GetSemantic(Key("out"), varName).GetString();
				}
				else
				{
					// By default, the semantic for each variable is its index with a "LOCATION_" prefix
					semantic = "LOCATION_" + std::to_string(it->first);
				}
				a_OutputStream << " : " << semantic << ";" << std::endl;
			}
		}
		a_OutputStream << "};" << std::endl;
		a_OutputStream << std::endl;
	}

	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ConstructorGLSLToHLSL::ConstructStructsHLSL(std::ostream& a_OutputStream)
{
	Database::KeyList& structList = m_Database->GetStructList();
	Database::KeyVarMultiMap& varMap = m_Database->GetStructVariableMap();
	Database::WordMultiMap& instructionMap = m_Database->GetInstructionMap();

	if (!structList.empty())
	{
		Database::KeyList::iterator structIt;

		for (structIt = structList.begin(); structIt != structList.end(); ++structIt)
		{
			a_OutputStream << "struct " + structIt->GetString() << std::endl;
			a_OutputStream << "{" << std::endl;

			// Variables
			Database::KeyVarMultiMap::iterator varIt1 = varMap.lower_bound(*structIt);
			Database::KeyVarMultiMap::iterator varIt2 = varMap.upper_bound(*structIt);

			while (varIt1 != varIt2)
			{
				a_OutputStream << "\t";
				Database::KeyList& list = varIt1->second.m_QualifierList;
				uint32 sizeType = list.size();
				for (uint32 i = 0; i < sizeType; i++)
				{
					if (HLSLEquivalentExists(list[i]))
					{
						const Key& type = GetHLSLKeyword(list[i]);
						a_OutputStream << type.GetString() + " ";
					}
					else
					{
						if (m_Database->StructExists(list[i]))
						{
							a_OutputStream << list[i].GetString() << " ";
						}
						else
						{
							return SAVVY_LOGICAL_ERROR;
						}
					}
				}
				a_OutputStream << varIt1->second.m_Name.GetString();

				// Do init instructions
				Database::WordMultiMap::iterator it1 = instructionMap.lower_bound(structIt->GetString() + "." + varIt1->second.m_Name.GetString());
				Database::WordMultiMap::iterator it2 = instructionMap.upper_bound(structIt->GetString() + "." + varIt1->second.m_Name.GetString());

				while (it1 != it2)
				{
					a_OutputStream << it1->second.m_Name.GetString() + " ";
					it1++;
				}

				a_OutputStream << ";" << std::endl;
				varIt1++;
			}

			a_OutputStream << "};" << std::endl;
			a_OutputStream << std::endl;
		}
	}

	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ConstructorGLSLToHLSL::ConstructGlobalVarsHLSL(std::ostream& a_OutputStream)
{
	Database::VarList& globalVars = m_Database->GetGlobalVariableList();
	Database::WordMultiMap& instructionMap = m_Database->GetInstructionMap();
	Database::VarList::iterator it;

	for (it = globalVars.begin(); it != globalVars.end(); ++it)
	{
		Database::KeyList& list = (*it).m_QualifierList;
		uint32 sizeType = list.size();
		a_OutputStream << "static ";
		for (uint32 i = 0; i < sizeType; i++)
		{
			if (HLSLEquivalentExists(list[i]))
			{
				const Key& type = GetHLSLKeyword(list[i]);
				a_OutputStream << type.GetString() + " ";
			}
			else
			{
				if (m_Database->StructExists(list[i]))
				{
					a_OutputStream << list[i].GetString() + " ";
				}
				else
				{
					if (m_Database->StructExists(list[i]))
					{
						a_OutputStream << list[i].GetString() << " ";
					}
					else
					{
						return SAVVY_LOGICAL_ERROR;
					}
				}
			}
		}

		// Output name
		std::string line;
		line.append((*it).m_Name.GetString() + " ");

		// Do the initialization instructions if we have any
		Database::WordMultiMap::iterator it1 = instructionMap.lower_bound((*it).m_Name);
		Database::WordMultiMap::iterator it2 = instructionMap.upper_bound((*it).m_Name);

		while (it1 != it2)
		{
			if (it1->second.m_Translate)
			{
				if (HLSLEquivalentExists(it1->second.m_Name))
				{
					const Key& type = GetHLSLKeyword(it1->second.m_Name);

					line.append(type.GetString() + " ");
				}
				else
				{
					if (m_Database->StructExists(it1->second.m_Name))
					{
						a_OutputStream << it1->second.m_Name.GetString() << " ";
					}
					else
					{
						return SAVVY_LOGICAL_ERROR;
					}
				}

			}
			else
			{
				line.append(it1->second.m_Name.GetString() + " ");
			}
			it1++;
		}
		line.append(";");
		TidyLineFormatting(line);
		a_OutputStream << line;
		a_OutputStream << std::endl;
	}
	if (!globalVars.empty())
		a_OutputStream << std::endl;
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ConstructorGLSLToHLSL::ConstructFunctionsHLSL(std::ostream& a_OutputStream)
{
	Database::FunctionList& funcList = m_Database->GetFunctionList();
	Database::WordMultiMap& instructionMap = m_Database->GetInstructionMap();
	ResultCode resCode;
	if (funcList.empty())
	{
		return SAVVY_LOGICAL_ERROR;
	}
	Database::FunctionList::iterator it;

	// Main function is compulsory for any GLSL shader
	bool mainFound = false;
	for (it = funcList.begin(); it != funcList.end(); ++it)
	{
		if (it->m_Name == "main")
		{
			if (it->m_ReturnType == "void")
			{
				mainFound = true;
				break;
			}
		}
	}

	if (!mainFound)
	{
		return SAVVY_LOGICAL_ERROR;
	}

	// Construct functions
	for (it = funcList.begin(); it != funcList.end(); ++it)
	{
		if (it->m_Name == "main")
		{
			if (m_CurrShader == FRAGMENT_SHADER)
			{
				// has to be a fragment shader
				a_OutputStream << "float4" << " " + m_OutputEntry.GetString();
				a_OutputStream << "("SAVVY_INPUT_STRUCT_TYPE" "SAVVY_INPUT_STRUCT_NAME") : SV_TARGET" << std::endl;
			}
			else
			{
				a_OutputStream << SAVVY_OUTPUT_STRUCT_TYPE << " " + m_OutputEntry.GetString();
				a_OutputStream << "("SAVVY_INPUT_STRUCT_TYPE" "SAVVY_INPUT_STRUCT_NAME")" << std::endl;
			}

			a_OutputStream << "{" << std::endl;

			// Function inner instructions
			resCode = ConstructInnerFunctionHLSL(it->m_Name, a_OutputStream);

			a_OutputStream << "}" << std::endl;
			a_OutputStream << std::endl;
		}
		else
		{
			if (it->m_TranslateReturnType)
			{
				if (HLSLEquivalentExists(it->m_ReturnType))
				{
					const Key& convertedType = GetHLSLKeyword(it->m_ReturnType);
					a_OutputStream << convertedType.GetString() << " " << it->m_Name.GetString();
				}
				else
				{
					return SAVVY_LOGICAL_ERROR;
				}
			}
			else
			{
				a_OutputStream << it->m_ReturnType.GetString() << " " << it->m_Name.GetString();
			}
			a_OutputStream << "(";

			// Arguments

			resCode = HandleExtraArgsInputs(a_OutputStream, it->m_Name);

			if (resCode != SAVVY_OK)
				return resCode;

			resCode = HandleExtraArgsOutputs(a_OutputStream, it->m_Name);

			if (resCode != SAVVY_OK)
				return resCode;

			Database::VarList::iterator varIt;
			for (varIt = it->m_Arguments.begin(); varIt != it->m_Arguments.end(); ++varIt)
			{
				Database::KeyList& list = varIt->m_QualifierList;
				uint32 sizeType = list.size();
				for (uint32 i = 0; i < sizeType; i++)
				{
					if (HLSLEquivalentExists(list[i]))
					{
						const Key& type = GetHLSLKeyword(list[i]);
						a_OutputStream << type.GetString() << " ";
					}
					else
					{
						// Is it a user defined type?
						if (m_Database->StructExists(list[i]))
						{
							a_OutputStream << list[i].GetString() << " ";
						}
						else
						{
							return SAVVY_LOGICAL_ERROR;
						}
					}

				}

				a_OutputStream << varIt->m_Name.GetString();

				// Do extra instructions
				Key currKey = it->m_Name + "." + varIt->m_Name;
				Database::WordMultiMap::iterator instructionIt1 = instructionMap.lower_bound(currKey);
				Database::WordMultiMap::iterator instructionIt2 = instructionMap.upper_bound(currKey);

				while (instructionIt1 != instructionIt2)
				{
					if (instructionIt1->second.m_Translate)
					{
						const Key& k = GetHLSLKeyword(instructionIt1->second.m_Name);
						a_OutputStream << k.GetString() + " ";
					}
					else
					{
						a_OutputStream << instructionIt1->second.m_Name.GetString() + " ";
					}
					++instructionIt1;
				}

				if (varIt != it->m_Arguments.end() - 1)
				{
					a_OutputStream << ", ";
				}
			}

			a_OutputStream << ")" << std::endl;
			a_OutputStream << "{" << std::endl;

			// Function inner instructions
			resCode = ConstructInnerFunctionHLSL(it->m_Name, a_OutputStream);

			if (resCode != SAVVY_OK)
				return resCode;

			a_OutputStream << "}" << std::endl;
			a_OutputStream << std::endl;
		}
	}

	return resCode;
}

Savvy::ResultCode Savvy::Internal::ConstructorGLSLToHLSL::ConstructInnerFunctionHLSL(const Key& a_FuncName, std::ostream& a_OutputStream)
{
	Database::KeyVarMap& outputVariables = m_Database->GetOutputMap();
	m_CurrFunction = a_FuncName;
	ResultCode result;
	if (a_FuncName == "main" && !outputVariables.empty())
	{
		result = ConstructMainFunctionHLSL(a_OutputStream);
	}
	else
	{
		result = ConstructGenericFunctionHLSL(a_FuncName, a_OutputStream);
	}

	return result;
}

Savvy::ResultCode Savvy::Internal::ConstructorGLSLToHLSL::ConstructMainFunctionHLSL(std::ostream& a_OutputStream)
{
	ResultCode result;

	switch (m_CurrShader)
	{
	case FRAGMENT_SHADER:
	{
		std::string returnValName = GetFragmentShaderReturnVariableName().GetString();
		a_OutputStream << "\tfloat4 " << returnValName + ";" << std::endl;
		result = ConstructGenericFunctionHLSL("main", a_OutputStream);
		a_OutputStream << "\treturn " << returnValName + ";" << std::endl;
	}
		break;

	case VERTEX_SHADER:
	{
		a_OutputStream << "\t"SAVVY_OUTPUT_STRUCT_TYPE" "SAVVY_OUTPUT_STRUCT_NAME";" << std::endl;
		result = ConstructGenericFunctionHLSL("main", a_OutputStream);
		a_OutputStream << "\treturn " SAVVY_OUTPUT_STRUCT_NAME";" << std::endl;
	}
		break;

	default:
		result = SAVVY_UNKNOWN_ERROR;
		break;
	}

	return result;
}

Savvy::ResultCode Savvy::Internal::ConstructorGLSLToHLSL::ConstructGenericFunctionHLSL(const Key& a_FuncName, std::ostream& a_OutputStream)
{
	Database::WordMultiMap& instructionMap = m_Database->GetInstructionMap();
	Database::KeyVarMultiMap& uniformVariables = m_Database->GetUniformMap();

	Database::WordMultiMap::iterator it1 = instructionMap.lower_bound(a_FuncName);
	Database::WordMultiMap::iterator it2 = instructionMap.upper_bound(a_FuncName);

	std::string currentLine;
	uint32 numTabs = 1;

	while (it1 != it2)
	{
		Database::Word word = it1->second;

		//if (a_FuncName == "main")
		//{
			// Determine whether the current instruction is an input or output variable
			if (m_Database->InputVariableExists(word.m_Name))
			{
				word.m_Name.SetString(SAVVY_INPUT_STRUCT_NAME"." + word.m_Name.GetString());
			}
			else if (m_Database->OutputVariableExists(word.m_Name))
			{
				if (m_CurrShader == VERTEX_SHADER)
				{
					word.m_Name.SetString(SAVVY_OUTPUT_STRUCT_NAME"." + word.m_Name.GetString());
				}
			}
		//}

		if (!word.m_Translate)
		{
			if (IsEndOfLine(word.m_Name, currentLine))
			{
				// New line it
				if (word.m_Name == "{")
				{
					std::string oldString = word.m_Name.GetString();
					word.m_Name.SetString("\n");

					// Indentation
					for (uint32 i = 0; i < numTabs; i++)
					{
						word.m_Name.Append("\t");
					}
					word.m_Name.Append(oldString);
				}
				else if (word.m_Name == "}")
				{
					if (numTabs > 0)
						numTabs--;
					std::string oldString = word.m_Name.GetString();

					word.m_Name.SetString("");

					// Indentation
					for (uint32 i = 0; i < numTabs; i++)
					{
						word.m_Name.Append("\t");
					}
					word.m_Name.Append(oldString);
				}
				currentLine.append(word.m_Name.GetString());
				TidyLineFormatting(currentLine);
				// Indentation
				if (!word.m_Name.Contains("}"))
				{
					for (uint32 i = 0; i < numTabs; i++)
					{
						a_OutputStream << "\t";
					}
				}
				a_OutputStream << currentLine << std::endl;
				currentLine.clear();

				// Handle proper indentation to make it look pretty
				if (word.m_Name.Contains("{"))
				{
					numTabs++;
				}
			}
			else
			{
				// Matrix multiplication case
				if (word.m_Name == "*")
				{
					if (it1 == instructionMap.lower_bound(a_FuncName))
						return SAVVY_SHADER_SYNTAX_ERROR;

					bool matMultHandled = false;
					ResultCode resMatMult = HandleMatrixMultiplication(currentLine, it1, matMultHandled);

					if (resMatMult != SAVVY_OK)
						return resMatMult;

					if (matMultHandled)
						continue;
				}
				currentLine.append(word.m_Name.GetString() + " ");
			}
		}
		else
		{
			// Check for casting down matrices
			bool transformConstructorToCast = IsMatrixDowncast(currentLine, a_FuncName, it1);

			Key k;
			if (HLSLEquivalentExists(it1->second.m_Name))
			{
				if (it1->second.m_Name.Contains("vec"))
				{
					it1++;
					if (it1->second.m_Name == "(")
					{
						it1--;
						ResultCode resCode = HandleNoEquivalentFunc(it1, currentLine);

						if (resCode != SAVVY_OK)
						{
							m_LastError = "Could not convert instruction with name: " + word.m_Name.GetString() + ".";
							return resCode;
						}
					}
					else
					{
						it1--;
						k = GetHLSLKeyword(it1->second.m_Name);
					}
				}
				else
				{
					k = GetHLSLKeyword(it1->second.m_Name);
				}
			}
			else
			{
				// Handle variables named the same as built in functions
				it1++;
				if (m_Database->VariableWithNameExists(word.m_Name, m_CurrFunction) && it1->second.m_Name != "(")
				{
					currentLine.append(word.m_Name.GetString() + " ");
					it1--;
				}
				else
				{
					it1--;
					ResultCode resCode = HandleNoEquivalentFunc(it1, currentLine);

					if (resCode != SAVVY_OK)
					{
						m_LastError = "Could not convert instruction with name: " + word.m_Name.GetString() + ".";
						return resCode;
					}
				}
			}


			if (transformConstructorToCast)
			{
				currentLine.append("(" + k.GetString() + ")");
				it1++;
				it1->second.m_Name.Clear();
				it1++; it1++;
				it1->second.m_Name.Clear();
				it1--; it1--; it1--;
			}
			else
			{
				currentLine.append(k.GetString() + " ");
			}
		}

		it1++;
	}

	return SAVVY_OK;
}

const bool Savvy::Internal::ConstructorGLSLToHLSL::IsEndOfLine(const Key& a_Key, const std::string& a_CurrLine)
{
	if ((a_Key.GetString() == ";" && a_CurrLine.find("for (") == std::string::npos)
		|| a_Key.GetString() == "{" || a_Key.GetString() == "}")
		return true;

	return false;
}

const bool Savvy::Internal::ConstructorGLSLToHLSL::IsMatrixDowncast(std::string& a_CurrLine, const Key& a_FuncName, Database::WordMultiMap::iterator& a_Iter)
{
	bool ret = false;
	if (a_Iter->second.m_Name.Contains("mat3"))
	{
		a_Iter++;

		// Check if it's a constructor
		if (a_Iter->second.m_Name.GetString() == "(")
		{
			a_Iter++;

			Database::KeyList testList;
			testList.push_back("mat4");
			testList.push_back("mat4x4");
			testList.push_back("mat4x3");

			Key name = a_Iter->second.m_Name;

			// Check if the actual identifier is a type that can be casted down to a 3x3 matrix
			ret = m_Database->FindVariableWithType(name, a_FuncName, testList);

			a_Iter--;
		}
		a_Iter--;
	}
	else if (a_Iter->second.m_Name.Contains("mat2"))
	{
		a_Iter++;

		// Check if it's a constructor
		if (a_Iter->second.m_Name.GetString() == "(")
		{
			a_Iter++;

			Database::KeyList testList;
			testList.push_back("mat4");
			testList.push_back("mat4x4");
			testList.push_back("mat4x3");
			testList.push_back("mat3x3");
			testList.push_back("mat3");

			Key name = a_Iter->second.m_Name;

			// Check if the actual identifier is a type that can be casted down to a 3x3 matrix
			ret = m_Database->FindVariableWithType(name, a_FuncName, testList);

			a_Iter--;
		}
		a_Iter--;
	}
	return ret;
}

const Savvy::Key& Savvy::Internal::ConstructorGLSLToHLSL::GetFragmentShaderReturnVariableName()
{
	const Key& varName = m_Database->GetOutputVariableWithIndex(0);
	return varName;
}

void Savvy::Internal::ConstructorGLSLToHLSL::TidyLineFormatting(std::string& a_Line)
{
	a_Line = ReplaceAll(a_Line, " . ", ".");
	a_Line = ReplaceAll(a_Line, " , ", ",");
	a_Line = ReplaceAll(a_Line, " ( ", "(");
	a_Line = ReplaceAll(a_Line, " (", "(");
	a_Line = ReplaceAll(a_Line, " ) ", ")");
	a_Line = ReplaceAll(a_Line, ") ", ")");
	a_Line = ReplaceAll(a_Line, " )", ")");
	a_Line = ReplaceAll(a_Line, " ;", ";");
	a_Line = ReplaceAll(a_Line, " ]", "]");
	a_Line = ReplaceAll(a_Line, "[ ", "[");
	a_Line = ReplaceAll(a_Line, " [ ", "[");
	a_Line = ReplaceAll(a_Line, " [", "[");
	a_Line = ReplaceAll(a_Line, "< <", "<<");
	a_Line = ReplaceAll(a_Line, "~ ", "~");
	a_Line = ReplaceAll(a_Line, " ++", "++");
	a_Line = ReplaceAll(a_Line, "min10 float", "min10float");
	a_Line = ReplaceAll(a_Line, "min16 float", "min16float");
	a_Line = ReplaceAll(a_Line, "min32 float", "min32float");
}

bool Savvy::Internal::ConstructorGLSLToHLSL::HLSLEquivalentExists(const Key& a_Name)
{
	return m_Database->OutputLangEquivalentExists(a_Name);
}

const Savvy::Key& Savvy::Internal::ConstructorGLSLToHLSL::GetHLSLKeyword(const Key& a_Name)
{
	return m_Database->GetOutputLangEquivalent(a_Name);
}

Savvy::ResultCode Savvy::Internal::ConstructorGLSLToHLSL::HandleNoEquivalentFunc(Database::WordMultiMap::iterator& it1, std::string& currentLine)
{
	ResultCode res = m_FuncConverter->ConvertFunction(it1, currentLine, m_CurrFunction, m_CurrShader);

	return res;
}

bool Savvy::Internal::ConstructorGLSLToHLSL::HandleBuiltInVariablesOut(const Key& a_Name, std::ostream& a_OutputStream)
{
	Database::KeyVarMap& outputVarMap = m_Database->GetOutputMap();
	bool handled = false;
	std::string name = a_Name.GetString();

	if (name.find("gl_CullDistance") != std::string::npos && m_CullDistHandled)
	{
		handled = true;
		return handled;
	}
	else if (name.find("gl_ClipDistance") != std::string::npos && m_ClipDistHandled)
	{
		handled = true;
		return handled;
	}

	if (!m_Database->OutputVariableExists(a_Name))
		return handled;

	Database::KeyList& list = outputVarMap[a_Name].m_QualifierList;
	uint32 sizeType = list.size();
	if (name == "gl_Position")
	{
		a_OutputStream << "\t";
		for (uint32 i = 0; i < sizeType; i++)
		{
			const Key& varKey = list[i];
			if (HLSLEquivalentExists(varKey))
			{
				const Key& varType = GetHLSLKeyword(varKey);
				a_OutputStream << varType.GetString() << " ";
			}
			else
			{
				return handled;
			}
		}
		a_OutputStream << name;

		std::string semantic = "SV_Position";
		a_OutputStream << " : " << semantic << ";" << std::endl;
		handled = true;
	}
	else if (name == "gl_PointSize")
	{
		a_OutputStream << "\t";
		for (uint32 i = 0; i < sizeType; i++)
		{
			const Key& varKey = list[i];
			if (HLSLEquivalentExists(varKey))
			{
				const Key& varType = GetHLSLKeyword(varKey);
				a_OutputStream << varType.GetString() << " ";
			}
			else
			{
				return handled;
			}
		}
		a_OutputStream << name;

		std::string semantic = "PSIZE";
		a_OutputStream << " : " << semantic << ";" << std::endl;
		handled = true;
	}
	else if (name == "gl_FragColor")
	{
		a_OutputStream << "\t";
		for (uint32 i = 0; i < sizeType; i++)
		{
			const Key& varKey = list[i];
			if (HLSLEquivalentExists(varKey))
			{
				const Key& varType = GetHLSLKeyword(varKey);
				a_OutputStream << varType.GetString() << " ";
			}
			else
			{
				return handled;
			}
		}
		a_OutputStream << a_Name.GetString();

		std::string semantic = "SV_Target";
		a_OutputStream << " : " << semantic << ";" << std::endl;
		handled = true;
	}
	else if (name == "gl_FragDepth")
	{
		a_OutputStream << "\t";
		for (uint32 i = 0; i < sizeType; i++)
		{
			const Key& varKey = list[i];
			if (HLSLEquivalentExists(varKey))
			{
				const Key& varType = GetHLSLKeyword(varKey);
				a_OutputStream << varType.GetString() << " ";
			}
			else
			{
				return handled;
			}
		}
		a_OutputStream << name;

		std::string semantic = "SV_Depth";
		a_OutputStream << " : " << semantic << ";" << std::endl;
		handled = true;
	}
	else if (name.find("gl_CullDistance") != std::string::npos)
	{
		uint32 openBracketPos = name.find("[");
		uint32 closeBracketPos = name.find("]");
		if (openBracketPos == std::string::npos || closeBracketPos == std::string::npos)
			return handled;

		uint32 arraySize = std::stoi(name.substr(openBracketPos + 1, closeBracketPos - openBracketPos - 1));

		// Make sure we start from the highest index
		Database::KeyVarMap::iterator it1;
		for (it1 = outputVarMap.begin(); it1 != outputVarMap.end(); ++it1)
		{
			std::string currName = it1->second.m_Name.GetString();
			if (currName.find("gl_CullDistance") != std::string::npos)
			{
				uint32 openBracketPos1 = name.find("[");
				uint32 closeBracketPos1 = name.find("]");
				if (openBracketPos1 == std::string::npos || closeBracketPos1 == std::string::npos)
					continue;

				uint32 arraySize1 = std::stoi(name.substr(openBracketPos1 + 1, closeBracketPos1 - openBracketPos1 - 1));

				if (arraySize1 > arraySize)
				{
					arraySize = arraySize1;
				}
			}
		}

		for (uint32 i = 0; i < arraySize; i++)
		{
			a_OutputStream << "\t";
			for (uint32 j = 0; j < sizeType; j++)
			{
				const Key& varKey = list[j];
				if (HLSLEquivalentExists(varKey))
				{
					const Key& varType = GetHLSLKeyword(varKey);
					a_OutputStream << varType.GetString() << " ";
				}
				else
				{
					return handled;
				}
			}

			std::string safeName = name.substr(0, openBracketPos);
			safeName.append(std::to_string(i));
			a_OutputStream << safeName;

			std::string semantic = "SV_CullDistance" + std::to_string(i);
			a_OutputStream << " : " << semantic << ";" << std::endl;

			std::string findName = name.substr(0, openBracketPos) + "[" + std::to_string(i) + "]";
			ReplaceVarName(findName, safeName);
		}

		handled = true;
		m_CullDistHandled = true;
	}
	else if (name.find("gl_ClipDistance") != std::string::npos)
	{
		uint32 openBracketPos = name.find("[");
		uint32 closeBracketPos = name.find("]");
		if (openBracketPos == std::string::npos || closeBracketPos == std::string::npos)
			return handled;

		uint32 arraySize = std::stoi(name.substr(openBracketPos + 1, closeBracketPos - openBracketPos - 1));

		// Make sure we start from the highest index
		Database::KeyVarMap::iterator it1;
		for (it1 = outputVarMap.begin(); it1 != outputVarMap.end(); ++it1)
		{
			std::string currName = it1->second.m_Name.GetString();
			if (currName.find("gl_ClipDistance") != std::string::npos)
			{
				uint32 openBracketPos1 = name.find("[");
				uint32 closeBracketPos1 = name.find("]");
				if (openBracketPos1 == std::string::npos || closeBracketPos1 == std::string::npos)
					continue;

				uint32 arraySize1 = std::stoi(name.substr(openBracketPos1 + 1, closeBracketPos1 - openBracketPos1 - 1));

				if (arraySize1 > arraySize)
				{
					arraySize = arraySize1;
				}
			}
		}

		for (uint32 i = 0; i < arraySize; i++)
		{
			a_OutputStream << "\t";
			for (uint32 j = 0; j < sizeType; j++)
			{
				const Key& varKey = list[j];
				if (HLSLEquivalentExists(varKey))
				{
					const Key& varType = GetHLSLKeyword(varKey);
					a_OutputStream << varType.GetString() << " ";
				}
				else
				{
					return handled;
				}
			}

			std::string safeName = name.substr(0, openBracketPos);
			safeName.append(std::to_string(i));
			a_OutputStream << safeName;

			std::string semantic = "SV_ClipDistance" + std::to_string(i);
			a_OutputStream << " : " << semantic << ";" << std::endl;

			std::string findName = name.substr(0, openBracketPos) + "[" + std::to_string(i) + "]";
			ReplaceVarName(findName, safeName);
		}
		handled = true;
		m_ClipDistHandled = true;
	}

	return handled;
}

Savvy::ResultCode Savvy::Internal::ConstructorGLSLToHLSL::HandleMatrixMultiplication(std::string& currentLine, Database::WordMultiMap::iterator& it1, bool& a_Handled)
{
	Database::WordMultiMap& instructionMap = m_Database->GetInstructionMap();
	Database::WordMultiMap::iterator beginIt = instructionMap.lower_bound(m_CurrFunction);
	Database::WordMultiMap::iterator endIt = instructionMap.upper_bound(m_CurrFunction);
	Database::WordMultiMap::iterator originalIterator = it1;
	Database::WordMultiMap::iterator funcEndIterator = it1;
	Database::WordMultiMap::iterator funcBeginlIterator = it1;
	a_Handled = false;
	// First we need to extract the first operand
	uint32 ctrOffset = 0;
	uint32 numCloseBrackets = 0;
	uint32 numOpenBrackets = 0;
	it1--;
	while (it1 != beginIt)
	{
		Database::Word word = it1->second;

		if (word.m_Name == ")")
		{
			numCloseBrackets++;
		}
		else if (word.m_Name == "(")
		{
			if (numCloseBrackets == numOpenBrackets)
			{
				funcBeginlIterator = it1;
				break;
			}

			numOpenBrackets++;
		}

		if (numCloseBrackets == numOpenBrackets)
		{
			if (word.m_Name == "*" || word.m_Name == "/" ||
				word.m_Name == "+" || word.m_Name == "-" ||
				word.m_Name == "," || word.m_Name == "=" ||
				word.m_Name == "==" || word.m_Name == "!=" ||
				word.m_Name == ">" || word.m_Name == "<" ||
				word.m_Name == ">=" || word.m_Name == "<=" ||
				word.m_Name == "(")
			{
				funcBeginlIterator = it1;
				break;
			}
		}

		it1--;
		ctrOffset++;
	}

	if (it1 == beginIt)
	{
		m_LastError = "Could not convert matrix multiplication.";
		return SAVVY_SHADER_SYNTAX_ERROR;
	}

	std::string firstArg;
	for (uint32 i = 0; i < ctrOffset; i++)
	{
		it1++;
		Database::Word word = it1->second;

		if (word.m_Translate)
		{
			bool matrixCast = IsMatrixDowncast(currentLine, m_CurrFunction, it1);
			Key equiv;
			if (m_Database->OutputLangEquivalentExists(word.m_Name))
			{
				equiv = m_Database->GetOutputLangEquivalent(word.m_Name);
			}

			if (matrixCast)
			{
				firstArg.append("(" + equiv.GetString() + ")");
				it1++;
				it1->second.m_Name.Clear();
				it1++; it1++;
				it1->second.m_Name.Clear();
				it1--; it1--; it1--;
			}
			else
			{
				if (!equiv.IsEmpty())
					firstArg.append(equiv.GetString());
			}
		}
		else
		{
			// Determine whether the current instruction is an input or output variable
			if (m_Database->InputVariableExists(word.m_Name))
			{
				word.m_Name.SetString(SAVVY_INPUT_STRUCT_NAME"." + word.m_Name.GetString());
			}
			else if (m_Database->OutputVariableExists(word.m_Name))
			{
				if (m_CurrShader == VERTEX_SHADER)
				{
					word.m_Name.SetString(SAVVY_OUTPUT_STRUCT_NAME"." + word.m_Name.GetString());
				}
			}

			firstArg.append(word.m_Name.GetString());
		}
	}

	// Return iterator to original place
	it1 = originalIterator;

	// Check if the first argument is a vector or matrix

	// Remove any unnecessary info, in order to query properly
	std::string queryArg1 = firstArg;
	uint32 structPos = queryArg1.find(SAVVY_INPUT_STRUCT_NAME".");
	if (structPos != std::string::npos)
	{
		std::string prev = queryArg1.substr(0, structPos);
		queryArg1 = queryArg1.substr(structPos+3);
		queryArg1 = prev + queryArg1;
	}
	else
	{
		structPos = queryArg1.find(SAVVY_OUTPUT_STRUCT_NAME".");
		if (structPos != std::string::npos)
		{
			std::string prev = queryArg1.substr(0, structPos);
			queryArg1 = queryArg1.substr(structPos+4);
			queryArg1 = prev + queryArg1;
		}
	}

	bool isArg1Vec = IsVarVector(queryArg1);
	bool isArg1Mat = IsVarMatrix(queryArg1);

	// Early out
	if (!isArg1Vec && !isArg1Mat)
		return SAVVY_OK;

	
	// It could already be a mul
	uint32 mulPos = currentLine.find("mul");
	if (mulPos != std::string::npos)
	{
		firstArg = currentLine.substr(mulPos); // Overwrite
	}

	// Extract second argument
	it1++;
	ctrOffset = 0;
	numCloseBrackets = 0;
	numOpenBrackets = 0;

	while (it1 != endIt)
	{
		Database::Word word = it1->second;

		if (word.m_Name == ")")
		{
			if (numCloseBrackets == numOpenBrackets)
			{
				funcEndIterator = it1;
				break;
			}

			numCloseBrackets++;
		}
		else if (word.m_Name == "(")
		{
			numOpenBrackets++;
		}

		if (numCloseBrackets == numOpenBrackets)
		{
			if (word.m_Name == "*" || word.m_Name == "/" ||
				word.m_Name == "+" || word.m_Name == "-" ||
				word.m_Name == "," || word.m_Name == "=" ||
				word.m_Name == "==" || word.m_Name == "!=" ||
				word.m_Name == ">" || word.m_Name == "<" ||
				word.m_Name == ">=" || word.m_Name == "<=" ||
				word.m_Name == ";")
			{
				funcEndIterator = it1;
				break;
			}
		}

		it1++;
		ctrOffset++;
	}

	if (it1 == endIt)
	{
		m_LastError = "Could not convert matrix multiplication.";
		return SAVVY_SHADER_SYNTAX_ERROR;
	}

	// Assemble the second argument
	std::string secondArg;
	it1 = originalIterator;
	for (uint32 i = 0; i < ctrOffset; i++)
	{
		it1++;
		Database::Word word = it1->second;

		if (word.m_Translate)
		{
			bool matrixCast = IsMatrixDowncast(currentLine, m_CurrFunction, it1);
			Key equiv;
			if (m_Database->OutputLangEquivalentExists(word.m_Name))
			{
				equiv = m_Database->GetOutputLangEquivalent(word.m_Name);
			}

			if (matrixCast)
			{
				secondArg.append("(" + equiv.GetString() + ")");
				it1++;
				it1->second.m_Name.Clear();
				it1++; it1++;
				it1->second.m_Name.Clear();
				it1--; it1--; it1--;
			}
			else
			{
				if (!equiv.IsEmpty())
					secondArg.append(equiv.GetString());
			}
		}
		else
		{
			// Determine whether the current instruction is an input or output variable
			if (m_Database->InputVariableExists(word.m_Name))
			{
				word.m_Name.SetString(SAVVY_INPUT_STRUCT_NAME"." + word.m_Name.GetString());
			}
			else if (m_Database->OutputVariableExists(word.m_Name))
			{
				if (m_CurrShader == VERTEX_SHADER)
				{
					word.m_Name.SetString(SAVVY_OUTPUT_STRUCT_NAME"." + word.m_Name.GetString());
				}
			}

			secondArg.append(word.m_Name.GetString());
		}
	}

	// Remove any unnecessary info, in order to query properly
	std::string queryArg2 = secondArg;
	structPos = queryArg2.find(SAVVY_INPUT_STRUCT_NAME".");
	if (structPos != std::string::npos)
	{
		std::string prev = queryArg2.substr(0, structPos);
		queryArg2 = queryArg2.substr(structPos + 3);
		queryArg2 = prev + queryArg2;
	}
	else
	{
		structPos = queryArg2.find(SAVVY_OUTPUT_STRUCT_NAME".");
		if (structPos != std::string::npos)
		{
			std::string prev = queryArg2.substr(0, structPos);
			queryArg2 = queryArg2.substr(structPos + 4);
			queryArg2 = prev + queryArg2;
		}
	}

	// Check if the second argument is a vector or matrix
	bool isArg2Vec = IsVarVector(queryArg2);
	bool isArg2Mat = IsVarMatrix(queryArg2);
	// Early out

	it1 = originalIterator;


	if (!isArg1Mat)
	{
		if ((!isArg2Vec && !isArg2Mat) || (isArg2Vec && isArg1Vec))
			return SAVVY_OK;
	}
	

	// Both arguments are legit, turn this into a mul() function
	//it1->second.m_Name.Clear();
	it1 = funcEndIterator;
	// Erase the unneeded first argument
	uint32 erasePos = currentLine.find(firstArg);

	if (erasePos == std::string::npos)
	{
		// Normally this should never happen
		m_LastError = "Could not convert matrix multiplication.";
		return SAVVY_UNKNOWN_ERROR;
	}

	currentLine.erase(erasePos);

	currentLine.append("mul(" + firstArg + ", " + secondArg + ")");
	a_Handled = true;
	return SAVVY_OK;
}

bool Savvy::Internal::ConstructorGLSLToHLSL::IsVarVector(std::string a_VarName)
{
	Database::KeyList typeList;
	typeList.push_back("vec");
	bool cond1 = m_Database->FindVariableWithType(Key(a_VarName), m_CurrFunction, typeList);

	if (cond1)
		return cond1;

	for (uint32 i = 2; i < 5; i++)
	{
		uint32 pos = a_VarName.find("float" + std::to_string(i));

		if (pos != std::string::npos)
		{
			pos = a_VarName.find("float" + std::to_string(i) + "x");
			if (pos == std::string::npos)
			{
				return true;
			}
		}
	}

	return false;
}

bool Savvy::Internal::ConstructorGLSLToHLSL::IsVarMatrix(std::string a_VarName)
{
	Database::KeyList typeList;
	typeList.push_back("mat");
	bool cond1 = m_Database->FindVariableWithType(Key(a_VarName), m_CurrFunction, typeList);

	if (cond1)
		return cond1;

	if (a_VarName.find("float2x") != std::string::npos ||
		a_VarName.find("float3x") != std::string::npos ||
		a_VarName.find("float4x") != std::string::npos ||
		a_VarName.find("matrix") != std::string::npos)
	{
		return true;
	}

	return false;
}

Savvy::ResultCode Savvy::Internal::ConstructorGLSLToHLSL::ConstructVertexOrFragment(std::ostream& a_OutputStream)
{
	ResultCode res;

	// Construct Structures
	res = ConstructStructsHLSL(a_OutputStream);

	if (res != SAVVY_OK)
	{
		m_LastError.append("Error while constructing structures.");
		return res;
	}

	// Construct constant buffers
	res = ConstructUniformBuffersHLSL(a_OutputStream);

	if (res != SAVVY_OK)
	{
		m_LastError.append("Error while constructing buffers.");
		return res;
	}

	// Construct constant buffers
	res = ConstructGenericBuffersHLSL(a_OutputStream);

	if (res != SAVVY_OK)
	{
		m_LastError.append("Error while constructing buffers.");
		return res;
	}

	// Construct Samplers/Textures
	res = ConstructSamplersHLSL(a_OutputStream);

	if (res != SAVVY_OK)
	{
		m_LastError.append("Error while constructing samplers\textures.");
		return res;
	}

	// Construct Input buffer
	res = ConstructInputVariablesHLSL(a_OutputStream);

	if (res != SAVVY_OK)
	{
		m_LastError.append("Error while constructing inputs.");
		return res;
	}

	// Construct Output buffer
	res = ConstructOutputVariablesHLSL(a_OutputStream);

	if (res != SAVVY_OK)
	{
		m_LastError.append("Error while constructing outputs.");
		return res;
	}

	//Construct global variables
	res = ConstructGlobalVarsHLSL(a_OutputStream);

	if (res != SAVVY_OK)
	{
		m_LastError.append("Error while constructing global variables.");
		return res;
	}

	// Construct Functions
	res = ConstructFunctionsHLSL(a_OutputStream);

	if (res != SAVVY_OK)
	{
		m_LastError.append("Error while constructing functions.");
		return res;
	}

	return SAVVY_OK;
}

bool Savvy::Internal::ConstructorGLSLToHLSL::HandleBuiltInVariablesIn(const Key& a_Name, std::ostream& a_OutputStream)
{
Database::KeyVarMap& outputVarMap = m_Database->GetInputMap();
	bool handled = false;

	std::string name = a_Name.GetString();

	if (!m_Database->InputVariableExists(a_Name))
		return handled;

	Database::KeyList& list = outputVarMap[a_Name].m_QualifierList;
	uint32 sizeType = list.size();
	if (name == "gl_FragCoord")
	{
		a_OutputStream << "\t";
		for (uint32 i = 0; i < sizeType; i++)
		{
			const Key& varKey = list[i];
			if (HLSLEquivalentExists(varKey))
			{
				const Key& varType = GetHLSLKeyword(varKey);
				a_OutputStream << varType.GetString() << " ";
			}
			else
			{
				return handled;
			}
		}
		a_OutputStream << name;

		std::string semantic = "SV_Position";
		a_OutputStream << " : " << semantic << ";" << std::endl;
		handled = true;
	}
	else if (name == "gl_FrontFacing")
	{
		a_OutputStream << "\t";
		for (uint32 i = 0; i < sizeType; i++)
		{
			const Key& varKey = list[i];
			if (HLSLEquivalentExists(varKey))
			{
				const Key& varType = GetHLSLKeyword(varKey);
				a_OutputStream << varType.GetString() << " ";
			}
			else
			{
				return handled;
			}
		}
		a_OutputStream << name;

		std::string semantic = "SV_IsFrontFace";
		a_OutputStream << " : " << semantic << ";" << std::endl;
		handled = true;
	}
	else if (name == "gl_PointCoord")
	{
		a_OutputStream << "\t";
		for (uint32 i = 0; i < sizeType; i++)
		{
			const Key& varKey = list[i];
			if (HLSLEquivalentExists(varKey))
			{
				const Key& varType = GetHLSLKeyword(varKey);
				a_OutputStream << varType.GetString() << " ";
			}
			else
			{
				return handled;
			}
		}
		a_OutputStream << name;

		std::string semantic = "SV_Position";
		a_OutputStream << " : " << semantic << ";" << std::endl;
		handled = true;
	}
	return handled;
}

Savvy::ResultCode Savvy::Internal::ConstructorGLSLToHLSL::HandleExtraArgsInputs(std::ostream& a_OutputStream, Key& a_CurrFunc)
{
	Database::WordMultiMap& instructionMap = m_Database->GetInstructionMap();
	Database::KeyVarMultiMap& uniformVariables = m_Database->GetUniformMap();
	Database::KeyVarMap& outputVariables = m_Database->GetOutputMap();
	Database::KeyVarMap& inputVariables = m_Database->GetInputMap();

	Database::WordMultiMap::iterator it1 = instructionMap.lower_bound(a_CurrFunc);
	Database::WordMultiMap::iterator it2 = instructionMap.upper_bound(a_CurrFunc);

	bool inputVarFound = false;
	while (it1 != it2)
	{
		Database::KeyVarMap::iterator inputVarIt;

		for (inputVarIt = inputVariables.begin(); inputVarIt != inputVariables.end(); ++inputVarIt)
		{
			if (it1->second.m_Name == inputVarIt->first)
			{
				inputVarFound = true;
				break;
			}
		}

		it1++;
	}

	if (inputVarFound)
	{
		bool argsEmpty = m_Database->GetFunction(a_CurrFunc).m_Arguments.empty();
		a_OutputStream << SAVVY_INPUT_STRUCT_TYPE" "SAVVY_INPUT_STRUCT_NAME;
		if (!argsEmpty)
		{
			a_OutputStream << ", ";
		}

		// Make sure we pass it in every function call
		Database::WordMultiMap::iterator instructionIt;
		for (instructionIt = instructionMap.begin(); instructionIt != instructionMap.end(); ++instructionIt)
		{
			if (instructionIt->second.m_Name == a_CurrFunc)
			{
				m_FuncToAddInputArg.push_back(a_CurrFunc);
				instructionIt++;
				if (instructionIt->second.m_Name == "(")
				{
					instructionIt->second.m_Name.Append(SAVVY_INPUT_STRUCT_NAME);
					if (!argsEmpty)
					{
						instructionIt->second.m_Name.Append(",");
					}
				}
			}
		}
	}
	else
	{
		// Check if it's maybe a function which itself calls a function that depends on the input argument
		Database::KeyList::iterator funcIt;
		bool found = false;
		bool argsEmpty = m_Database->GetFunction(a_CurrFunc).m_Arguments.empty();
		for (funcIt = m_FuncToAddInputArg.begin(); funcIt != m_FuncToAddInputArg.end(); ++funcIt)
		{
			it1 = instructionMap.lower_bound(a_CurrFunc);
			while (it1 != it2)
			{
				if (it1->second.m_Name == *funcIt)
				{
					it1++;
					if (it1->second.m_Name.Contains("("))
					{
						a_OutputStream << SAVVY_INPUT_STRUCT_TYPE" "SAVVY_INPUT_STRUCT_NAME;
						if (!argsEmpty)
						{
							a_OutputStream << ",";
						}
						found = true;
						break;
					}
				}
				if (found)
					break;
				it1++;
			}
		}

		if (found)
		{
			// Make sure we pass it in every function call
			Database::WordMultiMap::iterator instructionIt;
			for (instructionIt = instructionMap.begin(); instructionIt != instructionMap.end(); ++instructionIt)
			{
				if (instructionIt->second.m_Name == a_CurrFunc)
				{
					m_FuncToAddInputArg.push_back(a_CurrFunc);
					instructionIt++;
					if (instructionIt->second.m_Name == "(")
					{
						instructionIt->second.m_Name.Append(SAVVY_INPUT_STRUCT_NAME);
						if (!argsEmpty)
						{
							instructionIt->second.m_Name.Append(",");
						}
					}
				}
			}
		}
	}

	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ConstructorGLSLToHLSL::HandleExtraArgsOutputs(std::ostream& a_OutputStream, Key& a_CurrFunc)
{
	Database::WordMultiMap& instructionMap = m_Database->GetInstructionMap();
	Database::KeyVarMultiMap& uniformVariables = m_Database->GetUniformMap();
	Database::KeyVarMap& outputVariables = m_Database->GetOutputMap();

	Database::WordMultiMap::iterator it1 = instructionMap.lower_bound(a_CurrFunc);
	Database::WordMultiMap::iterator it2 = instructionMap.upper_bound(a_CurrFunc);

	bool outputVarFound = false;
	while (it1 != it2)
	{
		Database::KeyVarMap::iterator outputVarIt;

		for (outputVarIt = outputVariables.begin(); outputVarIt != outputVariables.end(); ++outputVarIt)
		{
			if (it1->second.m_Name == outputVarIt->first)
			{
				outputVarFound = true;
				break;
			}
		}

		it1++;
	}

	if (outputVarFound)
	{
		bool argsEmpty = m_Database->GetFunction(a_CurrFunc).m_Arguments.empty();
		a_OutputStream << SAVVY_OUTPUT_STRUCT_TYPE" OUT";
		if (!argsEmpty)
		{
			a_OutputStream << ", ";
		}

		// Make sure we pass it in every function call
		Database::WordMultiMap::iterator instructionIt;
		for (instructionIt = instructionMap.begin(); instructionIt != instructionMap.end(); ++instructionIt)
		{
			if (instructionIt->second.m_Name == a_CurrFunc)
			{
				m_FuncToAddOutputArg.push_back(a_CurrFunc);
				instructionIt++;
				if (instructionIt->second.m_Name == "(")
				{
					instructionIt->second.m_Name.Append("OUT");
					if (!argsEmpty)
					{
						instructionIt->second.m_Name.Append(",");
					}
				}
			}
		}
	}
	else
	{
		// Check if it's maybe a function which itself calls a function that depends on the input argument
		Database::KeyList::iterator funcIt;
		bool found = false;
		bool argsEmpty = m_Database->GetFunction(a_CurrFunc).m_Arguments.empty();
		for (funcIt = m_FuncToAddOutputArg.begin(); funcIt != m_FuncToAddOutputArg.end(); ++funcIt)
		{
			it1 = instructionMap.lower_bound(a_CurrFunc);
			while (it1 != it2)
			{
				if (it1->second.m_Name == *funcIt)
				{
					it1++;
					if (it1->second.m_Name.Contains("("))
					{
						a_OutputStream << SAVVY_OUTPUT_STRUCT_TYPE" "SAVVY_OUTPUT_STRUCT_NAME;
						if (!argsEmpty)
						{
							a_OutputStream << ", ";
						}
						found = true;
						break;
					}
				}
				if (found)
					break;
				it1++;
			}
		}

		if (found)
		{
			// Make sure we pass it in every function call
			Database::WordMultiMap::iterator instructionIt;
			for (instructionIt = instructionMap.begin(); instructionIt != instructionMap.end(); ++instructionIt)
			{
				if (instructionIt->second.m_Name == a_CurrFunc)
				{
					m_FuncToAddOutputArg.push_back(a_CurrFunc);
					instructionIt++;
					if (instructionIt->second.m_Name == "(")
					{
						instructionIt->second.m_Name.Append(SAVVY_OUTPUT_STRUCT_NAME);
						if (!argsEmpty)
						{
							instructionIt->second.m_Name.Append(",");
						}
					}
				}
			}
		}
	}
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ConstructorGLSLToHLSL::ConstructGenericBuffersHLSL(std::ostream& a_OutputStream)
{
	Database::KeyList& genericBuffers = m_Database->GetGenericBufferList();
	Database::IndexMap& bufferIndices = m_Database->GetGenericBufferIndexMap();

	uint32 startingIndex = 0;

	Database::IndexMap& texIndices = m_Database->GetSamplerIndexMap();
	Database::IndexMap::iterator texIt;

	for (texIt = texIndices.begin(); texIt != texIndices.end(); ++texIt)
	{
		if (texIt->first > startingIndex)
		{
			startingIndex = texIt->first;
		}
	}

	if (!genericBuffers.empty())
	{
		Database::KeyVarMultiMap& bufferVarMap = m_Database->GetGenericBufferVarMap();
		Database::KeyVarMultiMap::iterator bufferIt1;
		Database::KeyVarMultiMap::iterator bufferIt2;
		Database::IndexMap::iterator bufferIndicesIt;

		for (bufferIndicesIt = bufferIndices.begin(); bufferIndicesIt != bufferIndices.end(); ++bufferIndicesIt)
		{
			Key& key = bufferIndicesIt->second;
			
			bufferIt1 = bufferVarMap.lower_bound(key);
			bufferIt2 = bufferVarMap.upper_bound(key);

			while (bufferIt1 != bufferIt2)
			{
				if (bufferIt1->second.m_QualifierList.empty())
					return SAVVY_LOGICAL_ERROR;

				if (HLSLEquivalentExists(bufferIt1->second.m_QualifierList[0]))
				{
					const Key& k = GetHLSLKeyword(bufferIt1->second.m_QualifierList[0]);
					a_OutputStream << "StructuredBuffer<" + k.GetString() + "> ";
				}
				else
				{
					if (m_Database->StructExists(bufferIt1->second.m_QualifierList[0]))
					{
						a_OutputStream << "StructuredBuffer<" + bufferIt1->second.m_QualifierList[0].GetString() + "> ";
					}
					else
					{
						return SAVVY_LOGICAL_ERROR;
					}
				}
				
				a_OutputStream << bufferIt1->second.m_Name.GetString() + " : register(t" + std::to_string(startingIndex) + ");" << std::endl;
				bufferIt1++;
				startingIndex++;
			}
		}
	}

	return SAVVY_OK;
}
