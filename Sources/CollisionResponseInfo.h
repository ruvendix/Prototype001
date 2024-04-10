#pragma once

enum class ECollisionResponseState
{
	Ignore = 0,
	Overlap,
	Block
};

struct CollisionResponseInfo
{
	std::string strObjectTypeTag;
	ECollisionResponseState collisionResponseState = ECollisionResponseState::Ignore;
};