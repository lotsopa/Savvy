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
#ifndef SAVVY_DATABASE_HLSL_H
#define SAVVY_DATABASE_HLSL_H
#include "SavvyDatabase.h"

namespace Savvy
{
	namespace Internal
	{
		/*
		The HLSL Database deals with language specific data which needs to be stored.
		*/
		class DatabaseHLSL : public Database
		{
		public:
			DatabaseHLSL();
			virtual ~DatabaseHLSL();

			/*
			Resets the state of the Database and clears any data it holds.
			*/
			virtual void Reset();

			/*
			Checks if an equivalent for the input language keyword exists in the output language.
			Returns true if it exists, false otherwise.
			*/
			virtual const bool OutputLangEquivalentExists(const Key& a_Name);

			/*
			Returns a keyword with the same meaning from the output language.
			*/
			virtual const Key& GetOutputLangEquivalent(const Key& a_Name);

		private:
			static Database::KeyMap m_DataTypeMapHLSL;
			static Database::KeyMap CreateKeywordMap();
		};
	}
}

#endif // !SAVVY_DATABASE_HLSL_H