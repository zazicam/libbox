#include "timer.h"

#include <windows.h>

static NTSTATUS(__stdcall *NtDelayExecution)(BOOL Alertable, PLARGE_INTEGER DelayInterval) = (NTSTATUS(__stdcall*)(BOOL, PLARGE_INTEGER)) GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtDelayExecution");

static NTSTATUS(__stdcall *ZwSetTimerResolution)(IN ULONG RequestedResolution, IN BOOLEAN Set, OUT PULONG ActualResolution) = (NTSTATUS(__stdcall*)(ULONG, BOOLEAN, PULONG)) GetProcAddress(GetModuleHandleA("ntdll.dll"), "ZwSetTimerResolution");

namespace box {

	Timer timer;

	static void SleepShort(float milliseconds)
	{
		static bool once = true;
		if (once)
		{
			ULONG actualResolution;
			ZwSetTimerResolution(1, true, &actualResolution);
			once = false;
		}

		LARGE_INTEGER interval;
		interval.QuadPart = -1 * (int)(milliseconds * 10000.0f);
		NtDelayExecution(false, &interval);
	}

	Timer::Timer()
	{
		start_time = std::chrono::steady_clock::now();
	}

	int Timer::Elapsed() const
	{
		std::chrono::steady_clock::time_point current_time = std::chrono::steady_clock::now();
		int duration = static_cast<int>(
			std::chrono::duration_cast<std::chrono::milliseconds>(
				current_time - start_time
			).count()
		);
		return duration;
	}

	void Timer::Reset()
	{
		start_time = std::chrono::steady_clock::now();
	}

	int Timer::SyncLoopTo(int ms)
	{
		SleepShort(static_cast<float>(ms - Elapsed()));
		int sync_time = Elapsed();
		Reset();
		return sync_time;
	}

	void Timer::Wait(int ms)
	{
		SleepShort(static_cast<float>(ms));
	}

}

