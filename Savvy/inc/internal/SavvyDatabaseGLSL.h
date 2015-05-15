#ifndef SAVVY_DATABASE_GLSL_H
#define SAVVY_DATABASE_GLSL_H

#include "SavvyDatabase.h"

namespace Savvy
{
	namespace Internal
	{
		/*
		The GLSL Database deals with language specific data which needs to be stored.
		*/
		class DatabaseGLSL : public Database
		{
		public:
			DatabaseGLSL();
			virtual ~DatabaseGLSL();

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
			static Database::KeyMap m_DataTypeMapGLSL;
			static Database::KeyMap CreateKeywordMap();

		};
	}
}

#endif // !SAVVY_DATABASE_GLSL_H