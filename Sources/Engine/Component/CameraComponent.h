// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class CameraComponent : public Component
{
	DECLARE_COMPILETIME_ID(CameraComponent)

public:
	CameraComponent();
	virtual ~CameraComponent();

	virtual bool PostUpdate(float deltaSeconds) override;

	void SetTargetActor(Actor* pTargetActor) { m_pTargetActor = pTargetActor; }
	const Vec2d& GetCameraOffsetPosition() const { return m_cameraOffsetPos; }

private:
	Actor* m_pTargetActor = nullptr; // Ÿ���� ������ ���ʸ� ����
	Vec2d m_cameraOffsetPos;
};