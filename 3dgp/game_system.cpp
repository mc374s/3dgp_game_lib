#include "game_system.h"

#include "framework.h"
#include "blend.h"
#include "sprite.h"



LOAD_TEXTURE *g_load_texture[TEX_MAX] = { nullptr };

void SPRITE_DATA::draw(float _x, float _y, CUSTOM *_custom) {
	if (texNum >= 0 && texNum < TEX_MAX && g_load_texture[texNum] && g_load_texture[texNum]->img)
	{
		if (_custom) {
			g_load_texture[texNum]->img->render(framework::s_pDeviceContext, _x + ofsX, _y + ofsY, _custom->scaleX*width, _custom->scaleY*height, left, top, width, height, _custom->rgba, _custom->angle, _custom->centRotate, _custom->centX, _custom->centY, _custom->reflectX, _custom->scaleMode);
		}
		else {
			g_load_texture[texNum]->img->render(framework::s_pDeviceContext, _x + ofsX, _y + ofsY, width, height, left, top, width, height, 0xFFFFFFFF, 0.0f);
		}
	}
}

void SPRITE_DATA::draw(Vector3 &a_pos, CUSTOM *a_pCustom, CUSTOM3D *a_pCustom3d) {
	if (texNum >= 0 && texNum < TEX_MAX && g_load_texture[texNum] && g_load_texture[texNum]->img) {
		if (g_load_texture[texNum]->doProjection)
		{
			g_load_texture[texNum]->img->render3D(framework::s_pDeviceContext, a_pos.x + ofsX, a_pos.y + ofsY, a_pCustom->scaleX*width, a_pCustom->scaleY*height, left, top, width, height, a_pCustom->rgba, a_pCustom->angle, a_pCustom->centRotate, a_pCustom->centX, a_pCustom->centY, a_pCustom->reflectX, a_pCustom->scaleMode, a_pCustom3d);
		}
		else
		{
			draw(a_pos.x, a_pos.y, a_pCustom);
		}
	}
}

void TextureManager::loadTextures(LOAD_TEXTURE _data[])
{
	for (int i = _data[0].texNum; _data[i].texNum != -1 || _data[i].fileName != NULL; i++)
	{
		/*const size_t cSize = strlen(_data[i].fileName) + 1;
		wchar_t *wcFileName = new wchar_t[cSize];
		
		size_t temp;
		mbstowcs_s(&temp, wcFileName, cSize, _data[i].fileName, cSize);*/
		_data[i].img = new Sprite(framework::s_pDevice, _data[i].fileName, _data[i].doProjection);
		g_load_texture[i] = &_data[i];
		//delete[] wcFileName;
	}
}

void TextureManager::loadTexture(LOAD_TEXTURE _data[], int a_textureNO)
{
	if (_data[a_textureNO].texNum != -1 || _data[a_textureNO].fileName != NULL)
	{
		_data[a_textureNO].img = new Sprite(framework::s_pDevice, _data[a_textureNO].fileName, _data[a_textureNO].doProjection);
		g_load_texture[a_textureNO] = &_data[a_textureNO];
	}
}

