#include "SavvyPCH.h"
#include "SavvyDatabase.h"

Savvy::Database::Database() : m_GenericBufferIndex(0), m_UniformBufferIndex(0), m_InputsIndex(0),
m_SamplerIndex(0), m_OutputsIndex(0)
{

}

Savvy::Database::~Database()
{

}

Savvy::ResultCode Savvy::Database::AddInputVariable(Key& a_Name, KeyList& a_TypeQualifierList, KeyList& a_TemplateArgList)
{
	bool entryExists = InputVariableExists(a_Name);

	if (entryExists)
	{
		return SAVVY_SHADER_SYNTAX_ERROR;
	}
	
	Variable var;
	var.m_Name = a_Name;
	var.m_QualifierList = a_TypeQualifierList;
	var.m_TemplateArgList = a_TemplateArgList;
	m_InputVariables.insert(std::make_pair(Key(a_Name), var));
	bool idxExists = InputIndexExists(m_InputsIndex);
	if (idxExists)
	{
		// Find the first available index
		m_InputsIndex = 0;
		idxExists = InputIndexExists(m_InputsIndex);
		while (idxExists)
		{
			m_InputsIndex++;
			idxExists = InputIndexExists(m_InputsIndex);
		}
		m_InputIndexMap[m_InputsIndex] = a_Name;
	}
	else
	{
		m_InputIndexMap[m_InputsIndex] = a_Name;
		m_InputsIndex++;
	}
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Database::AddInputVariable(Key& a_Name, KeyList& a_TypeQualifierList, uint32 a_Idx, KeyList& a_TemplateArgList)
{
	bool entryExists = InputVariableExists(a_Name);

	if (entryExists)
	{
		return SAVVY_SHADER_SYNTAX_ERROR;
	}

	Variable var;
	var.m_Name = a_Name;
	var.m_QualifierList = a_TypeQualifierList;
	var.m_TemplateArgList = a_TemplateArgList;
	m_InputVariables.insert(std::make_pair(Key(a_Name), var));

	bool idxExists = InputIndexExists(a_Idx);

	if (idxExists)
	{
		return SAVVY_SHADER_SYNTAX_ERROR;
	}

	m_InputIndexMap[a_Idx] = a_Name;
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Database::AddOutputVariable(Key& a_Name, KeyList& a_TypeQualifierList, KeyList& a_TemplateArgList)
{
	bool entryExists = OutputVariableExists(a_Name);

	if (entryExists)
	{
		return SAVVY_SHADER_SYNTAX_ERROR;
	}
	Variable var;
	var.m_Name = a_Name;
	var.m_QualifierList = a_TypeQualifierList;
	var.m_TemplateArgList = a_TemplateArgList;
	m_OutputVariables.insert(std::make_pair(Key(a_Name), var));

	bool idxExists = OutputIndexExists(m_OutputsIndex);
	if (idxExists)
	{
		// Find the first available index
		m_OutputsIndex = 0;
		idxExists = OutputIndexExists(m_OutputsIndex);

		while (idxExists)
		{
			m_OutputsIndex++;
			idxExists = OutputIndexExists(m_OutputsIndex);
		}
		m_OutputIndexMap[m_OutputsIndex] = a_Name;
	}
	else
	{
		m_OutputIndexMap[m_OutputsIndex] = a_Name;
		m_OutputsIndex++;
	}
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Database::AddOutputVariable(Key& a_Name, KeyList& a_TypeQualifierList, uint32 a_Idx, KeyList& a_TemplateArgList)
{
	bool entryExists = OutputVariableExists(a_Name);

	if (entryExists)
	{
		return SAVVY_SHADER_SYNTAX_ERROR;
	}

	Variable var;
	var.m_Name = a_Name;
	var.m_QualifierList = a_TypeQualifierList;
	var.m_TemplateArgList = a_TemplateArgList;
	m_OutputVariables.insert(std::make_pair(Key(a_Name), var));

	bool idxExists = OutputIndexExists(a_Idx);

	if (idxExists)
	{
		return SAVVY_SHADER_SYNTAX_ERROR;
	}

	m_OutputIndexMap[a_Idx] = a_Name;
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Database::AddUniformVariable(Key& a_Name, KeyList& a_TypeQualifierList, Key& a_BufferName, KeyList& a_TemplateArgList)
{
	AddUniformBuffer(a_BufferName);
	bool entryExists = UniformVariableExists(a_BufferName, a_Name);

	if (entryExists)
	{
		return SAVVY_SHADER_SYNTAX_ERROR;
	}

	Variable var;
	var.m_Name = a_Name;
	var.m_QualifierList = a_TypeQualifierList;
	var.m_TemplateArgList = a_TemplateArgList;
	m_UniformVariables.insert(std::make_pair<Key, Variable>(Key(a_BufferName), Variable(var)));
	return SAVVY_OK;
}


Savvy::ResultCode Savvy::Database::AddLocalVariable(Key& a_Name, KeyList& a_TypeQualifierList, Key& a_FuncName, KeyList& a_TemplateArgList)
{
	bool entryExists = LocalVariableExists(a_FuncName, a_Name);

	if (entryExists)
	{
		return SAVVY_SHADER_SYNTAX_ERROR;
	}

	Variable var;
	var.m_Name = a_Name;
	var.m_QualifierList = a_TypeQualifierList;
	var.m_TemplateArgList = a_TemplateArgList;
	m_FunctionVariableMap.insert(std::make_pair<Key, Variable>(Key(a_FuncName), Variable(var)));
	return SAVVY_OK;
}


const bool Savvy::Database::InputVariableExists(const Key& a_Name)
{
	unsigned int count = m_InputVariables.count(a_Name);

	if (count > 0)
		return true;

	return false;
}

const bool Savvy::Database::OutputVariableExists(const Key& a_Name)
{
	unsigned int count = m_OutputVariables.count(a_Name);

	if (count > 0)
		return true;

	return false;
}

const bool Savvy::Database::UniformVariableExists(const Key& a_BufferName, const Key& a_Name)
{
	unsigned int count = m_UniformVariables.count(a_BufferName);

	if (count > 0)
	{
		KeyVarMultiMap::iterator it = m_UniformVariables.lower_bound(a_BufferName);
		KeyVarMultiMap::iterator it2 = m_UniformVariables.upper_bound(a_BufferName);

		while (it != it2)
		{
			if (it->second.m_Name == a_Name)
			{
				return true;
			}
			it++;
		}
	}

	return false;
}

const bool Savvy::Database::LocalVariableExists(const Key& a_FuncName, const Key& a_Name)
{
	unsigned int count = m_FunctionVariableMap.count(a_FuncName);

	if (count > 0)
	{
		KeyVarMultiMap::iterator it = m_FunctionVariableMap.lower_bound(a_FuncName);
		KeyVarMultiMap::iterator it2 = m_FunctionVariableMap.upper_bound(a_FuncName);

		while (it != it2)
		{
			if (it->second.m_Name == a_Name)
			{
				return true;
			}
			it++;
		}
	}

	return false;
}

void Savvy::Database::Reset()
{
	m_InputIndexMap.clear();;
	m_OutputIndexMap.clear();
	m_SamplerIndexMap.clear();
	m_UniformBufferIndexMap.clear();

	m_InputVariables.clear();
	m_OutputVariables.clear();
	m_UniformBuffers.clear();
	m_UniformVariables.clear();
	m_StructVariables.clear();
	m_StructList.clear();
	m_Samplers.clear();
	m_GlobalVarList.clear();
	m_FunctionVariableMap.clear();
	m_FuncInstructionMap.clear();
	m_FunctionList.clear();
	m_SemanticMap.clear();
	m_GenericBufferIndex = 0;
	m_UniformBufferIndex = 0;
	m_InputsIndex = 0;
	m_SamplerIndex = 0;
	m_OutputsIndex = 0;
}

Savvy::ResultCode Savvy::Database::AddLocalInstruction(Key& a_Instruction, bool a_Translate, Key& a_FuncName)
{
	Word word;
	word.m_Name = a_Instruction;
	word.m_Translate = a_Translate;
	m_FuncInstructionMap.insert(std::make_pair(Key(a_FuncName), Word(word)));
	return SAVVY_OK;
}

const bool Savvy::Database::UniformBufferExists(const Key& a_Name)
{
	KeyList::iterator it;
	for (it = m_UniformBuffers.begin(); it != m_UniformBuffers.end(); ++it)
	{
		if (*it == a_Name)
		{
			return true;
		}
	}

	return false;
}

Savvy::ResultCode Savvy::Database::AddUniformBuffer(const Key& a_Name)
{
	bool entryExists = UniformBufferExists(a_Name);

	if (entryExists)
	{
		return SAVVY_SHADER_SYNTAX_ERROR;
	}

	m_UniformBuffers.push_back(a_Name);

	bool idxExists = UniformBufferIndexExists(m_UniformBufferIndex);

	if (idxExists)
	{
		// Find the first available index
		m_UniformBufferIndex = 0;
		idxExists = UniformBufferIndexExists(m_UniformBufferIndex);
		while (idxExists)
		{
			m_UniformBufferIndex++;
			idxExists = UniformBufferIndexExists(m_UniformBufferIndex);
		}
		m_UniformBufferIndexMap[m_UniformBufferIndex] = a_Name;
	}
	else
	{
		m_UniformBufferIndexMap[m_UniformBufferIndex] = a_Name;
		m_UniformBufferIndex++;
	}
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Database::AddUniformBuffer(const Key& a_Name, uint32 a_Idx)
{
	bool entryExists = UniformBufferExists(a_Name);

	if (entryExists)
	{
		return SAVVY_SHADER_SYNTAX_ERROR;
	}

	m_UniformBuffers.push_back(a_Name);

	bool idxExists = UniformBufferIndexExists(a_Idx);

	if (idxExists)
	{
		return SAVVY_SHADER_SYNTAX_ERROR;
	}
	m_UniformBufferIndexMap[a_Idx] = a_Name;
	return SAVVY_OK;
}

const bool Savvy::Database::FunctionExists(const Key& a_Name, Key& a_FuncReturnType, VarList& a_Arguments)
{
	FunctionList::iterator it;
	for (it = m_FunctionList.begin(); it != m_FunctionList.end(); ++it)
	{
		if ( (*it).m_Name == a_Name)
		{
			if ((*it).m_ReturnType != a_FuncReturnType)
			{
				return true;
			}
			else
			{
				uint32 size = a_Arguments.size();
				for (uint32 i = 0; i < size; ++i)
				{
					uint32 typeSize = (*it).m_Arguments[i].m_QualifierList.size();

					for (uint32 j = 0; j < typeSize; j++)
					{
						if (a_Arguments[i].m_QualifierList[j] != (*it).m_Arguments[i].m_QualifierList[j])
						{
							return false;
						}
					}
				}
				return true;
			}
		}
	}

	return false;
}

const bool Savvy::Database::FunctionExists(const Key& a_Name)
{
	FunctionList::iterator it;
	for (it = m_FunctionList.begin(); it != m_FunctionList.end(); ++it)
	{
		if ((*it).m_Name == a_Name)
		{
			return true;
		}
		
	}

	return false;
}

Savvy::ResultCode Savvy::Database::AddFunction(Key& a_FuncName, Key& a_FuncReturnType, VarList& a_Arguments, bool a_TranslateReturnType)
{
	bool exists = FunctionExists(a_FuncName, a_FuncReturnType, a_Arguments);

	if (exists)
	{
		return SAVVY_SHADER_SYNTAX_ERROR;
	}
	Function func;
	func.m_Arguments = a_Arguments;
	func.m_Name = a_FuncName;
	func.m_ReturnType = a_FuncReturnType;
	func.m_TranslateReturnType = a_TranslateReturnType;
	m_FunctionList.push_back(func);
	return SAVVY_OK;
}

const bool Savvy::Database::FindVariableWithType(const Key& a_Name, const Key& a_FuncName, const Key& a_Type)
{
	if (InputVariableExists(a_Name))
	{
		KeyList& list = m_InputVariables.at(a_Name).m_QualifierList;

		uint32 size = list.size();
		for (uint32 i = 0; i < size; i++)
		{
			if (list[i].Contains(a_Type.GetString()))
			{
				return true;
			}
		}
	}
	
	if (OutputVariableExists(a_Name))
	{
		KeyList& list = m_OutputVariables.at(a_Name).m_QualifierList;

		uint32 size = list.size();
		for (uint32 i = 0; i < size; i++)
		{
			if (list[i].Contains(a_Type.GetString()))
			{
				return true;
			}
		}
	}

	// Check local variable list
	Database::KeyVarMultiMap::iterator localIt1 = m_FunctionVariableMap.lower_bound(a_FuncName);
	Database::KeyVarMultiMap::iterator localIt2 = m_FunctionVariableMap.upper_bound(a_FuncName);

	while (localIt1 != localIt2)
	{
		if (localIt1->second.m_Name == a_Name)
		{
			KeyList& list = localIt1->second.m_QualifierList;

			uint32 size = list.size();
			for (uint32 i = 0; i < size; i++)
			{
				if (list[i].Contains(a_Type.GetString()))
				{
					return true;
				}
			}
		}
		localIt1++;
	}

	// Check uniform buffers
	Database::KeyList::iterator it;
	for (it = m_UniformBuffers.begin(); it != m_UniformBuffers.end(); ++it)
	{
		if (UniformBufferExists(*it))
		{
			Database::KeyVarMultiMap::iterator uIt1 = m_UniformVariables.lower_bound(*it);
			Database::KeyVarMultiMap::iterator uIt2 = m_UniformVariables.upper_bound(*it);

			while (uIt1 != uIt2)
			{
				if (uIt1->second.m_Name == a_Name)
				{
					KeyList& list = uIt1->second.m_QualifierList;

					uint32 size = list.size();
					for (uint32 i = 0; i < size; i++)
					{
						if (list[i].Contains(a_Type.GetString()))
						{
							return true;
						}
					}
				}

				uIt1++;
			}
		}
	}

	// Check generic buffers
	Database::KeyList::iterator it3;
	for (it3 = m_GenericBuffers.begin(); it3 != m_GenericBuffers.end(); ++it3)
	{
		if (GenericBufferExists(*it3))
		{
			Database::KeyVarMultiMap::iterator uIt1 = m_GenericBufferVariables.lower_bound(*it3);
			Database::KeyVarMultiMap::iterator uIt2 = m_GenericBufferVariables.upper_bound(*it3);

			while (uIt1 != uIt2)
			{
				if (uIt1->second.m_Name == a_Name)
				{
					KeyList& list = uIt1->second.m_QualifierList;

					uint32 size = list.size();
					for (uint32 i = 0; i < size; i++)
					{
						if (list[i].Contains(a_Type.GetString()))
						{
							return true;
						}
					}
				}
				uIt1++;
			}
		}
	}

	// Check structs
	Database::KeyList::iterator it2;
	for (it2 = m_StructList.begin(); it2 != m_StructList.end(); ++it2)
	{
		if (StructExists(*it2))
		{
			Database::KeyVarMultiMap::iterator uIt1 = m_StructVariables.lower_bound(*it2);
			Database::KeyVarMultiMap::iterator uIt2 = m_StructVariables.upper_bound(*it2);

			while (uIt1 != uIt2)
			{
				if (uIt1->second.m_Name == a_Name)
				{
					KeyList& list = uIt1->second.m_QualifierList;

					uint32 size = list.size();
					for (uint32 i = 0; i < size; i++)
					{
						if (list[i].Contains(a_Type.GetString()))
						{
							return true;
						}
					}
				}

				uIt1++;
			}
		}
	}

	// Check global variables
	VarList::iterator globalIt;
	for (globalIt = m_GlobalVarList.begin(); globalIt != m_GlobalVarList.end(); ++globalIt)
	{
		if (GlobalVariableExists(a_Name))
		{
			KeyList& list = globalIt->m_QualifierList;
			uint32 size = list.size();
			for (uint32 i = 0; i < size; i++)
			{
				if (list[i].Contains(a_Type.GetString()))
				{
					return true;
				}
			}
		}
	}

	return false;
}

const bool Savvy::Database::FindVariableWithType(const Key& a_Name, const Key& a_FuncName, KeyList& a_TypeList)
{
	if (InputVariableExists(a_Name))
	{
		KeyList::iterator it;
		for (it = a_TypeList.begin(); it != a_TypeList.end(); ++it)
		{
			KeyList& list = m_InputVariables.at(a_Name).m_QualifierList;
			uint32 size = list.size();
			for (uint32 i = 0; i < size; i++)
			{
				if (list[i].Contains((*it).GetString()))
				{
					return true;
				}
			}
		}
	}

	if (OutputVariableExists(a_Name))
	{
		KeyList::iterator it;
		for (it = a_TypeList.begin(); it != a_TypeList.end(); ++it)
		{
			KeyList& list = m_OutputVariables.at(a_Name).m_QualifierList;
			uint32 size = list.size();
			for (uint32 i = 0; i < size; i++)
			{
				if (list[i].Contains((*it).GetString()))
				{
					return true;
				}
			}
		}
	}

	// Check local variable list
	Database::KeyVarMultiMap::iterator localIt1 = m_FunctionVariableMap.lower_bound(a_FuncName);
	Database::KeyVarMultiMap::iterator localIt2 = m_FunctionVariableMap.upper_bound(a_FuncName);

	while (localIt1 != localIt2)
	{
		if (localIt1->second.m_Name == a_Name)
		{
			KeyList::iterator it;
			for (it = a_TypeList.begin(); it != a_TypeList.end(); ++it)
			{
				KeyList& list = localIt1->second.m_QualifierList;

				uint32 size = list.size();
				for (uint32 i = 0; i < size; i++)
				{
					if (list[i].Contains((*it).GetString()))
					{
						return true;
					}
				}
			}
		}
		localIt1++;
	}

	// Check samplers
	Database::KeyVarMap::iterator samplerIt;

	for (samplerIt = m_Samplers.begin(); samplerIt != m_Samplers.end(); ++samplerIt)
	{
		for (uint32 i = 0; i < a_TypeList.size(); i++)
		{
			for (uint32 j = 0; j < samplerIt->second.m_QualifierList.size(); j++)
			{
				if (samplerIt->second.m_QualifierList[j] == a_TypeList[i])
					return true;
			}

			for (uint32 j = 0; j < samplerIt->second.m_TemplateArgList.size(); j++)
			{
				if (samplerIt->second.m_TemplateArgList[j] == a_TypeList[i])
					return true;
			}
		}
	}

	// Check uniform buffers
	Database::KeyList::iterator it2;
	for (it2 = m_UniformBuffers.begin(); it2 != m_UniformBuffers.end(); ++it2)
	{
		if (UniformBufferExists(*it2))
		{
			Database::KeyVarMultiMap::iterator uIt1 = m_UniformVariables.lower_bound(*it2);
			Database::KeyVarMultiMap::iterator uIt2 = m_UniformVariables.upper_bound(*it2);

			while (uIt1 != uIt2)
			{
				if (uIt1->second.m_Name == a_Name)
				{
					KeyList::iterator it;
					for (it = a_TypeList.begin(); it != a_TypeList.end(); ++it)
					{
						KeyList& list = uIt1->second.m_QualifierList;

						uint32 size = list.size();
						for (uint32 i = 0; i < size; i++)
						{
							if (list[i].Contains((*it).GetString()))
							{
								return true;
							}
						}
					}
				}

				uIt1++;
			}
		}
	}

	// Check generic buffers
	Database::KeyList::iterator it3;
	for (it3 = m_GenericBuffers.begin(); it3 != m_GenericBuffers.end(); ++it3)
	{
		if (GenericBufferExists(*it3))
		{
			Database::KeyVarMultiMap::iterator uIt1 = m_GenericBufferVariables.lower_bound(*it3);
			Database::KeyVarMultiMap::iterator uIt2 = m_GenericBufferVariables.upper_bound(*it3);

			while (uIt1 != uIt2)
			{
				if (uIt1->second.m_Name == a_Name)
				{
					KeyList::iterator it;
					for (it = a_TypeList.begin(); it != a_TypeList.end(); ++it)
					{
						KeyList& list = uIt1->second.m_QualifierList;

						uint32 size = list.size();
						for (uint32 i = 0; i < size; i++)
						{
							if (list[i].Contains((*it).GetString()))
							{
								return true;
							}
						}
					}
				}

				uIt1++;
			}
		}
	}

	// Check structs
	Database::KeyList::iterator it;
	for (it = m_StructList.begin(); it != m_StructList.end(); ++it)
	{
		if (StructExists(*it))
		{
			Database::KeyVarMultiMap::iterator uIt1 = m_StructVariables.lower_bound(*it);
			Database::KeyVarMultiMap::iterator uIt2 = m_StructVariables.upper_bound(*it);

			while (uIt1 != uIt2)
			{
				if (uIt1->second.m_Name == a_Name)
				{
					KeyList::iterator it;
					for (it = a_TypeList.begin(); it != a_TypeList.end(); ++it)
					{
						KeyList& list = uIt1->second.m_QualifierList;

						uint32 size = list.size();
						for (uint32 i = 0; i < size; i++)
						{
							if (list[i].Contains((*it).GetString()))
							{
								return true;
							}
						}
					}
				}

				uIt1++;
			}
		}
	}

	// Check global variables
	VarList::iterator globalIt;

	for (it = a_TypeList.begin(); it != a_TypeList.end(); ++it)
	{
		for (globalIt = m_GlobalVarList.begin(); globalIt != m_GlobalVarList.end(); ++globalIt)
		{
			if (GlobalVariableExists(a_Name))
			{
				KeyList& list = globalIt->m_QualifierList;
				uint32 size = list.size();
				for (uint32 i = 0; i < size; i++)
				{
					if (list[i].Contains((*it).GetString()))
					{
						return true;
					}
				}
			}
		}
	}

	return false;
}

const bool Savvy::Database::InputIndexExists(uint32 a_Idx)
{
	uint32 count = m_InputIndexMap.count(a_Idx);

	if (count > 0)
	{
		return true;
	}

	return false;
}

const bool Savvy::Database::OutputIndexExists(uint32 a_Idx)
{
	uint32 count = m_OutputIndexMap.count(a_Idx);

	if (count > 0)
	{
		return true;
	}

	return false;
}

Savvy::ResultCode Savvy::Database::AddSampler(Key& a_Name, Key& a_Type, KeyList& a_TemplateArgList)
{
	bool entryExists = SamplerExists(a_Name);

	if (entryExists)
	{
		return SAVVY_SHADER_SYNTAX_ERROR;
	}

	Variable var;
	var.m_Name = a_Name;
	var.m_QualifierList.push_back(a_Type);
	var.m_TemplateArgList = a_TemplateArgList;

	m_Samplers.insert(std::make_pair(Key(a_Name), var));

	bool idxExists = SamplerIndexExists(m_SamplerIndex);

	if (idxExists)
	{
		// Find the first available index
		m_SamplerIndex = 0;
		idxExists = SamplerIndexExists(m_SamplerIndex);
		while (idxExists)
		{
			m_SamplerIndex++;
			idxExists = SamplerIndexExists(m_SamplerIndex);
		}
		m_SamplerIndexMap[m_SamplerIndex] = a_Name;
	}
	else
	{
		m_SamplerIndexMap[m_SamplerIndex] = a_Name;
		m_SamplerIndex++;
	}
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Database::AddSampler(Key& a_Name, Key& a_Type, uint32 a_Idx, KeyList& a_TemplateArgList)
{
	bool entryExists = SamplerExists(a_Name);

	if (entryExists)
	{
		return SAVVY_SHADER_SYNTAX_ERROR;
	}

	Variable var;
	var.m_Name = a_Name;
	var.m_QualifierList.push_back(a_Type);
	var.m_TemplateArgList = a_TemplateArgList;
	m_Samplers.insert(std::make_pair(Key(a_Name), var));

	bool idxExists = SamplerIndexExists(a_Idx);

	if (idxExists)
	{
		return SAVVY_SHADER_SYNTAX_ERROR;
	}
	m_SamplerIndexMap[a_Idx] = a_Name;
	return SAVVY_OK;
}

const bool Savvy::Database::SamplerExists(const Key& a_Name)
{
	uint32 count = m_Samplers.count(a_Name);

	if (count > 0)
	{
		return true;
	}

	return false;
}

const bool Savvy::Database::SamplerIndexExists(uint32 a_Idx)
{
	uint32 count = m_SamplerIndexMap.count(a_Idx);

	if (count > 0)
	{
		return true;
	}

	return false;
}

const bool Savvy::Database::StructExists(const Key& a_Name)
{
	KeyList::iterator it;
	for (it = m_StructList.begin(); it != m_StructList.end(); ++it)
	{
		if (*it == a_Name)
		{
			return true;
		}
	}

	return false;
}

const bool Savvy::Database::StructVariableExists(const Key& a_StructName, const Key& a_Name)
{
	unsigned int count = m_StructVariables.count(a_StructName);

	if (count > 0)
	{
		KeyVarMultiMap::iterator it = m_StructVariables.lower_bound(a_StructName);
		KeyVarMultiMap::iterator it2 = m_StructVariables.upper_bound(a_StructName);

		while (it != it2)
		{
			if (it->second.m_Name == a_Name)
			{
				return true;
			}
			it++;
		}
	}

	return false;
}

Savvy::ResultCode Savvy::Database::AddStruct(const Key& a_Name)
{
	bool exists = StructExists(a_Name);
	if (exists)
	{
		return SAVVY_SHADER_SYNTAX_ERROR;
	}
	m_StructList.push_back(a_Name);
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Database::AddStructVariable(const Key& a_StructName, const Key& a_Name, KeyList& a_TypeQualifierList, KeyList& a_TemplateArgList)
{
	bool entryExists = StructVariableExists(a_StructName, a_Name);

	if (entryExists)
	{
		return SAVVY_SHADER_SYNTAX_ERROR;
	}

	Variable var;
	var.m_Name = a_Name;
	var.m_QualifierList = a_TypeQualifierList;
	var.m_TemplateArgList = a_TemplateArgList;
	m_StructVariables.insert(std::make_pair<Key, Variable>(Key(a_StructName), Variable(var)));
	return SAVVY_OK;
}

const bool Savvy::Database::UniformBufferIndexExists(uint32 a_Idx)
{
	uint32 count = m_UniformBufferIndexMap.count(a_Idx);

	if (count > 0)
	{
		return true;
	}

	return false;
}

Savvy::ResultCode Savvy::Database::AddGlobalVariable(const Key& a_Name, KeyList& a_TypeQualifiers, KeyList& a_TemplateArgList)
{
	bool varExists = GlobalVariableExists(a_Name);
	if (varExists)
	{
		return SAVVY_SHADER_SYNTAX_ERROR;
	}

	Variable var;
	var.m_Name = a_Name;
	var.m_QualifierList = a_TypeQualifiers;
	var.m_TemplateArgList = a_TemplateArgList;
	m_GlobalVarList.push_back(var);
	return SAVVY_OK;
}

const bool Savvy::Database::GlobalVariableExists(const Key& a_Name)
{
	VarList::iterator it;

	for (it = m_GlobalVarList.begin(); it != m_GlobalVarList.end(); ++it)
	{
		if ((*it).m_Name == a_Name)
			return true;
	}

	return false;
}

Savvy::Database::Function& Savvy::Database::GetFunction(const Key& a_Name)
{
	uint32 size = m_FunctionList.size();

	for (uint32 i = 0; i < size; i++)
	{
		if (m_FunctionList[i].m_Name == a_Name)
			return m_FunctionList[i];
	}
	return m_FunctionList.back();
}

Savvy::ResultCode Savvy::Database::AddSemantic(Key& a_StructName, Key& a_VarName, Key& a_SemanticName)
{
	VarWithSemantic var;
	var.m_Semantic = a_SemanticName;
	var.m_VarName = a_VarName;
	m_SemanticMap.insert(std::make_pair(a_StructName, var));
	return SAVVY_OK;
}

const bool Savvy::Database::SemanticExists(const Key& a_StructName, const Key& a_VarName)
{
	SemanticMap::iterator it1 = m_SemanticMap.lower_bound(a_StructName);
	SemanticMap::iterator it2 = m_SemanticMap.upper_bound(a_StructName);

	while (it1 != it2)
	{
		if (it1->second.m_VarName == a_VarName)
		{
			return true;
		}
		it1++;
	}

	return false;
}

const Savvy::Key& Savvy::Database::GetSemantic(const Key& a_StructName, const Key& a_VarName)
{
	SemanticMap::iterator it1 = m_SemanticMap.lower_bound(a_StructName);
	SemanticMap::iterator it2 = m_SemanticMap.upper_bound(a_StructName);

	while (it1 != it2)
	{
		if (it1->second.m_VarName == a_VarName)
		{
			return it1->second.m_Semantic;
		}
		it1++;
	}
	return a_VarName;
}

const bool Savvy::Database::VariableWithNameExists(const Key& a_Name, const Key& a_FuncName)
{
	if (InputVariableExists(a_Name))
	{
		return true;
	}

	if (OutputVariableExists(a_Name))
	{
		return true;
	}

	// Check local variable list
	Database::KeyVarMultiMap::iterator localIt1 = m_FunctionVariableMap.lower_bound(a_FuncName);
	Database::KeyVarMultiMap::iterator localIt2 = m_FunctionVariableMap.upper_bound(a_FuncName);

	while (localIt1 != localIt2)
	{
		if (localIt1->second.m_Name == a_Name)
		{
			return true;
		}
		localIt1++;
	}

	// Check uniform buffers
	Database::KeyList::iterator it;
	for (it = m_UniformBuffers.begin(); it != m_UniformBuffers.end(); ++it)
	{
		if (UniformBufferExists(*it))
		{
			Database::KeyVarMultiMap::iterator uIt1 = m_UniformVariables.lower_bound(*it);
			Database::KeyVarMultiMap::iterator uIt2 = m_UniformVariables.upper_bound(*it);

			while (uIt1 != uIt2)
			{
				if (uIt1->second.m_Name == a_Name)
				{
					return true;
				}

				uIt1++;
			}
		}
	}

	// Check structs
	Database::KeyList::iterator it2;
	for (it2 = m_StructList.begin(); it2 != m_StructList.end(); ++it2)
	{
		if (StructExists(*it2))
		{
			Database::KeyVarMultiMap::iterator uIt1 = m_StructVariables.lower_bound(*it2);
			Database::KeyVarMultiMap::iterator uIt2 = m_StructVariables.upper_bound(*it2);

			while (uIt1 != uIt2)
			{
				if (uIt1->second.m_Name == a_Name)
				{
					return true;
				}

				uIt1++;
			}
		}
	}

	// Check global variables
	VarList::iterator globalIt;
	for (globalIt = m_GlobalVarList.begin(); globalIt != m_GlobalVarList.end(); ++globalIt)
	{
		if (GlobalVariableExists(a_Name))
		{
			return true;
		}
	}

	return false;
}

Savvy::ResultCode Savvy::Database::AddGenericBufferVariable(Key& a_Name, KeyList& a_TypeQualifierList, Key& a_BufferName, KeyList& a_TemplateArgList /*= KeyList()*/)
{
	AddGenericBuffer(a_BufferName);
	bool entryExists = GenericBufferVariableExists(a_BufferName, a_Name);

	if (entryExists)
	{
		return SAVVY_SHADER_SYNTAX_ERROR;
	}

	Variable var;
	var.m_Name = a_Name;
	var.m_QualifierList = a_TypeQualifierList;
	var.m_TemplateArgList = a_TemplateArgList;
	m_GenericBufferVariables.insert(std::make_pair<Key, Variable>(Key(a_BufferName), Variable(var)));
	return SAVVY_OK;
}

const bool Savvy::Database::GenericBufferVariableExists(const Key& a_BufferName, const Key& a_Name)
{
	unsigned int count = m_GenericBufferVariables.count(a_BufferName);

	if (count > 0)
	{
		KeyVarMultiMap::iterator it = m_GenericBufferVariables.lower_bound(a_BufferName);
		KeyVarMultiMap::iterator it2 = m_GenericBufferVariables.upper_bound(a_BufferName);

		while (it != it2)
		{
			if (it->second.m_Name == a_Name)
			{
				return true;
			}
			it++;
		}
	}

	return false;
}

const bool Savvy::Database::GenericBufferExists(const Key& a_Name)
{
	KeyList::iterator it;
	for (it = m_GenericBuffers.begin(); it != m_GenericBuffers.end(); ++it)
	{
		if (*it == a_Name)
		{
			return true;
		}
	}

	return false;
}

Savvy::ResultCode Savvy::Database::AddGenericBuffer(const Key& a_Name)
{
	bool entryExists = GenericBufferExists(a_Name);

	if (entryExists)
	{
		return SAVVY_SHADER_SYNTAX_ERROR;
	}

	m_GenericBuffers.push_back(a_Name);

	bool idxExists = GenericBufferIndexExists(m_GenericBufferIndex);

	if (idxExists)
	{
		// Find the first available index
		m_GenericBufferIndex = 0;
		idxExists = GenericBufferIndexExists(m_GenericBufferIndex);
		while (idxExists)
		{
			m_GenericBufferIndex++;
			idxExists = GenericBufferIndexExists(m_GenericBufferIndex);
		}
		m_GenericBufferIndexMap[m_GenericBufferIndex] = a_Name;
	}
	else
	{
		m_GenericBufferIndexMap[m_GenericBufferIndex] = a_Name;
		m_GenericBufferIndex++;
	}
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Database::AddGenericBuffer(const Key& a_Name, uint32 a_Idx)
{
	bool entryExists = GenericBufferExists(a_Name);

	if (entryExists)
	{
		return SAVVY_SHADER_SYNTAX_ERROR;
	}

	m_GenericBuffers.push_back(a_Name);

	bool idxExists = GenericBufferIndexExists(a_Idx);

	if (idxExists)
	{
		return SAVVY_SHADER_SYNTAX_ERROR;
	}
	m_GenericBufferIndexMap[a_Idx] = a_Name;
	return SAVVY_OK;
}

const bool Savvy::Database::GenericBufferIndexExists(uint32 a_Idx)
{
	uint32 count = m_GenericBufferIndexMap.count(a_Idx);

	if (count > 0)
	{
		return true;
	}

	return false;
}
