// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "WeaponFactory.h"

#include "WeaponComponent/WeaponSequenceAttackComponent.h"

DEFINE_SINGLETON(WeaponFactory);

class WeaponFactory::Pimpl
{
	DEFINE_PIMPL(WeaponFactory)

public:
	void InitializeWeaponPrototypes();
	void InitializeProjectilePrototypes();
};

void WeaponFactory::Pimpl::InitializeWeaponPrototypes()
{
	std::unordered_map<int32, std::shared_ptr<WeaponActor>>& refMapWeaonPrototype = m_pOwner->m_mapPrototypeWeapon;

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

	auto insertedIter = refMapWeaonPrototype.insert(std::make_pair(0, spWeaponSword));
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

	insertedIter = refMapWeaonPrototype.insert(std::make_pair(1, WeaponShield));
	ASSERT_LOG_RETURN(insertedIter.second == true);
#pragma endregion

#pragma region 활
	std::shared_ptr<WeaponActor> spWeaponBow = std::make_shared<WeaponActor>();
	spWeaponBow->Startup();
	spWeaponBow->SetProjectileId(0);
	spWeaponBow->AddComponent<WeaponSequenceAttackComponent>();

	// 이걸 확실하게 맞추려면 인덱스마다 넣어야함
	spWeaponBow->LoadActorLookAtDirectionTexture("Assets/Texture/Player/PlayerLeft.bmp", EActorLookAtDirection::Left);
	spWeaponBow->LoadActorLookAtDirectionTexture("Assets/Texture/Player/PlayerRight.bmp", EActorLookAtDirection::Right);
	spWeaponBow->LoadActorLookAtDirectionTexture("Assets/Texture/Player/PlayerDown.bmp", EActorLookAtDirection::Down);
	spWeaponBow->LoadActorLookAtDirectionTexture("Assets/Texture/Player/PlayerUp.bmp", EActorLookAtDirection::Up);

	// 플레이어는 공격해도 무기는 Idle 그대로
	spWeaponBow->CreateActorStateLookAtDynamicSprites<PawnActorIdleState>("BowIdle", true);
	spWeaponBow->AddActorStateKeyFrames<PawnActorIdleState>(0, 7, 5, Size{ 200, 200 }, RGB(128, 128, 128), 0.06f);

	insertedIter = refMapWeaonPrototype.insert(std::make_pair(2, spWeaponBow));
	ASSERT_LOG_RETURN(insertedIter.second == true);
#pragma endregion
}

void WeaponFactory::Pimpl::InitializeProjectilePrototypes()
{
	std::unordered_map<int32, std::shared_ptr<ProjectileActor>>& refMapPrototypeProjectile = m_pOwner->m_mapPrototypeProjectile;

#pragma region 화살
	std::shared_ptr<ProjectileActor> spArrow = std::make_shared<ProjectileActor>();
	spArrow->Startup();
	//spArrow->AddComponent<WeaponSequenceAttackComponent>();

	// 이걸 확실하게 맞추려면 인덱스마다 넣어야함
	spArrow->LoadActorLookAtDirectionTexture("Assets/Texture/Item/Arrow.bmp");

	// 플레이어는 공격해도 무기는 Idle 그대로
	spArrow->CreateActorStateLookAtDynamicSprites<PawnActorIdleState>("ArrowIdle", false);
	spArrow->AddActorStateKeyFrame<PawnActorIdleState>(0, 0, 0, Size{ 100, 100 }, RGB(128, 128, 128), 0.0f, EActorLookAtDirection::Down);
	spArrow->AddActorStateKeyFrame<PawnActorIdleState>(0, 0, 2, Size{ 100, 100 }, RGB(128, 128, 128), 0.0f, EActorLookAtDirection::Right);
	spArrow->AddActorStateKeyFrame<PawnActorIdleState>(0, 0, 1, Size{ 100, 100 }, RGB(128, 128, 128), 0.0f, EActorLookAtDirection::Left);
	spArrow->AddActorStateKeyFrame<PawnActorIdleState>(0, 0, 3, Size{ 100, 100 }, RGB(128, 128, 128), 0.0f, EActorLookAtDirection::Up);

	// 기본 스프라이트 설정
	spArrow->ChangeActorStateDynamicSprite<PawnActorIdleState>();

	auto insertedIter = refMapPrototypeProjectile.insert(std::make_pair(0, spArrow));
	ASSERT_LOG_RETURN(insertedIter.second == true);
#pragma endregion
}

