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
	// 서버로부터 받은 이걸 클라이언트가 사용해야함
	std::shared_ptr<Protocol::S_GameCreatureInfo> m_spGameCreatureInfo;
};