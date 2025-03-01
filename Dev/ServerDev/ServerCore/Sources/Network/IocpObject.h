// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class RxIocpEvent;

class RxIocpObject : public std::enable_shared_from_this<RxIocpObject>
{
public:
	RxIocpObject() = default;
	virtual ~RxIocpObject() = default;

	virtual HANDLE BringHandle() abstract;
	virtual void Dispatch(RxIocpEvent* pIocpEvent, uint32 numOfBytes) abstract;
};