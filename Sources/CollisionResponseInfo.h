#pragma once

enum class ECollisionObjectType
{
	WorldStatic = 0,
	WorldDynamic,
	Pawn,
	PhysicsBody,
	Destructible,
	LastObjectType = 15,
	Count = 16
};

enum class ECollisionResponseState
{
	Ignore = 0,
	Overlap,
	Block
};