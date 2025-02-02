// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class WorldTileMapActor;

// �ϴ��� Ŭ���̾�Ʈ�� �ִ� �� ���
class PlayerActor : public AnimationActor
{
	DECLARE_PIMPL;
	using Super = AnimationActor;

public:
	using Super::Super;
	virtual ~PlayerActor();

	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
	virtual void Cleanup() override;

public:
	void SetWorldTileMapActor(const std::shared_ptr<WorldTileMapActor>& spWorldTileMapActor) { m_spWorldTileMapActor = spWorldTileMapActor; }

private:
	void OnDirectionKeyHandler(const InputActionValue* pInputAction);
	void OnSpaceBarKeyHandler(const InputActionValue* pInputAction);

private:
	std::shared_ptr<WorldTileMapActor> m_spWorldTileMapActor = nullptr;
};