#pragma once

#include <iostream>
#include <sstream>
#include <map>

#include <windows.h>

namespace box {
	using std::endl;

	enum {
		MD_TEXT,
		MD_NOSPACES,
	};

	enum {
	    CL_BLACK,
	    CL_DARKBLUE,
	    CL_DARKGREEN,
	    CL_DARKCYAN,
	    CL_DARKRED,
	    CL_DARKMAGENTA,
	    CL_DARKYELLOW,
	    CL_GRAY,
	    CL_DARKGRAY,
	    CL_BLUE,
	    CL_GREEN,
	    CL_CYAN,
	    CL_RED,
	    CL_MAGENTA,
	    CL_YELLOW,
	    CL_WHITE
	};

	class Display : public std::ostringstream {
			SHORT num_rows, num_cols;
			SHORT row, col, shift;
			CHAR_INFO *charInfo;
			SHORT text_color;
			SHORT bg_color;
			SHORT mode;

			// replacer map for characters
			std::map<CHAR, CHAR> char_map;

		public:
			Display();
			~Display();

			Display(const Display&) = delete;
			Display(Display&&) = delete;

			Display& operator=(Display&) = delete;
			Display& operator=(Display&&) = delete;

			void HideCursor();
			void ShowCursor();

			int Rows() const;
			int Cols() const;

			void SetTextColor(int color);
			void SetBgColor(int color);
			void SetMode(int mode);

			void Move(int x, int y);
			void Move(float x, float y);

			template<typename T>
			void Print(SHORT x, SHORT y, const T& smth) {
				Move(x, y);
				*this<<smth;
				Print();
			}

			void Print();

			void Clear();
			void Replace(CHAR from, CHAR to);
			void NoReplace();
			void Update();
	};

	extern Display disp;
}

