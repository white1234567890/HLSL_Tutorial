//You Oyadomari
//Kokusai Denshi business Vocational School
//Initial 2017/1/19
//LastUpdate 2017/2/2

#pragma once
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <windowsx.h>
#include <XInput.h>
#include "constants.h"
#include "GameError.h"

//for high-definition mouse

#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC ((USHORT) 0x01)
#endif // HID_USAGE_PAGE_GENERIC
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE ((USHORT) 0x02)
#endif // HID_USAGE_GENERIC_MOUSE

namespace inputNS
{
	const int KEYS_ARRAY_LEN = 256;

	//Set All keysDown array false
	const UCHAR KEYS_DOWN = 1;

	//Set all keysPressed array false
	const UCHAR KEYS_PRESSED = 2;

	//Set mouse X and Y 0
	const UCHAR MOUSE = 4;

	//Clear input text string
	const UCHAR TEXT_IN = 8;

	//Clear all input buffer
	const UCHAR KEYS_MOUSE_TEXT = KEYS_DOWN + KEYS_PRESSED + MOUSE + TEXT_IN;
}

const int MAX_CONTROLLERS = 1;

typedef struct ControllerState
{
	XINPUT_STATE state;
	XINPUT_VIBRATION vibration;
	float vibrationTimeLeft;	//Miri second
	float vibrationTimeRight;	//Miri second
	bool connected;
}ControllerState;

//Dead zone
//Left trigger, right trigger, and thumb stick are analog control
//Left trigger and right trigger return 0~255 BYTE type
//0 is released state, 255 is perfect pressed state
//Thumb stick return -32768~32768 signed SHORT type about each axis
//This value means thumb stick location, 0 is center, negative value is left or down, positive value is right or up
//If analog control is perfectly released, return not 0 sometimes
//This way, application needs dead zone if process analog data
//Dead zone is a simple threshold that defines an effective input
//As long as value exceeds dead zone, it is considered as an effective input
//««««««««««

//20% in area is default dead zone
const DWORD GAMEPAD_THUMBSTICK_DEADZONE = (DWORD)(0.20f * float(0X7FFFF));

//Trigger area is 0~255
const DWORD GAMEPAD_TRIGGER_DEADZONE = 30;

//ªªªªªªªªªª

//Bit correspond to state.Gamepad.wButtons
//wButtons has All buttons state
//Each button is defined as 1 bit in WORD type bits
//Specified button state is judged using AND operator(&) with binary number if corresponding bit is 1
//If calcuration result is true, the button is pressed
//««««««««««

const DWORD GAMEPAD_DPAD_UP = 0x0001;
const DWORD GAMEPAD_DPAD_DOWN = 0x0002;
const DWORD GAMEPAD_DPAD_LEFT = 0x0004;
const DWORD GAMEPAD_DPAD_RIGHT =0x0008;
const DWORD GAMEPAD_START_BUTTON = 0x0010;
const DWORD GAMEPAD_BACK_BUTTON = 0x0020;
const DWORD GAMEPAD_LEFT_THUMB = 0x0040;
const DWORD GAMEPAD_RIGHT_THUMB = 0x0080;
const DWORD GAMEPAD_LEFT_SHOULDER = 0x0100;
const DWORD GAMEPAD_RIGHT_SHOULDER = 0x0200;
const DWORD GAMEPAD_A = 0x1000;
const DWORD GAMEPAD_B = 0x2000;
const DWORD GAMEPAD_X = 0x4000;
const DWORD GAMEPAD_Y = 0x8000;

//ªªªªªªªªªª

class Input
{
private:  
	bool keysDown[inputNS::KEYS_ARRAY_LEN];				//If specified keys are pressed, this variable is true
	bool keysPressed[inputNS::KEYS_ARRAY_LEN];			//When specified keys are pressing, this variable is true
	std::string textIn;												//Text inputed by user
	char charIn;														//Last inputed charactor
	bool newLine;														//When start new line, this variable is true
	int mouseX,mouseY;												//Mouse location in window
	int mouseRawX,mouseRawY;									//High-definition mouse location in window
	RAWINPUTDEVICE Rid[1];										//For High definition mouse
	bool mouseCaptured;											//If mouse was captured, this variable is true
	bool mouseLButton;												//If mouse left button is pressed, this variable is true
	bool mouseMButton;												//If mouse middle button is pressed, this variable is true
	bool mouseRButton;												//If mouse right button is pressed, this variable is true
	bool mouseX1Button;											//If mouse X1 button is pressed, this variable is true
	bool mouseX2Button;											//If mouse X2 button is pressed, this vaiable is true
	ControllerState controllers[MAX_CONTROLLERS];	//Controller state	

public:
	Input(void);
	~Input(void);

