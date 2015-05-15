#include "SavvyPCH.h"
#include "SavvyBlob.h"

Savvy::Blob::Blob() : m_RawData(NULL), m_DataSize(0)
{

}

Savvy::Blob::Blob(const char8* a_Data, uint32 a_DataSize)
{
	m_DataSize = a_DataSize;
	m_RawData = new char8[a_DataSize];

	for (uint32 i = 0; i < m_DataSize; i++)
	{
		m_RawData[i] = a_Data[i];
	}
}

Savvy::Blob::Blob(const Blob& a_Blob)
{
	m_DataSize = a_Blob.m_DataSize;

	if (m_RawData)
		delete[] m_RawData;

	m_RawData = new char8[m_DataSize];

	for (uint32 i = 0; i < m_DataSize; i++)
	{
		m_RawData[i] = a_Blob.m_RawData[i];
	}
}

Savvy::Blob::~Blob()
{
	if (m_RawData)
		delete[] m_RawData;
}

void Savvy::Blob::SetData(const char8* a_DataPtr, uint32 a_DataSize)
{
	m_DataSize = a_DataSize;

	if (m_RawData)
		delete[] m_RawData;

	m_RawData = new char8[m_DataSize];

	for (uint32 i = 0; i < m_DataSize; i++)
	{
		m_RawData[i] = a_DataPtr[i];
	}
}

void Savvy::Blob::SetData(const Blob& a_Blob)
{
	m_DataSize = a_Blob.m_DataSize;

	if (m_RawData)
		delete[] m_RawData;

	m_RawData = new char8[m_DataSize];

	for (uint32 i = 0; i < m_DataSize; i++)
	{
		m_RawData[i] = a_Blob.m_RawData[i];
	}
}
