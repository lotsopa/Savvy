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
#ifndef SAVVY_BLOB_H
#define SAVVY_BLOB_H

#include "SavvyFrameworkDefines.h"

namespace Savvy
{
	/*
	The Blob class is just a container for raw data.
	It is responsible for deleting the data it holds when its lifespan expires.
	Warning: The class does not take ownership of the passed data.
	Instead, it copies it and holds its own internal copy.
	This means that you can delete the previously stored data, after constructing the Blob.
	*/
	class Blob
	{
	public:
		Blob();
		Blob(const Blob& a_Blob);
		Blob(const char8* a_Data, uint32 a_DataSize);
		~Blob();

		SAVVY_INLINE char8* GetRawDataPtr() { return m_RawData; }
		SAVVY_INLINE const uint32 GetDataSize() { return m_DataSize; }
		void SetData(const char8* a_DataPtr, uint32 a_DataSize);
		void SetData(const Blob& a_Blob);

	private:
		char8* m_RawData;
		uint32 m_DataSize;
	};
}

#endif // !SAVVY_BLOB_H