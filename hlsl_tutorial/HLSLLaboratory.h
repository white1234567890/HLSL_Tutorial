//You Oyadomari
//Kokusai Denshi Business College
//Initial 2017/7/13
//LastUpdate 2017/7/13

#pragma once
#define WIN32_LEAN_AND_MEAN

#include "game.h"

//////////////////////////////////////////////////////////////////////////////
//Game entity class
//inherit : Game class
//update(float frameTime), ai(), collosions(), render() need override
//////////////////////////////////////////////////////////////////////////////
class HLSLLaboratory :
	public Game
{
public:
	HLSLLaboratory(void);
	virtual ~HLSLLaboratory(void);

	//Initialize game
	void initialize(HWND hwnd);
	void update();
	void ai();
	void collisions();
	void render();
	void releaseAll();
	void resetAll();

};

