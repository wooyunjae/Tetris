#include <string>
#include <random>

#include "GameBoard.h"
#include "KeyInput.h"

GameBoard::GameBoard()
{
	Initializer();
}


void GameBoard::Initializer()
{
	mTime.Reset();
	mNorminalTime = 1.f;
	mTotalTime = 0.f;
	mBlockDownCounter = 0.f;
	mBlockLotation = 0;
	mScore = 0;
	memset(mGameBoard, CST::EMPTY, sizeof(mGameBoard));
	mGameState = GAMERUNNING;
	CreateBlock();
}

void GameBoard::Update()
{
	if (KI->IsKeyDown(static_cast<int>(eKeySet::P))
		&& mGameState == GAMEPUASE)
	{
		mGameState = GAMERUNNING;
	}
	else if (KI->IsKeyDown(static_cast<int>(eKeySet::P)))
	{
		mGameState = GAMEPUASE;
	}
	if (KI->IsKeyDown(static_cast<int>(eKeySet::R)))
	{
		Initializer();
	}
	if (mGameState == GAMERUNNING)
	{
		float deltaTime = mTime.GetDeltaTime();
		mTotalTime += deltaTime;
		mBlockDownCounter += deltaTime;
		if (KI->IsKeyDown(static_cast<int>(eKeySet::UP)))
		{
			LotateBlock();
		}
		else if (KI->IsKeyDown(static_cast<int>(eKeySet::DOWN)))
		{
			BlockDown();
		}
		else if (KI->IsKeyDown(static_cast<int>(eKeySet::LEFT)))
		{
			MoveBlock(mBlockX - 1, mBlockY);
		}
		else if (KI->IsKeyDown(static_cast<int>(eKeySet::RIGHT)))
		{
			MoveBlock(mBlockX + 1, mBlockY);
		}
		else if (KI->IsKeyDown(static_cast<int>(eKeySet::SPACE)))
		{
			while (BlockDown());
		}
		BlockDownByTime();
	}
}

int(*GameBoard::GetBoard())[CST::WIDTH]
{
	return mGameBoard;
}

void GameBoard::CreateBlock()
{

	mBlockX = CST::WIDTH / 2 - 1;
	mBlockY = 1;
	{
		std::random_device rd;
		std::mt19937 random(rd());
		std::uniform_int_distribution<int> rint(0, CST::I);
		mBlock = rint(random);
	}
	mBlockLotation = 0;
	if (!CheckNotCollide(mBlockX, mBlockY))
	{
		mGameState = GAMEOVER;
	}
	SetBlock();
}

void GameBoard::CalculateSocre()
{
	unsigned int completeLine = 0;
	bool isComplete;
	for (unsigned int y = 0; y < CST::WIDTH - 2; y++)
	{
		isComplete = true;
		for (unsigned int x = 0; x < CST::WIDTH - 2; x++)
		{
			if (mGameBoard[y][x] == CST::EMPTY)
			{
				isComplete = false;
				break;
			}
		}
		if (isComplete)
		{
			completeLine++;
		}
	}
	mScore += completeLine;
}

void GameBoard::LotateBlock()
{
	EraseBlock();
	mBlockLotation == 3 ? mBlockLotation = 0 : mBlockLotation++;
	if (CheckNotCollide(mBlockX, mBlockY))
	{
		goto setBlock;
	}
	else if (CheckNotCollide(mBlockX - 1, mBlockY))
	{
		mBlockX--;
		goto setBlock;
	}
	else if (CheckNotCollide(mBlockX + 1, mBlockY))
	{
		mBlockX++;
		goto setBlock;
	}
	if (CheckNotCollide(mBlockX, mBlockY - 1))
	{
		mBlockY--;
		goto setBlock;
	}
	else if (CheckNotCollide(mBlockX - 1, mBlockY - 1))
	{
		mBlockX--;
		mBlockY--;
		goto setBlock;
	}
	else if (CheckNotCollide(mBlockX + 1, mBlockY - 1))
	{
		mBlockX++;
		mBlockY--;
		goto setBlock;
	}
	mBlockLotation == 0 ? mBlockLotation = 3 : mBlockLotation--;
	mBlockY += 2;

setBlock:
	SetBlock();
}

