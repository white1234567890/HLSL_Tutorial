////Author You Oyadomari
////Kokusai Denshi Business Vocational School
////Initial 2017/1/11
////Last update 2017/2/2

#define WIN32_LEAN_AND_MEAN
#define SHIFTED 0x8000

//To detect memory leak
//Å´Å´Å´Å´Å´Å´Å´Å´Å´Å´
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
//Å™Å™Å™Å™Å™Å™Å™Å™Å™Å™

#include "HLSLLaboratory.h"

//Prototype function
int WINAPI WinMain(HINSTANCE , HINSTANCE , LPSTR , int);
bool CreateMainWindow(HWND &, HINSTANCE , int);
LRESULT WINAPI WinProc(HWND , UINT , WPARAM , LPARAM);
bool AnotherInstance();


HLSLLaboratory *game = NULL;	//Game Pointer
HWND hwnd = NULL;

//Start windows apprication
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow )
{
	//If build by debug mode, monitor memory leak
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	if(AnotherInstance()) PostQuitMessage(0);

	MSG msg;

	//Create the game and sets up message handler
	game = new HLSLLaboratory;

	//Create window
	if(!CreateMainWindow(hwnd, hInstance , nCmdShow)) return 1;

	try
	{
		game->initialize(hwnd);	//Throw GameError

	//Main loop
	int done = 0;

	//Main message loop
	while(!done)
	{
		//Check PeelMessage , WindowMessage
		if(PeekMessage(&msg , NULL , 0 , 0 , PM_REMOVE))
		{
			//Detect quit message
			if(msg.message == WM_QUIT) done = 1;

			//Decode message and translate WinProc
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			game->run(hwnd);
		}
	}

	//Release memory before finishong loop
	SAFE_DELETE(game);
	return msg.wParam;
	}

	catch(const GameError &err)
	{
		game->deleteAll();
		DestroyWindow(hwnd);
		MessageBox(NULL, err.getMessage(), "Error", MB_OK);
	}

	catch(...)
	{
		game->deleteAll();
		DestroyWindow(hwnd);
		MessageBox(NULL, "Unknown error occurred in game", "Error", MB_OK);
	}

	//Release memory before force shut down
	SAFE_DELETE(game);

	return 0;
}

//Window event callback function
LRESULT WINAPI WinProc(HWND hwnd , UINT msg , WPARAM wParam , LPARAM lParam)
{
	return (game->messageHandler(hwnd, msg, wParam, lParam));
}

//If error return : false
bool CreateMainWindow(HWND &hwnd, HINSTANCE hInstance , int nCmdShow)
{
	WNDCLASSEX wcx;

	//Set WNDCLASSEX to use main window
	wcx .cbSize = sizeof(wcx);													//Struct size
	wcx.style = CS_HREDRAW | CS_VREDRAW;									//Redraw when window size was changed
	wcx.lpfnWndProc = WinProc;													//Pointer to window procedure
	wcx.cbClsExtra = 0;																//Extension class memory is none
	wcx.cbWndExtra = 0;																//Extension window memory is none
	wcx.hInstance = hInstance;													//Handle to instance
	wcx.hIcon = NULL;
	wcx.hCursor = LoadCursor(NULL , IDC_ARROW);							//Default cursol
	wcx.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);	//Brack back screen
	wcx.lpszMenuName = NULL;														//Menu resource name
	wcx.lpszClassName = CLASS_NAME;											//Window class name
	wcx.hIconSm = NULL;																//Small icon

	//Register window class
	//If Error RegisterClassEx return 0
	if(RegisterClassEx(&wcx) == 0) return false;

	//Set window or fullscreen
	DWORD style;
	if(FULLSCREEN)
	{
		style = WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP;
	}
	else
	{
		style = WS_OVERLAPPEDWINDOW;
	}

	//Create window
	hwnd = CreateWindow(
		CLASS_NAME,				//Window class name
		GAME_TITLE,					//Title ber
		style,						//Window style
		CW_USEDEFAULT,			//Default of window horizontal location
		CW_USEDEFAULT,			//Default of window vertical locaiton
		SCREEN_WIDTH,				//Window horizontal size
		SCREEN_HEIGHT,				//Window vertical size
		(HWND)NULL,				//Nothing parent window
		(HMENU)NULL,				//Nothing menu
		hInstance,					//Handle to apprication instance
		(LPVOID)NULL);			//Nothing window parameter

	//If error to create window
	if(!hwnd) return false;

	//Process that window
	if (!FULLSCREEN)
	{
		//Adjust window size that crient area siza is GAME_WIDTH * GAME_HEIGHT
		RECT clientRect;

		//Get client size
		GetClientRect(hwnd, &clientRect);

		//Redraw window
		MoveWindow(hwnd,
			0,																				//Left
			0,																				//Top
			SCREEN_WIDTH+ (SCREEN_WIDTH - clientRect.right),		//Right
			SCREEN_HEIGHT + (SCREEN_HEIGHT - clientRect.bottom),	//Bottom
			TRUE);
	}

	//Draw window
	ShowWindow(hwnd , nCmdShow);

	return true;
}

//This apprecation is running already
//If Discover another instance : return true
//else : return false
bool AnotherInstance()
{
	HANDLE ourMutex;

	//Try to create mutex in unique string
	ourMutex = CreateMutex(NULL, true,	"Use_a_different_string_here_for_each_program_48161-XYZZY");
	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		//Release mutex
		ReleaseMutex(ourMutex);
		//Discover another instance
		return true;
	}

	//This is only instance
	return false;
}