void TextureManager::releaseTexture()
{
	int i;
	for (i = 0; i < TEX_MAX; i++)
	{
		if (g_load_texture[i])
		{
			delete g_load_texture[i]->img;
			ZeroMemory(g_load_texture[i], sizeof(*g_load_texture[i]));
			
		}
	}
	ZeroMemory(g_load_texture, sizeof(LOAD_TEXTURE)*i);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// edit by ChenYuezong 2018/05/25
// operator " = [] () -> " must be defined as memeber operator

XMFLOAT3& operator += (XMFLOAT3& lhv, const XMFLOAT3& rhv) { lhv.x += rhv.x; lhv.y += rhv.y; lhv.z += rhv.z; return lhv; }
XMFLOAT3& operator -= (XMFLOAT3& lhv, const XMFLOAT3& rhv) { lhv.x -= rhv.x; lhv.y -= rhv.y; lhv.z -= rhv.z; return lhv; }
XMFLOAT3& operator *= (XMFLOAT3& lhv, float rhv) { lhv.x *= rhv; lhv.y *= rhv; lhv.z *= rhv; return lhv; }
XMFLOAT3& operator /= (XMFLOAT3& lhv, float rhv) { lhv.x /= rhv; lhv.y /= rhv; lhv.z /= rhv; return lhv; }

//inline XMFLOAT3 operator + () const { XMFLOAT3 ret(x, y, z); return ret; }
//inline XMFLOAT3 operator - () const { XMFLOAT3 ret(-x, -y, -z); return ret; }

const XMFLOAT3 operator + (const XMFLOAT3& lhv, const XMFLOAT3& rhv) { return XMFLOAT3(lhv.x + rhv.x, lhv.y + rhv.y, lhv.z + rhv.z); }
const XMFLOAT3 operator - (const XMFLOAT3& lhv, const XMFLOAT3& rhv) { return XMFLOAT3(lhv.x - rhv.x, lhv.y - rhv.y, lhv.z - rhv.z); }
const XMFLOAT3 operator * (const XMFLOAT3& lhv, float rhv) { XMFLOAT3 ret(lhv.x*rhv, lhv.y*rhv, lhv.z*rhv); return ret; }
const XMFLOAT3 operator / (const XMFLOAT3& lhv, float rhv) { XMFLOAT3 ret(lhv.x / rhv, lhv.y / rhv, lhv.z / rhv); return ret; }

bool const operator == (const XMFLOAT3& lhv, const XMFLOAT3& rhv)
{
	return (fabsf(lhv.x - rhv.x) < FLT_EPSILON) && (fabsf(lhv.y - rhv.y) < FLT_EPSILON) && (fabsf(lhv.z - rhv.z) < FLT_EPSILON);
}
bool const operator != (const XMFLOAT3& lhv, const XMFLOAT3& rhv)
{
	return (fabsf(lhv.x - rhv.x) > FLT_EPSILON) || (fabsf(lhv.y - rhv.y) > FLT_EPSILON) || (fabsf(lhv.z - rhv.z) > FLT_EPSILON);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int  getInputKey()
{
	int command = 0x0;

	if (KEY_BOARD.W || GAME_PAD.IsLeftThumbStickUp()) {
		command |= PAD_UP;
	}
	if (KEY_BOARD.S || GAME_PAD.IsLeftThumbStickDown()) {
		command |= PAD_DOWN;
	}
	if (KEY_BOARD.A || GAME_PAD.IsLeftThumbStickLeft()){
		command |= PAD_LEFT;
	}
	if (KEY_BOARD.D || GAME_PAD.IsLeftThumbStickRight()){
		command |= PAD_RIGHT;
	}
	if (KEY_BOARD.Space || GAME_PAD.IsStartPressed()) {
		command |= PAD_START;
	}
	if (KEY_BOARD.Enter || GAME_PAD.IsBackPressed()) {
		command |= PAD_ENTER;
	}
	if (KEY_BOARD.Z || GAME_PAD.IsAPressed()) {
		command |= PAD_TRG1;
	}
	if (KEY_BOARD.X || GAME_PAD.IsBPressed()) {
		command |= PAD_TRG2;
	}
	if (KEY_BOARD.C || GAME_PAD.IsXPressed()) {
		command |= PAD_TRG3;
	}
	if (KEY_BOARD.V || GAME_PAD.IsYPressed()) {
		command |= PAD_TRG4;
	}

	return command;
}

void drawString(int _x, int _y, char *_buf, D3DCOLOR _color, int _format, int _sizeX, int _sizeY, float _angle)
{
	drawSprString(framework::s_pDeviceContext, _x, _y, _buf, _color, _format, _sizeX, _sizeY, _angle);
}

void drawRectangle(float _x, float _y, float _w, float _h, float _angle, UINTCOLOR _color)
{
	static Sprite rect(framework::s_pDevice);
	//MyBlending::setMode(framework::m_pDeviceContext, BLEND_REPLACE);
	rect.render(framework::s_pDeviceContext, _x, _y, _w, _h, _angle, _color);
	//MyBlending::setMode(framework::m_pDeviceContext, BLEND_ALPHA);
}


///////////////////////////////////////////////////////////////
// Class View
View::View(int a_viewWidth, int a_viewHeight) :m_doReflection(false)
{
	m_pRenderTarget = new RenderTarget(framework::s_pDevice, a_viewWidth, a_viewHeight);
	m_custom3d.clear();
}

View::View(float a_drawX, float a_drawY, float a_drawWidth, float a_drawHeight, float a_srcX, float a_srcY, float a_srcWidth, float a_srcHeight, float a_rotateAngle, UINTCOLOR a_blendColor, bool a_doReflection):
m_drawX(a_drawX), m_drawY(a_drawY), m_drawWidth(a_drawWidth), m_drawHeight(a_drawHeight), 
m_srcX(a_srcX), m_srcY(a_srcY), m_srcWidth(a_srcWidth), m_srcHeight(a_srcHeight),
m_rotateAngle(a_rotateAngle),
m_blendColor(a_blendColor),
m_doReflection(a_doReflection)
{
	m_pRenderTarget = new RenderTarget(framework::s_pDevice, a_drawWidth, a_drawHeight);
	m_custom3d.clear();
}

View::~View()
{
	if (m_pRenderTarget)
	{
		delete m_pRenderTarget;
		m_pRenderTarget = NULL;
	}
}

// View, looks like a textured sprite
void View::set(float a_drawX, float a_drawY, float a_drawWidth, float a_drawHeight, float a_srcX, float a_srcY, float a_srcWidth, float a_srcHeight, float a_rotateAngle, UINTCOLOR a_blendColor, bool a_doReflection)
{
	m_pRenderTarget->render3D(framework::s_pDeviceContext, a_drawX, a_drawY, a_drawWidth, a_drawHeight, a_srcX, a_srcY, a_srcWidth, a_srcHeight, a_rotateAngle, a_blendColor, &m_custom3d, a_doReflection);
}

void View::set()
{
	m_pRenderTarget->render3D(framework::s_pDeviceContext, m_drawX, m_drawY, m_drawWidth, m_drawHeight, m_srcX, m_srcY, m_srcWidth, m_srcHeight, m_rotateAngle, m_blendColor, &m_custom3d, m_doReflection);
}

// Reset ViewPort to real screen
void View::clear()
{
	framework::s_pDeviceContext->OMSetRenderTargets(1, &framework::s_pRenderTargetView, framework::s_pDepthStencilView);

	e_camera.viewPort.Width = SCREEN_WIDTH;
	e_camera.viewPort.Height = SCREEN_HEIGHT;
	framework::s_pDeviceContext->RSSetViewports(1, &e_camera.viewPort);
	setRenderTargetWH(SCREEN_WIDTH, SCREEN_HEIGHT);
}

///////////////////////////////////////////////////////////

// Class Cube
Cube::Cube(const XMFLOAT3 &a_position, const XMFLOAT3 &a_size, const UINTCOLOR &a_blendColor) :m_position(a_position), m_size(a_size), m_blendColor(a_blendColor)
{
	m_pPrimitive = new Primitive3D(framework::s_pDevice); 
	m_pPrimitive->initialize(framework::s_pDevice, GEOMETRY_CUBE);
}

Cube::~Cube()
{
	if (m_pPrimitive)
	{
		delete m_pPrimitive;
		m_pPrimitive = NULL;
	}
}

void Cube::draw()
{
	m_pPrimitive->drawCube(framework::s_pDeviceContext, m_position, m_size, m_blendColor, &m_custom3d);
}

////////////////////////////////////////////////////////////