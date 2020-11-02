/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		LevelManager.h
Purpose:		Manages loading and unloading levels
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Oct-21
*************************************************************************/
#pragma once



#include <stack>
#include <string>

namespace VannoEngine {
	class Level;
	class Camera;

	class LevelManager
	{
	public:
		static LevelManager* GetInstance();
		~LevelManager();

		void LoadLevel(const std::string& relativePath);
		void UnloadLevel();

		void UpdatePhysics(double deltaTime);
		void Update(double deltaTime);
		void Draw();

		Camera* GetCamera();

	private:
		LevelManager();
		Level* GetCurrentLevel();

	private:
		static LevelManager* mpInstance;

		std::stack<Level*> mLevels;
	};
}