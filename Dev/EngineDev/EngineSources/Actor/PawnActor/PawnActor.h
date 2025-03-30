// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

#include "EngineSources/Resource/ResourceMananger.h"
#include "EngineSources/Resource/Sprite/DynamicSprite.h"
#include "PawnActorState.h"

class WorldTileMapActor;

class PawnActor : public CellActor
{
	using Super = CellActor;

public:
	using ActorLookAtStringTable = std::array<std::string, TO_NUM(EActorLookAtDirection::Count)>;
	using ActorLookAtDynamicSpriteTable = std::array<DynamicSpritePtr, TO_NUM(EActorLookAtDirection::Count)>;

	static const std::array<Position2d, TO_NUM(EActorLookAtDirection::Count)> g_lookAtForwardCellPosTable;
	static const std::array<Position2d, TO_NUM(EActorLookAtDirection::Count)> g_lookAtBackwardCellPosTable;

	static const std::array<EActorLookAtDirection, TO_NUM(EActorLookAtDirection::Count)> g_oppositeLookAtDirTable;

public:
	using Super::Super;

	PawnActor(const PawnActor& src);
	virtual ~PawnActor();

public:
	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;

	virtual void ProcessDamaged(const std::shared_ptr<PawnActor>& spAttacker);
	virtual void InitializeActorStateTable();

public:
	template <typename TPawnActorState>
	void CreateActorStateLookAtDynamicSprites(const std::string& strPrefix, bool bLoopDynamicSprite)
	{
		if (m_vecActorStateDynamicSpriteTable.empty() == true)
		{
			m_vecActorStateDynamicSpriteTable.resize(m_vecActorStateTable.size());
		}
		ASEERT_VALIDATE_INDEX(TPawnActorState::s_id, m_vecActorStateDynamicSpriteTable.size());

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

		m_vecActorStateDynamicSpriteTable[TPawnActorState::s_id] = std::move(actorLookAtDynamicSpriteTable);
	}

	template <typename TPawnActorState>
	void AddActorStateKeyFrame(int32 startIdx, int32 endIdx, int32 spriteLine, const Size& drawSize, uint32 colorKey, float keepTime, EActorLookAtDirection actorLookAtDir)
	{
		ASEERT_VALIDATE_INDEX(TPawnActorState::s_id, m_vecActorStateDynamicSpriteTable.size());

		// 해당되는 스프라이트 가져오기
		const ActorLookAtDynamicSpriteTable& actorLookAtDynmaicSpriteTable = m_vecActorStateDynamicSpriteTable[TPawnActorState::s_id];
		const DynamicSpritePtr& spDynamicSprite = actorLookAtDynmaicSpriteTable[TO_NUM(actorLookAtDir)];
		ASSERT_LOG(spDynamicSprite != nullptr);
		spDynamicSprite->AddKeyFrames(startIdx, endIdx, spriteLine, drawSize, colorKey, keepTime);
	}

	template <typename TPawnActorState>
	void AddActorStateKeyFrames(int32 startIdx, int32 endIdx, int32 spriteLine, const Size& drawSize, uint32 colorKey, float keepTime)
	{
		ASEERT_VALIDATE_INDEX(TPawnActorState::s_id, m_vecActorStateDynamicSpriteTable.size());
		const ActorLookAtDynamicSpriteTable& actorLookAtDynmaicSpriteTable = m_vecActorStateDynamicSpriteTable[TPawnActorState::s_id];

		for (int32 i = 0; i < TO_NUM(EActorLookAtDirection::Count); ++i)
		{
			// 해당되는 스프라이트 가져오기
			const DynamicSpritePtr& spDynamicSprite = actorLookAtDynmaicSpriteTable[i];
			ASSERT_LOG(spDynamicSprite != nullptr);
			spDynamicSprite->AddKeyFrames(startIdx, endIdx, spriteLine, drawSize, colorKey, keepTime);
		}
	}

	template <typename TPawnActorState>
	DynamicSpritePtr FindActorStateLookAtDynamicSprite(EActorLookAtDirection actorLookAtDir) const
	{
		ASEERT_VALIDATE_INDEX(TPawnActorState::s_id, m_vecActorStateDynamicSpriteTable.size());
		const ActorLookAtDynamicSpriteTable& actorLookAtDynmaicSpriteTable = m_vecActorStateDynamicSpriteTable[TPawnActorState::s_id];
		return (actorLookAtDynmaicSpriteTable[TO_NUM(actorLookAtDir)]);
	}

	template <typename TPawnActorState>
	void ChangeActorStateDynamicSprite()
	{
		DynamicSpriteComponent* pDynamicSpriteComponent = GetComponent<DynamicSpriteComponent>();
		ASSERT_LOG(pDynamicSpriteComponent != nullptr);

		DynamicSpritePtr spChangeActorStateDynamicSprite = FindActorStateLookAtDynamicSprite<TPawnActorState>(m_lookAtDir);
		if (spChangeActorStateDynamicSprite != nullptr)
		{
			pDynamicSpriteComponent->ApplyDynamicSprite(spChangeActorStateDynamicSprite);
		}
	}

