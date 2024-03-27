#include "Pch.h"
#include "TransformComponent.h"

TransformComponent::TransformComponent()
{

}

TransformComponent::~TransformComponent()
{

}

void TransformComponent::AddPosition(const Point2d& pos)
{
	m_pos.x += pos.x;
	m_pos.y += pos.y;
}

void TransformComponent::AddPosition(uint32 x, uint32 y)
{
	m_pos.x += x;
	m_pos.y += y;
}