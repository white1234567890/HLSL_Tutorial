//Author You Oyadomari
//Kokusai Denshi Business Vocational School
//Initial 2017/1/11
//Last update 2017/2/20

#include "Graphics.h"

//////////////////////////////////////////////////////////////////////////////
//Constructor
//////////////////////////////////////////////////////////////////////////////
Graphics::Graphics(void)
{
	direct3d = NULL;
	device3d = NULL;
	fullscreen = false;
	//Width and height can replace initialize()
	width = SCREEN_WIDTH;
	height = SCREEN_HEIGHT;
	backColor = graphicsNS::BACK_COLOR;
}

//////////////////////////////////////////////////////////////////////////////
//Destructor
//////////////////////////////////////////////////////////////////////////////
Graphics::~Graphics(void)
{
	releaseAll();
}

//Private function
//««««««««««

//////////////////////////////////////////////////////////////////////////////
//Initialize D3DPresentatoinParameter
//////////////////////////////////////////////////////////////////////////////
void Graphics::initD3Dpp()
{
	try
	{
		//Set 0 in struct
		ZeroMemory(&d3dpp, sizeof(d3dpp));

		//Set necessary parameter
		//««««««««««
		d3dpp.BackBufferWidth = width;
		d3dpp.BackBufferHeight = height;
		
		//Process full screen
		if(fullscreen)
		{
			//Set 24 bit color
			d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
		}
		else
		{
			//Use desctop configuration
			d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
		}
		
		d3dpp.BackBufferCount = 1;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.hDeviceWindow = hwnd;
		d3dpp.Windowed = (!fullscreen);
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
		//ªªªªªªªªªª
	}
	catch(...)
	{
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing D3D presentation parameters"));
	}
}

//ªªªªªªªªªª

//Public function
//««««««««««

//////////////////////////////////////////////////////////////////////////////
//test for lost device
//////////////////////////////////////////////////////////////////////////////
HRESULT Graphics::getDeviceState()
{
	result = E_FAIL;	//default to fail, replace on success
	if(device3d == NULL) return result;
	result = device3d->TestCooperativeLevel();
	return result;
}

//////////////////////////////////////////////////////////////////////////////
//Release all	
//////////////////////////////////////////////////////////////////////////////
void Graphics::releaseAll()
{
	SAFE_RELEASE(device3d);
	SAFE_RELEASE(direct3d);
}

//////////////////////////////////////////////////////////////////////////////
//Initialize DirectX graphics
//When detect error, throw GameError
//////////////////////////////////////////////////////////////////////////////
void Graphics::initialize(HWND hw, int w, int h, bool full)
{
	hwnd = hw;
	width = w;
	height = h;
	fullscreen = full;
	
	//Initialize Direct3D
	direct3d = Direct3DCreate9(D3D_SDK_VERSION);
	if(direct3d == NULL)
	{
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing Direct3D"));
	}

	//Initialize D3DPresentationParameter
	initD3Dpp();

	//If fullscreen mode
	if(fullscreen)
	{
		//Check Adapter is compatible
		if(isAdapterCompatible())
		{
			//Set adapter reflesh rate
			d3dpp.FullScreen_RefreshRateInHz = pMode.RefreshRate;
		}
		else
		{
			throw(GameError(gameErrorNS::FATAL_ERROR, "The graphics device does not support the specified resolution and/or format"));
		}
	}

	//Check graphics card support hardware texturing, lighting, and vertex shader
	D3DCAPS9 caps;
	DWORD behavior;
	result = direct3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);

	//If device does not support HWtexturing and lighting, or 1.1 vertex shader, switch software vertex processing
	if((caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 || caps.VertexShaderVersion < D3DVS_VERSION(1,1))
	{
		//Process software only
		behavior = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}
	else
	{
		//Process hardware only
		behavior = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}

	//Create Direct3D device
	result = direct3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, behavior, &d3dpp, &device3d);
	if(FAILED(result))
	{
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error creating Direct3D device"));
	}

	//Create sprite
	result = D3DXCreateSprite(device3d, &sprite);
	if(FAILED(result))
	{
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error createing Direct3D sprite"));
	}
}