	//Set array of RAWINPUTDEVICE Rid
	//Resolution of standard conputer mouse is 400 dpi(dot per inch), but high-definition mouse is more than 800 dpi
	//High-definition mouse can not get from default WM_MOUSEMOVE message
	//If necessary capture mouse, clear array stored controller state
	//If mouse is captured, mouse input send to capture window
	//This is necessary game that player move is controled by mouse
	//Standard mouse bihavior do not need mouse capture
	void initialize(HWND hwnd, bool capture);

	//Specified key is in the hold state
	void keyDown(WPARAM wParam);

	//Specified key is in the release state
	void keyUp(WPARAM wParam);

	//Store input key in textIn array
	void keyIn(WPARAM wParam);

	//Specified key is in the pressed state
	bool isKeyDown(UCHAR vkey) const;

	//Specified key is in the hold state
	bool wasKeyPressed(UCHAR vkey) const;

	//Any key is in the hold state
	bool anyKeyPressed(UCHAR vkey) const;

	//Mouse locatoin X and Y
	void mouseIn(LPARAM lParam);

	//High-definition mouse location X and Y
	//This function can call when user use high-definition mouse
	void mouseRawIn(LPARAM lParam);

	//Store mouse left button state
	void setMouseLButton(bool b){mouseLButton = b;}

	//Store mouse middle button state
	void setMouseMButton(bool b){mouseMButton = b;}

	//Store mouse right button state
	void setMouseRButton(bool b){mouseRButton = b;}

	//Store mouse X buttons state
	void setMouseXButton(WPARAM wParam)
	{
		mouseX1Button = (wParam & MK_XBUTTON1) ? true:false;
		mouseX2Button = (wParam & MK_XBUTTON2) ? true:false;
	}

	//Return mouse location X coodrinate
	int getMouseX() const {return mouseX;}

	//Return mouse location Y coordinate
	int getMouseY() const {return mouseY;}

	//Return mouse moveing raw data X coordinate
	//If move left, return value less than 0, move right, return value more than 0
	//This function can call if user use high-definition mouse
	int getMouseRawX() const {return mouseRawX;}

	//Return mouse moveing raw data y coordinate
	//If move up, return value less than 0, move down, return value more than 0
	//This function can call if user use high-definition mouse
	int getMouseRawY() const {return mouseRawY;}

	//Return mouse left button state
	bool getMouseLButton() const {return mouseLButton;}

	//Return mouse middle button state
	bool getMouseMButton() const {return mouseMButton;}

	//Return mouse right button state
	bool getMouseRButton() const {return mouseRButton;}

	//Return mouse X1 button state
	bool getMouseX1Button() const {return mouseX1Button;}

	//Return mouse X2 button state
	bool getMouseX2Button() const {return mouseX2Button;}

	//Return input text as string
	std::string getTextIn(){return textIn;}

	//Return input last charactor
	char getCharIn(){return charIn;}

	//Clear specified key pressed state
	void clearKeyPress(UCHAR vkey);

	//Clear text buffer
	void clearTextIn(){textIn.clear();}
	
	//Clear specified input buffer
	//Param what : call from input_NS, and conbine any item to use OR operator(|) 
	//Example : input->clear(inputNS::KEYS_DOWN | input_NS::KEYS_PRESSED)
	void clear(UCHAR what);

	//Clear data keys, mouse, and input text state
	void clearAll(){clear(inputNS::KEYS_MOUSE_TEXT);}

	void checkControllers();

	//Read each controller state, and store them in controllers array
	//This function is added in the Game::run function to be called automatically in the game loop
	void readControllers();

