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
#ifndef SAVVY_SCANNER_H
#define SAVVY_SCANNER_H

#include "SavvyTokens.h"
#include "SavvyFrameworkDefines.h"
#include "SavvyDefines.h"

namespace Savvy
{
	/*
	The scanner class represents the interface base class for all lexical scanners.
	Its job is to scan an input stream of data and tokenize it.
	*/
	class Scanner
	{
	public:

		/*
		Default constructor
		*/
		Scanner();

		/*
		Default destructor
		*/
		virtual ~Scanner();

		/*
		Retrieve the next token from the current stream.
		*/
		virtual uint32 GetNextToken() = 0;

		/*
		Switch the input stream of the scanner.
		*/
		virtual void SwitchInputStream(std::istream* a_Stream) = 0;

		/*
		Get the name of the last token scanned as a string.
		*/
		virtual std::string GetLastTokenName() = 0;

		/*
		Get the line number we are currently scanning.
		*/
		virtual uint32 GetLineNumber() = 0;
	};
}

#endif // !SAVVY_SCANNER_H