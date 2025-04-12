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
	std::unordered_map<int32, WeaponActorPtr>& refMapWeaonPrototype = m_pOwner->m_mapPrototypeWeapon;

#pragma region ��
	WeaponActorPtr spWeaponSword = std::make_shared<WeaponActor>();
	spWeaponSword->Startup();
	spWeaponSword->AddComponent<WeaponSequenceAttackComponent>();

	// �̰� Ȯ���ϰ� ���߷��� �ε������� �־����
	spWeaponSword->LoadActorLookAtDirectionTexture("Assets/Texture/Player/PlayerLeft.bmp", EActorLookAtDirection::Left);
	spWeaponSword->LoadActorLookAtDirectionTexture("Assets/Texture/Player/PlayerRight.bmp", EActorLookAtDirection::Right);
	spWeaponSword->LoadActorLookAtDirectionTexture("Assets/Texture/Player/PlayerDown.bmp", EActorLookAtDirection::Down);
	spWeaponSword->LoadActorLookAtDirectionTexture("Assets/Texture/Player/PlayerUp.bmp", EActorLookAtDirection::Up);

	// �÷��̾�� �����ص� ����� Idle �״��
	spWeaponSword->CreateActorStateLookAtDynamicSprites<PawnActorIdleState>("WeaponSwordIdle", true);
	spWeaponSword->AddActorStateKeyFrames<PawnActorIdleState>(0, 7, 3, Size{ 200, 200 }, RGB(128, 128, 128), 0.06f);

	auto insertedIter = refMapWeaonPrototype.insert(std::make_pair(0, spWeaponSword));
	ASSERT_LOG_RETURN(insertedIter.second == true);
#pragma endregion

#pragma region ����
	WeaponActorPtr WeaponShield = std::make_shared<WeaponActor>();
	WeaponShield->Startup();

	// �̰� Ȯ���ϰ� ���߷��� �ε������� �־����
	WeaponShield->LoadActorLookAtDirectionTexture("Assets/Texture/Player/PlayerLeft.bmp", EActorLookAtDirection::Left);
	WeaponShield->LoadActorLookAtDirectionTexture("Assets/Texture/Player/PlayerRight.bmp", EActorLookAtDirection::Right);
	WeaponShield->LoadActorLookAtDirectionTexture("Assets/Texture/Player/PlayerDown.bmp", EActorLookAtDirection::Down);
	WeaponShield->LoadActorLookAtDirectionTexture("Assets/Texture/Player/PlayerUp.bmp", EActorLookAtDirection::Up);

	// �÷��̾�� �����ص� ����� Idle �״��
	WeaponShield->CreateActorStateLookAtDynamicSprites<PawnActorIdleState>("WeaponShieldIdle", false);
	WeaponShield->AddActorStateKeyFrames<PawnActorIdleState>(0, 5, 4, Size{ 200, 200 }, RGB(128, 128, 128), 0.01f);

	insertedIter = refMapWeaonPrototype.insert(std::make_pair(1, WeaponShield));
	ASSERT_LOG_RETURN(insertedIter.second == true);
#pragma endregion

#pragma region Ȱ
	WeaponActorPtr spWeaponBow = std::make_shared<WeaponActor>();
	spWeaponBow->Startup();
	spWeaponBow->SetProjectileId(0);
	spWeaponBow->AddComponent<WeaponSequenceAttackComponent>();

	// �̰� Ȯ���ϰ� ���߷��� �ε������� �־����
	spWeaponBow->LoadActorLookAtDirectionTexture("Assets/Texture/Player/PlayerLeft.bmp", EActorLookAtDirection::Left);
	spWeaponBow->LoadActorLookAtDirectionTexture("Assets/Texture/Player/PlayerRight.bmp", EActorLookAtDirection::Right);
	spWeaponBow->LoadActorLookAtDirectionTexture("Assets/Texture/Player/PlayerDown.bmp", EActorLookAtDirection::Down);
	spWeaponBow->LoadActorLookAtDirectionTexture("Assets/Texture/Player/PlayerUp.bmp", EActorLookAtDirection::Up);

	// �÷��̾�� �����ص� ����� Idle �״��
	spWeaponBow->CreateActorStateLookAtDynamicSprites<PawnActorIdleState>("BowIdle", true);
	spWeaponBow->AddActorStateKeyFrames<PawnActorIdleState>(0, 7, 5, Size{ 200, 200 }, RGB(128, 128, 128), 0.06f);

	insertedIter = refMapWeaonPrototype.insert(std::make_pair(2, spWeaponBow));
	ASSERT_LOG_RETURN(insertedIter.second == true);
