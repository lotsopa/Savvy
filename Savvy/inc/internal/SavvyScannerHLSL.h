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
#ifndef SAVVY_SCANNER_HLSL_H
#define SAVVY_SCANNER_HLSL_H

#include "SavvyScanner.h"
#include <string>

#undef yyFlexLexer
#define yyFlexLexer hlslFlexLexer
//#ifndef yyFlexLexerOnce
#include "FlexLexer.h"
//#endif

#undef YY_DECL
#define YY_DECL int Savvy::Internal::ScannerHLSL::yylex()

namespace Savvy
{
	namespace Internal
	{
		class ScannerHLSL : private yyFlexLexer, public Scanner
		{
		public:

			/*
			Default Constructor which uses cin as input
			stream and cout as output stream
			*/
			ScannerHLSL();

			virtual ~ScannerHLSL() {};

			/*
			Constructor with a specific input stream.
			Output stream is cout.
			*/
			ScannerHLSL(std::istream* a_Stream);

			/*
			Constructor with a specific input and output stream.
			*/
			ScannerHLSL(std::istream* a_InStream, std::ostream* a_OutStream);

			/*
			Retrieve the next token from the current stream.
			*/
			virtual uint32 GetNextToken();

			/*
			Switch the input stream of the scanner.
			*/
			virtual void SwitchInputStream(std::istream* a_Stream);

			/*
			Get the name of the last token scanned as a string.
			*/
			virtual std::string GetLastTokenName();

			virtual uint32 GetLineNumber();

		private:
			int yylex();

			ScannerHLSL(ScannerHLSL& Rhs); // Do not implement
			ScannerHLSL& operator=(ScannerHLSL& Rhs); // Do not implement
		};


		SAVVY_INLINE ScannerHLSL::ScannerHLSL()
			: yyFlexLexer(0, 0)
		{
		}

		SAVVY_INLINE ScannerHLSL::ScannerHLSL(std::istream* a_Stream)
			: yyFlexLexer(a_Stream, 0)
		{
		}

		SAVVY_INLINE ScannerHLSL::ScannerHLSL(std::istream* a_InStream, std::ostream* a_OutStream)
			: yyFlexLexer(a_InStream, a_OutStream)
		{
		}

		SAVVY_INLINE uint32 ScannerHLSL::GetNextToken()
		{
			return (uint32)yylex();
		}

		SAVVY_INLINE void ScannerHLSL::SwitchInputStream(std::istream* a_Stream)
		{
			switch_streams(a_Stream);
		}

		SAVVY_INLINE std::string ScannerHLSL::GetLastTokenName()
		{
			const char* strPtr = YYText();
			int strSize = YYLeng();
			return std::string(strPtr, strSize);
		}

		SAVVY_INLINE uint32 ScannerHLSL::GetLineNumber()
		{
			return (uint32)lineno();
		}
	}
}
#endif