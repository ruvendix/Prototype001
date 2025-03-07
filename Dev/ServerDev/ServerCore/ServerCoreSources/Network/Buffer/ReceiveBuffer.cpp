// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "ReceiveBuffer.h"

namespace
{
	const uint32 MAX_BUFFER_BLOCK_COUNT = 10;
}

RxReceiveBuffer::RxReceiveBuffer(uint32 eachBlockSize)
{
	m_eachBlockSize = eachBlockSize;
	m_capacity = (eachBlockSize * MAX_BUFFER_BLOCK_COUNT);
	m_blockBuffers.resize(m_capacity);
}

RxReceiveBuffer::~RxReceiveBuffer()
{

}

void RxReceiveBuffer::Cleanup()
{
	uint32 dataSize = GetDataSize();
	if (GetDataSize() != 0)
	{
		// 여유 공간이 블록 1개 크기 미만이면 데이터를 앞으로 땡김
		if (GetRemainSize() < m_eachBlockSize)
		{
			global::ValidateIndexRange(m_readPos, m_blockBuffers.size());
			::CopyMemory(m_blockBuffers.data(), &m_blockBuffers[m_readPos], dataSize * sizeof(BYTE));

			// 새로 시작해야 하므로 둘 다 리셋
			m_readPos = 0;
			m_wirtePos = 0;
		}
	}
	else
	{
		// 읽기와 쓰기 커서가 같은 위치라면 둘 다 리셋
		m_readPos = 0;
		m_wirtePos = 0;
	}
}

bool RxReceiveBuffer::ProcessRead(uint32 numOfBytes)
{
	if (numOfBytes > GetDataSize())
	{
		return false;
	}

	m_readPos += numOfBytes;
	return true;
}

bool RxReceiveBuffer::ProcessWrite(uint32 numOfBytes)
{
	if (numOfBytes > GetRemainSize())
	{
		return false;
	}

	m_wirtePos += numOfBytes;
	return true;
}