	//Return specified controller state
	//This function return controller all state
	const ControllerState* getControllerState(UINT n)
	{
		if(n > MAX_CONTROLLERS - 1) n = MAX_CONTROLLERS - 1;
		return &controllers[n];
	}

	//Return specified controller state
	//This function retutn controller button state
	const WORD getGamepadButtons(UINT n)
	{
		if(n > MAX_CONTROLLERS - 1) n = MAX_CONTROLLERS - 1;
		return controllers[n].state.Gamepad.wButtons;
	}

	//Return value of controller n left trigger
	BYTE getGamepadLeftTrigger(UINT n)
	{
		if(n > MAX_CONTROLLERS - 1) n = MAX_CONTROLLERS - 1;
		return controllers[n].state.Gamepad.bLeftTrigger;
	}

	//Return value of controller n right trigger
	BYTE getGamepadRightTrigger(UINT n)
	{
		if(n > MAX_CONTROLLERS - 1) n = MAX_CONTROLLERS - 1;
		return controllers[n].state.Gamepad.bRightTrigger;
	}

	//Return value of controller n left thumb stick X axis
	SHORT getGamepadThumbLX(UINT n)
	{
		if(n > MAX_CONTROLLERS - 1) n = MAX_CONTROLLERS - 1;
		return controllers[n].state.Gamepad.sThumbLX;
	}

	//Return value of controller n left thumb stick Y axis
	SHORT getGamepadThumbLY(UINT n)
	{
		if(n > MAX_CONTROLLERS - 1) n = MAX_CONTROLLERS - 1;
		return controllers[n].state.Gamepad.sThumbLY;
	}

	//Return value of controller n right thumb stick X axis
	SHORT getGamepadThumbRX(UINT n)
	{
		if(n > MAX_CONTROLLERS - 1) n = MAX_CONTROLLERS - 1;
		return controllers[n].state.Gamepad.sThumbRX;
	}

	//Return value of contoller n right thumb stick Y axis
	SHORT getGamepadThumbRY(UINT n)
	{
		if(n > MAX_CONTROLLERS - 1) n = MAX_CONTROLLERS - 1;
		return controllers[n].state.Gamepad.sThumbRY;
	}

	//Vibration
	//Xbox360 controller has motors left and right
	//Left motor vibrates at a low frequency
	//Right motor vibrates at a high frequency
	//Each motor's speed can be set to substitute vibration member of ControllerState struct 0 ~ 65,535 value
	//0 indicates that motor is disabled
	//65,535 indicates maximum motor use
	//It is necessary to control vibration speed and vibration time
	//These functions set each motor's turnover rate and rotation time
	//««««««««««

	//Vibrates controller n's left motor
	//Left motor is low frequency
	//speed : 0 is off, 65536 is full use
	//sec : vibration time
	void gamepadVibrateLeft(UINT n, WORD speed, float sec)
	{
		if(n > MAX_CONTROLLERS - 1) n = MAX_CONTROLLERS - 1;
		controllers[n].vibration.wLeftMotorSpeed = speed;
		controllers[n].vibrationTimeLeft = sec;
	}

	//Viberates controller n's right motor
	//Right motor is high frequency
	//speed : 0 is off, 65536 is full use
	//sec : vibration time
	void gamepadVibrateRight(UINT n, WORD speed, float sec)
	{
		if(n > MAX_CONTROLLERS - 1) n = MAX_CONTROLLERS - 1;
		controllers[n].vibration.wRightMotorSpeed = speed;
		controllers[n].vibrationTimeRight = sec;
	}

	//Process vibration of all controllers
	//This function is added in the Game::run function to be called automatically in the game loop
	//frameTime : delta time to contorl vibration time
	//If vibration time is negative value, motor's speed and vibration time are substituted 0
	void vibrateControllers(float frameTime);

	//ªªªªªªªªªª

	//Test function
	//««««««««««

	//Return direction pad up of controller n
	//If button is down, return true
	bool getGamepadDPadUp(UINT n)
	{
		if(n > MAX_CONTROLLERS - 1) n = MAX_CONTROLLERS - 1;
		return ((controllers[n].state.Gamepad.wButtons & GAMEPAD_DPAD_UP) != 0);
	}

