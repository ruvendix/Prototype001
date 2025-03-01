// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class RxSendBuffer : public std::enable_shared_from_this<RxSendBuffer>
{
public:
	RxSendBuffer(uint32 bufferSize);
	~RxSendBuffer();

	BYTE* GetBufferPointer() { return m_buffer.data(); }
	uint32 GetWriteSize() const { return m_writeSize; }
	size_t GetCapacity() const { return m_buffer.size(); }

	void CopyBuffer(void* buffer, uint32 size);
	void SetWriteSize(uint32 writeSize) { m_writeSize = writeSize; }

private:
	std::vector<BYTE> m_buffer;
	uint32 m_writeSize = 0;
};