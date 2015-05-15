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