#pragma once

#include "SystemBase.h"
#include "EnumBitFlag.h"

enum class EDebugOption
{
	None = 0,
	ShowCollider,
	ShowDebugTile,
};

class DebugSystem : public SystemBase
{
public:
	using DebugOption = EnumBitFlag<EDebugOption>;

public:
	DebugSystem();
	virtual ~DebugSystem();

	virtual bool Update() override;
	
	bool IsEnableOption(EDebugOption debugOption) { return (m_debugOption.TestBit(debugOption) == true); }

private:
	DebugOption m_debugOption;
};