#include "SavvyPCH.h"
#include "SavvyConstructorHLSLToGLSL.h"
#include "SavvyDatabaseHLSL.h"
#include "SavvyFuncConverter.h"
#include "SavvyFuncConverterHLSLToGLSL.h"
#include "SavvyTemplateConverterHLSLToGLSL.h"

Savvy::Internal::ConstructorHLSLToGLSL::ConstructorHLSLToGLSL() : Constructor()
{
	m_FuncConverter = new FuncConverterHLSLToGLSL();
	m_TemplateConverter = new TemplateConverterHLSLToGLSL();
}

Savvy::Internal::ConstructorHLSLToGLSL::~ConstructorHLSLToGLSL()
{
	if (m_FuncConverter)
		delete m_FuncConverter;

	if (m_TemplateConverter)
		delete m_TemplateConverter;
}

void Savvy::Internal::ConstructorHLSLToGLSL::Reset()
{
	Constructor::Reset();
	m_InputStructName.Clear();
	m_OutputStructName.Clear();
	m_CurrFunction.Clear();
	m_FuncConverter->Reset();
}

Savvy::ResultCode Savvy::Internal::ConstructorHLSLToGLSL::Initialize(Key& a_InputEntry, Key& a_OutputEntry, uint32 a_Inputlang, uint32 a_ShaderType, Database* a_Database)
{
	ResultCode res = Constructor::Initialize(a_InputEntry, a_OutputEntry, a_Inputlang, a_ShaderType, a_Database);

	if (res != SAVVY_OK)
		return res;
		
	res = m_FuncConverter->Initialize(m_Database);

	if (res != SAVVY_OK)
		return res;

	res = m_TemplateConverter->Initialize(a_Database);
	return res;
}

