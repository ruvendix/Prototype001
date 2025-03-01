// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class RxReceiveBuffer
{
public:
	RxReceiveBuffer(uint32 eachBlockSize);
	~RxReceiveBuffer();

	void Cleanup();

	bool ProcessRead(uint32 numOfBytes);
	bool ProcessWrite(uint32 numOfBytes);

	BYTE* GetReadPosition()
	{
		global::ValidateIndexRange(m_readPos, m_blockBuffers.size());
		return &(m_blockBuffers[m_readPos]);
	}

	BYTE* GetWrtiePosition()
	{
		global::ValidateIndexRange(m_wirtePos, m_blockBuffers.size());
		return &(m_blockBuffers[m_wirtePos]);
	}

	uint32 GetDataSize() const { assert(m_readPos < m_wirtePos); return (m_wirtePos - m_readPos); }
	uint32 GetRemainSize() const { assert(m_wirtePos < m_capacity); return (m_capacity - m_wirtePos); }

private:
	uint32 m_capacity = 0;
	uint32 m_readPos = 0;
	uint32 m_wirtePos = 0;

	// 내부적으로는 여러 개의 블록이 있는 느낌
	std::vector<BYTE> m_blockBuffers;
	uint32 m_eachBlockSize = 0;
};