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
#ifndef SAVVY_CONSTRUCTOR_H
#define SAVVY_CONSTRUCTOR_H

#include "SavvyFrameworkDefines.h"
#include "SavvyDefines.h"
#include "SavvyKey.h"
#include "SavvyDatabase.h"

namespace Savvy
{
	/*
	The Constructor class is used to construct the equivalent of a parsed shader.
	*/
	class Constructor
	{
	public:

		/*
		Default Constructor
		*/
		Constructor();

		/*
		Default Destructor
		*/
		virtual ~Constructor();


		/*
		The function which resets the Constructor to its initial state.
		*/
		virtual void Reset();

		/*
		The function which initializes the Constructor options.
		It MUST be called before the Construct() function for proper behaviour.
		*/
		virtual ResultCode Initialize(Key& a_InputEntry, Key& a_OutputEntry, uint32 a_Inputlang, uint32 a_ShaderType, Database* a_Database);

		/*
		The function which constructs the new shader to the specified output stream.
		*/
		virtual ResultCode Construct(std::ostream& a_OutputStream) = 0;

		static std::string ReplaceAll(std::string str, const std::string& from, const std::string& to);

		SAVVY_INLINE std::string GetLastError() { return m_LastError; }

	protected:

		Key m_InputEntry;
		Key m_OutputEntry;
		Database* m_Database;
		uint32 m_InputShaderLang;
		uint32 m_CurrShader;
		std::string m_LastError;

		// Replaces all occurrences of a variable everywhere in the database
		void ReplaceVarName(std::string& a_Var, std::string& a_NewVar);
	};
}

#endif // !SAVVY_CONSTRUCTOR_H