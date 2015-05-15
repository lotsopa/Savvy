#include "SavvyPCH.h"
#include "SavvyFuncConverter.h"

Savvy::FuncConverter::FuncConverter()
{

}

Savvy::FuncConverter::~FuncConverter()
{

}

Savvy::ResultCode Savvy::FuncConverter::Initialize(Database* a_Database)
{
	if (a_Database == NULL)
		return SAVVY_INVALID_ARG;

	m_Database = a_Database;

	return SAVVY_OK;
}

Savvy::ResultCode Savvy::FuncConverter::GetFuncArgumentList(Database::WordMultiMap::iterator& it1, StringList& a_OutputList, std::string& currentLine)
{
	Database::WordMultiMap& instructionMap = m_Database->GetInstructionMap();
	Database::WordMultiMap::iterator endIt = instructionMap.upper_bound(m_CurrFunction);

	if (it1 == endIt)
		return SAVVY_SHADER_SYNTAX_ERROR;

	it1++;

	if (it1->second.m_Name != "(")
	{
		return SAVVY_SHADER_SYNTAX_ERROR;
	}

	StringList retList;
	retList.push_back(std::string());
	uint32 openBrackets = 1;
	uint32 closeBrackets = 0;
	uint32 ctrAhead = 0;
	while (it1 != endIt)
	{
		ctrAhead++;
		it1++;

		Database::Word& currWord = it1->second;

		if (currWord.m_Name == ";")
		{
			return SAVVY_SHADER_SYNTAX_ERROR;
		}
		else if (currWord.m_Name == ")")
		{
			closeBrackets++;

			if (closeBrackets == openBrackets)
			{
				break;
			}
		}
		else if (currWord.m_Name == "(")
		{
			openBrackets++;
		}

		if (currWord.m_Name == ",")
		{
			if (closeBrackets == openBrackets - 1)
			{
				retList.push_back(std::string()); // Insert new argument
			}
			else
			{
				std::string add;
				if (currWord.m_Translate)
				{
					if (m_Database->OutputLangEquivalentExists(currWord.m_Name))
					{
						const Key& k = m_Database->GetOutputLangEquivalent(currWord.m_Name);
						add = k.GetString();
					}
					else
					{
						return SAVVY_LOGICAL_ERROR;
					}
				}
				else
				{
					add = currWord.m_Name.GetString();
				}
				retList.back().append(add);
			}
		}
		else
		{
			std::string add;
			if (currWord.m_Translate)
			{
				if (m_Database->OutputLangEquivalentExists(currWord.m_Name))
				{
					const Key& k = m_Database->GetOutputLangEquivalent(currWord.m_Name);
					add = k.GetString();
				}
				else
				{
					return SAVVY_LOGICAL_ERROR;
				}
			}
			else
			{
				add = currWord.m_Name.GetString();
			}
			retList.back().append(add);
		}
	}

	if (it1 == endIt)
		return SAVVY_SHADER_SYNTAX_ERROR;

	a_OutputList.swap(retList);

	return SAVVY_OK;
}

Savvy::uint32 Savvy::FuncConverter::StringContainsCharTimes(std::string& s, char8 c)
{
	uint32 count = 0;
	for (std::string::iterator p = s.begin(); p != s.end(); ++p)
	{
		if ((*p) == c)
		{
			count++;
		}
	}
	return count;
}

bool Savvy::FuncConverter::isFloat(std::string myString)
{
	std::istringstream iss(myString);
	float f;
	iss >> std::noskipws >> f; // noskipws considers leading whitespace invalid
	// Check the entire string was consumed and if either failbit or badbit is set
	return iss.eof() && !iss.fail();
}

bool Savvy::FuncConverter::isNumber(const std::string& s)
{
	std::string::const_iterator it = s.begin();
	while (it != s.end() && std::isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}