void WeaponFactory::Startup()
{
	ALLOC_PIMPL;

	m_spPimpl->InitializeWeaponPrototypes();
	m_spPimpl->InitializeProjectilePrototypes();
}

bool WeaponFactory::Update(float deltaSeconds)
{
	m_createProjectileActorEvent.ExcuteIfBound();
	return true;
}

std::shared_ptr<WeaponActor> WeaponFactory::CreateWeaponActor(int32 weaponId)
{
	const WeaponActor* pFoundPrototypeWeaponActor = FindWeaponActorRawPointer(weaponId);
	if (pFoundPrototypeWeaponActor == nullptr)
	{
		return nullptr;
	}

	// 복사 생성자 호출
	std::shared_ptr<WeaponActor> spNewWeaponActor = std::make_shared<WeaponActor>(*pFoundPrototypeWeaponActor);
	return spNewWeaponActor;
}

const WeaponActor* WeaponFactory::FindWeaponActorRawPointer(int32 weaponId) const
{
	auto foundIter = m_mapPrototypeWeapon.find(weaponId);
	if (foundIter == m_mapPrototypeWeapon.cend())
	{
		return nullptr;
	}

	return (foundIter->second.get());
}

std::shared_ptr<ProjectileActor> WeaponFactory::CreateProjectileActor(int32 projectileId)
{
	const ProjectileActor* pFoundPrototypeProjectileActor = FindProjectileActorRawPointer(projectileId);
	if (pFoundPrototypeProjectileActor == nullptr)
	{
		return nullptr;
	}

	// 복사 생성자 호출
	std::shared_ptr<ProjectileActor> spNewProjectileActor = std::make_shared<ProjectileActor>(*pFoundPrototypeProjectileActor);
	return spNewProjectileActor;
}

const ProjectileActor* WeaponFactory::FindProjectileActorRawPointer(int32 projectileId) const
{
	const std::shared_ptr<ProjectileActor>& spFoundProjectileActor = FindProjectileActor(projectileId);
	if (spFoundProjectileActor == nullptr)
	{
		return nullptr;
	}

	return (spFoundProjectileActor.get());
}

const std::shared_ptr<ProjectileActor>& WeaponFactory::FindProjectileActor(int32 projectileId) const
{
	auto foundIter = m_mapPrototypeProjectile.find(projectileId);
	ASSERT_LOG(foundIter != m_mapPrototypeProjectile.cend());
	return (foundIter->second);
}

void WeaponFactory::ReserveCreateProjectileActor(const ProjectileSpawnInfo& projectileSpawnInfo)
{
	m_createProjectileActorEvent.RegisterEventHandler(this, &WeaponFactory::OnCreateProjectileActor, projectileSpawnInfo);
}

void WeaponFactory::OnCreateProjectileActor(const ProjectileSpawnInfo& projectileSpawnInfo)
{
	Scene* pCurrentScene = SceneManager::I()->GetCurrentScene();
	ASSERT_LOG_RETURN(pCurrentScene != nullptr);

	const std::shared_ptr<ProjectileActor>& spFoundProjectile = FindProjectileActor(projectileSpawnInfo.projectileId);
	if (spFoundProjectile == nullptr)
	{
		return;
	}

	const std::shared_ptr<ProjectileActor>& spCreatedProjectile = pCurrentScene->CreateCloneActorToScene(spFoundProjectile);
	spCreatedProjectile->SpawnProjectile(projectileSpawnInfo);
	DEFAULT_TRACE_LOG("투사체 생성!");
}