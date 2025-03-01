// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class RxIocpCore
{
public:
	RxIocpCore();
	~RxIocpCore();

	void Startup();
	void Cleanup();

	bool Register(RxIocpObjectPtr spIocpObj);
	bool Dispatch(uint32 timeMilliseconds);

	HANDLE GetIocpHandle() const { return m_hIocp; }

private:
	HANDLE m_hIocp = nullptr;
};