// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "BufferReader.h"

RxBufferReader::RxBufferReader(BYTE* targetBuffer, uint32 targetSize, uint32 currentReadPos)
{
	m_targetBuffer = targetBuffer;
	m_targetSize = targetSize;
	m_currentReadPos = currentReadPos;
}

RxBufferReader::~RxBufferReader()
{

}

bool RxBufferReader::Read(void* pDest, uint32 numOfBytes)
{
	if (GetRemainSize() < numOfBytes)
	{
		return false;
	}

	::CopyMemory(pDest, &m_targetBuffer[m_currentReadPos], sizeof(BYTE) * numOfBytes);
	m_currentReadPos += numOfBytes;
	
	return true;
}