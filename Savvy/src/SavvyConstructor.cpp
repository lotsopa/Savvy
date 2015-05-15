#include "SavvyPCH.h"
#include "SavvyConstructor.h"

Savvy::Constructor::Constructor() : m_CurrShader(INVALID_SHADER_TYPE)
{

}

Savvy::Constructor::~Constructor()
{

}

void Savvy::Constructor::Reset()
{
	m_InputEntry.Clear();
	m_OutputEntry.Clear();
	m_Database = NULL;
	m_CurrShader = INVALID_SHADER_TYPE;
}

Savvy::ResultCode Savvy::Constructor::Initialize(Key& a_InputEntry, Key& a_OutputEntry, uint32 a_Inputlang, uint32 a_ShaderType, Database* a_Database)
{
	m_InputEntry = a_InputEntry;
	m_OutputEntry = a_OutputEntry;

	if (a_Database == NULL)
		return SAVVY_INVALID_ARG;

	m_Database = a_Database;
	m_InputShaderLang = a_Inputlang;
	m_CurrShader = a_ShaderType;
	return SAVVY_OK;
}

void Savvy::Constructor::ReplaceVarName(std::string& a_Var, std::string& a_NewVar)
{
	Database::WordMultiMap& instructionMap = m_Database->GetInstructionMap();
	Database::WordMultiMap::iterator it = instructionMap.begin();
	for (it = instructionMap.begin(); it != instructionMap.end(); ++it)
	{
		if (it->second.m_Name == a_Var)
		{
			std::string oldVar = it->second.m_Name.GetString();
			std::string replaced = ReplaceAll(oldVar, a_Var, a_NewVar);
			it->second.m_Name.SetString(replaced);
		}
	}

	Database::KeyVarMap& inputVarMap = m_Database->GetInputMap();
	Database::KeyVarMap::iterator inputIt;
	for (inputIt = inputVarMap.begin(); inputIt != inputVarMap.end(); ++inputIt)
	{
		if (inputIt->first == a_Var)
		{
			std::string oldVar = inputIt->first.GetString();
			std::string replaced = ReplaceAll(oldVar, a_Var, a_NewVar);
			inputVarMap.insert(std::make_pair(replaced, inputIt->second));
			inputVarMap.erase(inputIt);
			return;
		}
	}

	Database::KeyVarMap& outputVarMap = m_Database->GetOutputMap();
	Database::KeyVarMap::iterator outputIt;
	for (outputIt = outputVarMap.begin(); outputIt != outputVarMap.end(); ++outputIt)
	{
		if (outputIt->first == a_Var)
		{
			std::string oldVar = outputIt->first.GetString();
			std::string replaced = ReplaceAll(oldVar, a_Var, a_NewVar);
			outputVarMap.insert(std::make_pair(replaced, outputIt->second));
			outputVarMap.erase(outputIt);
			return;
		}
	}

	Database::KeyVarMultiMap& structVarMap = m_Database->GetStructVariableMap();
	Database::KeyVarMultiMap::iterator structIt;
	for (structIt = structVarMap.begin(); structIt != structVarMap.end(); ++structIt)
	{
		if (structIt->second.m_Name == a_Var)
		{
			std::string oldVar = structIt->second.m_Name.GetString();
			std::string replaced = ReplaceAll(oldVar, a_Var, a_NewVar);
			structIt->second.m_Name.SetString(replaced);
		}
	}

	Database::VarList& globalVarMap = m_Database->GetGlobalVariableList();
	Database::VarList::iterator globalIt;
	for (globalIt = globalVarMap.begin(); globalIt != globalVarMap.end(); ++globalIt)
	{
		if (globalIt->m_Name == a_Var)
		{
			std::string oldVar = globalIt->m_Name.GetString();
			std::string replaced = ReplaceAll(oldVar, a_Var, a_NewVar);
			globalIt->m_Name.SetString(replaced);
			return;
		}
	}
}

std::string Savvy::Constructor::ReplaceAll(std::string str, const std::string& from, const std::string& to)
{
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos)
	{
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
	}
	return str;
}
