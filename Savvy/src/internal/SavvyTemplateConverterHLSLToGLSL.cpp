#include "SavvyPCH.h"
#include "SavvyTemplateConverterHLSLToGLSL.h"

#define TEMPLATE_LIST_INSERT(a) m_TemplateConvertList.push_back(std::bind(&a, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3))

Savvy::Internal::TemplateConverterHLSLToGLSL::TemplateConverterHLSLToGLSL()
{
	TEMPLATE_LIST_INSERT(Savvy::Internal::TemplateConverterHLSLToGLSL::ConvertTextureTemplates);
}

Savvy::Internal::TemplateConverterHLSLToGLSL::~TemplateConverterHLSLToGLSL()
{

}

Savvy::ResultCode Savvy::Internal::TemplateConverterHLSLToGLSL::ConvertTemplatedVariable(Database::Variable& a_Var, Database::Variable& a_RetVar)
{
	a_RetVar = a_Var;

	TemplateConvertList::iterator it;
	bool handled = false;
	for (it = m_TemplateConvertList.begin(); it != m_TemplateConvertList.end(); ++it)
	{
		ResultCode res = (*it)(a_Var, a_RetVar, handled);

		if (res != SAVVY_OK)
			return res;

		if (handled)
			break;
	}

	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::TemplateConverterHLSLToGLSL::ConvertTemplatedInstruction(Database::WordMultiMap::iterator& it1, std::string& currentLine, Key& a_FuncName, uint32 a_Shader, Key& a_InputStructName /*= Key()*/, Key& a_OutputStructName /*= Key()*/)
{
	// TODO
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::TemplateConverterHLSLToGLSL::GetTemplateArguments(Database::WordMultiMap::iterator& it1, StringList& a_OutputList, std::string& currentLine)
{
	// TODO
	return SAVVY_OK;
}

Savvy::ResultCode Savvy::Internal::TemplateConverterHLSLToGLSL::ConvertTextureTemplates(Database::Variable& a_Var, Database::Variable& a_RetVar, bool& a_Handled)
{
	for (uint32 i = 1; i < 4; i++)
	{
		std::string ctrString = std::to_string(i);
		std::string checkString = "Texture" + ctrString + "D";
		std::string checkString2 = "Texture" + ctrString + "DArray";

		for (uint32 j = 0; j < a_Var.m_QualifierList.size(); j++)
		{
			if (a_Var.m_QualifierList[j] == checkString)
			{
				if (a_Var.m_TemplateArgList[0].Contains("float"))
				{
					a_RetVar.m_QualifierList[j] = m_Database->GetOutputLangEquivalent(a_Var.m_QualifierList[j]);
					a_Handled = true;
					return SAVVY_OK;
				}
				else if (a_Var.m_TemplateArgList[0].Contains("uint"))
				{
					a_RetVar.m_QualifierList[j] = "usampler" + ctrString + "D";
					a_Handled = true;
					return SAVVY_OK;
				}
				else if (a_Var.m_TemplateArgList[0].Contains("int"))
				{
					a_RetVar.m_QualifierList[j] = "isampler" + ctrString + "D";
					a_Handled = true;
					return SAVVY_OK;
				}
			}
			else if (a_Var.m_QualifierList[j] == checkString2)
			{
				if (a_Var.m_TemplateArgList[0].Contains("float"))
				{
					a_RetVar.m_QualifierList[j] = m_Database->GetOutputLangEquivalent(a_Var.m_QualifierList[j]);
					a_Handled = true;
					return SAVVY_OK;
				}
				else if (a_Var.m_TemplateArgList[0].Contains("uint"))
				{
					a_RetVar.m_QualifierList[j] = "usampler" + ctrString + "DArray";
					a_Handled = true;
					return SAVVY_OK;
				}
				else if (a_Var.m_TemplateArgList[0].Contains("int"))
				{
					a_RetVar.m_QualifierList[j] = "isampler" + ctrString + "DArray";
					a_Handled = true;
					return SAVVY_OK;
				}
			}
		}
	}

	return SAVVY_OK;
}

