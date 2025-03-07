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

	template <typename T> // �����͸� �����ؾ� �ϹǷ� rValue�� ����
	RxBufferReader& operator>>(OUT T& dest)
	{
		dest = *(reinterpret_cast<T*>(&m_targetBuffer[m_currentReadPos]));
		m_currentReadPos += sizeof(T);
		return *this;
	}

	uint32 GetRemainSize() { return m_targetSize - m_currentReadPos; }

private:
	BYTE* m_targetBuffer = nullptr; // ���� ����
	uint32 m_targetSize = 0; // �о�� �� ũ��
	uint32 m_currentReadPos = 0; // ���� �о�� �� ��󿡼��� ��ġ
};