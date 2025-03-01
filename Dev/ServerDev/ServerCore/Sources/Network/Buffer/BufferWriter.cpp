// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "BufferWriter.h"

RxBufferWriter::RxBufferWriter(BYTE* targetBuffer, uint32 targetSize, uint32 currentWritePos)
{
	m_targetBuffer = targetBuffer;
	m_targetSize = targetSize;
	m_currentWritePos = currentWritePos;
}

RxBufferWriter::~RxBufferWriter()
{

}