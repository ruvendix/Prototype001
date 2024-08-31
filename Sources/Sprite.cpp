#include "Pch.h"
#include "Sprite.h"

#include "GameApplication.h"
#include "SystemManager.h"
#include "PathSystem.h"
#include "ResourceSystem.h"
#include "Texture.h"

Sprite::Sprite(const std::string& strResPath) :
	ResourceBase(strResPath)
{
	if (strResPath.find(".sprite") == std::string::npos)
	{
		AddFileExtension(".sprite");
	}
}

Sprite::~Sprite()
{
	FOR_EACH_SAFE_DELETE(m_spriteInfos);
}

void Sprite::SaveResource()
{
	std::string strSpriteFullPath = GET_SYSTEM(PathSystem)->GetAssetsDirectory();
	strSpriteFullPath += GetResourcePath();

	// 스프라이트 파일 스트림 생성
	FILE* pSpriteFile = nullptr;
	fopen_s(&pSpriteFile, strSpriteFullPath.c_str(), "wb");
	if ((pSpriteFile == nullptr) ||
		(ferror(pSpriteFile) != 0))
	{
		return;
	}

	// 스프라이트 정보들로 텍스처 스트링 테이블 구성
	std::vector<std::string> texPathTable;

	uint32 spriteInfoCount = m_spriteInfos.size();
	for (uint32 i = 0; i < spriteInfoCount; ++i)
	{
		const SpriteInfo* pSpriteInfo = m_spriteInfos[i];
		if (pSpriteInfo == nullptr)
		{
			continue;
		}

		// 텍스처 경로부터 저장
		TexturePtr spTex = pSpriteInfo->spTex;
		texPathTable.emplace_back(spTex->GetResourcePath());
	}
	texPathTable.erase(std::unique(texPathTable.begin(), texPathTable.end()), texPathTable.end());

	uint32 texPathCount = texPathTable.size();
	fwrite(&texPathCount, sizeof(uint32), 1, pSpriteFile);

	// 텍스처 경로부터 저장
	for (uint32 i = 0; i < texPathCount; ++i)
	{		
		global::SaveStringToFile(texPathTable[i], pSpriteFile);
	}

	// 스프라이트 정보 개수 쓰기
	fwrite(&spriteInfoCount, sizeof(uint32), 1, pSpriteFile);

	// 스프라이트 정보
	for (uint32 i = 0; i < spriteInfoCount; ++i)
	{
		const SpriteInfo* pSpriteInfo = m_spriteInfos[i];
		if (pSpriteInfo == nullptr)
		{
			continue;
		}

		// 텍스처 경로 인덱스를 저장
		TexturePtr spTex = pSpriteInfo->spTex;
		const auto& foundIter = std::find(texPathTable.begin(), texPathTable.end(), spTex->GetResourcePath());
		uint32 texPathIdx = std::distance(texPathTable.begin(), foundIter);

		fwrite(&texPathIdx, sizeof(uint32), 1, pSpriteFile);
		fwrite(&pSpriteInfo->excludeColor, sizeof(uint32), 1, pSpriteFile);
		fwrite(&pSpriteInfo->startPos, sizeof(Int2d), 1, pSpriteFile);
		fwrite(&pSpriteInfo->size, sizeof(Size), 1, pSpriteFile);
	}

	fclose(pSpriteFile);
}

void Sprite::LoadResource()
{
	std::string strSpriteFullPath = GET_SYSTEM(PathSystem)->GetAssetsDirectory();
	strSpriteFullPath += GetResourcePath();

	// 스프라이트 파일 열기
	FILE* pSpriteFile = nullptr;
	fopen_s(&pSpriteFile, strSpriteFullPath.c_str(), "rb");
	if ((pSpriteFile == nullptr) ||
		(ferror(pSpriteFile) != 0))
	{
		return;
	}

	uint32 texPathCount = 0;
	fread(&texPathCount, sizeof(uint32), 1, pSpriteFile);

	// 텍스처 스트링 테이블 읽기
	std::vector<std::string> texPathTable;
	for (uint32 i = 0; i < 1; ++i)
	{
		texPathTable.push_back(global::LoadStringFromFile(pSpriteFile));
	}

	// 스프라이트 정보 개수 읽기
	uint32 spriteInfoCount = 0;
	fread(&spriteInfoCount, sizeof(uint32), 1, pSpriteFile);

	// 스프라이트 정보
	for (uint32 i = 0; i < spriteInfoCount; ++i)
	{
		SpriteInfo spriteInfo;

		// 일단은 텍스처부터 로딩
		uint32 texStrPathIdx = 0;
		fread(&texStrPathIdx, sizeof(uint32), 1, pSpriteFile);
		spriteInfo.spTex = GET_SYSTEM(ResourceSystem)->LoadTexture(texPathTable[texStrPathIdx]);

		fread(&spriteInfo.excludeColor, sizeof(uint32), 1, pSpriteFile);
		fread(&spriteInfo.startPos, sizeof(Int2d), 1, pSpriteFile);
		fread(&spriteInfo.size, sizeof(Size), 1, pSpriteFile);

		AddSpriteInfo(spriteInfo);
	}

	fclose(pSpriteFile);
}

void Sprite::AddSpriteInfo(const SpriteInfo& spriteInfo)
{
	SpriteInfo* pNewSpriteInfo = new SpriteInfo;
	*pNewSpriteInfo = spriteInfo;
	m_spriteInfos.push_back(pNewSpriteInfo);
}