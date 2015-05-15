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