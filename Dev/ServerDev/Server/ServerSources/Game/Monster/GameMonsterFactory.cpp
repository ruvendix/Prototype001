// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "GameMonsterFactory.h"

DEFINE_SINGLETON(GameMonsterFactory);

GameMonsterPtr GameMonsterFactory::CreateRandomMonster(uint32 gameEntityId)
{
	// ���ο� ���� �÷��̾�� �⺻ ���� �ֱ�
	Protocol::GameMonsterInfo newGameMonsterInfo;
	newGameMonsterInfo.set_monster_id(0); // �̰� ����

	Protocol::GameEntityInfo* pNewGameEntityInfo = newGameMonsterInfo.mutable_entity_info();
	pNewGameEntityInfo->set_entity_id(gameEntityId);
	pNewGameEntityInfo->set_entity_type(Protocol::EGameEntityType::Monster);
	pNewGameEntityInfo->set_entitye_look_at_dir(Protocol::EGameEntityLookAtDir::Down);
	pNewGameEntityInfo->set_entity_state(Protocol::EGameEntityState::Idle);

	// ���� �� ��ǥ
	const Position2d& randomCellPos = GameRoom::I()->GenerateRandomCellPosition();
	//pNewGameEntityInfo->set_cell_pos_x(randomCellPos.x);
	//pNewGameEntityInfo->set_cell_pos_y(randomCellPos.y);
	pNewGameEntityInfo->set_cell_pos_x(6);
	pNewGameEntityInfo->set_cell_pos_y(8);

	GameMonsterPtr spGameMonster = std::make_shared<GameMonster>();
	spGameMonster->Startup();
	spGameMonster->ApplyGameMonsterInfo(newGameMonsterInfo);

	return spGameMonster;
}