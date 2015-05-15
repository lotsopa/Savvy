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