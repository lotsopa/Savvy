#include "CustomConstructor.h"

CustomConstructor::CustomConstructor() : Constructor()
{

}

CustomConstructor::~CustomConstructor()
{

}

void CustomConstructor::Reset()
{
	Savvy::Constructor::Reset();
}

Savvy::ResultCode CustomConstructor::Initialize(Savvy::Key& a_InputEntry, Savvy::Key& a_OutputEntry, Savvy::uint32 a_Inputlang, Savvy::uint32 a_ShaderType, Savvy::Database* a_Database)
{
	Savvy::Constructor::Initialize(a_InputEntry, a_OutputEntry, a_Inputlang, a_ShaderType, a_Database);
	return Savvy::SAVVY_OK;
}

Savvy::ResultCode CustomConstructor::Construct(std::ostream& a_OutputStream)
{
	return Savvy::SAVVY_OK;
}