	template <typename TPawnActorState>
	void ReserveChangeNextState()
	{
		m_pawnActorStateChangeEvent.RegisterEventHandler(this, &PawnActor::OnChangePawnActorState, std::move(TPawnActorState::s_id));
	}

	template <typename TPawnActorState>
	void RegisterActorState()
	{
		ASEERT_VALIDATE_INDEX(TPawnActorState::s_id, m_vecActorStateTable.size());
		m_vecActorStateTable[TPawnActorState::s_id] = std::make_shared<TPawnActorState>(this);
		DEFAULT_TRACE_LOG("애니메이션 액터 상태 변경! (즉시)");
	}

	// Update보다 빠르니까 즉시 전환해도 됨
	template <typename TPawnActorState>
	void ImmediatelyChangeState()
	{
		ASEERT_VALIDATE_INDEX(TPawnActorState::s_id, m_vecActorStateTable.size());
		m_currentActorStateIdx = TPawnActorState::s_id;

		const PawnActorStatePtr& spCurrentActorState = GetCurrentPawnActorState();
		if (spCurrentActorState != nullptr)
		{
			spCurrentActorState->Startup();
		}

		DEFAULT_TRACE_LOG("애니메이션 액터 상태 변경! (즉시)");
	}

	template <typename TPawnActorState>
	bool IsSamePawnActorState() const
	{
		return (GetCurrentExactPawnActorState<TPawnActorState>() != nullptr);
	}

	template <typename TPawnActorState>
	std::shared_ptr<TPawnActorState> GetCurrentExactPawnActorState() const
	{
		ASEERT_VALIDATE_INDEX(m_currentActorStateIdx, m_vecActorStateTable.size());
		return (std::dynamic_pointer_cast<TPawnActorState>(GetCurrentPawnActorState()));
	}

public:
	bool ApplyLookAtDirectionSprite();
	bool ApplyLookAtDirectionSpriteOnDefaultState();

	DynamicSpritePtr FindCurrentActorStateLookAtDynamicSprite(EActorLookAtDirection actorLookAtDir) const;

	void ApplyMoveDirectionToLookAtDirection(const Vector2d& vMoveDir);
	void LoadActorLookAtDirectionTexture(const std::string& strActorLookAtDirTexturePath);
	void LoadActorLookAtDirectionTexture(const std::string& strActorLookAtDirTexturePath, EActorLookAtDirection actorLookAtDir);

	void ImmediatelyChangeState(uint32 actorStateId);
	void ChangeActorDynamicSpriteByExternal(const DynamicSpritePtr& spChangedDynamicSprite);

	Position2d CalculateForwardCellPosition() const;
	Position2d CalculateBackwardCellPosition() const;

	Vector2d CalculateBackwardDirection() const;

	bool CheckPossibleKnockback(EActorLookAtDirection srcLookAtDir) const;

	void SetWorldTileMapActor(const std::shared_ptr<WorldTileMapActor>& spWorldTileMapActor) { m_spWorldTileMapActor = spWorldTileMapActor; }
	const std::shared_ptr<WorldTileMapActor>& GetWorldTileMapActor() const { return m_spWorldTileMapActor; }

	void SetActorLookAtDirection(EActorLookAtDirection lookAtDir) { m_lookAtDir = lookAtDir; }
	EActorLookAtDirection GetActorLookAtDirection() const { return m_lookAtDir; }

	const PawnActorStatePtr& GetPawnActorState(uint32 actorStateId) const
	{
		ASEERT_VALIDATE_INDEX(actorStateId, m_vecActorStateTable.size());
		return m_vecActorStateTable[actorStateId];
	}

	const PawnActorStatePtr& GetCurrentPawnActorState() const
	{
		ASEERT_VALIDATE_INDEX(m_currentActorStateIdx, m_vecActorStateTable.size());
		return m_vecActorStateTable[m_currentActorStateIdx];
	};

private:
	void OnChangePawnActorState(uint32 nextActorStateIdx);

private:
	static ActorLookAtStringTable g_actorLookAtStringTable;

private:
	EActorLookAtDirection m_lookAtDir = EActorLookAtDirection::Count;
	std::shared_ptr<WorldTileMapActor> m_spWorldTileMapActor = nullptr;

	ActorLookAtStringTable m_actorLookAtDirTexturePathTable;
	std::vector<ActorLookAtDynamicSpriteTable> m_vecActorStateDynamicSpriteTable;

	uint32 m_currentActorStateIdx = PawnActorIdleState::s_id;
	std::vector<PawnActorStatePtr> m_vecActorStateTable;
	Event<uint32 /* nextActorStateIdx */> m_pawnActorStateChangeEvent;
};