/*
비트맵 리소스
*/

#include "Pch.h"
#include "Flipbook.h"

#include "GameApplication.h"
#include "SystemManager.h"
#include "ResourceSystem.h"
#include "Texture.h"

Flipbook::Flipbook()
{

}

Flipbook::Flipbook(SpritePtr spSprite)
{
	assert(spSprite != nullptr);
	m_spSprite = spSprite;

	uint32 spriteInfoCount = m_spSprite->GetSpriteInfoCount();
	m_timeTable.resize(spriteInfoCount);
}

Flipbook::~Flipbook()
{

}

void Flipbook::LoadResource(const std::string& strPath)
{
	UNREFERENCED_PARAMETER(strPath);
}

void Flipbook::CreateKeyframes(const std::string& strSpriteKey, const FlipbookData& flipbookData)
{
	const Size& texSize = flipbookData.spTex->GetSize();

	// 타일 한칸의 크기를 구함
	Size tileSize;
	tileSize.width = texSize.width / flipbookData.tileCount.x;
	tileSize.height = texSize.height / flipbookData.tileCount.y;

	m_spSprite = GET_SYSTEM(ResourceSystem)->FindSprite(strSpriteKey);
	if (m_spSprite == nullptr)
	{
		m_spSprite = GET_SYSTEM(ResourceSystem)->CreateSprite(strSpriteKey);
	}

	m_timeTable.resize(flipbookData.keyframeCount);
	for (uint32 i = 0; i < flipbookData.keyframeCount; ++i)
	{
		// 키프레임 만들기
		SpriteInfo keyframeInfo;
		keyframeInfo.pos = Point2d((i + flipbookData.tileStartPos.x) * tileSize.width, flipbookData.tileStartPos.y * tileSize.height);
		keyframeInfo.size = tileSize;
		keyframeInfo.spTex = flipbookData.spTex;
		keyframeInfo.excludeColor = flipbookData.excludeColorKey;
		m_spSprite->AddSpriteInfo(keyframeInfo);

		AddKeyframeTime(i, flipbookData.keyframeInterval);
	}
}

void Flipbook::AddKeyframeTime(uint32 keyFrameIdx, float keepTime)
{
	assert(m_timeTable.size() >= keyFrameIdx);

	// 한번 넣은 건 변경안됨
	if (m_timeTable[keyFrameIdx] != 0.0f)
	{
		return;
	}

	// 간격 시간으로 타임 테이블을 재구성
	m_flipbookEndTime += keepTime;
	m_timeTable[keyFrameIdx] = m_flipbookEndTime; // 키프레임이 종료될 시간으로 설정
}

const SpriteInfo* Flipbook::CalcKeyframe(float localTime) const
{
	// 타임 테이블의 마지막에 있는 시간을 넘어가면 빼줌
	float endTime = m_flipbookEndTime;
	while (endTime < localTime)
	{
		localTime -= endTime;
	}

	auto iter = std::lower_bound(m_timeTable.cbegin(), m_timeTable.cend(), localTime);
	uint32 currentKeyframeIdx = std::distance(m_timeTable.begin(), iter);
	//assert(m_timeTable.size() >= currentKeyframeIdx); // 범위를 벗어나는 건 불가능함
	return m_spSprite->GetSpriteInfo(currentKeyframeIdx);
}