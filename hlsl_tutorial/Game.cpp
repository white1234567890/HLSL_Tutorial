//You Oyadomari
//Kokusai Denshi business Vocational School
//Initial 2017/1/19
//LastUpdate 2017/2/16

#include "Game.h"

Game::Game(void)
{
	//Scures memory and initialize Input
	input = new Input();

	//Any other initialization will call input->initialize() after
	paused = false;	//game is not puseing
	graphics = NULL;
	fps = 100;
	fpsOn = true;
	initialized = false;
}


Game::~Game(void)
{
	//Release all memories
	deleteAll();

	ShowCursor(true);
}

//////////////////////////////////////////////////////////////////////////////
//Windows message handler
//////////////////////////////////////////////////////////////////////////////
LRESULT Game::messageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(initialized)	//If did not initialize, do not process message
	{
		switch (msg)
		{
		case WM_DESTROY:
			//Send message to quit this program to Windows
			PostQuitMessage(0);
			return 0;

			//Key is pressed
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			input->keyDown(wParam);
			if(input->wasKeyPressed(ESC_KEY))
				PostQuitMessage(0);
			return 0;

			//Key is released
		case WM_KEYUP:
		case WM_SYSKEYUP:
			input->keyUp(wParam);
			return 0;

			//Input charactor
		case WM_CHAR:
			input->keyIn(wParam);
			return 0;

			//Mouse was moved
		case WM_MOUSEMOVE:
			input->mouseIn(lParam);
			return 0;

			//Input raw data from mouse
		case WM_INPUT:
			input->mouseRawIn(lParam);
			return 0;

			//Mouse left button is pressed
		case WM_LBUTTONDOWN:
			input->setMouseLButton(true);
			input->mouseIn(lParam);
			return 0;

			//Mouse left button is released
		case WM_LBUTTONUP:
			input->setMouseLButton(false);
			input->mouseIn(lParam);
			return 0;

			//Mouse middle button is pressed
		case WM_MBUTTONDOWN:
			input->setMouseMButton(true);
			input->mouseIn(lParam);
			return 0;

			//Mouse middle button is released
		case WM_MBUTTONUP:
			input->setMouseMButton(false);
			input->mouseIn(lParam);
			return 0;

			//Mouse right button is pressed
		case WM_RBUTTONDOWN:
			input->setMouseRButton(true);
			input->mouseIn(lParam);
			return 0;

			//Mouse right button is released
		case WM_RBUTTONUP:
			input->setMouseRButton(false);
			input->mouseIn(lParam);
			return 0;

			//Mouse X button is pressed
		case WM_XBUTTONDOWN:
			input->setMouseXButton(true);
			input->mouseIn(lParam);
			return 0;

			//Mouse X button is released
		case WM_XBUTTONUP:
			input->setMouseXButton(false);
			input->mouseIn(lParam);
			return 0;

			//Check controller
		case WM_DEVICECHANGE:
			input->checkControllers();
			return 0;
		}
	}

	//Entrust processing Windows
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

//////////////////////////////////////////////////////////////////////////////
//Initialize game
//When error, throw GameError
//////////////////////////////////////////////////////////////////////////////
void Game::initialize(HWND hw)
{
	hwnd = hw; //Save window handle

	//Initialize graphics
	graphics = new Graphics();

	//Throw GameError
	graphics->initialize(hwnd, SCREEN_WIDTH, SCREEN_HEIGHT, FULLSCREEN);

	//Initialize input, do not capture mouse
	//Throw GameError
	input->initialize(hwnd, false);

	//create fileloader
	fileLoader = FileLoader::Create();

	//initialize font
	if(!font.initialize(graphics, gameNS::POINT_SIZE_DEBUG, false, false, gameNS::FONT))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize font"));
	font.setFontColor(gameNS::FONT_COLOR);

	//Try to prepare using high resolution timer
	if(QueryPerformanceFrequency(&timerFreq) == false)
	{
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing high resolution timer"));
	}

	//Get Start time;
	QueryPerformanceCounter(&timeStart);

	initialized = true;
}

