#pragma once
/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		Surface.h
Purpose:		Defines a basic graphics surface
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Oct-31
*************************************************************************/
#include "engine/systems/graphics/GLTexture.h"

namespace VannoEngine {
	class Surface {
	public:
		Surface(GLTexture* pTexture, unsigned int vboId, unsigned int vaoId, unsigned int iboId) :
			mVboID(vboId),
			mVaoID(vaoId),
			mIboID(iboId),
			mpTexture(pTexture)
		{ }
		~Surface() {};

		unsigned int GetVertexBufferId() { return mVboID;  }
		unsigned int GetVertexArrayId() { return mVaoID; }
		unsigned int GetIndexBufferId() { return mIboID; }
		unsigned int GetTextureId() { return mpTexture->id; }
		int GetHeight() { return mpTexture->height; }
		int GetWidth() { return mpTexture->width; }

	private:
		unsigned int mVboID; // Vertex buffer
		unsigned int mVaoID; // Vertex Array Object
		unsigned int mIboID; // Index buffer

		GLTexture* mpTexture;
	};
}