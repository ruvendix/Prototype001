// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class CameraActor : public Actor
{
	using Super = Actor;

public:
	using Super::Super;
	virtual ~CameraActor();

	virtual void Startup() override;
	virtual bool PostUpdate(float deltaSeconds) override;

	bool CheckInCameraVision(const Actor* pActor) const;
	const Vec2d& BringCameraOffsetPosition() const;
};