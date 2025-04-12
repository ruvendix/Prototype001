// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "WeaponActor.h"

#include "EngineSources/Actor/PawnActor/PawnActorState.h"

WeaponActor::WeaponActor(const WeaponActor& srcActor) :
	Super(srcActor)
{
	m_projectileId = srcActor.m_projectileId;
}

WeaponActor::~WeaponActor()
{

}

void WeaponActor::Startup()
{
	Super::Startup();
}

bool WeaponActor::Update(float deltaSeconds)
{
	if (Super::Update(deltaSeconds) == false)
	{
		return false;
	}

	return true;
}

void WeaponActor::Cleanup()
{
	Super::Cleanup();
}

ActorPtr WeaponActor::CreateClone()
{
	return (std::make_shared<WeaponActor>(*this));
}

void WeaponActor::ApplyDynamicSpriteToOwner()
{
	const std::shared_ptr<PawnActor>& spWeaponOwner = m_spWeaponOwner.lock();
	ASSERT_LOG_RETURN(spWeaponOwner != nullptr);

	const DynamicSpritePtr& spChangedWeaponDynamicSprite = FindActorStateLookAtDynamicSprite<PawnActorIdleState>(spWeaponOwner->GetActorLookAtDirection());
	spWeaponOwner->ChangeActorDynamicSpriteByExternal(spChangedWeaponDynamicSprite);
}

bool WeaponActor::TryCreateProjectile() const
{
	if (m_projectileId == HAS_NOT_PROJECTILE)
	{
		return false;
	}

	// 이번 패킷은 위치, 방향, 아이디 요거 3개만 서버로 전달
	const ActionableActorPtr& spWeaponOwner = GetWeaponOwner();
	const Position2d& spawnCellPos = spWeaponOwner->CalculateForwardCellPosition();
	const Vector2d& vMoveDir = spWeaponOwner->CalculateMoveDirectionByCellPosition(spawnCellPos);

	const RxSendBufferPtr& spSendCreateProjectilePacket = ClientPacketHandler::I()->MakeCreateProjectilePacket(spawnCellPos, vMoveDir, m_projectileId);
	NetworkManager::I()->SendPacket(spSendCreateProjectilePacket);

	return true;
}