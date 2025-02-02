// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

class WorldTileMapActor;

class EditorActor : public CellActor
{
	using Super = CellActor;

public:
	using Super::Super;
	virtual ~EditorActor();

	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
	virtual void Cleanup() override;

public:
	void SetWorldTileMapActor(const std::shared_ptr<WorldTileMapActor>& spWorldTileMapActor) { m_spWorldTileMapActor = spWorldTileMapActor; }

private:
	void OnMouseLeftButtonHandler(const InputActionValue* pInputAction);
	void OnMouseRightButtonHandler(const InputActionValue* pInputAction);
	void OnDirectionKeyHandler(const InputActionValue* pInputAction);

private:
	std::shared_ptr<WorldTileMapActor> m_spWorldTileMapActor = nullptr;
};