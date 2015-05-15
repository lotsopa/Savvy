#include "SavvyPCH.h"
#include "SavvyConstructorHLSLToGLSLNoUBONoLoc.h"
#include "SavvyDatabaseHLSL.h"
#include "SavvyFuncConverter.h"
#include "SavvyFuncConverterHLSLToGLSL.h"
#include "SavvyTemplateConverterHLSLToGLSL.h"

Savvy::Internal::ConstructorHLSLToGLSLNoUBONoLoc::ConstructorHLSLToGLSLNoUBONoLoc() : ConstructorHLSLToGLSL()
{
	
}

Savvy::Internal::ConstructorHLSLToGLSLNoUBONoLoc::~ConstructorHLSLToGLSLNoUBONoLoc()
{

}

void Savvy::Internal::ConstructorHLSLToGLSLNoUBONoLoc::Reset()
{
	ConstructorHLSLToGLSL::Reset();
}

Savvy::ResultCode Savvy::Internal::ConstructorHLSLToGLSLNoUBONoLoc::Initialize(Key& a_InputEntry, Key& a_OutputEntry, uint32 a_Inputlang, uint32 a_ShaderType, Database* a_Database)
{
	return ConstructorHLSLToGLSL::Initialize(a_InputEntry, a_OutputEntry, a_Inputlang, a_ShaderType, a_Database);
}

Savvy::ResultCode Savvy::Internal::ConstructorHLSLToGLSLNoUBONoLoc::Construct(std::ostream& a_OutputStream)
{
	return ConstructorHLSLToGLSL::Construct(a_OutputStream);
}

Savvy::ResultCode Savvy::Internal::ConstructorHLSLToGLSLNoUBONoLoc::ConstructInputsGLSL(std::ostream& a_OutputStream)
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

Savvy::ResultCode Savvy::Internal::ConstructorHLSLToGLSLNoUBONoLoc::ConstructOutputsGLSL(std::ostream& a_OutputStream)
{
	if (m_CurrShader == FRAGMENT_SHADER)
	{
		// Construct only one output
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
		a_OutputStream << "out ";

		a_OutputStream << entry.m_ReturnType.GetString() + " ";
		a_OutputStream << "OutputVariable" + std::to_string(locationCtr) + ";" << std::endl;
		locationCtr++;
	}
	a_OutputStream << std::endl;

	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ConstructorHLSLToGLSLNoUBONoLoc::ConstructUniformBuffersGLSL(std::ostream& a_OutputStream)
{
	Database::KeyList& uniformBuffers = m_Database->GetUniformBufferList();
	Database::IndexMap& uniformIndices = m_Database->GetUniformBufferIndexMap();
	Database::WordMultiMap& instructionMap = m_Database->GetInstructionMap();

	if (!uniformBuffers.empty())
	{
		Database::KeyVarMultiMap& uniformMap = m_Database->GetUniformMap();

		Database::KeyVarMultiMap::iterator it;
		Database::IndexMap::iterator uniformIndicesIt;
		uint32 locationCtr = 0;
		for (it = uniformMap.begin(); it != uniformMap.end(); ++it)
		{
			a_OutputStream << "uniform ";
			Database::KeyList& list = it->second.m_QualifierList;

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
			a_OutputStream << it->second.m_Name.GetString();

			// Do extra init instructions
			Database::WordMultiMap::iterator it1 = instructionMap.lower_bound(it->first.GetString() + "." + it->second.m_Name.GetString());
			Database::WordMultiMap::iterator it2 = instructionMap.upper_bound(it->first.GetString() + "." + it->second.m_Name.GetString());

			while (it1 != it2)
			{
				a_OutputStream << it1->second.m_Name.GetString();
				it1++;
			}

			a_OutputStream << ";";
			a_OutputStream << std::endl;
			locationCtr++;
		}
		a_OutputStream << std::endl;
	}

	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::ConstructorHLSLToGLSLNoUBONoLoc::ConstructSamplersGLSL(std::ostream& a_OutputStream)
{
	Database::KeyVarMap& samplerMap = m_Database->GetSamplerList();
	Database::IndexMap& indexMap = m_Database->GetSamplerIndexMap();

	if (!samplerMap.empty())
	{
		Database::IndexMap::iterator it;

		for (it = indexMap.begin(); it != indexMap.end(); ++it)
		{
			const Key& name = m_Database->GetSamplerWithIndex(it->first);
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

Savvy::ResultCode Savvy::Internal::ConstructorHLSLToGLSLNoUBONoLoc::ConstructGenericBuffersGLSL(std::ostream& a_OutputStream)
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

			a_OutputStream << "layout(std430) ";

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
				a_OutputStream << ";";
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
