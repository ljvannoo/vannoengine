#pragma once
/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		ShaderProgram.h
Purpose:		Provides functionality for loading, compiling, and linking
				a GLSL shader program.
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Oct-08
*************************************************************************/
#include "engine/core.h"

#include <string>

#include <GL/glew.h>

namespace VannoEngine {
	class ENGINE_API ShaderProgram
	{
	public:
		ShaderProgram();
		~ShaderProgram();

		void Init(const std::string& vertexShaderSrc, const std::string& fragmentShaderSrc);

		void Use();
		void Unuse();

		GLint GetUniformLocation(const std::string& uniformName);

	private:
		void LinkShaders();
		void CompileShader(const std::string& filePath, GLuint shaderId);
		void Print(const std::string& src);
	private:
		GLuint mProgramId;
		GLuint mVertexShaderId;
		GLuint mFragmentShaderId;
	};
}