Savvy::ResultCode Savvy::Internal::ConstructorHLSLToGLSL::Construct(std::ostream& a_OutputStream)
{
	m_LastError.clear();

	if (!m_Database->FunctionExists(m_InputEntry))
	{
		m_LastError = "Invalid entry point specified for input shader.";
		return SAVVY_INVALID_ENTRY_POINT;
	}

	// Figure out input and output struct names
	Database::Function& entry = m_Database->GetFunction(m_InputEntry);
	
	for (uint32 i = 0; i < entry.m_Arguments[0].m_QualifierList.size(); i++)
	{
		Key& k = entry.m_Arguments[0].m_QualifierList[i];
		if (k != "in" && k != "out" && k != "uniform" && k != "inout")
		{
			if (m_Database->StructExists(k))
			{
				m_InputStructName = k;
			}
		}
	}

	if (m_InputStructName.IsEmpty())
		return SAVVY_LOGICAL_ERROR;

	m_OutputStructName = entry.m_ReturnType;

	a_OutputStream << "#version 450 core" << std::endl;
	
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

void Savvy::Internal::ConstructorHLSLToGLSL::TidyLineFormatting(std::string& a_Line)
{
	a_Line = ReplaceAll(a_Line, " . ", ".");
	a_Line = ReplaceAll(a_Line, " , ", ",");
	a_Line = ReplaceAll(a_Line, " ( ", "(");
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
	a_Line = ReplaceAll(a_Line, " ++ ", "++");
	a_Line = ReplaceAll(a_Line, "++ ", "++");
	a_Line = ReplaceAll(a_Line, " .", ".");
	a_Line = ReplaceAll(a_Line, " . ", ".");
	a_Line = ReplaceAll(a_Line, ". ", ".");
}

Savvy::ResultCode Savvy::Internal::ConstructorHLSLToGLSL::ConstructInputsGLSL(std::ostream& a_OutputStream)
{
	Database::Function& entry = m_Database->GetFunction(m_InputEntry);

	// Determine the input
	uint32 size = entry.m_Arguments.size();

	if (size < 1)
		return SAVVY_SHADER_SYNTAX_ERROR;

	uint32 locationCtr = 0;
	for (uint32 i = 0; i < size; i++)
	{
		Key& inputName = entry.m_Arguments[i].m_Name;
		const Database::KeyList& inputQualifiers = entry.m_Arguments[i].m_QualifierList;

		// Check if it's a struct
		uint32 qualifierSize = inputQualifiers.size();
		bool isStruct = false;
		for (uint32 k = 0; k < qualifierSize; k++)
		{
			if (m_Database->StructExists(inputQualifiers[k]))
			{
				// loop through all variables
				isStruct = true;
				Database::KeyVarMultiMap& varMap = m_Database->GetStructVariableMap();
				Database::KeyVarMultiMap::iterator it1 = varMap.lower_bound(inputQualifiers[k]);
				Database::KeyVarMultiMap::iterator it2 = varMap.upper_bound(inputQualifiers[k]);
				while (it1 != it2)
				{
					// Check if it's a built-in variable
					if (m_Database->StructVariableExists(m_InputStructName, it1->second.m_Name))
					{
						Key semantic = m_Database->GetSemantic(m_InputStructName, it1->second.m_Name);
						if (HandleBuiltInSemantic(semantic, it1->second.m_Name))
						{
							it1++;
							continue;
						}
					}

					a_OutputStream << "layout(location=" + std::to_string(locationCtr) + ") ";
					
					a_OutputStream << "in ";

					// loop through the type qualifiers
					Database::KeyList& typeList = it1->second.m_QualifierList;
					uint32 typeSize = typeList.size();
					for (uint32 j = 0; j < typeSize; j++)
					{
						if (GLSLEquivalentExists(typeList[j]))
						{
							const Key& translated = GetGLSLKeyword(typeList[j]);
							a_OutputStream << translated.GetString() + " ";
						}
						else
						{
							a_OutputStream << typeList[j].GetString() + " ";
						}
					}

					// Make sure we avoid name clashes in GLSL
					Key oldName = it1->second.m_Name;
					while (m_Database->StructVariableExists(entry.m_ReturnType, it1->second.m_Name) ||
						m_Database->LocalVariableExists(m_InputEntry, it1->second.m_Name))
					{
						// Rename it
						it1->second.m_Name.SetString("in_" + it1->second.m_Name.GetString());
					}

					a_OutputStream << it1->second.m_Name.GetString() + "; ";
					a_OutputStream << SAVVY_ORIGINAL_SEMANTIC_DECL;
					a_OutputStream << m_Database->GetSemantic(m_InputStructName, oldName).GetString() << " */" << std::endl;
					locationCtr++;
					it1++;
				}
			}
		}

		if (!isStruct)
		{
			// It's a normal data type

			a_OutputStream << "layout(location=" + std::to_string(locationCtr) + ") ";
			
			a_OutputStream << "in ";

			// loop through the type qualifiers
			uint32 typeSize = inputQualifiers.size();
			for (uint32 j = 0; j < typeSize; j++)
			{
				a_OutputStream << inputQualifiers[j].GetString() + " ";
			}

			// Make sure we avoid name clashes in GLSL
			Key oldName = inputName;
			while (m_Database->StructVariableExists(entry.m_ReturnType, inputName) ||
				m_Database->LocalVariableExists(m_InputEntry, inputName))
			{
				// Rename it
				inputName.SetString("in_" + inputName.GetString());
			}

			a_OutputStream << inputName.GetString() + "; ";
			a_OutputStream << SAVVY_ORIGINAL_SEMANTIC_DECL;
			a_OutputStream << m_Database->GetSemantic(m_InputStructName, oldName).GetString() << " */" << std::endl;
			locationCtr++;
		}
	}
	a_OutputStream << std::endl;

	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ConstructorHLSLToGLSL::ConstructOutputsGLSL(std::ostream& a_OutputStream)
{
	if (m_CurrShader == FRAGMENT_SHADER)
	{
		// Construct only one output
		a_OutputStream << "layout(location=0) ";
		a_OutputStream << "out ";
		a_OutputStream << "vec4 " << SAVVY_FRAGMENT_SHADER_OUTPUT_NAME << ";" << std::endl;
		a_OutputStream << std::endl;
		return SAVVY_OK;
	}

	const Database::Function& entry = m_Database->GetFunction(m_InputEntry);

	// Determine if the output is a struct or a normal data type
	const Key& outputType = entry.m_ReturnType;
	uint32 locationCtr = 0;
	if (m_Database->StructExists(outputType))
	{
		Database::KeyVarMultiMap& varMap = m_Database->GetStructVariableMap();
		Database::KeyVarMultiMap::iterator it1 = varMap.lower_bound(outputType);
		Database::KeyVarMultiMap::iterator it2 = varMap.upper_bound(outputType);
		while (it1 != it2)
		{
			// Check if it's a built-in variable
			if (m_Database->StructVariableExists(m_OutputStructName, it1->second.m_Name))
			{
				Key semantic = m_Database->GetSemantic(m_OutputStructName, it1->second.m_Name);
				if (HandleBuiltInSemantic(semantic, it1->second.m_Name))
				{
					it1++;
					continue;
				}
			}

			a_OutputStream << "layout(location=" + std::to_string(locationCtr) + ") ";
			
			a_OutputStream << "out ";

			// loop through the type qualifiers
			Database::KeyList& typeList = it1->second.m_QualifierList;
			uint32 typeSize = typeList.size();
			for (uint32 j = 0; j < typeSize; j++)
			{
				if (GLSLEquivalentExists(typeList[j]))
				{
					const Key& translated = GetGLSLKeyword(typeList[j]);
					a_OutputStream << translated.GetString() + " ";
				}
				else
				{
					a_OutputStream << typeList[j].GetString() + " ";
				}
			}

			// Make sure we avoid name clashes in GLSL
			Key oldName = it1->second.m_Name;
			while (m_Database->LocalVariableExists(m_InputEntry, it1->second.m_Name))
			{
				// Rename it
				it1->second.m_Name.SetString("out_" + it1->second.m_Name.GetString());
			}

			a_OutputStream << it1->second.m_Name.GetString() + "; ";
			a_OutputStream << SAVVY_ORIGINAL_SEMANTIC_DECL; 
			a_OutputStream << m_Database->GetSemantic(m_OutputStructName, oldName).GetString() << " */" << std::endl;
			locationCtr++;
			it1++;
		}
		Database::KeyList& structList = m_Database->GetStructList();
	}
	else
	{
		// It's a normal data type
		a_OutputStream << "layout(location=" + std::to_string(locationCtr) + ") ";
		
		a_OutputStream << "out ";

		a_OutputStream << entry.m_ReturnType.GetString() + " ";
		a_OutputStream << "OutputVariable" + std::to_string(locationCtr) + ";" << std::endl;
		locationCtr++;
	}
	a_OutputStream << std::endl;

	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ConstructorHLSLToGLSL::ConstructUniformBuffersGLSL(std::ostream& a_OutputStream)
{
	Database::KeyList& uniformBuffers = m_Database->GetUniformBufferList();
	Database::IndexMap& uniformIndices = m_Database->GetUniformBufferIndexMap();
	Database::WordMultiMap& instructionMap = m_Database->GetInstructionMap();

	if (!uniformBuffers.empty())
	{
		Database::KeyVarMultiMap& uniformMap = m_Database->GetUniformMap();

		Database::KeyVarMultiMap::iterator uniformIt1;
		Database::KeyVarMultiMap::iterator uniformIt2;
		Database::IndexMap::iterator uniformIndicesIt;
		for (uniformIndicesIt = uniformIndices.begin(); uniformIndicesIt != uniformIndices.end(); ++uniformIndicesIt)
		{
			const Key& buffName = uniformIndicesIt->second;

			a_OutputStream << "layout(std140, binding=" + std::to_string(uniformIndicesIt->first) + ") ";
			
			a_OutputStream << "uniform ";
			a_OutputStream << buffName.GetString() << std::endl;
			a_OutputStream << "{" << std::endl;

			// Loop through all the variables
			uniformIt1 = uniformMap.lower_bound(buffName);
			uniformIt2 = uniformMap.upper_bound(buffName);

			while (uniformIt1 != uniformIt2)
			{
				// Output
				a_OutputStream << "\t";
				Database::KeyList& list = uniformIt1->second.m_QualifierList;

				uint32 size = list.size();

				for (uint32 i = 0; i < size; i++)
				{
					if (GLSLEquivalentExists(list[i]))
					{
						const Key& varType = GetGLSLKeyword(list[i]);
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
				a_OutputStream << uniformIt1->second.m_Name.GetString();
				// Do extra init instructions
				Database::WordMultiMap::iterator it1 = instructionMap.lower_bound(buffName.GetString() + "." + uniformIt1->second.m_Name.GetString());
				Database::WordMultiMap::iterator it2 = instructionMap.upper_bound(buffName.GetString() + "." + uniformIt1->second.m_Name.GetString());

				while (it1 != it2)
				{
					a_OutputStream << it1->second.m_Name.GetString();
					it1++;
				}

				a_OutputStream << ";";
				a_OutputStream << std::endl;
				++uniformIt1;
			}

			a_OutputStream << "};" << std::endl;
			a_OutputStream << std::endl;
			a_OutputStream << std::endl;
		}
	}

	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ConstructorHLSLToGLSL::ConstructSamplersGLSL(std::ostream& a_OutputStream)
{
	Database::KeyVarMap& samplerMap = m_Database->GetSamplerList();
	Database::IndexMap& indexMap = m_Database->GetSamplerIndexMap();

	if (!samplerMap.empty())
	{
		Database::IndexMap::iterator it;

		for (it = indexMap.begin(); it != indexMap.end(); ++it)
		{
			const Key& name = m_Database->GetSamplerWithIndex(it->first);

			a_OutputStream << "layout(binding=" + std::to_string(it->first) + ") ";
			a_OutputStream << "uniform ";

			// Handle templated samplers
			if (!samplerMap[name].m_TemplateArgList.empty())
			{
				Database::Variable convertedVar;
				ResultCode resultCode = m_TemplateConverter->ConvertTemplatedVariable(samplerMap[name], convertedVar);

				if (resultCode != SAVVY_OK)
					return resultCode;

				for (uint32 i = 0; i < convertedVar.m_QualifierList.size(); i++)
				{
					const Key& type = convertedVar.m_QualifierList[i];
					a_OutputStream << type.GetString() << " ";
				}
			}
			else
			{
				for (uint32 i = 0; i < samplerMap[name].m_QualifierList.size(); i++)
				{
					const Key& type = samplerMap[name].m_QualifierList[i];

					if (GLSLEquivalentExists(type))
					{
						const Key& convertedKey = GetGLSLKeyword(type);
						a_OutputStream << convertedKey.GetString() << " ";
					}
					else
					{
						return SAVVY_LOGICAL_ERROR;
					}
				}
			}
			
			a_OutputStream << name.GetString() << ";" << std::endl;
		}
		a_OutputStream << std::endl;
	}
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ConstructorHLSLToGLSL::ConstructStructsGLSL(std::ostream& a_OutputStream)
{
	Database::KeyList& structList = m_Database->GetStructList();
	Database::KeyVarMultiMap& varMap = m_Database->GetStructVariableMap();
	Database::WordMultiMap& instructionMap = m_Database->GetInstructionMap();

	if (!structList.empty())
	{
		Database::KeyList::iterator structIt;

		for (structIt = structList.begin(); structIt != structList.end(); ++structIt)
		{
			if (structIt->GetString() == m_InputStructName.GetString() || structIt->GetString() == m_OutputStructName.GetString())
				continue;

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
					if (GLSLEquivalentExists(list[i]))
					{
						const Key& type = GetGLSLKeyword(list[i]);
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

Savvy::ResultCode Savvy::Internal::ConstructorHLSLToGLSL::ConstructGlobalVarsGLSL(std::ostream& a_OutputStream)
{
	Database::VarList& globalVars = m_Database->GetGlobalVariableList();
	Database::WordMultiMap& instructionMap = m_Database->GetInstructionMap();
	Database::VarList::iterator it;

	for (it = globalVars.begin(); it != globalVars.end(); ++it)
	{
		Database::KeyList& list = (*it).m_QualifierList;
		uint32 sizeType = list.size();
		//a_OutputStream << "static ";

		// If it's an HLSL sampler, skip it
		bool skip = false;
		for (uint32 i = 0; i < sizeType; i++)
		{
			if (list[i].Contains("sampler") || list[i].Contains("SamplerState"))
			{
				skip = true;
				break;
			}
		}

		if (skip)
			continue;

		for (uint32 i = 0; i < sizeType; i++)
		{
			if (GLSLEquivalentExists(list[i]))
			{
				const Key& type = GetGLSLKeyword(list[i]);
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
					return SAVVY_LOGICAL_ERROR;
				}
			}
		}

		// Output name
		std::string line;

		// Check if we have a duplicate and rename the variable if we do
		Key& name = (*it).m_Name;
		bool existsInOutputStruct = m_Database->StructVariableExists(m_OutputStructName, name);
		bool existsInInputStruct = m_Database->StructVariableExists(m_InputStructName, name);
		while (existsInInputStruct || existsInOutputStruct)
		{
			name.SetString("g_" + name.GetString());
			existsInOutputStruct = m_Database->StructVariableExists(m_OutputStructName, name);
			existsInInputStruct = m_Database->StructVariableExists(m_InputStructName, name);
		}
		line.append(name.GetString() + " ");

		// Do the initialization instructions if we have any
		Database::WordMultiMap::iterator it1 = instructionMap.lower_bound(name);
		Database::WordMultiMap::iterator it2 = instructionMap.upper_bound(name);

		while (it1 != it2)
		{
			if (it1->second.m_Translate)
			{
				if (GLSLEquivalentExists(it1->second.m_Name))
				{
					const Key& type = GetGLSLKeyword(it1->second.m_Name);

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

Savvy::ResultCode Savvy::Internal::ConstructorHLSLToGLSL::ConstructFunctionsGLSL(std::ostream& a_OutputStream)
{
	Database::FunctionList& funcList = m_Database->GetFunctionList();
	Database::WordMultiMap& instructionMap = m_Database->GetInstructionMap();

	if (funcList.empty())
	{
		return SAVVY_LOGICAL_ERROR;
	}

	Database::FunctionList::iterator it;
	ResultCode resCode;

	for (it = funcList.begin(); it != funcList.end(); ++it)
	{
		if (it->m_Name == m_InputEntry)
		{
			// Construct main
			a_OutputStream << "void main()" << std::endl;
			a_OutputStream << "{" << std::endl;

			resCode = ConstructMainInnerFunctionGLSL(a_OutputStream);

			a_OutputStream << "}" << std::endl;
		}
		else
		{
			Key returnType;
			if (GLSLEquivalentExists(it->m_ReturnType))
			{
				returnType = GetGLSLKeyword(it->m_ReturnType);
			}
			else
			{
				returnType = it->m_ReturnType;
			}
			a_OutputStream << returnType.GetString() + " " + it->m_Name.GetString();
			a_OutputStream << "(";

			uint32 argSize = it->m_Arguments.size();
			for (uint32 i = 0; i < argSize; ++i)
			{
				bool skipArg = false;
				uint32 typeSize = it->m_Arguments[i].m_QualifierList.size();
				Database::KeyList& list = it->m_Arguments[i].m_QualifierList;

				for (uint32 j = 0; j < typeSize; ++j)
				{
					if (list[j] == m_InputStructName || (list[j] == m_OutputStructName && m_CurrShader != FRAGMENT_SHADER))
						skipArg = true;

					if (skipArg)
						break;

					if (GLSLEquivalentExists(list[j]))
					{
						const Key& k = GetGLSLKeyword(list[j]);
						a_OutputStream << k.GetString() + " ";
					}
					else
					{
						// User type
						a_OutputStream << list[j].GetString() + " ";
					}
				}

				if (skipArg)
					continue;

				a_OutputStream << it->m_Arguments[i].m_Name.GetString();

				// Do extra instructions
				Key currKey = it->m_Name + "." + it->m_Arguments[i].m_Name;
				Database::WordMultiMap::iterator instructionIt1 = instructionMap.lower_bound(currKey);
				Database::WordMultiMap::iterator instructionIt2 = instructionMap.upper_bound(currKey);

				while (instructionIt1 != instructionIt2)
				{
					if (instructionIt1->second.m_Translate)
					{
						const Key& k = GetGLSLKeyword(instructionIt1->second.m_Name);
						a_OutputStream << k.GetString() + " ";
					}
					else
					{
						a_OutputStream << instructionIt1->second.m_Name.GetString() + " ";
					}
					++instructionIt1;
				}

				if (i != argSize - 1)
				{
					a_OutputStream << ", ";
				}
			}
			a_OutputStream << ")" << std::endl;
			a_OutputStream << "{" << std::endl;

			resCode = ConstructGenericInnerFunctionGLSL(a_OutputStream, it->m_Name);
			if (resCode != SAVVY_OK)
				return resCode;
			a_OutputStream << "}" << std::endl;
			a_OutputStream << std::endl;
		}
	}

	return resCode;
}

Savvy::ResultCode Savvy::Internal::ConstructorHLSLToGLSL::ConstructMainInnerFunctionGLSL(std::ostream& a_OutputStream)
{
	ResultCode res = ConstructGenericInnerFunctionGLSL(a_OutputStream, m_InputEntry);
	return res;
}

Savvy::ResultCode Savvy::Internal::ConstructorHLSLToGLSL::ConstructGenericInnerFunctionGLSL(std::ostream& a_OutputStream, Key& a_FuncName)
{
	m_CurrFunction = a_FuncName;
	const Database::Function& entry = m_Database->GetFunction(m_InputEntry);
	Database::WordMultiMap& instructionMap = m_Database->GetInstructionMap();
	Database::KeyVarMultiMap& uniformVariables = m_Database->GetUniformMap();

	Database::WordMultiMap::iterator it1 = instructionMap.lower_bound(a_FuncName);
	Database::WordMultiMap::iterator it2 = instructionMap.upper_bound(a_FuncName);

	std::string currentLine;

	// Indicates whether the current line is a matrix multiplication line
	bool matrixMulLine = false;
	Key outputStructName;
	
	if (m_CurrShader != FRAGMENT_SHADER)
	{
		ResultCode result = RetrieveOutputStructName(outputStructName);

		if (result != SAVVY_OK)
			return result;
	}

	uint32 numTabs = 1;
	Key inputStructName = entry.m_Arguments[0].m_Name;

	ResultCode res = RemoveExtraFuncArgs(inputStructName, outputStructName);

	if (res != SAVVY_OK)
		return res;

	while (it1 != it2)
	{
		RemoveAutomaticVars(it1);
		Database::Word word = it1->second;

		if (!word.m_Translate)
		{

			// Check if  it's a global variable and we renamed it
			Key nameToSearch = "g_" + it1->second.m_Name.GetString();
			if (m_Database->GlobalVariableExists(nameToSearch))
			{
				it1->second.m_Name.SetString("g_" + it1->second.m_Name.GetString());
			}

			if (a_FuncName == m_InputEntry)
			{
				if (word.m_Name == "return")
				{
					if (m_CurrShader == FRAGMENT_SHADER)
					{
						it1++;
						if (it1->second.m_Name != SAVVY_FRAGMENT_SHADER_OUTPUT_NAME)
						{
							word.m_Name.SetString(SAVVY_FRAGMENT_SHADER_OUTPUT_NAME);
							word.m_Name.Append(" =");
						}
						else
						{
							word.m_Name.Clear();
							it1->second.m_Name.Clear();

							it1++;
							it1->second.m_Name.Clear();
							it1--;
						}
						it1--;
					}
				}

				if (word.m_Name == entry.m_ReturnType)
				{
					// Skip the declaration of the output structure
					word.m_Name.Clear();
					it1++;

					if (!it1->second.m_Translate)
					{
						outputStructName = it1->second.m_Name;
					}

					it1++; 
				}

			}

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
				if (!word.m_Name.IsEmpty())
					currentLine.append(word.m_Name.GetString() + " ");
			}
		}
		else
		{
			ResultCode resCode;
			if (!GLSLEquivalentExists(word.m_Name) && word.m_Name != "mul")
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

					if (it1->second.m_Name.IsEmpty())
					{
						it1++;
						continue;
					}

					resCode = HandleNoEquivalentFunc(it1, currentLine, inputStructName, outputStructName);

					if (resCode != SAVVY_OK)
					{
						m_LastError = "Could not convert instruction with name: " + word.m_Name.GetString() + ".";
						return resCode;
					}
				}
			}

			if (a_FuncName == m_InputEntry && m_CurrShader == FRAGMENT_SHADER)
			{
				if (word.m_Name == "float4")
				{
					it1++;
					if (it1->second.m_Name == SAVVY_FRAGMENT_SHADER_OUTPUT_NAME)
					{
						// Skip
						word.m_Name.Clear();
						it1->second.m_Name.Clear();
						it1++;
						it1->second.m_Name.Clear();
						it1--;
					}
					it1--;
				}
			}

			if (!GLSLEquivalentExists(word.m_Name))
			{
				resCode = HandleMatrixMultiplication(it1, currentLine);

				if (resCode != SAVVY_OK)
				{
					m_LastError = "Could not convert instruction with name: " + word.m_Name.GetString() + ".";
					return resCode;
				}

				it1++;
				continue;
			}

			// Check if it's a cast

			if (it1 != instructionMap.lower_bound(a_FuncName))
			{
				it1--;
				if (it1->second.m_Name == "(")
				{
					it1++; it1++;

					if (it1->second.m_Name == ")")
					{
						uint32 pos = currentLine.rfind("(");
						currentLine.erase(pos, pos + 1);
						// C-style casts not allowed in GLSL - turn them into constructors
						it1->second.m_Name.SetString("(");
						it1++;
						it1->second.m_Name.SetString(it1->second.m_Name.GetString() + ") ");
						it1--;
					}
					it1--; it1--;
				}
				it1++;
			}

			const Key& translatedKey = GetGLSLKeyword(word.m_Name);

			currentLine.append(translatedKey.GetString() + " ");
		}

		it1++;
	}
	return SAVVY_OK;
}

const bool Savvy::Internal::ConstructorHLSLToGLSL::IsEndOfLine(const Key& a_Key, const std::string& a_CurrLine)
{
	if ((a_Key.GetString() == ";" && a_CurrLine.find("for (") == std::string::npos)
		|| a_Key.GetString() == "{" || a_Key.GetString() == "}")
		return true;

	return false;
}

bool Savvy::Internal::ConstructorHLSLToGLSL::GLSLEquivalentExists(const Key& a_Name)
{
	return m_Database->OutputLangEquivalentExists(a_Name);
}

const Savvy::Key& Savvy::Internal::ConstructorHLSLToGLSL::GetGLSLKeyword(const Key& a_Name)
{
	return m_Database->GetOutputLangEquivalent(a_Name);
}

Savvy::ResultCode Savvy::Internal::ConstructorHLSLToGLSL::HandleNoEquivalentFunc(Database::WordMultiMap::iterator& it1, std::string& currentLine, Key& a_InputStruct, Key& a_OutputStruct)
{
	ResultCode res = m_FuncConverter->ConvertFunction(it1, currentLine, m_CurrFunction, m_CurrShader, a_InputStruct, a_OutputStruct);

	return res;
}

Savvy::ResultCode Savvy::Internal::ConstructorHLSLToGLSL::HandleMatrixMultiplication(Database::WordMultiMap::iterator& it1, std::string& currentLine)
{
	Database::Word word = it1->second;
	if (word.m_Name == "mul")
	{
		uint32 ctrAhead = 0;
		uint32 openBrackets = 0;
		uint32 closeBrackets = 0;
		bool stop = false;
		while (!stop)
		{
			it1++;
			ctrAhead++;
			Key& currKey = it1->second.m_Name;

			if (currKey == ";")
			{
				m_LastError = "Could not convert matrix multiplication.";
				return SAVVY_SHADER_SYNTAX_ERROR;
			}
			else if (currKey == "(")
			{
				openBrackets++;
			}
			else if (currKey == ")")
			{
				closeBrackets++;

				if (closeBrackets == openBrackets)
				{
					currKey.Clear(); // Get rid of closing bracket
					stop = true;
				}
			}
			else if (currKey == ",")
			{
				// Check scope
				if (closeBrackets == openBrackets - 1)
				{
					// Replace comma with multiplication
					currKey.SetString("*");
				}
			}

			if (ctrAhead == 1)
			{
				// Compulsory open bracket here
				if (currKey != "(")
				{
					m_LastError = "Could not convert matrix multiplication.";
					return SAVVY_SHADER_SYNTAX_ERROR;
				}

				currKey.Clear();
			}
		}

		// Return back to proper place
		for (uint32 i = 0; i < ctrAhead; ++i)
		{
			it1--;
		}
	}
	return SAVVY_OK;
}

const bool Savvy::Internal::ConstructorHLSLToGLSL::HandleBuiltInSemantic(Key& a_Name, Key& a_VarName, bool a_Rename /*= false*/)
{
	if (a_Name.Contains("SV_CullDistance") || a_Name.Contains("SV_CULLDISTANCE"))
	{
		std::string semanticName = a_Name.GetString();
		uint32 pos = semanticName.rfind("SV_CullDistance");
		if (pos == std::string::npos)
		{
			pos = semanticName.rfind("SV_CULLDISTANCE");
		}
		std::string indexString = semanticName.substr(pos + 1);
		std::string newVar = "gl_CullDistance[" + indexString + "]";

		std::string oldVarName = a_VarName.GetString();
		//ReplaceVarName(oldVarName, newVar);

		if (a_Rename)
			a_VarName = newVar;
		return true;
	}
	else if (a_Name.Contains("SV_ClipDistance") || a_Name.Contains("SV_CLIPDISTANCE"))
	{
		std::string semanticName = a_Name.GetString();
		uint32 pos = semanticName.rfind("SV_ClipDistance");

		if (pos == std::string::npos)
		{
			pos = semanticName.rfind("SV_CLIPDISTANCE");
		}
		std::string indexString = semanticName.substr(pos + 1);
		std::string newVar = "gl_ClipDistance[" + indexString + "]";

		std::string oldVarName = a_VarName.GetString();
		//ReplaceVarName(oldVarName, newVar);
		if (a_Rename)
			a_VarName = newVar;
		return true;
	}
	else if (a_Name.Contains("SV_Target") || a_Name.Contains("SV_TARGET"))
	{
		std::string semanticName = a_Name.GetString();
		uint32 pos = semanticName.rfind("SV_Target");
		if (pos == std::string::npos)
		{
			pos = semanticName.rfind("SV_TARGET");
		}
		std::string indexString = semanticName.substr(pos + 1);
		std::string newVar = "gl_FragColor[" + indexString + "]";

		std::string oldVarName = a_VarName.GetString();
		//ReplaceVarName(oldVarName, newVar);
		if (a_Rename)
			a_VarName = newVar;
		return true;
	}
	else if (a_Name == "SV_Position" || a_Name == "SV_POSITION")
	{
		std::string newVar;
		if (m_CurrShader == FRAGMENT_SHADER)
		{
			newVar = "gl_FragCoord";
		}
		else
		{
			newVar = "gl_Position";
		}
		std::string oldVarName = a_VarName.GetString();
		//ReplaceVarName(oldVarName, newVar);
		if (a_Rename)
			a_VarName = newVar;
		return true;
	}
	else if (a_Name == "SV_Depth" || a_Name == "SV_DEPTH")
	{
		std::string newVar = "gl_FragDepth";
		std::string oldVarName = a_VarName.GetString();
		//ReplaceVarName(oldVarName, newVar);
		if (a_Rename)
			a_VarName = newVar;
		return true;
	}
	else if (a_Name == "PSIZE" || a_Name == "psize")
	{
		std::string newVar = "gl_PointSize";
		std::string oldVarName = a_VarName.GetString();
		//ReplaceVarName(oldVarName, newVar);
		if (a_Rename)
			a_VarName = newVar;
		return true;
	}
	else if (a_Name == "SV_IsFrontFace" || a_Name == "SV_ISFRONTFACE")
	{
		std::string newVar = "gl_FrontFacing";
		std::string oldVarName = a_VarName.GetString();
		//ReplaceVarName(oldVarName, newVar);
		if (a_Rename)
			a_VarName = newVar;
		return true;
	}

	return false;
}

Savvy::ResultCode Savvy::Internal::ConstructorHLSLToGLSL::ConstructDefines(std::ostream& a_OutputStream)
{
	Database::KeyList& defineList = m_Database->GetDefinesList();
	Database::KeyList::iterator it;

	for (it = defineList.begin(); it != defineList.end(); ++it)
	{
		a_OutputStream << (*it).GetString() << std::endl;
	}
	a_OutputStream << "\n";
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ConstructorHLSLToGLSL::ConstructVertexOrFragment(std::ostream& a_OutputStream)
{
	ResultCode res;

	res = ConstructDefines(a_OutputStream);

	if (res != SAVVY_OK)
	{
		m_LastError.append("Error while constructing defines.");
		return res;
	}

	// Construct Structures
	res = ConstructStructsGLSL(a_OutputStream);

	if (res != SAVVY_OK)
	{
		m_LastError.append("Error while constructing structures.");
		return res;
	}

	res = ConstructUniformBuffersGLSL(a_OutputStream);

	if (res != SAVVY_OK)
	{
		m_LastError.append("Error while constructing buffers.");
		return res;
	}

	res = ConstructGenericBuffersGLSL(a_OutputStream);

	if (res != SAVVY_OK)
	{
		m_LastError.append("Error while constructing buffers.");
		return res;
	}

	res = ConstructSamplersGLSL(a_OutputStream);

	if (res != SAVVY_OK)
	{
		m_LastError.append("Error while constructing samplers\textures.");
		return res;
	}

	// Inputs
	res = ConstructInputsGLSL(a_OutputStream);

	if (res != SAVVY_OK)
	{
		m_LastError.append("Error while constructing inputs.");
		return res;
	}

	// Outputs
	res = ConstructOutputsGLSL(a_OutputStream);

	if (res != SAVVY_OK)
	{
		m_LastError.append("Error while constructing outputs.");
		return res;
	}

	//Construct global variables
	res = ConstructGlobalVarsGLSL(a_OutputStream);

	if (res != SAVVY_OK)
	{
		m_LastError.append("Error while constructing global variables.");
		return res;
	}

	// Construct Functions
	res = ConstructFunctionsGLSL(a_OutputStream);

	if (res != SAVVY_OK)
	{
		m_LastError.append("Error while constructing functions.");
		return res;
	}

	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ConstructorHLSLToGLSL::RemoveAutomaticVars(Database::WordMultiMap::iterator& it1)
{
	if (it1->second.m_Name == "float")
	{
		it1++;
		bool handled = false;
		if (it1->second.m_Name.Contains(SAVVY_TEX_HEIGHT) || it1->second.m_Name.Contains(SAVVY_TEX_WIDTH))
		{
			it1++;
			it1->second.m_Name.Clear();
			handled = true;
		}

		if (!handled)
			it1--;
	}
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ConstructorHLSLToGLSL::RemoveExtraFuncArgs( Key& a_InputStructName, Key& a_OutputStructName )
{
	// Make sure we delete it in every function call
	Database::WordMultiMap::iterator instructionIt;
	Database::WordMultiMap& instructionMap = m_Database->GetInstructionMap();
	for (instructionIt = instructionMap.begin(); instructionIt != instructionMap.end(); ++instructionIt)
	{
		if (instructionIt->second.m_Name == a_InputStructName)
		{
			instructionIt->second.m_Name.Clear();
			instructionIt++;
			if (instructionIt->second.m_Name == ",")
			{
				instructionIt->second.m_Name.Clear();
			}
			instructionIt--;

			if (m_CurrFunction == m_InputEntry)
			{
				instructionIt--;

				if (instructionIt->second.m_Name == "return")
					instructionIt->second.m_Name.Clear();

				instructionIt++;
				instructionIt++;
				if (instructionIt->second.m_Name == ";")
					instructionIt->second.m_Name.Clear();

				instructionIt--;
			}

			instructionIt++;
			if (instructionIt->second.m_Name == ".")
			{
				instructionIt->second.m_Name.Clear();
				instructionIt++;
				// Check if it's a built-in variable
				Key& name = instructionIt->second.m_Name;
				if (m_Database->StructVariableExists(m_InputStructName, name))
				{
					Key semantic = m_Database->GetSemantic(m_InputStructName, name);
					bool handled = HandleBuiltInSemantic(semantic, name, true);

					if (!handled)
					{
						// Check if we renamed this one
						while (!m_Database->StructVariableExists(m_InputStructName, instructionIt->second.m_Name))
						{
							instructionIt->second.m_Name.SetString("out_" + instructionIt->second.m_Name.GetString());
						}
					}
				}
			}
		}

		if (m_CurrShader != FRAGMENT_SHADER)
		{
			if (instructionIt->second.m_Name == a_OutputStructName)
			{
				instructionIt->second.m_Name.Clear();

				instructionIt++;
				if (instructionIt->second.m_Name == ",")
				{
					instructionIt->second.m_Name.Clear();
				}
				instructionIt--;

				if (m_CurrFunction == m_InputEntry)
				{
					instructionIt--;

					if (instructionIt->second.m_Name == "return")
						instructionIt->second.m_Name.Clear();

					instructionIt++;
					instructionIt++;
					if (instructionIt->second.m_Name == ";")
						instructionIt->second.m_Name.Clear();

					instructionIt--;
				}

				instructionIt++;
				if (instructionIt->second.m_Name == ".")
				{
					instructionIt->second.m_Name.Clear();

					instructionIt++;

					// Check if it's a built-in variable
					Key& name = instructionIt->second.m_Name;
					if (m_Database->StructVariableExists(m_OutputStructName, name))
					{
						Key semantic = m_Database->GetSemantic(m_OutputStructName, name);
						bool handled = HandleBuiltInSemantic(semantic, name, true);

						if (!handled)
						{
							// Check if we renamed this one
							while (!m_Database->StructVariableExists(m_OutputStructName, instructionIt->second.m_Name))
							{
								instructionIt->second.m_Name.SetString("out_" + instructionIt->second.m_Name.GetString());
							}
						}
					}
				}
			}
		}
	}

	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ConstructorHLSLToGLSL::RetrieveOutputStructName(Key& a_Name)
{
	Database::WordMultiMap& instructionMap = m_Database->GetInstructionMap();

	Database::WordMultiMap::iterator it1 = instructionMap.lower_bound(m_InputEntry);
	Database::WordMultiMap::iterator it2 = instructionMap.upper_bound(m_InputEntry);

	while (it1 != it2)
	{
		if (it1->second.m_Name == "return")
		{
			it1++;
			a_Name = it1->second.m_Name;
		}
		it1++;
	}

	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ConstructorHLSLToGLSL::ConstructGenericBuffersGLSL(std::ostream& a_OutputStream)
{
	Database::KeyList& genericBuffers = m_Database->GetGenericBufferList();
	Database::IndexMap& genericIndices = m_Database->GetGenericBufferIndexMap();

	if (!genericBuffers.empty())
	{
		Database::KeyVarMultiMap& varMap = m_Database->GetGenericBufferVarMap();

		Database::KeyVarMultiMap::iterator genericIt1;
		Database::KeyVarMultiMap::iterator genericIt2;
		Database::IndexMap::iterator genericIndicesIt;
		for (genericIndicesIt = genericIndices.begin(); genericIndicesIt != genericIndices.end(); ++genericIndicesIt)
		{
			const Key& buffName = genericIndicesIt->second;

			a_OutputStream << "layout(std430, binding=" + std::to_string(genericIndicesIt->first) + ") ";

			a_OutputStream << "buffer ";
			a_OutputStream << buffName.GetString() + "Buffer" << std::endl;
			a_OutputStream << "{" << std::endl;

			// Loop through all the variables
			genericIt1 = varMap.lower_bound(buffName);
			genericIt2 = varMap.upper_bound(buffName);

			while (genericIt1 != genericIt2)
			{
				// Output
				a_OutputStream << "\t";
				Database::KeyList& list = genericIt1->second.m_QualifierList;

				uint32 size = list.size();

				for (uint32 i = 0; i < size; i++)
				{
					if (GLSLEquivalentExists(list[i]))
					{
						const Key& varType = GetGLSLKeyword(list[i]);
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
				a_OutputStream << genericIt1->second.m_Name.GetString();
				a_OutputStream << "[];";
				a_OutputStream << std::endl;
				++genericIt1;
			}

			a_OutputStream << "};" << std::endl;
			a_OutputStream << std::endl;
			a_OutputStream << std::endl;
		}
	}
	return SAVVY_OK;
}
