#include "SavvyPCH.h"
#include "SavvyKey.h"

Savvy::Key::StringHash Savvy::Key::m_StringHasher;

Savvy::Key::Key() : m_Name()
{
	m_StringHasher(m_Name);
}

Savvy::Key::Key(const std::string& a_Name) : m_Name(a_Name)
{
	m_HashedValue = m_StringHasher(m_Name);
}

Savvy::Key::Key(const char* a_Name) : m_Name(a_Name)
{
	m_HashedValue = m_StringHasher(m_Name);
}

Savvy::Key::~Key()
{

}

const std::string& Savvy::Key::GetString() const
{
	return m_Name;
}

void Savvy::Key::SetString(const std::string& a_Name)
{
	m_Name = a_Name;
	m_HashedValue = m_StringHasher(m_Name);
}

const size_t Savvy::Key::GetHashedValue() const
{
	return m_HashedValue;
}

void Savvy::Key::Append(const std::string& a_Name)
{
	m_Name.append(a_Name);
	m_HashedValue = m_StringHasher(m_Name);
}

bool Savvy::Key::Contains(const std::string& a_Str)
{
	return (m_Name.find(a_Str) != std::string::npos);
}
