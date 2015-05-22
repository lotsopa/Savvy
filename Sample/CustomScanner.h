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
#ifndef CUSTOM_SCANNER_H
#define CUSTOM_SCANNER_H
#include "SavvyFramework.h"

class CustomScanner : public Savvy::Scanner
{
public:

	CustomScanner();

	virtual ~CustomScanner();

	/*
	Retrieve the next token from the current stream.
	*/
	virtual Savvy::uint32 GetNextToken();

	/*
	Switch the input stream of the scanner.
	*/
	virtual void SwitchInputStream(std::istream* a_Stream);

	/*
	Get the name of the last token scanned as a string.
	*/
	virtual std::string GetLastTokenName();

	/*
	Get the line number we are currently scanning.
	*/
	virtual Savvy::uint32 GetLineNumber();
};

CustomScanner::CustomScanner() : Scanner()
{

}

CustomScanner::~CustomScanner()
{

}

Savvy::uint32 CustomScanner::GetNextToken()
{
	// Normally yylex() would return this, if using the Flex lexical analyzer
	return Savvy::SAVVY_EOF;
}

void CustomScanner::SwitchInputStream(std::istream* a_Stream)
{

}

std::string CustomScanner::GetLastTokenName()
{
	return std::string("Not implemented");
}

Savvy::uint32 CustomScanner::GetLineNumber()
{
	return 0;
}

#endif // !CUSTOM_SCANNER_H