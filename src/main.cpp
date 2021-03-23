#include <iostream>
#include <time.h>
#include <vector>

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class Snake : public olc::PixelGameEngine
{
public:
	Snake() {
		sAppName = "Snake Game";
	}

	int get_score() const {
		return score;
	}
private:
	int score = 0;
	float speed = 0.2f;
	float frameT = 0.0f;

	short dir = 0, lastDir = 0; // 0: UP, 1: RIGHT, 2: DOWN, 3: LEFT 


	std::vector<olc::vi2d> snake;

	
	olc::vi2d apple;
	void newApple() {
		do {
			apple = { rand() % ScreenWidth(), rand() % ScreenHeight() };
		} while (std::count(snake.begin(), snake.end(), apple) > 0);
	}

	// pixel out of boundries
	bool pxOOB(olc::vi2d px) {
		return (px.x < 0 || px.x > ScreenWidth() || px.y < 0 || px.y > ScreenHeight());
	}

	olc::vi2d midScreen() {
		return { ScreenWidth() >> 1, ScreenHeight() >> 1 };
	}

protected:
	bool OnUserCreate() override {
		srand(time(NULL));

		newApple();

		snake.clear();
		snake.push_back(midScreen());
		snake.push_back({ midScreen().x, midScreen().y + 1 });
		snake.push_back({ midScreen().x, midScreen().y + 2 });


		return true;
	}

	bool OnUserUpdate(float elapsedTime) override {

		if (GetKey(olc::Key::ESCAPE).bPressed) return false;

		// Input
		
		if (GetKey(olc::Key::W).bPressed && lastDir != 2) dir = 0;
		if (GetKey(olc::Key::A).bPressed && lastDir != 1) dir = 3;
		if (GetKey(olc::Key::S).bPressed && lastDir != 0) dir = 2;
		if (GetKey(olc::Key::D).bPressed && lastDir != 3) dir = 1;

		frameT += elapsedTime;
		if (frameT < speed) return true;
		Clear(0);
		do {
			frameT -= speed;
		} while (frameT >= speed);

		lastDir = dir;
		
		

		// Add move

		
		olc::vi2d lastBlock = snake[0];

		switch (dir) {

		case 0: //UP
			snake.insert(snake.begin(), { lastBlock.x, lastBlock.y - 1 });
			break;

		case 1: // RIGHT
			snake.insert(snake.begin(), { lastBlock.x + 1, lastBlock.y });
			break;

		case 2: // DOWN
			snake.insert(snake.begin(), { lastBlock.x, lastBlock.y + 1 });
			break;

		case 3: // LEFT
			snake.insert(snake.begin(), { lastBlock.x - 1 , lastBlock.y });
			break;
		}

		// Hit Apple
		if (snake[0] == apple) {
			score++;
			speed -= 0.002f;
			newApple();

		}else snake.pop_back();

		// Kill
		for (olc::vi2d px : snake) {
			// Check if Self Collide
			if (std::count(snake.begin(), snake.end(), px) > 1) {
				std::cout << "Self Colide\n";
				return false;
			}

			// out of boundries
			if (pxOOB(px)) {
				std::cout << "Out of Boundries\n";
				return false;
			}
		}

		// Draw snake
		for (olc::vi2d px : snake) {
			Draw(px, olc::WHITE);
		}
		Draw(apple, olc::RED);

		return true;
	}
};

int main() {
	Snake game;
	if (game.Construct(32, 32, 30, 30))
		game.Start();
	std::cout << "Score: " << game.get_score();
	return 0;
}