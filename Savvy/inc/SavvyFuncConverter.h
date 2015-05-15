#ifndef SAVVY_FUNC_CONVERTER_H
#define SAVVY_FUNC_CONVERTER_H

#include "SavvyKey.h"
#include "SavvyConstructor.h"
#include "SavvyDatabase.h"

namespace Savvy
{
	/*
	The Function Converter takes care of functions which do not have direct equivalents.
	*/
	class FuncConverter
	{
	public:
		typedef std::vector <std::string> StringList;

		FuncConverter();
		virtual ~FuncConverter();
		virtual ResultCode Initialize(Database* a_Database);
		virtual ResultCode ConvertFunction(Database::WordMultiMap::iterator& it1, std::string& currentLine, Key& a_FuncName, uint32 a_Shader, Key& a_InputStructName = Key(), Key& a_OutputStructName = Key()) = 0;
		virtual ResultCode GetFuncArgumentList(Database::WordMultiMap::iterator& it1, StringList& a_OutputList, std::string& currentLine);
		virtual void Reset() = 0;

		static bool isNumber(const std::string& s);

		static bool isFloat(std::string myString);

	protected:
		Database* m_Database;
		Key m_CurrFunction;
		Key m_InputStructName;
		Key m_OutputStructName;
		uint32 m_CurrShader;

		// returns how many times a char is contained in a string
		static uint32 StringContainsCharTimes(std::string& s, char8 c);
	};
}

#endif