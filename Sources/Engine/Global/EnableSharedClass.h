// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

// �� ���� ���Ϲ����� ���� ���
class EnableSharedClass : public ICoreLoop, public std::enable_shared_from_this<EnableSharedClass>
{
public:
	EnableSharedClass() = default;
	virtual ~EnableSharedClass() = default;
};