// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "SendBuffer.h"

RxSendBuffer::RxSendBuffer(uint32 bufferSize)
{
	m_buffer.resize(bufferSize);
}

RxSendBuffer::~RxSendBuffer()
{

}

void RxSendBuffer::CopyBuffer(void* buffer, uint32 size)
{
	assert(GetCapacity() >= size);
	::CopyMemory(GetBufferPointer(), buffer, size);
	m_writeSize = size;
}