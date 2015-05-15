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