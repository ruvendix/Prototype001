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
	using PawnActor::PawnActor;

	using Super = PawnActor;

public:
	virtual ~PlayerActor();

	virtual void Startup() override;
	virtual bool Update()  override;
	virtual void Cleanup() override;

	void ApplyNextCellPosition(const Int2d& nextCellPos, bool bDelay);
	void ApplyNextCellPositionToCurrentCellPosition();

	Vec2d ConvertCellPositionToWorldPosition(const Int2d& cellPos) const;
	Vec2d CalcDestinationWorldPosition() const;

	void OnCollisionHit(CollisionComponentPtr spTargetCollisionComponent);
	void OnCollisionOverlap(CollisionComponentPtr spTargetCollisionComponent);

	float GetSpeed() const { return m_speed; }
	EPlayerDirection GetCurrentDirection() const { return m_currentDir; }
	void SetDirection(EPlayerDirection nextDir) { m_currentDir = nextDir; }
	void SetPlayerStateIndex(uint32 nextStateIdx) { m_currentStateIdx = nextStateIdx; }
	PlayerStateBasePtr& GetCurrentState() { return m_playerStates[m_currentStateIdx]; }
	
	const Int2d& GetCurrentCellPosition() const { return m_currentCellPos; }
	void SetCurrentCellPosition(const Int2d& cellPos) { m_currentCellPos = cellPos; }

private:
	Int2d m_currentCellPos;
	Int2d m_destCellPos;

	float m_speed = 0.0f;
	uint32 m_currentStateIdx = 0;
	std::vector<PlayerStateBasePtr> m_playerStates;
	EPlayerDirection m_currentDir = EPlayerDirection::Down;
};