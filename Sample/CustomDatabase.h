#ifndef CUSTOM_DATABASE_H
#define CUSTOM_DATABASE_H
#include "SavvyFramework.h"

class CustomDatabase : public Savvy::Database
{
public:
	/*
	Default Constructor
	*/
	CustomDatabase();

	/*
	Destructor
	*/
	virtual ~CustomDatabase();

	/*
	Checks if an equivalent for the input language keyword exists in the output language.
	Returns true if it exists, false otherwise.
	*/
	virtual const bool OutputLangEquivalentExists(const Savvy::Key& a_Name);

	/*
	Returns a keyword with the same meaning from the output language.
	*/
	virtual const Savvy::Key& GetOutputLangEquivalent(const Savvy::Key& a_Name);
};

#endif