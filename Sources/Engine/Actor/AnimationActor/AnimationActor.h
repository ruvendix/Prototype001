// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

#include "Engine/Resource/ResourceMananger.h"
#include "Engine/Resource/Sprite/DynamicSprite.h"

class WorldTileMapActor;

class AnimationActor : public CellActor
{
	using Super = CellActor;

public:
	using ActorLookAtStringTable = std::array<std::string, TO_NUM(EActorLookAtType::Count)>;
	using ActorLookAtDynamicSpriteTable = std::array<DynamicSpritePtr, TO_NUM(EActorLookAtType::Count)>;

public:
	using Super::Super;
	virtual ~AnimationActor();

public:
	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;

public:
	template <typename TActorState>
	void CreateActorStateLookAtDynamicSprites(const std::string& strPrefix, bool bLoopDynamicSprite)
	{
		auto foundIter = m_mapActorStateDynamicSprite.find(TActorState::s_id);
		if (foundIter != m_mapActorStateDynamicSprite.cend())
		{
			return;
		}

		ActorLookAtDynamicSpriteTable actorLookAtDynamicSpriteTable;
		for (int32 i = 0; i < TO_NUM(EActorLookAtType::Count); ++i)
		{
			std::string strDynamicSpriteName;
			MakeFormatString(strDynamicSpriteName, "%s%s", strPrefix.c_str(), s_actorLookAtStringTable[i].c_str());

			// ��������Ʈ �ε��ε� ������ ����
			const DynamicSpritePtr& spDynamicSprite = ResourceMananger::I()->CreateDynamicSprite(strDynamicSpriteName);
			ASSERT_LOG(spDynamicSprite != nullptr);
			spDynamicSprite->SetLoopDynamicSprite(bLoopDynamicSprite);
			spDynamicSprite->FindAndSetTexture(m_actorLookAtTexturePathTable[i]);

			actorLookAtDynamicSpriteTable[i] = spDynamicSprite;
		}

		auto resultIter = m_mapActorStateDynamicSprite.insert(std::make_pair(TActorState::s_id, actorLookAtDynamicSpriteTable));
		ASSERT_LOG(resultIter.second == true);
	}

	template <typename TActorState>
	void AddActorStateKeyFrame(int32 startIdx, int32 endIdx, int32 spriteLine,
		const Size& drawSize, uint32 colorKey, float keepTime, EActorLookAtType actorLookAtType)
	{
		auto foundIter = m_mapActorStateDynamicSprite.find(TActorState::s_id);
		if (foundIter == m_mapActorStateDynamicSprite.cend())
		{
			return;
		}

		// �ش�Ǵ� ��������Ʈ ��������
		const ActorLookAtDynamicSpriteTable& actorLookAtDynmaicSpriteTable = foundIter->second;		
		const DynamicSpritePtr& spDynamicSprite = actorLookAtDynmaicSpriteTable[TO_NUM(actorLookAtType)];
		ASSERT_LOG(spDynamicSprite != nullptr);
		spDynamicSprite->AddKeyFrames(startIdx, endIdx, spriteLine, drawSize, colorKey, keepTime);
	}

	template <typename TActorState>
	void AddActorStateKeyFrames(int32 startIdx, int32 endIdx, int32 spriteLine, const Size& drawSize, uint32 colorKey, float keepTime)
	{
		auto foundIter = m_mapActorStateDynamicSprite.find(TActorState::s_id);
		if (foundIter == m_mapActorStateDynamicSprite.cend())
		{
			return;
		}

		const ActorLookAtDynamicSpriteTable& actorLookAtDynmaicSpriteTable = foundIter->second;
		for (int32 i = 0; i < TO_NUM(EActorLookAtType::Count); ++i)
		{
			// �ش�Ǵ� ��������Ʈ ��������
			const DynamicSpritePtr& spDynamicSprite = actorLookAtDynmaicSpriteTable[i];
			ASSERT_LOG(spDynamicSprite != nullptr);
			spDynamicSprite->AddKeyFrames(startIdx, endIdx, spriteLine, drawSize, colorKey, keepTime);
		}
	}

	template <typename TActorState>
	DynamicSpritePtr FindActorStateLookAtDynamicSprite(EActorLookAtType actorLookAtType) const
	{
		auto foundIter = m_mapActorStateDynamicSprite.find(TActorState::s_id);
		if (foundIter == m_mapActorStateDynamicSprite.cend())
		{
			return nullptr;
		}

		const ActorLookAtDynamicSpriteTable& actorLookAtDynmaicSpriteTable = foundIter->second;
		return (actorLookAtDynmaicSpriteTable[TO_NUM(actorLookAtType)]);
	}

	template <typename TActorState>
	void ChangeActorStateDynamicSprite()
	{
		DynamicSpriteComponent* pDynamicSpriteComponent = FindComponent<DynamicSpriteComponent>();
		ASSERT_LOG(pDynamicSpriteComponent != nullptr);

		const DynamicSpritePtr& spChangeActorStateDynamicSprite = FindActorStateLookAtDynamicSprite<TActorState>(m_lookAtType);
		pDynamicSpriteComponent->ApplyDynamicSprite(spChangeActorStateDynamicSprite);
	}

	template <typename TAnimationActorState>
	void ReserveNextPlayerState()
	{
		AnimationActorStatePtr spNextAnimationActorState = std::make_shared<TAnimationActorState>(this);
		m_animationActorStateChangeEvent.RegisterEventHandler(this, &AnimationActor::OnChangeAnimationActorState, spNextAnimationActorState);
	}

	// Update���� �����ϱ� ��� ��ȯ�ص� ��
	template <typename TAnimationActorState>
	void ImmediatelyChangePlayerState()
	{
		m_spAnimationActorState = std::make_shared<TAnimationActorState>(this);
	}

	template <typename TAnimationActorState>
	bool IsSameAnimationActorState()
	{
		return (std::dynamic_pointer_cast<TAnimationActorState>(m_spAnimationActorState) != nullptr);
	}

public:
	void ApplyMoveDirection(const Vec2d& vMoveDir);
	void LoadActorLookAtTexture(const std::string& strActorLookAtTexturePath);
	void LoadActorLookAtTexture(const std::string& strActorLookAtTexturePath, EActorLookAtType actorLookAtType);
	
	Position2d CalculateForwardCellPosition() const;

	void SetWorldTileMapActor(const std::shared_ptr<WorldTileMapActor>& spWorldTileMapActor) { m_spWorldTileMapActor = spWorldTileMapActor; }
	const std::shared_ptr<WorldTileMapActor>& GetWorldTileMapActor() const { return m_spWorldTileMapActor; }

private:
	void OnChangeAnimationActorState(const AnimationActorStatePtr& spAnimationActorState);

private:
	static ActorLookAtStringTable s_actorLookAtStringTable;

private:
	EActorLookAtType m_lookAtType = EActorLookAtType::Down;
	std::shared_ptr<WorldTileMapActor> m_spWorldTileMapActor = nullptr;

	ActorLookAtStringTable m_actorLookAtTexturePathTable;
	std::unordered_map<int32, std::array<DynamicSpritePtr, TO_NUM(EActorLookAtType::Count)>> m_mapActorStateDynamicSprite;

	AnimationActorStatePtr m_spAnimationActorState = nullptr;
	Event<const AnimationActorStatePtr& /* spNextAnimationActorState */> m_animationActorStateChangeEvent;
};