bool GameBoard::MoveBlock(int x, int y)
{
	bool result = false;

	EraseBlock();
	if (CheckNotCollide(x, y))
	{
		mBlockX = x;
		mBlockY = y;
		result = true;
	}
	SetBlock();
	return result;
}

bool GameBoard::BlockDown()
{
	EraseBlock();
	if (CheckNotCollide(mBlockX, mBlockY + 1))
	{
		mBlockY++;
		SetBlock();
		return true;
	}

	SetBlock();
	for (unsigned int y = 0; y < CST::HEIGHT; y++)
	{
		bool isLine = true;
		for (unsigned int x = 0; x < CST::WIDTH; x++)
		{
			if (mGameBoard[y][x] == CST::EMPTY)
			{
				isLine = false;
				break;
			}
		}
		if (isLine)
		{
			for (int i = y; i > 0; i--)
			{
				memmove(&(mGameBoard[i][0]), &(mGameBoard[i - 1][0]), sizeof(mGameBoard) / CST::HEIGHT);
			}
			memset(&(mGameBoard[0][0]), CST::EMPTY, sizeof(mGameBoard) / CST::HEIGHT);
		}
	}
	CreateBlock();
	return false;
}

void GameBoard::BlockDownByTime()
{
	if (mNorminalTime < mBlockDownCounter)
	{
		BlockDown();
		mBlockDownCounter = 0.f;
	}
}

bool GameBoard::CheckNotCollide(int x, int y)
{
	auto bk = CST::BLOCK[mBlock][mBlockLotation];
	for (unsigned int i = 0; i < 8; i += 2)
	{
		if (bk[i] + x < 0 || bk[i] + x > CST::WIDTH - 1)
		{
			return false;
		}
	}

	for (unsigned int i = 1; i < 8; i += 2)
	{
		if (bk[i] + y < 0 || bk[i] + y > CST::HEIGHT - 1)
		{
			return false;
		}
	}
	for (unsigned int i = 0; i < 7; i += 2)
	{
		if (mGameBoard[bk[i + 1] + y][bk[i] + x] != CST::EMPTY)
		{
			return false;
		}
	}
	return true;
}

void GameBoard::EraseBlock()
{
	int x;
	int y;
	x = CST::BLOCK[mBlock][mBlockLotation][0] + mBlockX;
	y = CST::BLOCK[mBlock][mBlockLotation][1] + mBlockY;
	mGameBoard[y][x] = CST::EMPTY;
	x = CST::BLOCK[mBlock][mBlockLotation][2] + mBlockX;
	y = CST::BLOCK[mBlock][mBlockLotation][3] + mBlockY;
	mGameBoard[y][x] = CST::EMPTY;
	x = CST::BLOCK[mBlock][mBlockLotation][4] + mBlockX;
	y = CST::BLOCK[mBlock][mBlockLotation][5] + mBlockY;
	mGameBoard[y][x] = CST::EMPTY;
	x = CST::BLOCK[mBlock][mBlockLotation][6] + mBlockX;
	y = CST::BLOCK[mBlock][mBlockLotation][7] + mBlockY;
	mGameBoard[y][x] = CST::EMPTY;
}

void GameBoard::SetBlock()
{
	int x;
	int y;
	x = CST::BLOCK[mBlock][mBlockLotation][0] + mBlockX;
	y = CST::BLOCK[mBlock][mBlockLotation][1] + mBlockY;
	mGameBoard[y][x] = mBlock;
	x = CST::BLOCK[mBlock][mBlockLotation][2] + mBlockX;
	y = CST::BLOCK[mBlock][mBlockLotation][3] + mBlockY;
	mGameBoard[y][x] = mBlock;
	x = CST::BLOCK[mBlock][mBlockLotation][4] + mBlockX;
	y = CST::BLOCK[mBlock][mBlockLotation][5] + mBlockY;
	mGameBoard[y][x] = mBlock;
	x = CST::BLOCK[mBlock][mBlockLotation][6] + mBlockX;
	y = CST::BLOCK[mBlock][mBlockLotation][7] + mBlockY;
	mGameBoard[y][x] = mBlock;
}
