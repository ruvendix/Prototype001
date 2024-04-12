#pragma once

#include "IGameDefault.h"

class GameApplication : public IGameDefault
{
public:
	DEFINE_SINGLETON(GameApplication);

public:
	GameApplication();
	~GameApplication();

	int32 Drive();

	virtual void Startup() override;
	virtual bool Update() override;
	virtual bool PostUpdate() override;
	virtual void Render() override;
	virtual void Cleanup() override;

	void RenderUi2d();

	HDC CreateCompatibleDc();

	HINSTANCE GetApplicationInstanceHandle() const { return m_hInst; }
	HWND GetApplicationWindowHandle() const { return m_hAppWnd; }
	HDC GetBackBufferDc() const { return m_hBackBufferDc; }

	void SetInstanceHandle(HINSTANCE hInst) { m_hInst = hInst; }

	const Size& GetResolution() const { return m_resolution; }
	const Size& GetHalfResolution() const { return m_halfResolution; }

	CameraPtr GetCurrentCamera() const { return m_spCurrentCamera; }
	void SetCurrentCamera(CameraPtr spCamera) { m_spCurrentCamera = spCamera; }

	const Size& GetWorldSize() const { return m_worldSize; }
	void SetWorldSize(const Size& worldSize) { m_worldSize = worldSize; }

private:
	HINSTANCE m_hInst = nullptr;
	HWND      m_hAppWnd = nullptr;

	Size m_resolution;
	Size m_halfResolution;

	// 더블 버퍼링에 사용
	HDC     m_hDc = nullptr;
	HDC     m_hBackBufferDc = nullptr;
	HBITMAP m_hBackBuffer = nullptr;

	Size      m_worldSize;
	CameraPtr m_spCurrentCamera = nullptr;
};