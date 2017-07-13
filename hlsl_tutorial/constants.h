#pragma once
//Author You Oyadomari
//Kokusai Denshi Business Vocational School
//Initial 2017/1/11
//Last update 2017/2/16

#include <Windows.h>
#include <d3dx9.h>

//////////////////////////////////////////////////////////////////////////////
// Function templates for safely dealing with pointer referenced items.
// The functions defined by these templates may be called using a normal
// function call syntax. The compiler will create a function that replaces T
// with the type of the calling parameter.
//////////////////////////////////////////////////////////////////////////////
// Safely release pointer referenced item
template <typename T>
inline void safeRelease(T& ptr)
{
    if (ptr)
    { 
        ptr->Release(); 
        ptr = NULL;
    }
}
#define SAFE_RELEASE safeRelease            // for backward compatiblility

// Safely delete pointer referenced item
template <typename T>
inline void safeDelete(T& ptr)
{
    if (ptr)
    { 
        delete ptr; 
        ptr = NULL;
    }
}
#define SAFE_DELETE safeDelete              // for backward compatiblility

#define TRANSCOLOR SETCOLOR_ARGB(0,255,0,255)	//transparant color (magenta)
#define VECTOR2 D3DXVECTOR2	//define vector2
#define VECTOR3 D3DXVECTOR3	//define vector3
#define LP_DXFONT LPD3DXFONT	//font
#define noDEBUG


struct Matrix2x2
{
	VECTOR2 vector1;
	VECTOR2 vector2;

	//constructor
	Matrix2x2()
	{
		vector1 = VECTOR2();
		vector2 = VECTOR2();
	}

	//argument constructor
	Matrix2x2(const VECTOR2 &v1, const VECTOR2 &v2)
	{
		vector1 = v1;
		vector2 = v2;
	}

	//operator overload
	//Å´Å´Å´Å´Å´Å´Å´Å´Å´Å´

	//Matrix2x2 + Matrix2x2
	Matrix2x2 operator + (const Matrix2x2 &mat) const
	{
		Matrix2x2 ret;
		ret.vector1 = this->vector1 + mat.vector1;
		ret.vector2 = this->vector2 + mat.vector2;
		return ret;
	}

	//Matrix2x2 += Matrix2x2
	Matrix2x2& operator += (const Matrix2x2 &mat)
	{
		this->vector1 += mat.vector1;
		this->vector2 += mat.vector2;
		return *this;
	}
	
	//Matrix2x2 * float
	Matrix2x2 operator * (const float &val) const
	{
		Matrix2x2 ret;
		ret.vector1 = this->vector1 * val;
		ret.vector2 = this->vector2 * val;
		return ret;
	}

	//Matrix2x2 * float
	Matrix2x2& operator *= (const float &val)
	{
		this->vector1 *= val;
		this->vector2 *= val;
		return *this;
	}

	Matrix2x2 operator * (const Matrix2x2 &mat) const
	{
		Matrix2x2 ret;
		ret.vector1.x = this->vector1.x * mat.vector1.x + this->vector2.x * mat.vector1.y;
		ret.vector1.y = this->vector1.y * mat.vector1.x + this->vector2.y * mat.vector1.y;
		ret.vector2.x = this->vector1.x * mat.vector2.x + this->vector2.x * mat.vector2.y;
		ret.vector2.y = this->vector1.y * mat.vector2.x + this->vector2.y * mat.vector2.y;
		return ret;
	}

	Matrix2x2& operator *= (const Matrix2x2 &mat)
	{
		this->vector1.x = this->vector1.x * mat.vector1.x + this->vector2.x * mat.vector1.y;
		this->vector1.y = this->vector1.y * mat.vector1.x + this->vector2.y * mat.vector1.y;
		this->vector2.x = this->vector1.x * mat.vector2.x + this->vector2.x * mat.vector2.y;
		this->vector2.y = this->vector1.y * mat.vector2.x + this->vector2.y * mat.vector2.y;
		return *this;
	}

	VECTOR2 operator * (const VECTOR2 &vec) const
	{
		VECTOR2 ret;
		ret.x = this->vector1.x * vec.x + this->vector2.x * vec.y;
		ret.y = this->vector1.y * vec.x + this->vector2.y * vec.y;
		return ret;
	}

	//Å™Å™Å™Å™Å™Å™Å™Å™Å™Å™

	//return determinant matrix
	float determinant() const
	{
		return (this->vector1.x * this->vector2.y - this->vector1.y * this->vector2.x);
	}

	//return inverse matrix
	Matrix2x2 inverse() const
	{
		Matrix2x2 ret;
		float det = determinant();
		ret.vector1.x = this->vector2.y / det;
		ret.vector1.y = -this->vector1.y / det;
		ret.vector2.x = -this->vector2.x / det;
		ret.vector2.y = this->vector1.x / det;

		return ret;
	}
};

