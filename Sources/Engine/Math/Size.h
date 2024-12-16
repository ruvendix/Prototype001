// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

// �̰� �ϵ� ���� ����ؼ� ����
struct Size
{
	int32 width = 0;
	int32 height = 0;

	Size operator/ (int32 denominator) const
	{
		ASSERT_LOG(denominator != 0);
		return Size(width / denominator, height / denominator);
	}
};