#pragma once

#include <windows.h>

namespace box {

	class Keyboard {

		bool pressed[256];
		bool released[256];
		bool down[256];

		int GetState(int vk_code);

	public:
		Keyboard() = default;

		Keyboard(const Keyboard&) = delete;
		Keyboard(Keyboard&&) = delete;

		Keyboard& operator=(Keyboard&) = delete;
		Keyboard& operator=(Keyboard&&) = delete;

		bool Pressed();
		bool Pressed(int vk_code);
		bool Released(int vk_code);
		bool Down(int vk_code);
		bool Up(int vk_code);
		int ReadKey();
	};

	extern Keyboard keyboard;
}