//create 
struct Matrix3x3
{
	VECTOR3 vector1;
	VECTOR3 vector2;
	VECTOR3 vector3;
	
	//constructor
	Matrix3x3()
	{
		vector1 = VECTOR3();
		vector2 = VECTOR3();
		vector3 = VECTOR3();
	}

	//argument constructor
	Matrix3x3(VECTOR3 v1, VECTOR3 v2, VECTOR3 v3)
	{
		vector1 = v1;
		vector2 = v2;
		vector3 = v3;
	}

	//operator overload
	//Å´Å´Å´Å´Å´Å´Å´Å´Å´Å´

	//Matrix3x3 + Matrix3x3
	Matrix3x3 operator + (const Matrix3x3 &mat) const
	{
		Matrix3x3 ret;
		ret.vector1 = this->vector1 + mat.vector1;
		ret.vector2 = this->vector2 + mat.vector2;
		ret.vector3 = this->vector3 + mat.vector3;
		return ret;
	}

	//Matrix3x3 += Matrix3x3
	Matrix3x3& operator += (const Matrix3x3 &mat)
	{
		this->vector1 += mat.vector1;
		this->vector2 += mat.vector2;
		this->vector3 += mat.vector3;
		return *this;
	}
	
	//Matrix3x3 * float
	Matrix3x3 operator * (const float &val) const
	{
		Matrix3x3 ret;
		ret.vector1 = this->vector1 * val;
		ret.vector2 = this->vector2 * val;
		ret.vector3 = this->vector3 * val;
		return ret;
	}

	//Matrix3x3 *= float
	Matrix3x3& operator *= (const float &val)
	{
		this->vector1 *= val;
		this->vector2 *= val;
		this->vector3 *= val;
		return *this;
	}

	Matrix3x3 operator * (const Matrix3x3 &mat) const
	{
		Matrix3x3 ret;
		ret.vector1.x = this->vector1.x * mat.vector1.x + this->vector2.x * mat.vector1.y + this->vector3.x * mat.vector1.z;
		ret.vector1.y = this->vector1.y * mat.vector1.x + this->vector2.y * mat.vector1.y + this->vector3.y * mat.vector1.z;
		ret.vector1.z = this->vector1.z * mat.vector1.x + this->vector2.z * mat.vector1.y + this->vector3.z * mat.vector1.z;
		ret.vector2.x = this->vector1.x * mat.vector2.x + this->vector2.x * mat.vector2.y + this->vector3.x * mat.vector2.z;
		ret.vector2.y = this->vector1.y * mat.vector2.x + this->vector2.y * mat.vector2.y + this->vector3.y * mat.vector2.z;
		ret.vector2.z = this->vector1.z * mat.vector2.x + this->vector2.z * mat.vector2.y + this->vector3.z * mat.vector2.z;
		ret.vector3.x = this->vector1.x * mat.vector3.x + this->vector2.x * mat.vector3.y + this->vector3.x * mat.vector3.z;
		ret.vector3.y = this->vector1.y * mat.vector3.x + this->vector2.y * mat.vector3.y + this->vector3.y * mat.vector3.z;
		ret.vector3.z = this->vector1.z * mat.vector3.x + this->vector2.z * mat.vector3.y + this->vector3.z * mat.vector3.z;
		return ret;
	}

	VECTOR3 operator * (const VECTOR3 &vec) const
	{
		VECTOR3 ret;
		ret.x = this->vector1.x * vec.x + this->vector2.x * vec.y + this->vector3.x * vec.z;
		ret.y = this->vector1.y * vec.x + this->vector2.y * vec.y + this->vector3.y * vec.z;
		ret.z = this->vector1.z * vec.x + this->vector2.z * vec.y + this->vector3.z * vec.z;
		return ret;
	}

	//Å™Å™Å™Å™Å™Å™Å™Å™Å™Å™

	//return determinant matrix
	float determinant() const
	{
		return (this->vector1.x * this->vector2.y * this->vector3.z + 
			this->vector2.x * this->vector3.y * this->vector1.z +
			this->vector3.x * this->vector1.y * this->vector2.z -
			this->vector3.x * this->vector2.y * this->vector1.z -
			this->vector1.x * this->vector3.y * this->vector2.z -
			this->vector2.x * this->vector1.y * this->vector3.z);
	}

