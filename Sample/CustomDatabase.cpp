#include "CustomDatabase.h"

CustomDatabase::CustomDatabase()
{

}

CustomDatabase::~CustomDatabase()
{

}

const bool CustomDatabase::OutputLangEquivalentExists(const Savvy::Key& a_Name)
{
	return false;
}

const Savvy::Key& CustomDatabase::GetOutputLangEquivalent(const Savvy::Key& a_Name)
{
	return a_Name;
}
