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
		// ���� ������ ��� 1�� ũ�� �̸��̸� �����͸� ������ ����
		if (GetRemainSize() < m_eachBlockSize)
		{
			global::ValidateIndexRange(m_readPos, m_blockBuffers.size());
			::CopyMemory(m_blockBuffers.data(), &m_blockBuffers[m_readPos], dataSize * sizeof(BYTE));

			// ���� �����ؾ� �ϹǷ� �� �� ����
			m_readPos = 0;
			m_wirtePos = 0;
		}
	}
	else
	{
		// �б�� ���� Ŀ���� ���� ��ġ��� �� �� ����
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