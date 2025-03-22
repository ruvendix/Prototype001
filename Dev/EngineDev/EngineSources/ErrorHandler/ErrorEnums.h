// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

enum class EErrorCode
{
	Unknown = 0,
	ExistedResource,
	ExistedComponent,
	NotFoundResource,
	InvalidIntegerRange,
	InvalidRenderingLayer,
	InvalidFileStream,
	NoReturnNullObject,
	ServerGameEntityInsertFailed,
	ConnectFailedToServer,
	DisconnectFromServer,
};