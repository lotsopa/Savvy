#ifndef SAVVY_KEY_H
#define SAVVY_KEY_H

#include "SavvyFrameworkDefines.h"

namespace Savvy
{
	/*
	The Key class is just an utility class that wraps strings and uses hashing
	so lookups and compares are faster than normal strings.
	*/
	class Key
	{
	public:
		Key();
		Key(const std::string& a_Name);
		Key(const char* a_Name);
		~Key();

		const std::string& GetString() const;
		void SetString(const std::string& a_Name);
		void Append(const std::string& a_Name);
		const size_t GetHashedValue() const;
		SAVVY_INLINE const bool IsEmpty() { return m_Name.empty(); }
		SAVVY_INLINE void Clear() { m_Name.clear(); m_HashedValue = 0; }

		bool operator<(const Key& key1) const { return m_HashedValue < key1.m_HashedValue; }

		Key operator=(const Key &k)
		{
			m_Name = k.m_Name;
			m_HashedValue = k.m_HashedValue;
			return *this;
		}

		Key operator=(const std::string& str)
		{
			m_Name = str;
			m_HashedValue = m_StringHasher(m_Name);
			return *this;
		}

		bool operator==(const Key& k) const
		{
			return m_HashedValue == k.m_HashedValue;
		}

		bool operator!=(const Key& k) const
		{
			return m_HashedValue != k.m_HashedValue;
		}

		Key operator+(const Key& k)
		{
			return Key(m_Name + k.m_Name);
		}

		bool Contains(const std::string& a_Str);

	private:
		typedef std::hash<std::string> StringHash;
		std::string m_Name;
		static StringHash m_StringHasher;
		size_t m_HashedValue;
	};
}

#endif // !SAVVY_KEY_H