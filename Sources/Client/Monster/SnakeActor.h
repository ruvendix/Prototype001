// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class WorldTileMapActor;

// �ϴ��� Ŭ���̾�Ʈ�� �ִ� �� ���
class SnakeActor : public AnimationActor
{
	DECLARE_PIMPL;
	using Super = AnimationActor;

public:
	using Super::Super;
	virtual ~SnakeActor();

	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
	virtual void Cleanup() override;
};