// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class RxBufferWriter
{
public:
	RxBufferWriter(BYTE* targetBuffer, uint32 targetSize, uint32 currentWritePos);
	~RxBufferWriter();

	template <typename T>
	T* Reserve() // �̸� ���ุ
	{
		if (GetRemainSize() < sizeof(T))
		{
			return nullptr;
		}

		T* pBlock = reinterpret_cast<T*>(&m_targetBuffer[m_currentWritePos]);
		m_currentWritePos += sizeof(T);
		return pBlock;
	}

	template <typename T> // �����͸�ŭ Ȯ���ؾ� �ϹǷ� lValue�� ����
	RxBufferWriter& operator<<(T&& src)
	{
		// ���۷��� ������ �Ŀ� const ����
		using TData = std::remove_const_t<std::remove_reference_t<T>>;
		*(reinterpret_cast<TData*>(&m_targetBuffer[m_currentWritePos])) = std::forward<T>(src);
		m_currentWritePos += sizeof(T);
		return *this;
	}

	uint32 GetRemainSize() const  { return m_targetSize - m_currentWritePos; }
	uint32 GetCurrentWritePos() const { return m_currentWritePos; }

private:
	BYTE* m_targetBuffer = nullptr; // �� ����
	uint32 m_targetSize = 0; // ���� ������ ũ��
	uint32 m_currentWritePos = 0; // ���� ��� �� ��󿡼��� ��ġ
};