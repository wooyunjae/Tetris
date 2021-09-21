#include <string>
#include <random>
#include <assert.h>
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
	mStoredBlock = CST::EMPTY;
	memset(mGameBoard, CST::EMPTY, sizeof(mGameBoard));
	mGameState = GAMERUNNING;
	mBlockX = CST::WIDTH / 2 - 1;
	mBlockY = 1;
	{
		std::random_device rd;
		std::mt19937 random(rd());
		std::uniform_int_distribution<int> rint(0, CST::I);

		for (unsigned int i = 0; i < BLOCK_QUENTITY; i++)
		{
			mBlocks[i] = rint(random);
		}
	}
	mBlockLotation = 0;
	if (!CheckNotCollide(mBlockX, mBlockY))
	{
		mGameState = GAMEOVER;
	}
	SetBlock();
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
			mBlockMoveSound();
			LotateBlock();
		}
		else if (KI->IsKeyDown(static_cast<int>(eKeySet::DOWN)))
		{
			mBlockMoveSound();
			BlockDown();
		}
		else if (KI->IsKeyDown(static_cast<int>(eKeySet::LEFT)))
		{
			mBlockMoveSound();
			MoveBlock(mBlockX - 1, mBlockY);
		}
		else if (KI->IsKeyDown(static_cast<int>(eKeySet::RIGHT)))
		{
			mBlockMoveSound();
			MoveBlock(mBlockX + 1, mBlockY);
		}
		else if (KI->IsKeyDown(static_cast<int>(eKeySet::SPACE)))
		{
			mBlockMoveSound();
			while (BlockDown());
		}
		else if (KI->IsKeyDown(static_cast<int>(eKeySet::S)))
		{
			mBlockMoveSound();
			Swap();
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

		for (unsigned int i = 0; i < BLOCK_QUENTITY - 1; i++)
		{
			mBlocks[i] = mBlocks[i + 1];
		}
		mBlocks[BLOCK_QUENTITY - 1] = rint(random);
	}
	mBlockLotation = 0;
	if (!CheckNotCollide(mBlockX, mBlockY))
	{
		mGameState = GAMEOVER;
	}
	SetBlock();
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
	
	int counter = 0;
	int addScore = 1;
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
			counter++;
			addScore *= counter;
			for (int i = y; i > 0; i--)
			{
				memmove(&(mGameBoard[i][0]), &(mGameBoard[i - 1][0]), sizeof(mGameBoard) / CST::HEIGHT);
			}
			memset(&(mGameBoard[0][0]), CST::EMPTY, sizeof(mGameBoard) / CST::HEIGHT);
		}
	}
	if (counter > 0)
	{
		mScore += addScore;
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
	auto bk = CST::BLOCK[mBlocks[0]][mBlockLotation];
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
	x = CST::BLOCK[mBlocks[0]][mBlockLotation][0] + mBlockX;
	y = CST::BLOCK[mBlocks[0]][mBlockLotation][1] + mBlockY;
	mGameBoard[y][x] = CST::EMPTY;
	x = CST::BLOCK[mBlocks[0]][mBlockLotation][2] + mBlockX;
	y = CST::BLOCK[mBlocks[0]][mBlockLotation][3] + mBlockY;
	mGameBoard[y][x] = CST::EMPTY;
	x = CST::BLOCK[mBlocks[0]][mBlockLotation][4] + mBlockX;
	y = CST::BLOCK[mBlocks[0]][mBlockLotation][5] + mBlockY;
	mGameBoard[y][x] = CST::EMPTY;
	x = CST::BLOCK[mBlocks[0]][mBlockLotation][6] + mBlockX;
	y = CST::BLOCK[mBlocks[0]][mBlockLotation][7] + mBlockY;
	mGameBoard[y][x] = CST::EMPTY;
}

void GameBoard::SetBlock()
{
	int x;
	int y;
	x = CST::BLOCK[mBlocks[0]][mBlockLotation][0] + mBlockX;
	y = CST::BLOCK[mBlocks[0]][mBlockLotation][1] + mBlockY;
	mGameBoard[y][x] = mBlocks[0];
	x = CST::BLOCK[mBlocks[0]][mBlockLotation][2] + mBlockX;
	y = CST::BLOCK[mBlocks[0]][mBlockLotation][3] + mBlockY;
	mGameBoard[y][x] = mBlocks[0];
	x = CST::BLOCK[mBlocks[0]][mBlockLotation][4] + mBlockX;
	y = CST::BLOCK[mBlocks[0]][mBlockLotation][5] + mBlockY;
	mGameBoard[y][x] = mBlocks[0];
	x = CST::BLOCK[mBlocks[0]][mBlockLotation][6] + mBlockX;
	y = CST::BLOCK[mBlocks[0]][mBlockLotation][7] + mBlockY;
	mGameBoard[y][x] = mBlocks[0];
}

void GameBoard::Swap()
{
	EraseBlock();
	if (mStoredBlock == CST::EMPTY)
	{
		mStoredBlock = mBlocks[0];
		CreateBlock();
	}
	else
	{
		int lotate = mBlockLotation;
		int block = mStoredBlock;
		mStoredBlock = mBlocks[0];
		mBlocks[0] = block;
		mBlockLotation = 0;
		if (!CheckNotCollide(mBlockX, mBlockY))
		{
			block = mStoredBlock;
			mStoredBlock = mBlocks[0];
			mBlocks[0] = block;
			mBlockLotation = lotate;
		}
		SetBlock();
	}
}

unsigned int GameBoard::GetScore()
{
	return mScore;
}

unsigned int GameBoard::GetBlock(int block)
{
	if (block >= BLOCK_QUENTITY)
	{
		assert(false);
	}
	return mBlocks[block];
}

int GameBoard::GetStoredBlock()
{
	return mStoredBlock;
}

void GameBoard::SetBlockMoveSound(std::function<void()> sound)
{
	mBlockMoveSound = sound;
}
