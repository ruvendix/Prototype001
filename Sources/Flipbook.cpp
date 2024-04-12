/*
��Ʈ�� ���ҽ�
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

	// Ÿ�� ��ĭ�� ũ�⸦ ����
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
		// Ű������ �����
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

	// �ѹ� ���� �� ����ȵ�
	if (m_timeTable[keyFrameIdx] != 0.0f)
	{
		return;
	}

	// ���� �ð����� Ÿ�� ���̺��� �籸��
	m_flipbookEndTime += keepTime;
	m_timeTable[keyFrameIdx] = m_flipbookEndTime; // Ű�������� ����� �ð����� ����
}

const SpriteInfo* Flipbook::CalcKeyframe(float localTime) const
{
	// Ÿ�� ���̺��� �������� �ִ� �ð��� �Ѿ�� ����
	float endTime = m_flipbookEndTime;
	while (endTime < localTime)
	{
		localTime -= endTime;
	}

	auto iter = std::lower_bound(m_timeTable.cbegin(), m_timeTable.cend(), localTime);
	uint32 currentKeyframeIdx = std::distance(m_timeTable.begin(), iter);
	//assert(m_timeTable.size() >= currentKeyframeIdx); // ������ ����� �� �Ұ�����
	return m_spSprite->GetSpriteInfo(currentKeyframeIdx);
}