//You Oyadomari
//Kokusai Denshi Business College
//Initial 2017/5/30
//LastUpdate 2017/5/30

#include "TextDX.h"


TextDX::TextDX(void)
{
	 color = SETCOLOR_ARGB(255,255,255,255); // default to white font

    // set font position
    fontRect.top = 0;
    fontRect.left = 0;
    fontRect.right = GAME_WIDTH;
    fontRect.bottom = GAME_HEIGHT;
    dxFont = NULL;
    angle  = 0;
}


TextDX::~TextDX(void)
{
}

//////////////////////////////////////////////////////////////////////////////
//create DirectX font
//////////////////////////////////////////////////////////////////////////////
bool TextDX::initialize(Graphics *g, int height, bool bold, bool italic, const std::string &fontName)
{
	graphics = g;	//graphics system
	UINT weight = FW_NORMAL;
	if(bold)
		weight = FW_BOLD;
	
	//create DirectX font
	if(FAILED(D3DXCreateFont(graphics->get3Ddevice(),
		height,									//height of font (pixels)
		0,											//width of font (pixels)
		weight,									//thickness of font ///use font is bold
		1,											//MipMap level. always 1
		italic,									//if use italic font, true, else otherwise 
		DEFAULT_CHARSET,					//ditect char set from current system locale
		OUT_DEFAULT_PRECIS,				//windows how search match the font of interest with the actual font
		DEFAULT_QUALITY,					//if use raster font, windows how search match the font of interest with the actual font
													//do not effect TrueType font
		DEFAULT_PITCH | FF_DONTCARE,	//define Pitch and Famiry
													//pitch:		width of font (lower 2 bits)
													//famiry:	Font appearance (upper 4 bits)
		fontName.c_str(),					//font name
		&dxFont)))								//pointer to ID3DXFontInterface
		return false;
	//initialize transformation matrix
	D3DXMatrixTransformation2D(&matrix, NULL, 0.0f, NULL, NULL, 0.0f, NULL);
	return true;
}

//////////////////////////////////////////////////////////////////////////////
//output text at x, y
//if failed, return 0, else text height
//pre:	spriteBegin()
//post:	spriteEnd()
//////////////////////////////////////////////////////////////////////////////
int TextDX::print(const std::string &str, int x, int y)
{
	if(dxFont == NULL)
		return 0;
	//set font position
	fontRect.top = y;
	fontRect.left = x;
	//center of rotation
	D3DXVECTOR2 rCenter = D3DXVECTOR2((float)x, (float)y);
	//prepare matrix to rotate text
	D3DXMatrixTransformation2D(&matrix, NULL, 0.0f, NULL, &rCenter, angle, NULL);
	//appry matrix to sprite
	graphics->getSprite()->SetTransform(&matrix);
	return dxFont->DrawTextA(
		graphics->getSprite(),	//pointer to ID3DXSprite.
											//can set NULL, but then Direct3D use unique sprite object
											//if call DrawText() more than once, specify sprite object to Improve performance
		str.c_str(),					//pointer to render string
											//if Count is -1, must be NULL terminator
		-1,								//string length
											//if this is -1, pString must be NULL terminator
		&fontRect,						//define area on screen
		DT_LEFT,						//text fotmat, use to combine some define
		color);							//text color
}

//////////////////////////////////////////////////////////////////////////////
//output text in RECT using DirectX text format
//if failed, return 0, else text height
//pre:	spriteBegin()
//post:	spriteEnd()
//////////////////////////////////////////////////////////////////////////////
int TextDX::print(const std::string &str, RECT &rect, UINT format)
{
	if(dxFont == NULL)
		return 0;
	//prepare matrix to not rotate text
	D3DXMatrixTransformation2D(&matrix, NULL, 0.0f, NULL, NULL, NULL, NULL);
	//appry matrix to sprite
	graphics->getSprite()->SetTransform(&matrix);
	return dxFont->DrawTextA(
		graphics->getSprite(),	//pointer to ID3DXSprite.
											//can set NULL, but then Direct3D use unique sprite object
											//if call DrawText() more than once, specify sprite object to Improve performance
		str.c_str(),					//pointer to render string
											//if Count is -1, must be NULL terminator
		-1,								//string length
											//if this is -1, pString must be NULL terminator
		&rect,							//define area on screen
		format,							//text fotmat, use to combine some define
		color);							//text color
}

//////////////////////////////////////////////////////////////////////////////
//called when graphics device is lost
//////////////////////////////////////////////////////////////////////////////
void TextDX::onLostDevice()
{
	if(dxFont == NULL)
		return;
	dxFont->OnLostDevice();
}

//////////////////////////////////////////////////////////////////////////////
//called when graphics device is reset
//////////////////////////////////////////////////////////////////////////////
void TextDX::onResetDevice()
{
	if(dxFont == NULL)
		return;
	dxFont->OnLostDevice();
}