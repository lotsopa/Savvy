#ifndef SAVVY_PARSER_H
#define SAVVY_PARSER_H

#include "SavvyDefines.h"
#include "SavvyTokens.h"
#include "SavvyKey.h"
#include "SavvyDatabase.h"

namespace Savvy
{
	/*
	The parser class takes care of parsing all the tokens which it is supplied with.
	It internally keeps track of its state, so that a syntax error in the current shader
	can easily be detected.
	*/
	class Database;
	class Parser
	{
	public:

		/*
		Default Constructor
		*/
		Parser();

		/*
		Default Destructor
		*/
		virtual ~Parser();

		/*
		Initialization
		*/
		virtual ResultCode Initialize(Savvy::Database* a_Database);

		/*
		Parses a token.
		*/
		virtual ResultCode ParseToken(uint32 a_Token, std::string& a_TokenName) = 0;

		/*
		Resets the parser state.
		*/
		virtual void Reset() = 0;

		/*
		Returns the last error string.
		*/
		SAVVY_INLINE std::string GetLastError() { return m_LastError; };
	
	protected:
		Database* m_Database;
		std::string m_LastError;
	};
}

#endif // !SAVVY_PARSER_H