#include <vector>
#include <string>
#include <memory>

#include "box/box.h"
using namespace box;

class Hero {
protected:
	int x, y;
	int time, latency;
	int color;
	std::string name;
public:
	Hero(std::string name, int color = CL_WHITE)
		: x(0), y(0), time(0), latency(1), color(color), name(name) {}
	virtual void Draw() const = 0;
	virtual void Move() = 0;
	void Update() { Draw(); Move(); }
	virtual ~Hero() {}
};

class BotHero : public Hero {
	int vx, vy;
public:
	BotHero(std::string name, int color = CL_WHITE) 
		: Hero(name, color) {
		x = random.Int(0, disp.Cols()-40);
		y = random.Int(0, disp.Rows()-1);
		vx = random.choice<int>({ -1, 1 });
		vy = random.choice<int>({ -1, 1 });
		latency = 20;
	}

	void Draw() const override {
		disp.Move(x, y);
		disp.SetTextColor(color);
		disp << name << endl;
		disp << "  o_/ " << endl;
		disp << " /|   " << endl;
		disp << " /|   " << endl;
	}

	void Move() override {
		if (time++ % latency == 0) {
			x += vx;
			y += vy;
			if (x < 3) { x = 3; vx = -vx; }
			if (y < 3) { y = 3; vy = -vy; }
			if (x > disp.Cols() - 40) { x = disp.Cols() - 40; vx = -vx; }
			if (y > disp.Rows() - 5) { y = disp.Rows() - 5; vy = -vy; }
		}
	}
};

class UserHero : public Hero {
public:
	UserHero(std::string name, int color) :
		Hero(name, color) {}

	void Draw() const override {
		disp.Move(x, y);
		disp.SetTextColor(color);
		disp << name << endl;
		disp << " _O_  " << endl;
		disp << "/|s|\\" << endl;
		disp << " | |  " << endl;
	}

	void Move() override {
		if (keyboard.Down(VK_RIGHT)) x++;
		if (keyboard.Down(VK_LEFT)) x--;
		if (keyboard.Down(VK_DOWN)) y++;
		if (keyboard.Down(VK_UP)) y--;
	}
};

int main() {
	random.RandomizeTimer();
	disp.HideCursor();
	disp.SetMode(MD_NOSPACES);

	std::vector<std::shared_ptr<Hero>> heroes{
		std::make_shared<BotHero>("Batman", CL_BLUE),
		std::make_shared<BotHero>("Iron man", CL_DARKRED),
		std::make_shared<BotHero>("Spider-man", CL_CYAN),
		std::make_shared<BotHero>("Groot", CL_GREEN),
		std::make_shared<BotHero>("Black Widow", CL_GRAY),
		std::make_shared<UserHero>("(SuperMan)", CL_RED)
	};
	
	while (1) {
		disp.Clear();
		for (const auto& hero : heroes)
			hero->Update();
		disp.Update();
		timer.SyncLoopTo(20);
	}
	keyboard.ReadKey();
}
