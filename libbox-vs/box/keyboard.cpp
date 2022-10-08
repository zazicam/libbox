#include "keyboard.h"

namespace box {

	Keyboard keyboard;

	// we are to react only if console is a current window
	static bool IsActiveWindow() {
		return GetForegroundWindow() == GetConsoleWindow();
	}

	int Keyboard::GetState(int vk_code) {
		int x = GetAsyncKeyState(vk_code);
		if(x && !down[vk_code]) {
			down[vk_code] = true;
			pressed[vk_code] = true;
		}
		if(!x && down[vk_code]) {
			down[vk_code] = false;
			released[vk_code] = true;
		}

		return x;
	}

	bool Keyboard::Pressed() {
		// we are to react only if console is a current window
		if(!IsActiveWindow())
			return false;

		for(int i=0; i<256; i++) {
			if(pressed[i]) {
				return true;
			}
		}
		return false;
	}

	bool Keyboard::Pressed(int vk_code) {
		if(!IsActiveWindow())
			return false;

		GetState(vk_code);

		bool res = pressed[vk_code];
		pressed[vk_code] = false;
		return res;

	}

	bool Keyboard::Released(int vk_code) {
		if(!IsActiveWindow())
			return false;

		GetState(vk_code);

		bool res = released[vk_code];
		released[vk_code] = false;
		return res;
	}


	bool Keyboard::Down(int vk_code) {
		GetState(vk_code);
		return down[vk_code];
	}

	bool Keyboard::Up(int vk_code) {
		GetState(vk_code);
		return !down[vk_code];
	}

	int Keyboard::ReadKey() {
		while(1) {
			for(int i=0; i<256; i++) {
				if(GetAsyncKeyState(i) & 1)
					return i;
			}
			Sleep(100);
		}
		return -1;
	}

}
