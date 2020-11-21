/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		GraphicsManager.cpp
Purpose:		Handles everything graphics related
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Oct-14
*************************************************************************/
#include "GraphicsManager.h"

#include "engine/systems/objects/GameObjectFactory.h"
#include "engine/systems/levels/LevelManager.h"
#include "engine/systems/ResourceManager.h"
#include "engine/systems/TimeManager.h"
#include "engine/systems/FramerateController.h"
#include "engine/systems/objects/GameObject.h"

#include "engine/components/Sprite.h"
#include "engine/components/Transform.h"
#include "engine/components/Camera.h"

#include "engine/systems/graphics/GLTexture.h"
#include "engine/systems/graphics/Vertex.h"
#include "engine/systems/graphics/ShaderProgram.h"

#include "engine/core/Log.h"

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <stdexcept>
#include <sstream>
#include <algorithm>

namespace VannoEngine {
	GraphicsManager* GraphicsManager::mpInstance = nullptr;

	void GLAPIENTRY openglCallbackFunction(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
		std::stringstream ss;
		ss << std::endl;
		ss << "---------------------opengl-callback-start------------" << std::endl;
		ss << "message: " << message << std::endl;
		ss << "type: ";
		switch (type) {
		case GL_DEBUG_TYPE_ERROR:
			ss << "ERROR";
			break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			ss << "DEPRECATED_BEHAVIOR";
			break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			ss << "UNDEFINED_BEHAVIOR";
			break;
		case GL_DEBUG_TYPE_PORTABILITY:
			ss << "PORTABILITY";
			break;
		case GL_DEBUG_TYPE_PERFORMANCE:
			ss << "PERFORMANCE";
			break;
		case GL_DEBUG_TYPE_OTHER:
			ss << "OTHER";
			break;
		}
		ss << std::endl;

		ss << "id: " << id << std::endl;
		ss << "severity: ";
		switch (severity) {
		case GL_DEBUG_SEVERITY_LOW:
			ss << "LOW";
			break;
		case GL_DEBUG_SEVERITY_MEDIUM:
			ss << "MEDIUM";
			break;
		case GL_DEBUG_SEVERITY_HIGH:
			ss << "HIGH";
			break;
		}
		ss << std::endl;
		ss << "---------------------opengl-callback-end--------------" << std::endl;

		LOG_CORE_ERROR(ss.str());
	}

	GraphicsManager* GraphicsManager::GetInstance() {
		if (!mpInstance) {
			mpInstance = new GraphicsManager;
		}

		return mpInstance;
	}
	GraphicsManager::GraphicsManager() : 
		mpWindow(nullptr), 
		VAO(0), 
		VBO(0), 
		mpFontShader(nullptr), 
		mContext(nullptr),
		mWindowHeight(800),
		mWindowWidth(600),
		mPrimitiveSquare(nullptr, 0, 0, 0),
		mPrimitiveCircle(nullptr, 0, 0, 0),
		mPrimitiveLine(nullptr, 0, 0, 0)
	{}

	GraphicsManager::~GraphicsManager() {
		SDL_GL_DeleteContext(mContext);

		if (mpWindow) {
			SDL_DestroyWindow(mpWindow);
		}

		SDL_Quit();
	}

	void GraphicsManager::Init(int windowWidth, int windowHeight) {
		mWindowWidth = windowWidth;
		mWindowHeight = windowHeight;
		/* Initialize the library */
		if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			return;
		}

		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

