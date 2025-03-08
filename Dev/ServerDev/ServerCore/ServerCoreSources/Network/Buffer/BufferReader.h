// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class RxBufferReader
{
public:
	RxBufferReader(BYTE* targetBuffer, uint32 targetSize, uint32 currentReadPos);
	~RxBufferReader();

	template <typename T>
	bool Read(T* pDest) { return Read(pDest, sizeof(T)); }
	bool Read(void* pDest, uint32 numOfBytes);

	uint32 GetRemainSize() { return (m_targetSize - m_currentReadPos); }

private:
	BYTE* m_targetBuffer = nullptr; // 읽을 버퍼
	uint32 m_targetSize = 0; // 읽어야 할 크기
	uint32 m_currentReadPos = 0; // 현재 읽어야 할 대상에서의 위치
};