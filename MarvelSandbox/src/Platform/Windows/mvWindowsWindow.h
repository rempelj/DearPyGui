#pragma once

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <tchar.h>

#include "Core/mvWindow.h"

class mvWindowsWindow : public mvWindow
{

public:

	mvWindowsWindow();

	virtual void show() override;

	virtual void setup() override;
	virtual void prerender() override;
	virtual void postrender() override;
	virtual void cleanup() override;

private:

	bool CreateDeviceD3D(HWND hWnd);

	void CleanupDeviceD3D();

	void CreateRenderTarget();

	void CleanupRenderTarget();

public:

	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

private:

	HWND m_hwnd;
	WNDCLASSEX m_wc;
	MSG m_msg;

private:

	// Data
	static ID3D11Device* s_pd3dDevice;
	static ID3D11DeviceContext* s_pd3dDeviceContext;
	static IDXGISwapChain* s_pSwapChain;
	static ID3D11RenderTargetView* s_mainRenderTargetView;

};
