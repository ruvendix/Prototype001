// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class CameraComponent : public Component
{
	DECLARE_COMPILETIME_ID;

public:
	CameraComponent();
	virtual ~CameraComponent();

	virtual bool Update(float deltaSeconds) override;

	void SetTargetActor(const ActorPtr& spTargetActor) { m_spTargetActor = spTargetActor; }
	const Vector2d& GetCameraOffsetPosition() const { return m_cameraOffsetPos; }

private:
	ActorPtr m_spTargetActor = nullptr; // 타겟이 없으면 오너를 참조
	Vector2d m_cameraOffsetPos;
};