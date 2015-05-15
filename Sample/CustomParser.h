#ifndef CUSTOM_PARSER_H
#define CUSTOM_PARSER_H

#include "SavvyFramework.h"

class CustomParser : public Savvy::Parser
{
public:

	CustomParser();

	virtual ~CustomParser();

	/*
	Initialization
	*/
	virtual Savvy::ResultCode Initialize(Savvy::Database* a_Database);

	/*
	Parses a token.
	*/
	virtual Savvy::ResultCode ParseToken(Savvy::uint32 a_Token, std::string& a_TokenName);

	/*
	Resets the parser state.
	*/
	virtual void Reset();

};

#endif