void Game::handleLostGraphicsDevice()
{
	//Test device was lost, and execute apropriate processing
	hr = graphics->getDeviceState();

	//If graphics device is disabled
	if(FAILED(hr))
	{
		//Device was lost, and it can not reset
		if(hr == D3DERR_DEVICELOST)
		{
			//Surrender CPU time (100 miri second)
			Sleep(100);
			return;
		}
		//Device was lost, but it can reset
		else if(hr == D3DERR_DEVICENOTRESET)
		{
			releaseAll();
			//Try to reset graphics device
			hr = graphics->reset();
			//If reset failed
			if(FAILED(hr)) return;
			resetAll();
		}
		//Other devices error
		else return;	
	}
}

//////////////////////////////////////////////////////////////////////////////
//Called over and over again in WinMain loop
//////////////////////////////////////////////////////////////////////////////
void Game::run(HWND hwnd)
{
	//If graphics is not inisialized
	if(graphics == NULL) return;

	//Calculate elapsed time from last frame and save to frameTime
	QueryPerformanceCounter(&timeEnd);
	frameTime = (float)(timeEnd.QuadPart - timeStart.QuadPart) / (float)timerFreq.QuadPart;

	//Power-saving code(need winmm.lib to use)
	//If frame rate is short than you want
	if(frameTime < MIN_FRAME_TIME)
	{
		sleepTime = (DWORD)((MIN_FRAME_TIME - frameTime) * 1000);
		timeBeginPeriod(1);	//Request 1 milli second Windows timer
		Sleep(sleepTime);		//Release CPU while sleepTime
		timeEndPeriod(1);		//Finish 1 milli second timer resolution
		return;
	}

	if(frameTime > 0.0f) fps = (fps * 0.99f) + (0.01f / frameTime); //Average fps
	//If frame rate is very late
	if(frameTime > MAX_FRAME_TIME)
	{
		frameTime = MAX_FRAME_TIME;	//Limit maximum frameTime
	}

	timeStart = timeEnd;
	input->readControllers();

	//update(float frameTime), ai(), collisions() is pure virtual function
	//These functions need override in inherited Game class
	if(!paused)
	{
		update();												//Update all game items
		ai();														//Artificial intelligence
		collisions();											//Process collisions
		input->vibrateControllers(frameTime);	//Vibrate controllers
	}

	renderGame();												//Draw all game items

	//Clear input
	//Call this function after finish all key check
	input->clear(inputNS::KEYS_PRESSED);
}

//Render game items
void Game::renderGame()
{
	const int BUF_SIZE = 20;
	static char buffer[BUF_SIZE];

	//Start rendering
	if(SUCCEEDED(graphics->beginScene()))
	{
		//render() is pure virtual function that need to override in inherited class
		//Call render() in inherited class 
		render();

		graphics->spriteBegin();	//start drawing sprite
		//if draw frame rate
		if(fpsOn)
		{
			//convert fps to Cstring
			_snprintf_s(buffer, BUF_SIZE, "fps %d", (int)fps);
			font.print(buffer, 0, 0);
		}
		graphics->spriteEnd();	//end drawing sprite

		//Finish rendering
		graphics->endScene();
	}

	handleLostGraphicsDevice();
	//Draw back buffer on window
	graphics->showBackbuffer();
}

//////////////////////////////////////////////////////////////////////////////
//If graphics devices lost, release all video memories to be able to reset graphics devices
//////////////////////////////////////////////////////////////////////////////
void Game::releaseAll()
{
	font.onLostDevice();
}

//////////////////////////////////////////////////////////////////////////////
//Recreate all surfaces and reset all entities
//////////////////////////////////////////////////////////////////////////////
void Game::resetAll()
{
	font.onResetDevice();
}

//////////////////////////////////////////////////////////////////////////////
//Delete all reseved memories
//////////////////////////////////////////////////////////////////////////////
void Game::deleteAll()
{
	releaseAll();	//Call onLostDevice() for every fraphics items
	SAFE_DELETE(graphics);
	SAFE_DELETE(input);
	initialized = false;
}