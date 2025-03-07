// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class TransformComponent : public Component
{
	DECLARE_COMPILETIME_ID;

public:
	TransformComponent();
	virtual ~TransformComponent();

	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
	virtual void Cleanup() override;

	virtual ComponentPtr CreateClone() override;

public:
	void AddPosition(const Vector2d& pos) { m_pos += pos; }
	void BuildWolrdMatrix();

	float GetPositionX() const { return (m_pos.x); }
	float GetPositionY() const { return (m_pos.y); }

	const Vector2d& GetPosition() const { return m_pos; }

	void SetPosition(const Vector2d& vPos) { m_pos = vPos; }
	void SetPosition(float x, float y) { m_pos = Vector2d{ x, y }; }

	int32 GetWidth() const { return (m_size.width); }
	int32 GetHeight() const { return (m_size.height); }

	void SetSize(const Size& size) { m_size = size; }
	void SetSize(int32 width, int32 height) { m_size = Size{ width, height }; }

private:
	Vector2d m_pos;
	Size m_size;
};