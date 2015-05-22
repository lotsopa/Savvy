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