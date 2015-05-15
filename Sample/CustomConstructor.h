#ifndef CUSTOM_CONSTRUCTOR_H
#define CUSTOM_CONSTRUCTOR_H

#include "SavvyFramework.h"

class CustomConstructor : public Savvy::Constructor
{
public:
	CustomConstructor();
	virtual ~CustomConstructor();

	/*
	The function which resets the Constructor to its initial state.
	*/
	virtual void Reset();

	/*
	The function which initializes the Constructor options.
	It MUST be called before the Construct() function for proper behaviour.
	*/
	virtual Savvy::ResultCode Initialize(Savvy::Key& a_InputEntry, Savvy::Key& a_OutputEntry, Savvy::uint32 a_Inputlang, Savvy::uint32 a_ShaderType, Savvy::Database* a_Database);

	/*
	The function which constructs the new shader to the specified output stream.
	*/
	virtual Savvy::ResultCode Construct(std::ostream& a_OutputStream);

};

#endif // !CUSTOM_CONSTRUCTOR_H