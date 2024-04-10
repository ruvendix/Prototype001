#pragma once

#include "functional"
#include "IDefault.h"
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

	static const char* s_szNullObjectType;
	static CollisionResponseInfo s_nullCollisionResponseInfo;

public:
	CollisionComponent();
	virtual ~CollisionComponent();

	virtual void Render() override;
	virtual void PostUpdate() override;

	void AddCollisionResponseInfo(const std::string& strObjectTypeTag, ECollisionResponseState collisionResponseState);
	bool TestIntersect(CollisionComponent::Ptr spTargetCollisionComponent);

	void ProcessHitEvent(CollisionComponent::Ptr spTargetCollisionComponent);
	void ProcessOverlapEvent(CollisionComponent::Ptr spTargetCollisionComponent);

	void ApplyCollider(ColliderBase::Ptr spColider);

	const CollisionResponseInfo& FindCollisionResponseInfo(const std::string& strObjectTypeTag);

	ColliderBase::Ptr GetCollider() const { return m_spCollider; }

	const std::string& GetObjectTypeTag() const { return m_strObjectTypeTag; }
	void SetObjectTypeTag(const std::string strObjectTypeTag) { m_strObjectTypeTag = strObjectTypeTag; }

	CollisionEnableOption& GetCollsionEnableOption() { return m_collsionEnableOption; }

	void SetHitCallback(CollisionCallback hitCallback) { m_hitCallback = hitCallback; }
	void SetOverlapCallback(CollisionCallback overlapCallback) { m_overlapCallback = overlapCallback; }

private:
	std::string m_strObjectTypeTag;
	ColliderBase::Ptr m_spCollider = nullptr;
	
	CollisionCallback m_hitCallback; // Block
	CollisionCallback m_overlapCallback; // Overlap

	CollisionEnableOption m_collsionEnableOption;
	std::vector<CollisionResponseInfo> m_collisionResponseInfos;
};