	//return inverse matrix
	Matrix3x3 inverse() const
	{
		Matrix3x3 ret;
		float det = determinant();
		ret.vector1.x = (this->vector2.y * this->vector3.z - this->vector3.y * this->vector2.z) / det;
		ret.vector1.y = (this->vector3.y * this->vector1.z - this->vector1.y * this->vector3.z) / det;
		ret.vector1.z = (this->vector1.y * this->vector2.z - this->vector2.y * this->vector1.z) / det;
		ret.vector2.x = (this->vector3.x * this->vector2.z - this->vector2.x * this->vector3.z) / det;
		ret.vector2.y = (this->vector1.x * this->vector3.z - this->vector3.x * this->vector1.z) / det;
		ret.vector2.z = (this->vector2.x * this->vector1.z - this->vector1.x * this->vector2.z) / det;
		ret.vector3.x = (this->vector2.x * this->vector3.y - this->vector3.x * this->vector2.y) / det;
		ret.vector3.y = (this->vector3.x * this->vector1.y - this->vector1.x * this->vector3.y) / det;
		ret.vector3.z = (this->vector1.x * this->vector2.y - this->vector2.x * this->vector1.y) / det;
		return ret;
	}
};

//////////////////////////////////////////////////////////////////////////////
//Constant
//////////////////////////////////////////////////////////////////////////////

//Window
const char CLASS_NAME[] = "ReCreation";
const char GAME_TITLE[] = "ReCreation";
const bool FULLSCREEN = false;								//Default is Window , or set fullscreen
static int GAME_WIDTH = 640;									//Game width(pixels)
static int GAME_HEIGHT = 480;									//Game height(pixels)
static int SCREEN_WIDTH = 960;								//screen width(pixels)
static int SCREEN_HEIGHT = 640;								//screen height(pixels)
const int VANISH_AREA_TOP = -128;								//over this area, vanish bullet
const int VANISH_AREA_LEFT = -128;
const int VANISH_AREA_RIGHT = GAME_WIDTH + 128;
const int VANISH_AREA_BOTTOM = GAME_HEIGHT + 128;	

//Game
const double PI = 3.14159265;
const float FRAME_RATE = 1000.0f;									//The target frame rate
const float MIN_FRAME_RATE = 10.0f;								//The minimum frame rate
const float MIN_FRAME_TIME = 1.0f / FRAME_RATE;			//Minimum desired time for 1 frame
const float MAX_FRAME_TIME = 1.0f / MIN_FRAME_RATE;	//Maximum time used in calculations
const float SCROLL_SPEED = 100.0f;								//scroll speed of back groung image

//graphics images
const char PLAYER_IMAGE[] = "textures\\player.png";							//picture of player
const char PARTS_IMAGE[] = "textures\\parts.png";								//picture of parts
const char BULLET_IMAGE[] = "textures\\bullet.png";							//picture of bullet
const char EXPLOSION_IMAGE[] = "textures\\explosion.png";					//plcture of explosion
const char BACK_GROUND_IMAGE[] = "textures\\back_ground_image_dark.bmp";	//picture of back ground image

//audio source
//WAVE_BANK is location of .xwb
const char WAVE_BANK[] = "audio\\Wave Bank.xwb";
//SOUND_FILE is location of .xsb
const char SOUND_BANK[] = "audio\\Sound Bank.xsb";
//sound cue
const char MAINBGM[] = "gamemain";

//Key mapping
//This game use simple constant value to key mapping
//If use variable, save key mapping to data file, can reuse it
//Å´Å´Å´Å´Å´Å´Å´Å´Å´Å´

const UCHAR ESC_KEY = VK_ESCAPE;			//Esc key
const UCHAR ALT_KEY = VK_MENU;				//Alt ley
const UCHAR ENTER_KEY = VK_RETURN;			//Enter key
const UCHAR LEFT_KEY = VK_LEFT;				//Left arrow key
const UCHAR RIGHT_KEY = VK_RIGHT;			//Right arrow key
const UCHAR UP_KEY = VK_UP;					//Up arrow key
const UCHAR DOWN_KEY = VK_DOWN;				//Down arrow key
const UCHAR PLAYER_FIRE_KEY = 'Z';			//Z key
const UCHAR PLAYER_BOMB_KEY = VK_SPACE;	//space key
const UCHAR SHOULDER_L_CHANGE_KEY = 'X';	//X key
const UCHAR SHOULDER_R_CHANGE_KEY = 'C';	//C key
//Å™Å™Å™Å™Å™Å™Å™Å™Å™Å™

//weapon types
enum WEAPON : BYTE
{
	PLAYER,
	ENEMY,
	PLAYER_BULLET_STRAIGHT,
	PLAYER_BULLET_ROTATION,
	PLAYER_BULLET_HOMING,
	PLAYER_BULLET_PENETRATE,
	PLAYER_BULLET_BOMB,
	PUTIT_FLARE,
	HYPERNOVA,
	BIG_BANG,
	ENEMY_BULLET};

//event list
namespace eventNS
{
	enum EVENT_LIST
	{
		SCENE_CHANGE,
		PLAYER_DIE,
		PLAYER_SCALE_CHANGE,
		KILL_ENEMY,
		KILL_BOSS,
		AVOID_BULLET,
		HIT_BULLET_TO_PLAYER,
		HIT_BULLET_TO_ENEMY,
	};
}