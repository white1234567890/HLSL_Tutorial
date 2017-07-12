//You Oyadomari
//Kokusai Denshi Business Vocational School
//Initial 2017/1/19
//LastUpdate 2017/4/4

#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <mmsystem.h>
#include <memory>

//My class
#include "Graphics.h"
#include "Input.h"
#include "constants.h"
#include "GameError.h"
#include "TextDX.h"
#include "FileLoader.h"
//#include "TextureManager.h"

namespace gameNS
{
	const char FONT[] = "‚l‚r ƒSƒVƒbƒN";													//font
																									//usable "‚l‚r ƒSƒVƒbƒN", "‚l‚r –¾’©", "‚l‚r –¾’©", "HGPºŞ¼¯¸E", "HGPºŞ¼¯¸M", "HGs‘‘Ì"
	const int POINT_SIZE_DEBUG = 14;													//point size to debug consol
	const int POINT_SIZE = 18;															//point size
	const COLOR_ARGB FONT_COLOR = graphicsNS::BLACK;						//black
	const COLOR_ARGB FONT_COLOR_DEBUG = graphicsNS::WHITE;				//white
	
	const int PARTS_NUM = 3;

	namespace textBoxNS
	{
		const char TEXT_BOX[] = "textures\\textbox.png";
		const int WIDTH = 512;				//image width
		const int HEIGHT = 128;			//image height
		const int TEXTURE_COLS = 1;	//texture cols
		const int BORDER_WIDTH = 16;		//border width
	}
}

//Game class contains variables and functions commonly used in game
//Game class is base class to inherite in main class used in game
//Game class contains Input class object and Graphics class object
//So, This class contains functions drawing graphics and getting input from keyboard
class Game
{
protected:
	//Common game property
	Graphics *graphics;					//Graphics pointer
	Input *input;								//Input pointer
	TextDX font;								//font for draw string
	HWND hwnd;									//Window handle
	HRESULT hr;								//Default return value
	LARGE_INTEGER timeStart;			//Performance counter initial value
	LARGE_INTEGER timeEnd;				//Performance counter last value
	LARGE_INTEGER timerFreq;			//Performance counter frequency
	float frameTime;						//Last frame time
	float fps;									//Frame rate (frame per a second)
	bool fpsOn;								//if true, draw frame rate, else otherwise
	DWORD sleepTime;						//Sleep time between frame and frame
	bool paused;								//If game is paused, true
	bool initialized;						//If game is initialized, true
	FileLoader *fileLoader;				//fileloader

public:
	//Constructor
	//Constructor create Input class object to access keyboard as soon as
	//Then, call input->initialize and finish input preparation to enable accessing to mouse
	Game(void);

	//Destructor
	~Game(void);

	//Member function
	//««««««««««

	//Windows message handler
	//This function make accessing to function in Input class object easy
	//Keyboard and mouse are processed through Windows event
	//When Windows detect keyboard event or mouse event, this function is called to process message
	//Then, messageHandler function call proper function in Input class object, and process input data
	//wParam : store key data
	//lParam : store mouse data
	LRESULT messageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	//Create Graphics object, and initialize graphics
	//Then, call initialize() frome input object, prepare mouse input
	//Last, prepare high-definition mouse to control speed of game items
	//QuaryPerformanveFrequency(&timerFreq) store high-definition timer's frequency to timerFreq
	//QuaryPerformanceCounter(&timeStart) store high-definition timer's count now to timeStart
	//Initialize game 
	//Before run : hwnd is handle to window
	virtual void initialize(HWND hwnd);

	//This function call QueryPerfoemanceCounter(&timeEnd) after check an effective graphics pointer
	//It store now count from high-definition performance counter to timeEnd
	
	//Calculate time between instance when timer is called and start counting
	//Calculation details	
	//			frameTime = (timeEnd - timeStart) / timerFreq
	//This value is elapsed time that is Extremelu accurate measured time between instance when gameloop is called and start game loop
	//Elapsed time value is saved in frameTime
	//frameTime is used to control animation speed
	//This code reads high-definition performance timer per game loop
	//So, frameTime is the elapsed time between the previous game loop and current game loop
	//There is another parttern that frameTime is defined time to render 1 frame
	//This value is obtained by substructing end of game loop from start of game loop
	//This method is not considered time to use Windows OS or other applications caluculating
	//So, this value is hard to say, accurate
	//This function is recurring in WinMain message loop
	virtual void run(HWND);

	//When graphic device vanished, call this function
	//Release all video memories to be available reset graphic device
	virtual void releaseAll();

	//Recreate all surface, and reset all entity
	virtual void resetAll();

	//Release all memories
	virtual void deleteAll();

	//Call render() function defining in inherited class from start graphics scene
	//render() function is pure virtual function, so need to rewritten in inherited class
	//render() function is actually drawing game graphics
	//After call render() function, quit graphics scene
	//Then, test graphics device was lost, and do appropriate process
	//Last, display back buffer 
	//Render game item
	virtual void renderGame();

	//If lost graphics device, restore it
	//Checking lost graphics device and processing it execute in this function
	virtual void handleLostGraphicsDevice();

	//Return pointer to Graphics
	Graphics* getGraphics(){return graphics;}

	//Return pointer to Input
	Input* getInput(){return input;}

	//return pointer to text
	TextDX* getText(){return &font;}

	//Exit game
	void exitGame(){PostMessage(hwnd, WM_DESTROY, 0, 0);}

	//Pure virtual function
	//Pure virtual function works like a place holder
	//These functions need to be overridden in class to be inherited in
	//Game class does not know this game's detail, these function is pure virtual function
	//««««««««««

	//Update game item
	//This function need to override
	virtual void update() = 0;

	//Run AI
	//This function need to override
	virtual void ai() = 0;

	//Check collisions
	//This function need to override
	virtual void collisions() = 0;

	//Render graphics
	//Call graphics->spriteBegin();
	//Render sprite
	//Call graphics->spriteEnd();
	//Render others
	//This function need to override
	virtual void render() = 0;

	//ªªªªªªªªªª

	//ªªªªªªªªªª
};