#include "SavvyPCH.h"
#include "SavvyException.h"

Savvy::Exception::Exception(std::string a_Msg) : m_Error(a_Msg)
{

}

Savvy::Exception::~Exception()
{

}

const char* Savvy::Exception::what() const throw ()
{
	return m_Error.c_str();
}