		/* Create a windowed mode window and its OpenGL context */
		mpWindow = SDL_CreateWindow("Game",		// window title
			SDL_WINDOWPOS_UNDEFINED,			// initial x position
			SDL_WINDOWPOS_UNDEFINED,			// initial y position
			mWindowWidth,								// width, in pixels
			mWindowHeight,								// height, in pixels
			SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

		if (mpWindow == NULL) {
			LOG_CORE_CRITICAL("Failed to open window");
		}

		/* Make the window's context current */
		mContext = SDL_GL_CreateContext(mpWindow);
		if (!mContext) {
			LOG_CORE_CRITICAL("Failed to create context");
		}

		if (glewInit() != GLEW_OK)
		{
			LOG_CORE_CRITICAL("Failed to initialize GLEW");
		}
		else
		{
			LOG_CORE_INFO(glGetString(GL_VERSION));
		}

		mpGeneralShader = ResourceManager::GetInstance()->LoadShaderProgram("shaders/sprite.shader");
		mpPrimitiveShader = ResourceManager::GetInstance()->LoadShaderProgram("shaders/primitive.shader");

		glViewport(0, 0, mWindowWidth, mWindowHeight);
		glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
		glEnable(GL_TEXTURE_2D);

		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(openglCallbackFunction, nullptr);
		GLuint unusedIds = 0;
		glDebugMessageControl(GL_DONT_CARE,
			GL_DONT_CARE,
			GL_DONT_CARE,
			0,
			&unusedIds,
			true);

		FT_Library ft;
		FT_Face face;

		if (FT_Init_FreeType(&ft)) {
			LOG_CORE_CRITICAL("Failed to initialize Freetype");
		}

		if (FT_New_Face(ft, "resources/fonts/arial.ttf", 0, &face)) {
			LOG_CORE_CRITICAL("Failed to initialize font");
		}

		FT_Set_Pixel_Sizes(face, 0, 24);

		if (FT_Load_Char(face, 'X', FT_LOAD_RENDER)) {
			LOG_CORE_CRITICAL("Failed to initialize freetype glyph");
		}

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

		for (unsigned char c = 0; c < 128; c++)
		{
			// load character glyph 
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				LOG_CORE_ERROR("ERROR::FREETYTPE: Failed to load Glyph");
				continue;
			}
			// generate texture
			unsigned int texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);
			// set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// now store character for later use
			Character character = {
				texture,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				static_cast<unsigned int>(face->glyph->advance.x)
			};
			Characters.insert(std::pair<char, Character>(c, character));
		}

		FT_Done_Face(face);
		FT_Done_FreeType(ft);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		mpFontShader = ResourceManager::GetInstance()->LoadShaderProgram("shaders/font.shader");

		// Build Primitives
		BuildPrimitiveSquare();
		BuildPrimitiveCircle();
		BuildPrimitiveLine();
	}

	void GraphicsManager::RenderText(std::string text, float x, float y, float scale, glm::vec3 color)
	{
		// activate corresponding render state	
		mpFontShader->Use();

		//glm::mat4 projection = pCamera->GetProjectionMatrix();
		glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(mWindowWidth), 0.0f, static_cast<float>(mWindowHeight));
		//projection = glm::perspective(glm::radians(45.0f), (float)mWindowWidth / (float)mWindowHeight, 0.1f, 100.0f);
		GLuint projectionLocation = mpFontShader->GetUniformLocation("projection");
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));


		glUniform3f(mpFontShader->GetUniformLocation("textColor"), color.x, color.y, color.z);
		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(VAO);

		// iterate through all characters
		std::string::const_iterator c;
		for (c = text.begin(); c != text.end(); c++)
		{
			Character ch = Characters[*c];

			float xpos = x + ch.Bearing.x * scale;
			float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

			float w = ch.Size.x * scale;
			float h = ch.Size.y * scale;
			// update VBO for each character
			float vertices[6][4] = {
				{ xpos,     ypos + h,   0.0f, 0.0f },
				{ xpos,     ypos,       0.0f, 1.0f },
				{ xpos + w, ypos,       1.0f, 1.0f },

				{ xpos,     ypos + h,   0.0f, 0.0f },
				{ xpos + w, ypos,       1.0f, 1.0f },
				{ xpos + w, ypos + h,   1.0f, 0.0f }
			};
			// render glyph texture over quad
			glBindTexture(GL_TEXTURE_2D, ch.TextureID);
			// update content of VBO memory
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			// render quad
			glDrawArrays(GL_TRIANGLES, 0, 6);
			// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
			x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
		}
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);

		mpFontShader->Unuse();
	}

	void GraphicsManager::StartSpriteBatch(Surface* pSurface, float spriteSheetWidth, float spriteSheetHeight, float spriteWidth, float spriteHeight) {
		mpGeneralShader->Use();

		GLuint loc = 0;
		LevelManager* pLevelManager = LevelManager::GetInstance();
		glm::mat4 projection = pLevelManager->GetCamera()->GetProjectionMatrix();
		loc = mpGeneralShader->GetUniformLocation("projection");
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(projection));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, pSurface->GetTextureId());
		loc = mpGeneralShader->GetUniformLocation("spriteSheet");
		glUniform1i(loc, 0);

		loc = mpGeneralShader->GetUniformLocation("spriteSheetSize");
		glUniform2f(loc, spriteSheetWidth, spriteSheetHeight);

		loc = mpGeneralShader->GetUniformLocation("spriteSize");
		glUniform2f(loc, spriteWidth, spriteHeight);

		loc = mpGeneralShader->GetUniformLocation("flipHorizontal");
		glUniform1i(loc, 0);

		loc = mpGeneralShader->GetUniformLocation("debugMode");
		glUniform1i(loc, 0);

		glBindVertexArray(pSurface->GetVertexArrayId());
		glBindBuffer(GL_ARRAY_BUFFER, pSurface->GetVertexBufferId());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pSurface->GetIndexBufferId());

		glEnableVertexAttribArray(0);

		// Position
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

		// Color
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));

		// Texture coordinates
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	}

	void GraphicsManager::BatchRender(glm::mat4 const& transformation, int spriteIndex) {
		GLuint loc = 0;
		loc = mpGeneralShader->GetUniformLocation("model");
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(transformation));

		loc = mpGeneralShader->GetUniformLocation("index");
		glUniform1i(loc, spriteIndex);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		mBatchRenderCalls++;
	}

	void GraphicsManager::EndSpriteBatch() {
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		// Unbind the buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		glBindTexture(GL_TEXTURE_2D, 0);
		mpGeneralShader->Unuse();
	}

	void GraphicsManager::Render(Surface* pSurface, glm::mat4* transformation, float spriteSheetWidth, float spriteSheetHeight, float spriteWidth, float spriteHeight, int spriteIndex, bool flipHorizontal, int debugMode) {
		mpGeneralShader->Use();

		GLuint loc = 0;
		if (transformation) {
			loc = mpGeneralShader->GetUniformLocation("model");
			glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(*transformation));
		}

		LevelManager* pLevelManager = LevelManager::GetInstance();
		glm::mat4 projection = pLevelManager->GetCamera()->GetProjectionMatrix();
		loc = mpGeneralShader->GetUniformLocation("projection");
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(projection));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, pSurface->GetTextureId());
		loc = mpGeneralShader->GetUniformLocation("spriteSheet");
		glUniform1i(loc, 0);

		loc = mpGeneralShader->GetUniformLocation("spriteSheetSize");
		//LOG_CORE_DEBUG("SpritesheetSize: ({},{})", spriteSheetWidth, spriteSheetHeight);
		glUniform2f(loc, spriteSheetWidth, spriteSheetHeight);

		//LOG_CORE_DEBUG("SpriteSize: ({},{})", spriteWidth, spriteHeight);
		loc = mpGeneralShader->GetUniformLocation("spriteSize");
		glUniform2f(loc, spriteWidth, spriteHeight);

		loc = mpGeneralShader->GetUniformLocation("index");
		glUniform1i(loc, spriteIndex);

		loc = mpGeneralShader->GetUniformLocation("flipHorizontal");
		glUniform1i(loc, flipHorizontal ? 1 : 0);

		loc = mpGeneralShader->GetUniformLocation("debugMode");
		glUniform1i(loc, debugMode);

		glBindVertexArray(pSurface->GetVertexArrayId());
		glBindBuffer(GL_ARRAY_BUFFER, pSurface->GetVertexBufferId());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pSurface->GetIndexBufferId());

		glEnableVertexAttribArray(0);

		// Position
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

		// Color
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));

		// Texture coordinates
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

		if(debugMode) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		if (debugMode) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		// Unbind the buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		glBindTexture(GL_TEXTURE_2D, 0);
		mpGeneralShader->Unuse();
		mRenderCalls++;
	}

	void GraphicsManager::RenderSquare(glm::vec2 center, float width, float height, glm::vec4 color, bool fill) {
		mpPrimitiveShader->Use();

		GLuint loc = 0;
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(center.x, center.y, 0.0f));
		model = glm::scale(model, glm::vec3(width, height, 1.0f));
		

		loc = mpPrimitiveShader->GetUniformLocation("model");
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(model));

		LevelManager* pLevelManager = LevelManager::GetInstance();
		glm::mat4 projection = pLevelManager->GetCamera()->GetProjectionMatrix();
		loc = mpPrimitiveShader->GetUniformLocation("projection");
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(projection));

		loc = mpPrimitiveShader->GetUniformLocation("color");
		glUniform4fv(loc, 1, glm::value_ptr(color));

		glBindVertexArray(mPrimitiveSquare.GetVertexArrayId());
		glBindBuffer(GL_ARRAY_BUFFER, mPrimitiveSquare.GetVertexBufferId());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mPrimitiveSquare.GetIndexBufferId());

		glEnableVertexAttribArray(0);

		// Position
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), nullptr);

		if (fill) {
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		} else {
			glDrawElements(GL_LINE_STRIP, 6, GL_UNSIGNED_INT, nullptr);
		}

		glDisableVertexAttribArray(0);
		
		// Unbind the buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		mpPrimitiveShader->Unuse();
	}

	void GraphicsManager::RenderCircle(glm::vec2 center, float radius, glm::vec4 color, bool fill) {
		mpPrimitiveShader->Use();

		GLuint loc = 0;
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(center.x, center.y, 0.0f));
		model = glm::scale(model, glm::vec3(radius, radius, 1.0f));


		loc = mpPrimitiveShader->GetUniformLocation("model");
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(model));

		LevelManager* pLevelManager = LevelManager::GetInstance();
		glm::mat4 projection = pLevelManager->GetCamera()->GetProjectionMatrix();
		loc = mpPrimitiveShader->GetUniformLocation("projection");
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(projection));

		loc = mpPrimitiveShader->GetUniformLocation("color");
		glUniform4fv(loc, 1, glm::value_ptr(color));

		glBindVertexArray(mPrimitiveCircle.GetVertexArrayId());
		glBindBuffer(GL_ARRAY_BUFFER, mPrimitiveCircle.GetVertexBufferId());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mPrimitiveCircle.GetIndexBufferId());

		glEnableVertexAttribArray(0);

		// Position
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

		if (fill) {
			glDrawElements(GL_TRIANGLE_FAN, cCircleSegments * 2, GL_UNSIGNED_INT, nullptr);
		}
		else {
			glDrawElements(GL_LINE_STRIP, cCircleSegments * 2, GL_UNSIGNED_INT, nullptr);
		}

		glDisableVertexAttribArray(0);

		// Unbind the buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		mpPrimitiveShader->Unuse();
	}

	void GraphicsManager::RenderLine(glm::vec2 start, float length, float angleDeg, glm::vec4 color) {
		mpPrimitiveShader->Use();

		GLuint loc = 0;
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(start.x, start.y, 0.0f));
		model = glm::rotate(model, glm::radians(angleDeg), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(length, 1.0f, 1.0f));

		loc = mpPrimitiveShader->GetUniformLocation("model");
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(model));

		LevelManager* pLevelManager = LevelManager::GetInstance();
		glm::mat4 projection = pLevelManager->GetCamera()->GetProjectionMatrix();
		loc = mpPrimitiveShader->GetUniformLocation("projection");
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(projection));

		loc = mpPrimitiveShader->GetUniformLocation("color");
		glUniform4fv(loc, 1, glm::value_ptr(color));

		glBindVertexArray(mPrimitiveLine.GetVertexArrayId());
		glBindBuffer(GL_ARRAY_BUFFER, mPrimitiveLine.GetVertexBufferId());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mPrimitiveLine.GetIndexBufferId());

		glEnableVertexAttribArray(0);

		// Position
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

		glDrawElements(GL_LINE_STRIP, 6, GL_UNSIGNED_INT, nullptr);

		glDisableVertexAttribArray(0);

		// Unbind the buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		mpPrimitiveShader->Unuse();
	}

	void GraphicsManager::RenderLine(glm::vec2 start, glm::vec2 end, glm::vec4 color) {
		mpPrimitiveShader->Use();

		float length = sqrtf(powf(end.x - start.x, 2) + powf(end.y - start.y, 2));
		glm::vec2 v = glm::normalize(end - start);

		GLuint loc = 0;
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(start.x, start.y, 0.0f));
		model = glm::rotate(model, atan2f(v.y, v.x), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(length, 1.0f, 1.0f));

		loc = mpPrimitiveShader->GetUniformLocation("model");
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(model));

		LevelManager* pLevelManager = LevelManager::GetInstance();
		glm::mat4 projection = pLevelManager->GetCamera()->GetProjectionMatrix();
		loc = mpPrimitiveShader->GetUniformLocation("projection");
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(projection));

		loc = mpPrimitiveShader->GetUniformLocation("color");
		glUniform4fv(loc, 1, glm::value_ptr(color));

		glBindVertexArray(mPrimitiveLine.GetVertexArrayId());
		glBindBuffer(GL_ARRAY_BUFFER, mPrimitiveLine.GetVertexBufferId());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mPrimitiveLine.GetIndexBufferId());

		glEnableVertexAttribArray(0);

		// Position
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

		glDrawElements(GL_LINE_STRIP, 6, GL_UNSIGNED_INT, nullptr);

		glDisableVertexAttribArray(0);

		// Unbind the buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		mpPrimitiveShader->Unuse();
	}

	void GraphicsManager::StartDraw() {
		glClear(GL_COLOR_BUFFER_BIT);
		mRenderCalls = 0;
		mBatchRenderCalls = 0;
	}
	
	void GraphicsManager::EndDraw() {
		TimeManager* pTimeManager = TimeManager::GetInstance();
		glm::vec3 color(1.0f, 1.0f, 1.0f);

		char buff[30];
		snprintf(buff, sizeof(buff), "%.2f fps", FramerateController::GetInstance()->GetFPS());
		RenderText(buff, 10.0f, static_cast<float>(mWindowHeight) - 25.0f, 0.75f, color);

		snprintf(buff, sizeof(buff), "Physics: %lims", pTimeManager->GetTimerMillis("physics"));
		RenderText(buff, 10.0f, static_cast<float>(mWindowHeight) - 40.0f, 0.5f, color);

		snprintf(buff, sizeof(buff), "Update: %lims", pTimeManager->GetTimerMillis("update"));
		RenderText(buff, 10.0f, static_cast<float>(mWindowHeight) - 55.0f, 0.5f, color);

		snprintf(buff, sizeof(buff), "Draw: %lims", pTimeManager->GetTimerMillis("draw"));
		RenderText(buff, 10.0f, static_cast<float>(mWindowHeight) - 70.0f, 0.5f, color);

		snprintf(buff, sizeof(buff), "Waste: %lims", pTimeManager->GetTimerMillis("framewaste"));
		RenderText(buff, 10.0f, static_cast<float>(mWindowHeight) - 85.0f, 0.5f, color);

		snprintf(buff, sizeof(buff), "Renders: %i", mRenderCalls);
		RenderText(buff, 10.0f, static_cast<float>(mWindowHeight) - 100.0f, 0.5f, color);
		
		snprintf(buff, sizeof(buff), "Batch Renders: %i", mBatchRenderCalls);
		RenderText(buff, 10.0f, static_cast<float>(mWindowHeight) - 115.0f, 0.5f, color);

		SDL_GL_SwapWindow(mpWindow);
	}

	Surface* GraphicsManager::BuildSurface(GLTexture* pTexture, Vertex vertexData[4]) {
		if (!pTexture) {
			LOG_CORE_ERROR("Unable to build surface from null texture!");
			return nullptr;
		}

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		unsigned int vaoId, vboId, iboId;
		glGenVertexArrays(1, &vaoId);
		glBindVertexArray(vaoId);

		glGenBuffers(1, &vboId);
		glBindBuffer(GL_ARRAY_BUFFER, vboId);
		glBufferData(GL_ARRAY_BUFFER, 4*sizeof(Vertex), vertexData, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);


		glGenBuffers(1, &iboId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(unsigned int), indices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBindVertexArray(0);

		return new Surface(pTexture, vboId, vaoId, iboId);
	}

	void GraphicsManager::BuildPrimitiveSquare() {
		float vertexData[] = {
			-0.5f,  0.5f, // Upper left
			-0.5f, -0.5f, // Bottom Left
			 0.5f, -0.5f, // Bottom right
			 0.5f,  0.5f // Upper Right
		};
		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		unsigned int vaoId, vboId, iboId;
		glGenVertexArrays(1, &vaoId);
		glBindVertexArray(vaoId);

		glGenBuffers(1, &vboId);
		glBindBuffer(GL_ARRAY_BUFFER, vboId);
		glBufferData(GL_ARRAY_BUFFER, 2 * 4 * sizeof(float), vertexData, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);


		glGenBuffers(1, &iboId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBindVertexArray(0);

		mPrimitiveSquare = Surface(nullptr, vboId, vaoId, iboId);
	}

	void GraphicsManager::BuildPrimitiveCircle() {
		std::vector<float> vertexData;
		vertexData.push_back(0.0f);	
		vertexData.push_back(0.0f);

		float angle = (2.0f * PI) / (float)cCircleSegments;
		for (int i = 0; i < cCircleSegments; i++) {
			vertexData.push_back(cosf((float)i * angle));
			vertexData.push_back(sinf((float)i * angle));
		}

		std::vector<int> indices;
		for (int i = 1; i < cCircleSegments; i++) {
			indices.push_back(i);			
			indices.push_back(i + 1 );
		}
		indices.push_back(cCircleSegments);
		indices.push_back(1);

		unsigned int vaoId, vboId, iboId;
		glGenVertexArrays(1, &vaoId);
		glBindVertexArray(vaoId);

		glGenBuffers(1, &vboId);
		glBindBuffer(GL_ARRAY_BUFFER, vboId);
		glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);


		glGenBuffers(1, &iboId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBindVertexArray(0);

		mPrimitiveCircle = Surface(nullptr, vboId, vaoId, iboId);
	}

	void GraphicsManager::BuildPrimitiveLine() {
		float vertexData[] = {
			 0.0f, 0.0f, 
			 1.0f, 0.0f, 
		};
		unsigned int indices[] = {
			0, 1
		};

		unsigned int vaoId, vboId, iboId;
		glGenVertexArrays(1, &vaoId);
		glBindVertexArray(vaoId);

		glGenBuffers(1, &vboId);
		glBindBuffer(GL_ARRAY_BUFFER, vboId);
		glBufferData(GL_ARRAY_BUFFER, 2 * 2 * sizeof(float), vertexData, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);


		glGenBuffers(1, &iboId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * sizeof(unsigned int), indices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBindVertexArray(0);

		mPrimitiveLine = Surface(nullptr, vboId, vaoId, iboId);
	}
}