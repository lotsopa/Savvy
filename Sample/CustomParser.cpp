#include "CustomParser.h"
#include "CustomDatabase.h"

CustomParser::CustomParser(): Parser()
{

}

CustomParser::~CustomParser()
{

}

Savvy::ResultCode CustomParser::ParseToken(Savvy::uint32 a_Token, std::string& a_TokenName)
{
	return Savvy::SAVVY_OK;
}

void CustomParser::Reset()
{

}

Savvy::ResultCode CustomParser::Initialize(Savvy::Database* a_Database)
{
	return Savvy::Parser::Initialize(a_Database);
}
