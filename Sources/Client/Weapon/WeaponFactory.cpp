// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "WeaponFactory.h"

#include "WeaponComponent/WeaponSequenceAttackComponent.h"

DEFINE_SINGLETON(WeaponFactory);

void WeaponFactory::Startup()
{
#pragma region 검
	std::shared_ptr<WeaponActor> spWeaponSword = std::make_shared<WeaponActor>();
	spWeaponSword->Startup();
	spWeaponSword->AddComponent<WeaponSequenceAttackComponent>();

	// 이걸 확실하게 맞추려면 인덱스마다 넣어야함
	spWeaponSword->LoadActorLookAtDirectionTexture("Assets/Texture/Player/PlayerLeft.bmp", EActorLookAtDirection::Left);
	spWeaponSword->LoadActorLookAtDirectionTexture("Assets/Texture/Player/PlayerRight.bmp", EActorLookAtDirection::Right);
	spWeaponSword->LoadActorLookAtDirectionTexture("Assets/Texture/Player/PlayerDown.bmp", EActorLookAtDirection::Down);
	spWeaponSword->LoadActorLookAtDirectionTexture("Assets/Texture/Player/PlayerUp.bmp", EActorLookAtDirection::Up);

	// 플레이어는 공격해도 무기는 Idle 그대로
	spWeaponSword->CreateActorStateLookAtDynamicSprites<PawnActorIdleState>("WeaponSwordIdle", true);
	spWeaponSword->AddActorStateKeyFrames<PawnActorIdleState>(0, 7, 3, Size{ 200, 200 }, RGB(128, 128, 128), 0.06f);

	auto insertedIter = m_mapWeaponPrototype.insert(std::make_pair(0, spWeaponSword));
	ASSERT_LOG_RETURN(insertedIter.second == true);
#pragma endregion

#pragma region 방패
	std::shared_ptr<WeaponActor> WeaponShield = std::make_shared<WeaponActor>();
	WeaponShield->Startup();

	// 이걸 확실하게 맞추려면 인덱스마다 넣어야함
	WeaponShield->LoadActorLookAtDirectionTexture("Assets/Texture/Player/PlayerLeft.bmp", EActorLookAtDirection::Left);
	WeaponShield->LoadActorLookAtDirectionTexture("Assets/Texture/Player/PlayerRight.bmp", EActorLookAtDirection::Right);
	WeaponShield->LoadActorLookAtDirectionTexture("Assets/Texture/Player/PlayerDown.bmp", EActorLookAtDirection::Down);
	WeaponShield->LoadActorLookAtDirectionTexture("Assets/Texture/Player/PlayerUp.bmp", EActorLookAtDirection::Up);

	// 플레이어는 공격해도 무기는 Idle 그대로
	WeaponShield->CreateActorStateLookAtDynamicSprites<PawnActorIdleState>("WeaponShieldIdle", false);
	WeaponShield->AddActorStateKeyFrames<PawnActorIdleState>(0, 5, 4, Size{ 200, 200 }, RGB(128, 128, 128), 0.01f);

	insertedIter = m_mapWeaponPrototype.insert(std::make_pair(1, WeaponShield));
	ASSERT_LOG_RETURN(insertedIter.second == true);
#pragma endregion

#pragma region 활
	std::shared_ptr<WeaponActor> spWeaponBow = std::make_shared<WeaponActor>();
	spWeaponBow->Startup();

	// 이걸 확실하게 맞추려면 인덱스마다 넣어야함
	spWeaponBow->LoadActorLookAtDirectionTexture("Assets/Texture/Player/PlayerLeft.bmp", EActorLookAtDirection::Left);
	spWeaponBow->LoadActorLookAtDirectionTexture("Assets/Texture/Player/PlayerRight.bmp", EActorLookAtDirection::Right);
	spWeaponBow->LoadActorLookAtDirectionTexture("Assets/Texture/Player/PlayerDown.bmp", EActorLookAtDirection::Down);
	spWeaponBow->LoadActorLookAtDirectionTexture("Assets/Texture/Player/PlayerUp.bmp", EActorLookAtDirection::Up);

	// 플레이어는 공격해도 무기는 Idle 그대로
	spWeaponBow->CreateActorStateLookAtDynamicSprites<PawnActorIdleState>("BowIdle", true);
	spWeaponBow->AddActorStateKeyFrames<PawnActorIdleState>(0, 7, 5, Size{ 200, 200 }, RGB(128, 128, 128), 0.06f);

	insertedIter = m_mapWeaponPrototype.insert(std::make_pair(2, spWeaponBow));
	ASSERT_LOG_RETURN(insertedIter.second == true);
#pragma endregion
}

std::shared_ptr<WeaponActor> WeaponFactory::CreateWeaponActor(int32 weaponId)
{
	const std::shared_ptr<WeaponActor>& spFoundPrototypeWeaponActor = FindWeaponActor(weaponId);
	const WeaponActor& foundPrototypeWeaponActor = *(spFoundPrototypeWeaponActor.get());

	// 복사 생성자 호출
	std::shared_ptr<WeaponActor> spNewWeaponActor = std::make_shared<WeaponActor>(foundPrototypeWeaponActor);
	return spNewWeaponActor;
}

std::shared_ptr<WeaponActor> WeaponFactory::FindWeaponActor(int32 weaponId) const
{
	auto foundIter = m_mapWeaponPrototype.find(weaponId);
	if (foundIter == m_mapWeaponPrototype.cend())
	{
		return nullptr;
	}

	return (foundIter->second);
}