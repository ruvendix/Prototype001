// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

class CreatureActor : public PawnActor
{
	using Super = PawnActor;

public:
	using Super::Super;
	virtual ~CreatureActor();

	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
	virtual void Cleanup() override;

private:
	// �����κ��� ���� �̰� Ŭ���̾�Ʈ�� ����ؾ���
	std::shared_ptr<Protocol::S_GameCreatureInfo> m_spGameCreatureInfo;
};