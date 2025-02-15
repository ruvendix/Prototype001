// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

#include "Engine/Resource/ResourceMananger.h"
#include "Engine/Resource/Sprite/DynamicSprite.h"
#include "AnimationActorState.h"

class WorldTileMapActor;

class AnimationActor : public CellActor
{
	using Super = CellActor;

public:
	using ActorLookAtStringTable = std::array<std::string, TO_NUM(EActorLookAtDirection::Count)>;
	using ActorLookAtDynamicSpriteTable = std::array<DynamicSpritePtr, TO_NUM(EActorLookAtDirection::Count)>;

	static const std::array<Position2d, TO_NUM(EActorLookAtDirection::Count)> g_lookAtForwardCellPosTable;

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
		for (int32 i = 0; i < TO_NUM(EActorLookAtDirection::Count); ++i)
		{
			std::string strDynamicSpriteName;
			MakeFormatString(strDynamicSpriteName, "%s%s", strPrefix.c_str(), g_actorLookAtStringTable[i].c_str());

			// 스프라이트 로딩인데 없으니 생성
			const DynamicSpritePtr& spDynamicSprite = ResourceMananger::I()->CreateDynamicSprite(strDynamicSpriteName);
			ASSERT_LOG(spDynamicSprite != nullptr);
			spDynamicSprite->SetLoopDynamicSprite(bLoopDynamicSprite);
			spDynamicSprite->FindAndSetTexture(m_actorLookAtDirTexturePathTable[i]);

			actorLookAtDynamicSpriteTable[i] = spDynamicSprite;
		}

		auto resultIter = m_mapActorStateDynamicSprite.insert(std::make_pair(TActorState::s_id, actorLookAtDynamicSpriteTable));
		ASSERT_LOG(resultIter.second == true);
	}

	template <typename TActorState>
	void AddActorStateKeyFrame(int32 startIdx, int32 endIdx, int32 spriteLine,
		const Size& drawSize, uint32 colorKey, float keepTime, EActorLookAtDirection actorLookAtDir)
	{
		auto foundIter = m_mapActorStateDynamicSprite.find(TActorState::s_id);
		if (foundIter == m_mapActorStateDynamicSprite.cend())
		{
			return;
		}

		// 해당되는 스프라이트 가져오기
		const ActorLookAtDynamicSpriteTable& actorLookAtDynmaicSpriteTable = foundIter->second;		
		const DynamicSpritePtr& spDynamicSprite = actorLookAtDynmaicSpriteTable[TO_NUM(actorLookAtDir)];
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
		for (int32 i = 0; i < TO_NUM(EActorLookAtDirection::Count); ++i)
		{
			// 해당되는 스프라이트 가져오기
			const DynamicSpritePtr& spDynamicSprite = actorLookAtDynmaicSpriteTable[i];
			ASSERT_LOG(spDynamicSprite != nullptr);
			spDynamicSprite->AddKeyFrames(startIdx, endIdx, spriteLine, drawSize, colorKey, keepTime);
		}
	}

	template <typename TActorState>
	DynamicSpritePtr FindActorStateLookAtDynamicSprite(EActorLookAtDirection actorLookAtDir) const
	{
		auto foundIter = m_mapActorStateDynamicSprite.find(TActorState::s_id);
		if (foundIter == m_mapActorStateDynamicSprite.cend())
		{
			return nullptr;
		}

		const ActorLookAtDynamicSpriteTable& actorLookAtDynmaicSpriteTable = foundIter->second;
		return (actorLookAtDynmaicSpriteTable[TO_NUM(actorLookAtDir)]);
	}

	template <typename TActorState>
	void ChangeActorStateDynamicSprite()
	{
		DynamicSpriteComponent* pDynamicSpriteComponent = FindComponent<DynamicSpriteComponent>();
		ASSERT_LOG(pDynamicSpriteComponent != nullptr);

		DynamicSpritePtr spChangeActorStateDynamicSprite = FindActorStateLookAtDynamicSprite<TActorState>(m_lookAtDir);
		if (spChangeActorStateDynamicSprite == nullptr)
		{
			spChangeActorStateDynamicSprite = FindActorStateLookAtDynamicSprite<AnimationActorIdleState>(m_lookAtDir);
		}

		pDynamicSpriteComponent->ApplyDynamicSprite(spChangeActorStateDynamicSprite);
	}

	template <typename TAnimationActorState>
	void ReserveChangeNextState()
	{
		AnimationActorStatePtr spNextAnimationActorState = std::make_shared<TAnimationActorState>(this);
		m_animationActorStateChangeEvent.RegisterEventHandler(this, &AnimationActor::OnChangeAnimationActorState, spNextAnimationActorState);
	}

	// Update보다 빠르니까 즉시 전환해도 됨
	template <typename TAnimationActorState>
	void ImmediatelyChangeState()
	{
		m_spAnimationActorState = std::make_shared<TAnimationActorState>(this);
		m_spAnimationActorState->Startup();
		DEFAULT_TRACE_LOG("애니메이션 액터 상태 변경! (즉시)");
	}

	template <typename TAnimationActorState>
	bool IsSameAnimationActorState() const
	{
		return (std::dynamic_pointer_cast<TAnimationActorState>(m_spAnimationActorState) != nullptr);
	}

public:
	void ApplyMoveDirectionToLookAtDirection(const Vec2d& vMoveDir);
	void LoadActorLookAtDirectionTexture(const std::string& strActorLookAtDirTexturePath);
	void LoadActorLookAtDirectionTexture(const std::string& strActorLookAtDirTexturePath, EActorLookAtDirection actorLookAtDir);
	
	Position2d CalculateForwardCellPosition() const;

	void SetWorldTileMapActor(const std::shared_ptr<WorldTileMapActor>& spWorldTileMapActor) { m_spWorldTileMapActor = spWorldTileMapActor; }
	const std::shared_ptr<WorldTileMapActor>& GetWorldTileMapActor() const { return m_spWorldTileMapActor; }

private:
	void OnChangeAnimationActorState(const AnimationActorStatePtr& spAnimationActorState);

private:
	static ActorLookAtStringTable g_actorLookAtStringTable;

private:
	EActorLookAtDirection m_lookAtDir = EActorLookAtDirection::Down;
	std::shared_ptr<WorldTileMapActor> m_spWorldTileMapActor = nullptr;

	ActorLookAtStringTable m_actorLookAtDirTexturePathTable;
	std::unordered_map<int32, std::array<DynamicSpritePtr, TO_NUM(EActorLookAtDirection::Count)>> m_mapActorStateDynamicSprite;

	AnimationActorStatePtr m_spAnimationActorState = nullptr;
	Event<const AnimationActorStatePtr& /* spNextAnimationActorState */> m_animationActorStateChangeEvent;
};