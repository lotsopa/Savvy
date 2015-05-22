/*
The MIT License (MIT)

Copyright (c) 2015 Apostol Dadachev

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
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