#pragma once
/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		Tileset.h
Purpose:		Describes a tileset
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Oct-29
*************************************************************************/

#include <rapidjson/document.h>

namespace VannoEngine {
	struct GLTexture;
	class Surface;

	class Tileset
	{
	public: // Variables

	public: // Methods
		Tileset(int tileWidth, int tileHeight);
		~Tileset();

		void LoadData(const rapidjson::Value* pData);

		int GetStartIndex() { return mStartIndex; }
		int GetEndIndex() { return mEndIndex; }
		void SetEndIndex(int index) { mEndIndex = index; }

		unsigned int Tileset::GetTextureId();

		float GetWidth();
		float GetHeight();

		int GetTileWidth() { return mTileWidth; }
		int GetTileHeight() { return mTileHeight; }

		unsigned int GetVertexBufferId();
		unsigned int GetVertexArrayId();
		unsigned int GetIndexBufferId();

		Surface* GetSurface() { return mpSurface; }
	private: // Methods

	private: // Variables
		Surface* mpSurface;

		int mTileWidth;
		int mTileHeight;

		int mStartIndex;
		int mEndIndex;
	};
}
