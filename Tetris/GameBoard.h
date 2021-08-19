#pragma once
#include "Cst.h"
#include "Time.h"
enum class eKeySet;

class GameBoard
{
public:
	GameBoard();
	~GameBoard() = default;

public:
	void Update();
	int(*GetBoard())[CST::WIDTH];

private:
	void Initializer();
	void CreateBlock();
	void CalculateSocre();
	void LotateBlock();
	bool MoveBlock(int x, int y);
	bool BlockDown();
	void BlockDownByTime();
	bool CheckNotCollide(int x, int y);
	void EraseBlock();
	void SetBlock();

private:
	enum {
		GAMERUNNING = -99,
		GAMEPUASE,
		GAMEOVER
	};
	int mGameState;
	int mGameBoard[CST::HEIGHT][CST::WIDTH];
	int mBlockX;
	int mBlockY;
	int mBlock;
	unsigned int mBlockLotation;
	unsigned int mScore;
	Time mTime;
	float mTotalTime;
	float mNorminalTime;
	float mBlockDownCounter;
};
