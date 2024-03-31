#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <thread>
#include <cstdint>

namespace offsets {
	constexpr std::ptrdiff_t dwLocalPlayerPawn = 0x17371A8;
	constexpr std::ptrdiff_t dwForceJump = 0x1730530;
	constexpr std::ptrdiff_t m_iFlags = 0x1F8;
	constexpr std::ptrdiff_t m_iPawnHealth = 0x7F0;
}

void BunnyHop(const HMODULE instance) noexcept {

	const auto client = reinterpret_cast<std::uintptr_t>(GetModuleHandle("client.dll"));

	while (!GetAsyncKeyState(VK_END))
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		if (!GetAsyncKeyState(VK_SPACE)) {
			continue;
		}

		const auto localPLayer = *reinterpret_cast<std::uintptr_t*>(client + offsets::dwLocalPlayerPawn);
		if (!localPLayer) {
			continue;
		}

		const auto health = *reinterpret_cast<std::int32_t*>(localPLayer + offsets::m_iPawnHealth);
		if (!health) {
			continue;
		}

		const auto flags = *reinterpret_cast<std::int32_t*>(localPLayer + offsets::m_iFlags);

		(flags & (1 << 0)) ?
			*reinterpret_cast<std::uintptr_t*>(client + offsets::dwForceJump) = 6 :
			*reinterpret_cast<std::uintptr_t*>(client + offsets::dwForceJump) = 4;
	}


	FreeLibraryAndExitThread(instance, 0);

}

int __stdcall DllMain(
	const HMODULE instance,
	const std::uintptr_t reason,
	const void* reserved
)
{

	if (reason == 1) {
		DisableThreadLibraryCalls(instance);

		const auto thread = CreateThread(
			nullptr,
			0,
			reinterpret_cast<LPTHREAD_START_ROUTINE>(BunnyHop),
			instance,
			0,
			nullptr
		);

		if (thread) {
			CloseHandle(thread);
		}

	}
	return 1;
}