#pragma once

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