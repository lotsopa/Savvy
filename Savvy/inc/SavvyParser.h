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