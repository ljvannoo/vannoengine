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
#include "GameObjectFactory.h"
#include "LevelManager.h"
#include "ResourceManager.h"
#include "TimeManager.h"
#include "FramerateController.h"
#include "engine/core/GameObject.h"

#include "engine/core/components/Sprite.h"
#include "engine/core/components/Transform.h"
#include "engine/core/components/Camera.h"

#include "engine/Log.h"

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
	GraphicsManager::GraphicsManager() : \
		mpWindow(nullptr), 
		VAO(0), 
		VBO(0), 
		mpFontShader(nullptr), 
		mContext(nullptr),
		mWindowHeight(800),
		mWindowWidth(600)
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

		glViewport(0, 0, mWindowWidth, mWindowHeight);
		glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
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

		FT_Set_Pixel_Sizes(face, 0, 12);

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

		mpFontShader = ResourceManager::GetInstance()->LoadShaderProgram("font.shader");
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

	void GraphicsManager::StartDraw() {
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void GraphicsManager::Draw() {
		GameObjectFactory* pFactory = GameObjectFactory::GetInstance();
		LevelManager* pLevelManager = LevelManager::GetInstance();

		glClear(GL_COLOR_BUFFER_BIT);
		Camera* pCamera = pLevelManager->GetCamera();
		if (!mSprites.empty() && pCamera) {
			for (auto it = mSprites.begin(); it != mSprites.end(); ++it) {
				Sprite* pSprite = *it;
				GameObject* pObject = pSprite->GetOwner();
				Transform* pTransform = static_cast<Transform*>(pObject->GetComponent(TRANSFORM_COMPONENT));

				if (pTransform) {
					ShaderProgram* pShaderProgram = pSprite->GetShaderProgram();

					pShaderProgram->Use();

					glm::mat4 t = pTransform->GetTranslationMatrix();
					glm::mat4 r = pTransform->GetRotationMatrix();
					glm::mat4 s = pTransform->GetScaleMatrix();
					glm::mat4 model = t * r * s;

					GLuint loc = pShaderProgram->GetUniformLocation("model");
					glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(model));

					glm::mat4 projection = pCamera->GetProjectionMatrix();
					loc = pShaderProgram->GetUniformLocation("projection");
					glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(projection));

					pSprite->Draw();

					pShaderProgram->Unuse();
				}
			}
		}

		char buff[20];
		snprintf(buff, sizeof(buff), "%.2f fps", FramerateController::GetInstance()->GetFPS());
		RenderText(buff, static_cast<float>(mWindowWidth) - 60.0f, static_cast<float>(mWindowHeight) - 45.0f, 1.0f, glm::vec3(1.0, 0.0f, 0.0f));

		SDL_GL_SwapWindow(mpWindow);
	}

	void GraphicsManager::EndDraw() {
		SDL_GL_SwapWindow(mpWindow);
	}

	void GraphicsManager::AddSprite(Sprite* pSprite) {
		mSprites.push_back(pSprite);
	}
	bool CompareSprites(Sprite* lhs, Sprite* rhs) {
		return lhs->GetZOrder() < rhs->GetZOrder();
	}

	void GraphicsManager::SortSprites() {
		std::sort(mSprites.begin(), mSprites.end(), CompareSprites);
	}
}