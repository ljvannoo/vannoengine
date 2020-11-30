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

#define PI           3.14159265358979323846f

#include "engine/systems/graphics/GLTexture.h"
#include "engine/systems/graphics/Vertex.h"
#include "engine/systems/graphics/Surface.h"

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

		void StartSpriteBatch(Surface* pSurface, float spriteSheetWidth, float spriteSheetHeight, float spriteWidth, float spriteHeight);
		void BatchRender(glm::mat4 const& transformation, int col, int row);
		void EndSpriteBatch();

		void Render(
			Surface* pSurface,
			glm::mat4* transformation,
			float spriteSheetWidth,
			float spriteSheetHeight,
			float spriteWidth,
			float spriteHeight,
			int col,
			int row,
			bool flipHorizontal,
			int debugMode = 0,
			bool uiMode = false);
		void RenderSquare(glm::vec2 center, float width, float height, glm::vec4 color, bool fill);
		void RenderCircle(glm::vec2 center, float radius, glm::vec4 color, bool fill);
		void RenderLine(glm::vec2 start, float length, float angleDeg, glm::vec4 color);
		void RenderLine(glm::vec2 start, glm::vec2 end, glm::vec4 color);
		static Surface* BuildSurface(GLTexture* pTexture, Vertex vertexData[4]);

	private:
		GraphicsManager();

		void BuildPrimitiveSquare();
		void BuildPrimitiveCircle();
		void BuildPrimitiveLine();

	private:
		static GraphicsManager* mpInstance;

		SDL_Window* mpWindow;

		ShaderProgram* mpFontShader;
		ShaderProgram* mpGeneralShader;
		ShaderProgram* mpPrimitiveShader;

		Surface mPrimitiveSquare;
		Surface mPrimitiveCircle;
		Surface mPrimitiveLine;

		unsigned int VAO, VBO;
		std::map<char, Character> Characters;

		SDL_GLContext mContext;

		int mWindowWidth, mWindowHeight;
		const int cCircleSegments = 16;

		int mBatchRenderCalls = 0;
		int mRenderCalls = 0;
	};
}