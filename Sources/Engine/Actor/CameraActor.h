// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class CameraActor : public Actor
{
	using Super = Actor;

public:
	using Super::Super;
	virtual ~CameraActor();

	virtual void Startup() override;

	bool CheckInCameraVision(const Actor* pActor) const;
	bool CheckInCameraVision(const Vector2d& worldPos) const;

	const Vector2d& BringCameraOffsetPosition() const;
};