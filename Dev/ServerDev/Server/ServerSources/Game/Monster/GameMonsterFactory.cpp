// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "GameMonsterFactory.h"

DEFINE_SINGLETON(GameMonsterFactory);

GameMonsterPtr GameMonsterFactory::CreateRandomMonster(uint32 entityId)
{
	// 새로운 게임 플레이어에게 기본 정보 넣기
	Protocol::MonsterInfo newGameMonsterInfo;
	newGameMonsterInfo.set_monster_id(0); // 이게 랜덤

	Protocol::EntityInfo* pNewMonsterInfo = newGameMonsterInfo.mutable_monster_info();
	pNewMonsterInfo->set_entity_id(entityId);
	pNewMonsterInfo->set_entity_type(Protocol::EEntityType::Monster);
	pNewMonsterInfo->set_entity_look_at_dir(Protocol::EEntityLookAtDirection::Down);
	pNewMonsterInfo->set_entity_state(Protocol::EEntityState::Idle);
	pNewMonsterInfo->set_max_hp(20);
	pNewMonsterInfo->set_hp(20);
	pNewMonsterInfo->set_attack(5);
	//pNewMonsterInfo->set_attack(15); // 테스트용
	pNewMonsterInfo->set_defense(0);

	// 랜덤 셀 좌표
	const Position2d& randomCellPos = GameRoom::I()->GenerateRandomCellPosition();
	//pNewMonsterInfo->set_cell_pos_x(randomCellPos.x);
	//pNewMonsterInfo->set_cell_pos_y(randomCellPos.y);
	pNewMonsterInfo->set_cell_pos_x(6);
	pNewMonsterInfo->set_cell_pos_y(8);

	GameMonsterPtr spMonster = std::make_shared<GameMonster>();
	spMonster->Startup();
	spMonster->ApplyGameMonsterInfo(newGameMonsterInfo);

	return spMonster;
}