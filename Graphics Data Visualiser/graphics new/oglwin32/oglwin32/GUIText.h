#pragma once
#include "Renderable.h"
#include <string>
#include "GLFont.h"

class GUIText : public Renderable
{
	private:
		std::string	    m_text;
		GLFont			*pGLFont;
		void			glInit();
		float			coordX;
		float			coordY;
		float			coordZ;
		bool			is3D = false;

	public:
						GUIText();
						GUIText(char* inText);
						GUIText(char* inText, float inX, float inY);
						GUIText(char* inText, float inX, float inY, float inZ);
						~GUIText(); 

		void			SetText(const char* text);
		std::string inline GetString() { return m_text; };

		void			SetIs3D(bool is3DIn);
		bool	inline	GetIs3D() { return is3D; };

		void			SetX(float inX);
		void			SetY(float inY);
		void			SetZ(float inZ);
		float	inline	GetX() { return coordX; };
		float	inline	GetY() { return coordY; };
		float	inline	GetZ() { return coordZ; };
		
		void			Render();
};