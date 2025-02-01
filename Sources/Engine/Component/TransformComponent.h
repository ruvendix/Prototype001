// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class TransformComponent : public Component
{
	DECLARE_COMPILETIME_ID(TransformComponent)

public:
	TransformComponent();
	virtual ~TransformComponent();

	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
	virtual void Cleanup() override;

public:
	void AddPosition(const Vec2d& pos) { m_pos += pos; }
	void BuildWolrdMatrix();

	float GetPositionX() const { return (m_pos.x); }
	float GetPositionY() const { return (m_pos.y); }

	const Vec2d& GetPosition() const { return m_pos; }

	void SetPosition(const Vec2d& vPos) { m_pos = vPos; }
	void SetPosition(float x, float y) { m_pos = Vec2d{ x, y }; }

	const int32 GetWidth() const { return (m_size.width); }
	const int32 GetHeight() const { return (m_size.height); }

	void SetSize(const Size& size) { m_size = size; }
	void SetSize(int32 width, int32 height) { m_size = Size{ width, height }; }

private:
	Vec2d m_pos;
	Size m_size;
};