#pragma endregion
}

void WeaponFactory::Pimpl::InitializeProjectilePrototypes()
{
	std::unordered_map<int32, std::shared_ptr<ProjectileActor>>& refMapPrototypeProjectile = m_pOwner->m_mapPrototypeProjectile;

#pragma region ȭ��
	std::shared_ptr<ProjectileActor> spArrow = std::make_shared<ProjectileActor>();
	spArrow->Startup();
	//spArrow->AddComponent<WeaponSequenceAttackComponent>();

	// �̰� Ȯ���ϰ� ���߷��� �ε������� �־����
	spArrow->LoadActorLookAtDirectionTexture("Assets/Texture/Item/Arrow.bmp");

	// �÷��̾�� �����ص� ����� Idle �״��
	spArrow->CreateActorStateLookAtDynamicSprites<PawnActorIdleState>("ArrowIdle", false);
	spArrow->AddActorStateKeyFrame<PawnActorIdleState>(0, 0, 0, Size{ 100, 100 }, RGB(128, 128, 128), 0.0f, EActorLookAtDirection::Down);
	spArrow->AddActorStateKeyFrame<PawnActorIdleState>(0, 0, 2, Size{ 100, 100 }, RGB(128, 128, 128), 0.0f, EActorLookAtDirection::Right);
	spArrow->AddActorStateKeyFrame<PawnActorIdleState>(0, 0, 1, Size{ 100, 100 }, RGB(128, 128, 128), 0.0f, EActorLookAtDirection::Left);
	spArrow->AddActorStateKeyFrame<PawnActorIdleState>(0, 0, 3, Size{ 100, 100 }, RGB(128, 128, 128), 0.0f, EActorLookAtDirection::Up);

	// �⺻ ��������Ʈ ����
	spArrow->SetActorLookAtDirection(EActorLookAtDirection::Down);
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

WeaponActorPtr WeaponFactory::CreateWeaponActor(uint32 weaponId) const
{
	const WeaponActor* pFoundPrototypeWeaponActor = FindWeaponActorRawPointer(weaponId);
	if (pFoundPrototypeWeaponActor == nullptr)
	{
		return nullptr;
	}

	// ���� ������ ȣ��
	WeaponActorPtr spNewWeaponActor = std::make_shared<WeaponActor>(*pFoundPrototypeWeaponActor);
	spNewWeaponActor->InitializeActorStateTable();
	return spNewWeaponActor;
}

const WeaponActor* WeaponFactory::FindWeaponActorRawPointer(uint32 weaponId) const
{
	auto foundIter = m_mapPrototypeWeapon.find(weaponId);
	if (foundIter == m_mapPrototypeWeapon.cend())
	{
		return nullptr;
	}

	return (foundIter->second.get());
}

std::shared_ptr<ProjectileActor> WeaponFactory::CreateProjectileActor(uint32 projectileId) const
{
	const ProjectileActor* pFoundPrototypeProjectileActor = FindProjectileActorRawPointer(projectileId);
	if (pFoundPrototypeProjectileActor == nullptr)
	{
		return nullptr;
	}

	// ���� ������ ȣ��
	std::shared_ptr<ProjectileActor> spNewProjectileActor = std::make_shared<ProjectileActor>(*pFoundPrototypeProjectileActor);
	return spNewProjectileActor;
}

const ProjectileActor* WeaponFactory::FindProjectileActorRawPointer(uint32 projectileId) const
{
	const std::shared_ptr<ProjectileActor>& spFoundProjectileActor = FindProjectileActor(projectileId);
	if (spFoundProjectileActor == nullptr)
	{
		return nullptr;
	}

	return (spFoundProjectileActor.get());
}

const std::shared_ptr<ProjectileActor>& WeaponFactory::FindProjectileActor(uint32 projectileId) const
{
	auto foundIter = m_mapPrototypeProjectile.find(projectileId);
	ASSERT_LOG(foundIter != m_mapPrototypeProjectile.cend());
	return (foundIter->second);
}