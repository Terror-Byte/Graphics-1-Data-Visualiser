#include <string>
#include "GUIText.h"
#include "GLFont.h"

GUIText::GUIText()
{
	glInit();
}

GUIText::GUIText(char* inText)
{
	m_text = inText;
	coordZ = 0;
	glInit();
}

GUIText::GUIText(char* inText, float inX, float inY)
{
	m_text = inText;
	coordX = inX;
	coordY = inY;
	coordZ = 0;
	glInit();
}

GUIText::GUIText(char* inText, float inX, float inY, float inZ)
{
	m_text = inText;
	coordX = inX;
	coordY = inY;
	coordZ = inZ;
	is3D = true;
	glInit();
}

GUIText::~GUIText()
{
	delete pGLFont;
}

void GUIText::glInit()
{
	pGLFont = new GLFont();
	// const char* file = "franklin_gothic.glf";
	pGLFont->Create("franklin_gothic.glf");
}

void GUIText::SetText(const char* text)
{
	m_text = text;
}

void GUIText::SetIs3D(bool is3DIn)
{
	is3D = is3DIn;
}

void GUIText::SetX(float inX)
{
	coordX = inX;
}

void GUIText::SetY(float inY)
{
	coordY = inY;
}

void GUIText::SetZ(float inZ)
{
	coordZ = inZ;
}

void GUIText::Render()
{
	pGLFont->Begin();
	if (is3D)
	{
		pGLFont->TextOut(m_text.c_str(), coordX, coordY, coordZ);
	}
	else
	{
		pGLFont->TextOut(m_text.c_str(), coordX, coordY, 0);
	}
	pGLFont->End();
}