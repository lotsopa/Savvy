#include "SavvyPCH.h"
#include "SavvyConstructorHLSLToGLSLNoUBO.h"
#include "SavvyDatabaseHLSL.h"
#include "SavvyFuncConverter.h"
#include "SavvyFuncConverterHLSLToGLSL.h"

Savvy::Internal::ConstructorHLSLToGLSLNoUBO::ConstructorHLSLToGLSLNoUBO() : ConstructorHLSLToGLSL()
{

}

Savvy::Internal::ConstructorHLSLToGLSLNoUBO::~ConstructorHLSLToGLSLNoUBO()
{

}

void Savvy::Internal::ConstructorHLSLToGLSLNoUBO::Reset()
{
	ConstructorHLSLToGLSL::Reset();
}

Savvy::ResultCode Savvy::Internal::ConstructorHLSLToGLSLNoUBO::Initialize(Key& a_InputEntry, Key& a_OutputEntry, uint32 a_Inputlang, uint32 a_ShaderType, Database* a_Database)
{
	return ConstructorHLSLToGLSL::Initialize(a_InputEntry, a_OutputEntry, a_Inputlang, a_ShaderType, a_Database);
}

Savvy::ResultCode Savvy::Internal::ConstructorHLSLToGLSLNoUBO::Construct(std::ostream& a_OutputStream)
{
	return ConstructorHLSLToGLSL::Construct(a_OutputStream);
}

Savvy::ResultCode Savvy::Internal::ConstructorHLSLToGLSLNoUBO::ConstructUniformBuffersGLSL(std::ostream& a_OutputStream)
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
			a_OutputStream << "layout(location=" + std::to_string(locationCtr) + ") ";
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