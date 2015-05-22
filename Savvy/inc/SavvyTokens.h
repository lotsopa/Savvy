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


#ifndef SAVVY_TOKENS_H
#define SAVVY_TOKENS_H

/*
This header file includes all token types supported by the scanner.
*/

namespace Savvy
{
	enum TokenType
	{
		SAVVY_EOF = 0,
		SAVVY_INTEGER_VALUE,
		SAVVY_FLOAT_VALUE,
		SAVVY_BOOL_VALUE,
		SAVVY_IDENTIFIER,
		SAVVY_OPEN_BRACKET,
		SAVVY_CLOSE_BRACKET,
		SAVVY_OPEN_SQUARE_BRACKET,
		SAVVY_CLOSE_SQUARE_BRACKET,
		SAVVY_OPEN_BRACE,
		SAVVY_CLOSE_BRACE,
		SAVVY_DATA_TYPE,
		SAVVY_OPERATOR,
		SAVVY_INSTRUCTION_END,
		SAVVY_RETURN_KEYWORD,
		SAVVY_IF_STATEMENT,
		SAVVY_ELSE_STATEMENT,
		SAVVY_SWITCH_STATEMENT,
		SAVVY_CASE_STATEMENT,
		SAVVY_BREAK_STATEMENT,
		SAVVY_FOR_STATEMENT,
		SAVVY_DO_STATEMENT,
		SAVVY_WHILE_STATEMENT,
		SAVVY_CONTINUE_STATEMENT,
		SAVVY_BUILT_IN_FUNCTION,
		SAVVY_LANGUAGE_SPECIFIC_KEYWORD,
		SAVVY_INCLUDE,
		SAVVY_STRING,
		SAVVY_PREPROCESSOR,
		SAVVY_NEW_LINE,
		SAVVY_UNKNOWN_TOKEN
	};
}

#endif // !SAVVY_TOKENS_H