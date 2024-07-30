#pragma once

#include<d3d12.h>
#include<dxgi1_4.h>
#include<d3d11.h>
#include<dxgi.h>

#include<stdio.h>

class Swapchain_t
{
private:
	HWND hWindow = 0;
	HMODULE hD3D12Module = 0;
	HMODULE hDXGIModule = 0;
public:
	IDXGISwapChain* pSwapchain = nullptr;
	ID3D12Device* pDevice = nullptr;
public:
	Swapchain_t(HWND class_, HMODULE d3d12_module_, HMODULE dxgi_module_)
	{
		hWindow = class_;
		hD3D12Module = d3d12_module_;
		hDXGIModule = dxgi_module_;
	}
	Swapchain_t(void) = default;
private:
	HWND GetWindow() const { return hWindow; }
	HMODULE GetD3D12Module() const { return hD3D12Module; }
	HMODULE GetDXGIModule() const { return hDXGIModule; }
public:
	void GetSwapchain()
	{
		if (this->GetWindow() == 0 || this->GetD3D12Module() == 0 || this->GetDXGIModule() == 0)
			printf("window || d3d12 || dxgi error\n");

		void* CreateDXGIFactory = ::GetProcAddress(this->GetDXGIModule(), "CreateDXGIFactory");
		if (CreateDXGIFactory == 0)
			printf("CreateDXGIFactory failed\n");

		IDXGIFactory* pFactory;
		if (((long(__stdcall*)(const IID&, void**))(CreateDXGIFactory))(__uuidof(IDXGIFactory), (void**)&pFactory) < 0)
			printf("pFactory error\n");

		IDXGIAdapter* pAdapter;
		if (pFactory->EnumAdapters(0, &pAdapter) == DXGI_ERROR_NOT_FOUND)
			printf("pFactory->EnumAdapters Error\n");

		void* D3D12CreateDevice = ::GetProcAddress(this->GetD3D12Module(), "D3D12CreateDevice");
		if (D3D12CreateDevice == 0)
			printf("D3D12CreateDevice failed\n");

		if (((long(__stdcall*)(IUnknown*, D3D_FEATURE_LEVEL, const IID&, void**))(D3D12CreateDevice))(pAdapter, D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), (void**)&pDevice) < 0)
			printf("pDevice error\n");

		D3D12_COMMAND_QUEUE_DESC QueueDesc;
		QueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		QueueDesc.Priority = 0;
		QueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		QueueDesc.NodeMask = 0;

		ID3D12CommandQueue* CommandQueue;
		if (pDevice->CreateCommandQueue(&QueueDesc, __uuidof(ID3D12CommandQueue), (void**)&CommandQueue) < 0)
			printf("pDevice->CreateCommandQueue error\n");

		ID3D12CommandAllocator* CommandAllocator;
		if (pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(ID3D12CommandAllocator), (void**)&CommandAllocator) < 0)
			printf("pDevice->CreateCommandAllocator error\n");

		ID3D12GraphicsCommandList* CommandList;
		if (pDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, CommandAllocator, NULL, __uuidof(ID3D12GraphicsCommandList), (void**)&CommandList) < 0)
			printf("pDevice->CreateCommandList error\n");

		DXGI_RATIONAL RefreshRate;
		RefreshRate.Numerator = 60;
		RefreshRate.Denominator = 1;

		DXGI_MODE_DESC BufferDesc;
		BufferDesc.Width = 100;
		BufferDesc.Height = 100;
		BufferDesc.RefreshRate = RefreshRate;
		BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		DXGI_SAMPLE_DESC SampleDesc;
		SampleDesc.Count = 1;
		SampleDesc.Quality = 0;

		DXGI_SWAP_CHAIN_DESC SwapChainDesc = {};
		SwapChainDesc.BufferDesc = BufferDesc;
		SwapChainDesc.SampleDesc = SampleDesc;
		SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		SwapChainDesc.BufferCount = 2;
		SwapChainDesc.OutputWindow = GetWindow();
		SwapChainDesc.Windowed = 1;
		SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		if (pFactory->CreateSwapChain(CommandQueue, &SwapChainDesc, &pSwapchain) < 0)
			printf("pFactory->CreateSwapChain error\n");
	}
};