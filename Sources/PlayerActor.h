#pragma once

#include "PawnActor.h"

enum class EPlayerDirection
{
	Left = 0,
	Right,
	Up,
	Down,
	Count,
};

class PlayerActor : public PawnActor
{
public:
	DEFINE_SMART_PTR(PlayerActor);
	using PawnActor::PawnActor;

public:
	virtual ~PlayerActor();

	virtual void Startup() override;
	virtual bool Update()  override;
	virtual void Render()  override;
	virtual void Cleanup() override;

private:
	float            m_speed = 0.0f;
	EPlayerDirection m_currentDir = EPlayerDirection::Down;
};