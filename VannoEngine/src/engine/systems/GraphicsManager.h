/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		GraphicsManager.h
Purpose:		Handles everything graphics related
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Oct-14
*************************************************************************/
#pragma once

#include "engine/systems/graphics/GLTexture.h"
#include "engine/systems/graphics/Vertex.h"

#include <ft2build.h>
#include FT_FREETYPE_H  

#include <SDL.h>
#include <glm/glm.hpp>

#include <string>
#include <map>
#include <vector>

namespace VannoEngine {
	class GameObject;
	class Transform;
	class ShaderProgram;
	class Camera;
	class Sprite;
	class Surface;

	class GraphicsManager
	{
	public:
		struct Character {
			unsigned int TextureID;
			glm::ivec2 Size;
			glm::ivec2 Bearing;
			unsigned int Advance;
		};
		static GraphicsManager* GetInstance();
		~GraphicsManager();

		void Init(int windowWidth, int windowHeight);
		void StartDraw();
		void EndDraw();
		void RenderText(std::string text, float x, float y, float scale, glm::vec3 color);

		void AddSprite(Sprite* pSprite);
		void SortSprites();

		void Render(
			Surface* pSurface,
			glm::mat4* transformation,
			float spriteSheetWidth,
			float spriteSheetHeight,
			float spriteWidth,
			float spriteHeight,
			int spriteIndex,
			bool flipHorizontal);
		static Surface* BuildSurface(GLTexture* pTexture, Vertex vertexData[4]);

	private:
		GraphicsManager();

	private:
		static GraphicsManager* mpInstance;

		SDL_Window* mpWindow;

		ShaderProgram* mpFontShader;
		ShaderProgram* mpGeneralShader;

		unsigned int VAO, VBO;
		std::map<char, Character> Characters;

		std::vector<Sprite*> mSprites;
		SDL_GLContext mContext;

		int mWindowWidth, mWindowHeight;

	};
}