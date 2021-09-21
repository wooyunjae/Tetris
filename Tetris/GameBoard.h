#pragma once
#include <functional>
#include "Cst.h"
#include "Time.h"

class GameBoard
{
public:
	GameBoard();
	~GameBoard() = default;

public:
	void Update();
	int(*GetBoard())[CST::WIDTH];
	unsigned int GetScore();
	unsigned int GetBlock(int block);
	int GetStoredBlock();
	void SetBlockMoveSound(std::function<void()> sound);
private:
	void Initializer();
	void CreateBlock();
	void LotateBlock();
	bool MoveBlock(int x, int y);
	bool BlockDown();
	void BlockDownByTime();
	bool CheckNotCollide(int x, int y);
	void EraseBlock();
	void SetBlock();
	void Swap();
private:
	enum {
		GAMERUNNING = -99,
		GAMEPUASE,
		GAMEOVER
	};
	enum {
		BLOCK_QUENTITY = 5
	};
	int mGameState;
	int mGameBoard[CST::HEIGHT][CST::WIDTH];
	int mBlockX;
	int mBlockY;
	int mBlocks[BLOCK_QUENTITY];
	int mStoredBlock;
	unsigned int mBlockLotation;
	unsigned int mScore;
	Time mTime;
	float mTotalTime;
	float mNorminalTime;
	float mBlockDownCounter;

	std::function<void()> mBlockMoveSound;
};
