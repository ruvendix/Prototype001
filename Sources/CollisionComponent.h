#pragma once

#include "functional"
#include "IGameDefault.h"
#include "ComponentBase.h"
#include "ColliderBase.h"
#include "EnumBitFlag.h"
#include "CollisionResponseInfo.h"

enum ECollisionEnableOption
{
	None = 0,
	Query,
	Physics,
};

class CollisionComponent : public ComponentBase
{
public:
	DEFINE_SMART_PTR(CollisionComponent);

	using CollisionEnableOption = EnumBitFlag<ECollisionEnableOption>;
	using CollisionCallback = std::function<void(CollisionComponent::Ptr)>;

public:
	CollisionComponent();
	virtual ~CollisionComponent();

	virtual void Render() override;
	virtual bool PostUpdate() override;

	void AddCollisionResponse(ECollisionObjectType collisionObjectType, ECollisionResponseState collisionResponseState);
	ECollisionResponseState TestCollisionResponseBit(ECollisionObjectType collisionObjectType) const;

	bool TestIntersect(CollisionComponent::Ptr spTargetCollisionComponent);

	void ProcessHitEvent(CollisionComponent::Ptr spTargetCollisionComponent);
	void ProcessOverlapEvent(CollisionComponent::Ptr spTargetCollisionComponent);

	void ApplyCollider(ColliderBase::Ptr spColider);

	ColliderBase::Ptr GetCollider() const { return m_spCollider; }

	ECollisionObjectType GetCollisionObjectType() const { return m_collisionObjectType; }
	void SetCollisionObjectType(ECollisionObjectType collisionObjectType) { m_collisionObjectType = collisionObjectType; }

	CollisionEnableOption& GetCollsionEnableOption() { return m_collsionEnableOption; }

	void SetHitCallback(CollisionCallback hitCallback) { m_hitCallback = hitCallback; }
	void SetOverlapCallback(CollisionCallback overlapCallback) { m_overlapCallback = overlapCallback; }

private:
	ECollisionObjectType m_collisionObjectType = ECollisionObjectType::Count;
	ColliderBase::Ptr m_spCollider = nullptr;
	
	CollisionCallback m_hitCallback; // Block
	CollisionCallback m_overlapCallback; // Overlap

	CollisionEnableOption m_collsionEnableOption; // 이건 컬리전 반응과 별개임
	uint32 m_collisionResponseFlag = 0; // 컬리전 컴포넌트당 4바이트로 16개
};