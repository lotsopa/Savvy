#include "SavvyPCH.h"
#include "SavvyTemplateConverter.h"

Savvy::TemplateConverter::TemplateConverter()
{

}

Savvy::TemplateConverter::~TemplateConverter()
{

}

Savvy::ResultCode Savvy::TemplateConverter::Initialize(Database* a_Database)
{
	if (a_Database == NULL)
		return SAVVY_INVALID_ARG;

	m_Database = a_Database;

	return SAVVY_OK;
}
