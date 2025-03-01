// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class RxBufferWriter
{
public:
	RxBufferWriter(BYTE* targetBuffer, uint32 targetSize, uint32 currentWritePos);
	~RxBufferWriter();

	template <typename T>
	T* Reserve() // 미리 예약만
	{
		if (GetRemainSize() < sizeof(T))
		{
			return nullptr;
		}

		T* pBlock = reinterpret_cast<T*>(&m_targetBuffer[m_currentWritePos]);
		m_currentWritePos += sizeof(T);
		return pBlock;
	}

	template <typename T> // 데이터만큼 확보해야 하므로 lValue도 가능
	RxBufferWriter& operator<<(T&& src)
	{
		// 레퍼런스 제거한 후에 const 제거
		using TData = std::remove_const_t<std::remove_reference_t<T>>;
		*(reinterpret_cast<TData*>(&m_targetBuffer[m_currentWritePos])) = std::forward<T>(src);
		m_currentWritePos += sizeof(T);
		return *this;
	}

	uint32 GetRemainSize() const  { return m_targetSize - m_currentWritePos; }
	uint32 GetCurrentWritePos() const { return m_currentWritePos; }

private:
	BYTE* m_targetBuffer = nullptr; // 쓸 버퍼
	uint32 m_targetSize = 0; // 쓰기 가능한 크기
	uint32 m_currentWritePos = 0; // 현재 써야 할 대상에서의 위치
};