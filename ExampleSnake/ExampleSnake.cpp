// ExampleSnake.cpp : Definiert den Einstiegspunkt f�r die Konsolenanwendung.
//

#include "stdafx.h"

#include "BBE\BrotBoxEngine.h"

enum Direction
{
	up, down, left, right
};

struct BodyPart
{
	int x;
	int y;

	bool operator==(BodyPart &other)
	{
		return (other.x == x && other.y == y);
	}
};

class MyGame : public bbe::Game
{
public:
	const int GRIDWIDTH = 20;
	const int GRIDHEIGHT = 20;
	const int CELLSIZE = 10;
	const float TICKTIME = 0.25f;

	float x = 0;
	Direction dir = left;
	Direction nextDir = left;

	bbe::List<BodyPart> bodyParts;
	BodyPart food;

	float timeSinceLastTick = 0;

	bbe::Random rand;

	bool gameOver = false;

	virtual void onStart() override
	{
		bodyParts.add({ GRIDWIDTH / 2, GRIDHEIGHT / 2 });
		bodyParts.add({ -1, -1 });
		bodyParts.add({ -1, -1 });

		placeFood();
	}

	virtual void update(float timeSinceLastFrame) override
	{
		input();
		timeSinceLastTick += timeSinceLastFrame;
		if (timeSinceLastTick > TICKTIME)
		{
			timeSinceLastTick -= TICKTIME;
			tick();
		}
	}

	void tick()
	{
		if (gameOver)
		{
			return;
		}

		dir = nextDir;

		for (int i = bodyParts.getLength() - 1; i >= 1; i--)
		{
			bodyParts[i] = bodyParts[i - 1];
		}
		switch (dir)
		{
		case up:
			bodyParts[0].y--;
			break;
		case down:
			bodyParts[0].y++;
			break;
		case left:
			bodyParts[0].x--;
			break;
		case right:
			bodyParts[0].x++;
			break;
		default:
			throw bbe::IllegalStateException();
		}

		if (bodyParts[0].x < 0)
		{
			bodyParts[0].x = GRIDWIDTH - 1;
		}
		if (bodyParts[0].y < 0)
		{
			bodyParts[0].y = GRIDHEIGHT - 1;
		}
		if (bodyParts[0].x >= GRIDWIDTH)
		{
			bodyParts[0].x = 0;
		}
		if (bodyParts[0].y >= GRIDHEIGHT)
		{
			bodyParts[0].y = 0;
		}


		if (bodyParts[0].x == food.x && bodyParts[0].y == food.y)
		{
			bodyParts.add({ -1, -1 });
			placeFood();
		}

		checkGameOver();
	}

	void input()
	{
		if (dir == left || dir == right)
		{
			if (isKeyDown(bbe::KEY_W))
			{
				nextDir = up;
			}
			else if (isKeyDown(bbe::KEY_S))
			{
				nextDir = down;
			}
		}
		else if (dir == up || dir == down)
		{
			if (isKeyDown(bbe::KEY_A))
			{
				nextDir = left;
			}
			else if (isKeyDown(bbe::KEY_D))
			{
				nextDir = right;
			}
		}
	}

	void checkGameOver()
	{
		for (int i = 1; i < bodyParts.getLength(); i++)
		{
			if (bodyParts[i] == bodyParts[0])
			{
				gameOver = true;
			}
		}
	}

	void placeFood()
	{
		start:
		int randX = rand.randomInt(GRIDWIDTH);
		int randY = rand.randomInt(GRIDHEIGHT);
		for (BodyPart bp : bodyParts)
		{
			if (bp.x == randX && bp.y == randY)
			{
				goto start; //I also like to live dangerously.
			}
		}
		food.x = randX;
		food.y = randY;
	}

	virtual void draw2D(bbe::PrimitiveBrush2D & brush) override
	{
		for (BodyPart bp : bodyParts)
		{
			brush.fillRect(bp.x * CELLSIZE, bp.y * CELLSIZE, CELLSIZE, CELLSIZE);
		}

		brush.setColor(0.5f, 1, 0.5f);
		brush.fillCircle(food.x * CELLSIZE, food.y * CELLSIZE, CELLSIZE, CELLSIZE);
	}

	virtual void onEnd() override
	{
	}
};


int main()
{
	MyGame mg;
	mg.start(mg.CELLSIZE * mg.GRIDWIDTH, mg.CELLSIZE * mg.GRIDHEIGHT, "Snake!");

    return 0;
}
