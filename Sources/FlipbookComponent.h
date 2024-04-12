#pragma once

#include "ComponentBase.h"
#include "Sprite.h"

class FlipbookComponent : public ComponentBase
{
public:
	FlipbookComponent();
	virtual ~FlipbookComponent();

	virtual bool Update() override;
	virtual void Render() override;

	float BringFlipbookEndTime() const;

	void SetFlipbook(FlipbookPtr spFlipbook) { m_spFlipbook = spFlipbook; }
	void SetLoop(bool bLoop) { m_bLoop = bLoop; }	

private:
	FlipbookPtr m_spFlipbook;
	bool m_bLoop = false;
	float m_localTime = 0.0f;
	const SpriteInfo* m_pCurrentKeyframe = nullptr;
};