#include "SavvyPCH.h"
#include "SavvyParser.h"
#include "SavvyException.h"

Savvy::Parser::Parser()
{

}

Savvy::Parser::~Parser()
{

}

Savvy::ResultCode Savvy::Parser::Initialize(Savvy::Database* a_Database)
{
	if (a_Database == NULL)
	{
		m_LastError = "Database passed to Parser is NULL.";
#ifndef SAVVY_NO_EXCEPTIONS
		throw Exception(m_LastError);
#endif
		return SAVVY_INVALID_ARG;
	}

	m_Database = a_Database;

	return SAVVY_OK;
}
