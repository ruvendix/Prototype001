#pragma once

#include "IGameDefault.h"
#include "ComponentBase.h"
#include "Flipbook.h"

class FlipbookComponent : public ComponentBase
{
public:
	DEFINE_SMART_PTR(FlipbookComponent);

public:
	FlipbookComponent();
	virtual ~FlipbookComponent();

	virtual bool Update() override;
	virtual void Render() override;

	void SetFlipbook(Flipbook::Ptr spFlipbook) { m_spFlipbook = spFlipbook; }
	void SetLoop(bool bLoop) { m_bLoop = bLoop; }
	
	float GetFlipbookEndTime() const { return m_spFlipbook->GetFlipbookEndTime(); }

private:
	Flipbook::Ptr     m_spFlipbook;
	bool              m_bLoop = false;
	float             m_localTime = 0.0f;
	const SpriteInfo* m_pCurrentKeyframe = nullptr;
};