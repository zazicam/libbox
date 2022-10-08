#include "display.h"

#include <cmath>

namespace box {

	Display disp;

	void setCursorVisible(bool vis)
	{
		CONSOLE_CURSOR_INFO CCI { 1, vis };
		SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CCI);
	}

	Display::Display()
	{

		// Get num of rows and columns of current console
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
		num_cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
		num_rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

		// default value to initialize buffer
		CHAR_INFO empty_char {0, 0};
//        empty_char.Char.AsciiChar = 0;
//        empty_char.Attributes = 0;

		charInfo = new CHAR_INFO[num_rows * num_cols] {empty_char};

		// Set current cursor position
		row = 0;
		col = 0;
		shift = 0;

		text_color = CL_GRAY;
		bg_color = CL_BLACK;
		mode = MD_TEXT;
	}

	Display::~Display()
	{
		if(charInfo)
		{
			delete [] charInfo;
			charInfo = nullptr;
		}
	}

	void Display::HideCursor()
	{
		setCursorVisible(false);
	}

	void Display::ShowCursor()
	{
		setCursorVisible(true);
	}

	int Display::Rows() const
	{
		return num_rows;
	}

	int Display::Cols() const
	{
		return num_cols;
	}

	void Display::SetTextColor(int color)
	{
		Print();
		text_color = color;
	}

	void Display::SetBgColor(int color)
	{
		Print();
		bg_color = color;
	}

	void Display::SetMode(int mode) 
	{
		this->mode = mode;
	}

	void Display::Move(int x, int y)
	{
		Print();
		this->row = static_cast<SHORT>(y);
		this->col = static_cast<SHORT>(x);
		shift = 0;
	}

	void Display::Move(float x, float y)
	{
		Print();
		this->row = static_cast<SHORT>(std::lround(y));
		this->col = static_cast<SHORT>(std::lround(x));
		shift = 0;
	}

	void Display::Print()
	{
		const std::string& out_str = str(); // ostringstream::str()
		for(int i=0; i<out_str.size(); i++)
		{
			char c = out_str[i];

			// replacer
			if(char_map.count(c)>0)
			{
				c = char_map[c];
			}

			if(c=='\n')
			{
				++row;
				shift = 0;
			}
			else
			{
				int col_pos = col + shift;
				++shift;
				
				// skip if it's out of display range
				if(row<0 || col_pos < 0 || row >= num_rows || col_pos >= num_cols)
					continue;

				// skip spaces in MD_NOSPACES mode
				if(mode == MD_NOSPACES && c==' ')
					continue;
				
				int index = row * num_cols + col_pos;
				charInfo[index].Char.AsciiChar = c;
				charInfo[index].Attributes = bg_color << 4 | text_color;
				
			}
		}
		
		str("");
	}

	void Display::Clear()
	{
		str("");
		for(int i=0; i<num_rows; i++)
			for(int j=0; j<num_cols; j++)
			{
				charInfo[i*num_cols + j].Char.AsciiChar = ' ';
				charInfo[i*num_cols + j].Attributes = bg_color << 4 | text_color;
			}
		
		row = 0;
		col = 0;
		shift = 0;
	}
	
	void Display::Replace(CHAR from, CHAR to)
	{
		char_map[from] = to;
	}

	void Display::NoReplace()
	{
		char_map.clear();
	}

	void Display::Update()
	{
		Print();
		
		COORD charBufSize = {num_cols, num_rows};
		COORD characterPos = {0, 0};
		SMALL_RECT writeArea = {0, 0, num_cols, num_rows};

		HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
		WriteConsoleOutputA(console, charInfo, charBufSize, characterPos, &writeArea);
	}

}




