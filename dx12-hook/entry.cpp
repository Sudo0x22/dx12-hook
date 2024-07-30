#include"present/present.hpp"
static void EntryPoint(LPVOID hBuffer)
{
	AllocConsole();
	FILE* in = {};
	freopen_s(&in, "CONOUT$", "w", stdout);

	Swapchain_t swapchain(FindWindowA("", nullptr), GetModuleHandleA("d3d12.dll"), GetModuleHandleA("dxgi.dll"));
	swapchain.GetSwapchain();
	printf("pSwapchain: 0x%p\n", (void*)swapchain.pSwapchain);

	Hook_t present(swapchain.pSwapchain, 140);
	PresentFn = (Present_t)CallHook(present, (std::uintptr_t)PresentDetour);
}
static bool DllMain(HMODULE hModule, DWORD hReasons, LPVOID hBuffer)
{
	if (hReasons != 0x1)
		return false;

	void* thread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)EntryPoint, 0, 0, 0);
	if (thread)
		CloseHandle(thread);

	return true;
}