	//Return direction pad down of controller n
	//If button is down, return true
	bool getGamepadDPadDown(UINT n)
	{
		if(n > MAX_CONTROLLERS - 1) n = MAX_CONTROLLERS - 1;
		return ((controllers[n].state.Gamepad.wButtons & GAMEPAD_DPAD_DOWN) != 0);
	}

	//Return direction pad left of controller n
	//If button is down, return true
	bool getGamepadDPadLeft(UINT n)
	{
		if(n > MAX_CONTROLLERS - 1) n = MAX_CONTROLLERS - 1;
		return ((controllers[n].state.Gamepad.wButtons & GAMEPAD_DPAD_LEFT) != 0);
	}

	//Return direction pad right of controller n
	//If button is down, return true
	bool getGamepadDPadRight(UINT n)
	{
		if(n > MAX_CONTROLLERS - 1) n = MAX_CONTROLLERS - 1;
		return ((controllers[n].state.Gamepad.wButtons & GAMEPAD_DPAD_RIGHT) != 0);
	}

	//Return START button of controller n
	//If button is down, return true
	bool getGamepadStart(UINT n)
	{
		if(n > MAX_CONTROLLERS - 1) n = MAX_CONTROLLERS - 1;
		return ((controllers[n].state.Gamepad.wButtons & GAMEPAD_START_BUTTON) != 0);
	}

	//Return BACK button of controller n
	//If button is down, return true
	bool getGamepadBack(UINT n)
	{
		if(n > MAX_CONTROLLERS - 1) n = MAX_CONTROLLERS - 1;
		return ((controllers[n].state.Gamepad.wButtons & GAMEPAD_BACK_BUTTON) != 0);
	}

	//Return left thumb button of controller n
	//If button is down, return true
	bool getGamepadLeftThumb(UINT n)
	{
		if(n > MAX_CONTROLLERS - 1) n = MAX_CONTROLLERS - 1;
		return ((controllers[n].state.Gamepad.wButtons & GAMEPAD_LEFT_THUMB) != 0);
	}

	//Return right thumb button of controller n
	//If button is down, return true
	bool getGamepadRightThumb(UINT n)
	{
		if(n > MAX_CONTROLLERS - 1) n = MAX_CONTROLLERS - 1;
		return ((controllers[n].state.Gamepad.wButtons & GAMEPAD_RIGHT_THUMB) != 0);
	}

	//Return left shoulder button of controller n
	//If button is down, return true
	bool getGamepadLeftShoulder(UINT n)
	{
		if(n > MAX_CONTROLLERS - 1) n = MAX_CONTROLLERS - 1;
		return ((controllers[n].state.Gamepad.wButtons & GAMEPAD_LEFT_SHOULDER) != 0);
	}

	//Return right shoulder button of controller n
	//If button is down, return true
	bool getGamepadRightShoulder(UINT n)
	{
		if(n > MAX_CONTROLLERS - 1) n = MAX_CONTROLLERS - 1;
		return ((controllers[n].state.Gamepad.wButtons & GAMEPAD_RIGHT_SHOULDER) != 0);
	}

	//Return A button of controller n
	//If button is down, return true
	bool getGamepadA(UINT n)
	{
		if(n > MAX_CONTROLLERS - 1) n = MAX_CONTROLLERS - 1;
		return ((controllers[n].state.Gamepad.wButtons & GAMEPAD_A) != 0);
	}

	//Return B button of controller n
	//If button is down, return true
	bool getGamepadB(UINT n)
	{
		if(n > MAX_CONTROLLERS - 1) n = MAX_CONTROLLERS - 1;
		return ((controllers[n].state.Gamepad.wButtons & GAMEPAD_B) != 0);
	}

	//Return X button of controller n
	//If button is down, return true
	bool getGamepadX(UINT n)
	{
		if(n > MAX_CONTROLLERS - 1) n = MAX_CONTROLLERS - 1;
		return ((controllers[n].state.Gamepad.wButtons & GAMEPAD_X) != 0);
	}

	//Return Y button of controller n
	//If button is down, return true
	bool getGamepadY(UINT n)
	{
		if(n > MAX_CONTROLLERS - 1) n = MAX_CONTROLLERS - 1;
		return ((controllers[n].state.Gamepad.wButtons & GAMEPAD_Y) != 0);
	}
	//ªªªªªªªªªª
};