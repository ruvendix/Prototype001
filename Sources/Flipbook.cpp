/*
비트맵 리소스
*/

#include "Pch.h"
#include "Flipbook.h"

#include "GameApplication.h"
#include "SystemManager.h"
#include "ResourceSystem.h"
#include "PathSystem.h"
#include "Texture.h"

Flipbook::Flipbook(const std::string& strResPath) :
	ResourceBase(strResPath)
{
	if (strResPath.find(".flipbook") == std::string::npos)
	{
		AddFileExtension(".flipbook");
	}
}

Flipbook::~Flipbook()
{

}

void Flipbook::CreateTimeTable()
{
	uint32 keyframeCount = m_spSprite->GetSpriteInfoCount();
	m_timeTable.resize(keyframeCount);

	for (uint32 i = 0; i < keyframeCount; ++i)
	{
		AddKeyframeTime(i, m_keyframeIntervals[i]);
	}
}

void Flipbook::CreateKeyframes(const std::string strSprite, float keyframeInterval)
{
	m_spSprite = GET_SYSTEM(ResourceSystem)->LoadSprite(strSprite);

	m_keyframeIntervals.resize(m_spSprite->GetSpriteInfoCount());
	for (uint32 i = 0; i < m_spSprite->GetSpriteInfoCount(); ++i)
	{
		m_keyframeIntervals[i] = keyframeInterval;
	}

	CreateTimeTable();
}

void Flipbook::SaveResource()
{
	std::string strFlipbookFullPath = GET_SYSTEM(PathSystem)->GetAssetsDirectory();
	strFlipbookFullPath += GetResourcePath();

	// 플립북 파일 열기
	FILE* pFlipbookFile = nullptr;
	fopen_s(&pFlipbookFile, strFlipbookFullPath.c_str(), "wb");
	if ((pFlipbookFile == nullptr) ||
		(ferror(pFlipbookFile) != 0))
	{
		return;
	}

	global::SaveStringToFile(m_spSprite->GetResourcePath(), pFlipbookFile);
	fwrite(m_keyframeIntervals.data(), sizeof(float), m_keyframeIntervals.size(), pFlipbookFile);

	fclose(pFlipbookFile);
}

void Flipbook::LoadResource()
{
	std::string strFlipbookFullPath = GET_SYSTEM(PathSystem)->GetAssetsDirectory();
	strFlipbookFullPath += GetResourcePath();

	// 플립북 파일 열기
	FILE* pFlipbookFile = nullptr;
	fopen_s(&pFlipbookFile, strFlipbookFullPath.c_str(), "rb");
	if ((pFlipbookFile == nullptr) ||
		(ferror(pFlipbookFile) != 0))
	{
		return;
	}

	std::string strSpritePath = global::LoadStringFromFile(pFlipbookFile);
	m_spSprite = GET_SYSTEM(ResourceSystem)->LoadSprite(strSpritePath);

	m_keyframeIntervals.resize(m_spSprite->GetSpriteInfoCount());
	fread(m_keyframeIntervals.data(), sizeof(float), m_keyframeIntervals.size(), pFlipbookFile);

	fclose(pFlipbookFile);

	CreateTimeTable();
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