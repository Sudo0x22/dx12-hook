#pragma once
#include"../swapchain/swapchain.hpp"
#include"../hook/hook.hpp"

typedef HRESULT(__stdcall* Present_t)(IDXGISwapChain*, UINT, UINT);
typedef LRESULT(__stdcall* WndProc_t)(HWND, UINT, WPARAM, LPARAM);

Present_t PresentFn = 0;
WndProc_t WndProcFn = 0;

HRESULT __stdcall PresentDetour(IDXGISwapChain3* pthis, UINT uint, UINT uflags)
{
	printf("hooked\n");
	return PresentFn(pthis, uint, uflags);
}