HRESULT Graphics::showBackbuffer()
{
	result = E_FAIL;//Default is error. Replace when success

	//Draw back buffer on display
	result = device3d->Present(NULL, NULL, NULL, NULL);
	
	return result;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//Check adapter and, check BackBuffer height, width and refresh rate specified by d3dpp supported
//If supported adapter was discovered, set proper format to pMode
//Before run	: d3dpp was initialized
//After run		: If discoverd proper format and set it to pMode, return true, else return false
///////////////////////////////////////////////////////////////////////////////////////////////////
bool Graphics::isAdapterCompatible()
{
	UINT modes = direct3d->GetAdapterModeCount(D3DADAPTER_DEFAULT, d3dpp.BackBufferFormat);
	for (unsigned int i = 0; i < modes; i++)
	{
		result = direct3d->EnumAdapterModes(D3DADAPTER_DEFAULT, d3dpp.BackBufferFormat, i, &pMode);

		if(pMode.Height == d3dpp.BackBufferHeight && pMode.Width == d3dpp.BackBufferWidth && pMode.RefreshRate == d3dpp.FullScreen_RefreshRateInHz) return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////////
//Draw the sprite described in SpriteData structure
//Color is optional, it is applied like a filter, WHITE is default (no change)
//Pre : sprite->Begin() is called
//Post:	sprite->End() is called
//			spriteData.rect defines the portion of spriteData.texture to draw
//			spriteData.rect.right must be right edge + 1
//			spriteData.rect.bottom must be bottom edge + 1
//////////////////////////////////////////////////////////////////////////////
void Graphics::drawSprite(const SpriteData &spriteData, COLOR_ARGB color)
{
	//if no texture
	if(spriteData.texture == NULL) return;

	//find center of sprite
	D3DXVECTOR2 spriteCenter = D3DXVECTOR2((float)(spriteData.width / 2 * spriteData.scale), (float)(spriteData.height / 2 * spriteData.scale));

	//location of sprite in screen
	D3DXVECTOR2 translate = D3DXVECTOR2((float)spriteData.x, (float)spriteData.y);

	//scaling vector (x,y)
	D3DXVECTOR2 scaling(spriteData.scale, spriteData.scale);

	//if flip horizontally
	if(spriteData.flipHorizontal)
	{
		scaling.x *= -1;
		//get center of flipped texture
		spriteCenter.x -= float(spriteData.width * spriteData.scale);
		//flip occurs around left edge,
		//move right to put sprite in same locasion as original
		translate.x += (float)(spriteData.width * spriteData.scale);

	}

	//if flip vertically
	if(spriteData.flipVertical)
	{
		scaling.y *= -1;
		//get center of flipped texture
		spriteCenter.y -= float(spriteData.height * spriteData.scale);
		//flip occurs around top edge,
		//move down to put sprite in same locasion as original
		translate.y += (float)(spriteData.height * spriteData.scale);
	}

	//create a martix to rotate, scale and locate sprite
	D3DXMATRIX matrix;
	D3DXMatrixTransformation2D(
		&matrix,							//matrix
		NULL,									//origin is left top when scale
		0.0f,									//don't rotate when scale
		&scaling,							//scale amount
		&spriteCenter,					//center of rotation
		(float)(spriteData.angle),	//rotation angle
		&translate);						//x,y location

	//apply matrix to sprite
	sprite->SetTransform(&matrix);

	//draw the sprite
	sprite->Draw(spriteData.texture, &spriteData.rect, NULL, NULL, color);
}


//////////////////////////////////////////////////////////////////////////////
//Reset graphics device
//////////////////////////////////////////////////////////////////////////////
HRESULT Graphics::reset()
{
	result = E_FAIL;	//Fail is default, if success, substitute exchange
	
	//Initialize D3D presentation parameter
	initD3Dpp();

	//Try to reset graphics device
	result = device3d->Reset(&d3dpp);
	return result;
}
//ªªªªªªªªªª

//Public function
//««««««««««

//////////////////////////////////////////////////////////////////////////////
//Load texture to defalt D3D memory
//this function use in engine only
//use TextureManager class
//Before run :	filname is texture file name
//						transcolor is invisible color
//After run :	width and height = texture size
//					texture = pointer to texture
//&height : height
//&texture : type of texture
//////////////////////////////////////////////////////////////////////////////
HRESULT Graphics::loadTexture(const char *filename, COLOR_ARGB transcolor, UINT &width, UINT &height, LP_TEXTURE &texture)
{
	//struct to read file data
	D3DXIMAGE_INFO info;
	result = E_FAIL;
	try
	{
		if(filename == NULL)
		{
			texture = NULL;
			return D3DERR_INVALIDCALL;
		}

		//get width and height from file
		result = D3DXGetImageInfoFromFile(filename, &info);
		if(result != D3D_OK) return result;
		width = info.Width;
		height = info.Height;

		//Load file and create new texture
		result = D3DXCreateTextureFromFileEx(
			device3d,				//3D device
			filename,				//name of texture file
			info.Width,				//texture width
			info.Height,			//texture height
			1,							//Mipmap level (nothing chain)
			0,							//usage
			D3DFMT_UNKNOWN,		//surface format(default)
			D3DPOOL_DEFAULT,	//memory type for texture
			D3DX_DEFAULT,		//image filter
			D3DX_DEFAULT,		//Mipmap filter
			transcolor,			//code key of invisible color
			&info,					//Bitmap file data
			NULL,						//color palette
			&texture);				//texture
	}catch(...)
	{
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error int Graphics::loadTexture"));
	}

	return result;
}

//////////////////////////////////////////////////////////////////////////////
//Load texture to system memory (system memory can lock)
//this function be able to access directry to pixels data
//use TextureManager class to load texture
//pre:	filename is name of texture file
//			transcolor is color to be treated as transparent
//post:	width, height is texture size
//			texture is pointer to texture
//return HRESULT store data to TextureData structure
//////////////////////////////////////////////////////////////////////////////
HRESULT Graphics::loadTextureSystemMem(const char *filename, COLOR_ARGB transcolor, UINT &width, UINT &height, LP_TEXTURE &texture)
{
	//structure to load bit map file
	D3DXIMAGE_INFO info;
	result = E_FAIL;	//windows default return
	try
	{
		if(filename == NULL)
		{
			texture = NULL;
			return D3DERR_INVALIDCALL;
		}

		//get width and height frome bit map file
		result = D3DXGetImageInfoFromFile(filename, &info);

		if(result != D3D_OK)
			return result;

		width = info.Width;
		height = info.Height;

		//load bit map image file, then create new texture
		result = D3DXCreateTextureFromFileEx(
			device3d,					//3d device
			filename,					//name of bit map file
			info.Width,					//width of bit map image
			info.Height,				//height of bit map image
			1,								//mip map level (no chain is 1)
			0,								//usage
			D3DFMT_UNKNOWN,			//surface format (default)
			D3DPOOL_SYSTEMMEM,	//system memory can lock
			D3DX_DEFAULT,			//image filter
			D3DX_DEFAULT,			//mip filter
			transcolor,				//transeparate color key
			&info,						//bit map file data
			NULL,							//color paret
			&texture);					//pointer to texture
	}
	catch(...)
	{
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error in Graphics::loadTextureSystemMem"));
	}

	return result;
}

//////////////////////////////////////////////////////////////////////////////
//return the number of pixels colliding between the two sprites
//pre:	the device support a stencil buffer and pOcclusionQuery points to a valid occlusionQuery object
//post:	returns the number of pixels of overlap
//////////////////////////////////////////////////////////////////////////////
DWORD Graphics::pixelCollision(const SpriteData &sprite1,	const SpriteData &sprite2)
{
	//if no stencil buffer support
	if(!stencilSupport) return 0;

	beginScene();

	//set up stencil bufffer using current entity
	device3d->SetRenderState(D3DRS_STENCILENABLE, true);
	device3d->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
	device3d->SetRenderState(D3DRS_STENCILREF, 0x1);
	device3d->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
	device3d->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
	device3d->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
	device3d->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);
	
	//write a 1 into the stencil buffer for each non-transparent pixel in ent
	spriteBegin();
	//enable stencil buffer (must be after spriteBegin)
	device3d->SetRenderState(D3DRS_STENCILENABLE, true);
	drawSprite(sprite2);
	spriteEnd();

	//change stencil buffer to only allow writes where the stencil velue is 1
	//(where the ent sprite is colliding with the current sprite)
	device3d->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
	device3d->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);

	//begin occlusion query to count pixels that are drawn
	pOcclusionQuery->Issue(D3DISSUE_BEGIN);

	spriteBegin();
	
	//enable stencil buffer (must be after spriteBegin)
	device3d->SetRenderState(D3DRS_STENCILENABLE, true);
	drawSprite(sprite1);	//draw current entity
	spriteEnd();

	//end occlusion query
	pOcclusionQuery->Issue(D3DISSUE_END);

	//wait until the gpu is finished
	while(S_FALSE == pOcclusionQuery->GetData(&numberOfPixelsColliding, sizeof(DWORD), D3DGETDATA_FLUSH)){}

	//turn off stencil
	device3d->SetRenderState(D3DRS_STENCILENABLE, false);

	endScene();
	return numberOfPixelsColliding;
}


//ªªªªªªªªªª