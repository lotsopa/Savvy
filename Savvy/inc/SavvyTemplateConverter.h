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
#ifndef SAVVY_TEMPLATE_CONVERTER_H
#define SAVVY_TEMPLATE_CONVERTER_H

#include "SavvyKey.h"
#include "SavvyDatabase.h"
namespace Savvy
{
	class TemplateConverter
	{
	public:
		typedef std::vector <std::string> StringList;

		TemplateConverter();
		virtual ~TemplateConverter();
		virtual ResultCode Initialize(Database* a_Database);
		virtual ResultCode ConvertTemplatedVariable(Database::Variable& a_Var, Database::Variable& a_RetVar) = 0;
		virtual ResultCode ConvertTemplatedInstruction(Database::WordMultiMap::iterator& it1, std::string& currentLine, Key& a_FuncName, uint32 a_Shader, Key& a_InputStructName = Key(), Key& a_OutputStructName = Key()) = 0;
		virtual ResultCode GetTemplateArguments(Database::WordMultiMap::iterator& it1, StringList& a_OutputList, std::string& currentLine) = 0;
	protected:
		Database* m_Database;
		Key m_CurrFunction;
		Key m_InputStructName;
		Key m_OutputStructName;
		uint32 m_CurrShader;
	};
}

#endif // !SAVVY_TEMPLATE_CONVERTER_H