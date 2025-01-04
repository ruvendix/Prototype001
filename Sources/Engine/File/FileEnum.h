// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

enum class EFileStreamMode
{
	WriteOnlyText = 0,
	ReadOnlyText,
	AppendText,
	WriteOnlyBinary,
	ReadOnlyBinary,

	Count,
};