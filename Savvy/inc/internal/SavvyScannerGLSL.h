#ifndef SAVVY_SCANNER_GLSL_H
#define SAVVY_SCANNER_GLSL_H

#include "SavvyScanner.h"
#include <string>

#undef yyFlexLexer
#define yyFlexLexer glslFlexLexer

#ifndef yyFlexLexerOnce
#include "FlexLexer.h"
#endif

#undef YY_DECL
#define YY_DECL int Savvy::Internal::ScannerGLSL::yylex()

namespace Savvy
{
	namespace Internal
	{
		class ScannerGLSL : private yyFlexLexer, public Scanner
		{
		public:

			/*
			Default Constructor which uses cin as input
			stream and cout as output stream
			*/
			ScannerGLSL();

			virtual ~ScannerGLSL() {};

			/*
			Constructor with a specific input stream.
			Output stream is cout.
			*/
			ScannerGLSL(std::istream* a_Stream);

			/*
			Constructor with a specific input and output stream.
			*/
			ScannerGLSL(std::istream* a_InStream, std::ostream* a_OutStream);

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

			ScannerGLSL(ScannerGLSL& Rhs); // Do not implement
			ScannerGLSL& operator=(ScannerGLSL& Rhs); // Do not implement
		};


		SAVVY_INLINE ScannerGLSL::ScannerGLSL()
			: yyFlexLexer(0, 0)
		{
		}

		SAVVY_INLINE ScannerGLSL::ScannerGLSL(std::istream* a_Stream)
			: yyFlexLexer(a_Stream, 0)
		{
		}

		SAVVY_INLINE ScannerGLSL::ScannerGLSL(std::istream* a_InStream, std::ostream* a_OutStream)
			: yyFlexLexer(a_InStream, a_OutStream)
		{
		}

		SAVVY_INLINE uint32 ScannerGLSL::GetNextToken()
		{
			return (uint32)yylex();
		}

		SAVVY_INLINE void ScannerGLSL::SwitchInputStream(std::istream* a_Stream)
		{
			switch_streams(a_Stream);
		}

		SAVVY_INLINE std::string ScannerGLSL::GetLastTokenName()
		{
			const char* strPtr = YYText();
			int strSize = YYLeng();
			return std::string(strPtr, strSize);
		}

		SAVVY_INLINE uint32 ScannerGLSL::GetLineNumber()
		{
			return (uint32)lineno();
		}
	}
}
#endif