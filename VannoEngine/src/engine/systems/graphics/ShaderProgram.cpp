/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		ShaderProgram.cpp
Purpose:		Implementation of the GLSLProgram class.
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Oct-08
*************************************************************************/
#include "ShaderProgram.h"

#include "engine/systems/ResourceManager.h"

#include "engine/core/Log.h"

#include <sstream>
#include <stdexcept>
#include <vector>

namespace VannoEngine {
	ShaderProgram::ShaderProgram() : mProgramId(0), mVertexShaderId(0), mFragmentShaderId(0) {
	}

	ShaderProgram::~ShaderProgram() {

	}

	void ShaderProgram::Init(const std::string& vertexShaderSrc, const std::string& fragmentShaderSrc) {
		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		mProgramId = glCreateProgram();

		// Load and compile the Vertex Shader
		mVertexShaderId = glCreateShader(GL_VERTEX_SHADER);
		if (mVertexShaderId == 0) {
			throw std::runtime_error("Failed to create vertex shader");
		}

		mFragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
		if (mFragmentShaderId == 0) {
			throw std::runtime_error("Failed to create fragment shader");
		}

		CompileShader(vertexShaderSrc, mVertexShaderId);
		CompileShader(fragmentShaderSrc, mFragmentShaderId);

		LinkShaders();
	}

	void ShaderProgram::LinkShaders() {
		// Attach our shaders to our program
		glAttachShader(mProgramId, mVertexShaderId);
		glAttachShader(mProgramId, mFragmentShaderId);

		// Link our program
		glLinkProgram(mProgramId);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(mProgramId, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(mProgramId, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(mProgramId, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(mProgramId);
			// Don't leak shaders either.
			glDeleteShader(mVertexShaderId);
			glDeleteShader(mFragmentShaderId);

			std::printf("%s\n", &infoLog[0]);
			throw std::runtime_error("Failed to compile shader");
		}

		// Always detach shaders after a successful link.
		glDetachShader(mProgramId, mVertexShaderId);
		glDetachShader(mProgramId, mFragmentShaderId);
		glDeleteShader(mVertexShaderId);
		glDeleteShader(mFragmentShaderId);
	}

	void ShaderProgram::CompileShader(const std::string& src, GLuint shaderId) {
		const char* contentsPtr = src.c_str();
		glShaderSource(shaderId, 1, &contentsPtr, nullptr);
		glCompileShader(shaderId);

		GLint success = 0;
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
		if (success == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(shaderId, maxLength, &maxLength, &errorLog[0]);

			// Provide the infolog in whatever manor you deem best.
			// Exit with failure.
			glDeleteShader(shaderId); // Don't leak the shader.

			Print(src);
			std::printf("%s\n", &errorLog[0]);
			LOG_CORE_CRITICAL("Failed to compile shader");
		}

	}

	GLint ShaderProgram::GetUniformLocation(const std::string& uniformName) {
		GLint location = glGetUniformLocation(mProgramId, uniformName.c_str());
		if (location == GL_INVALID_INDEX) {
			LOG_CORE_CRITICAL("Uniform {0} not found in shader", uniformName);
		}

		return location;
	}

	void ShaderProgram::Print(const std::string& src) {
		int line = 1;
		std::stringstream ss;

		ss << line << "  ";
		for (unsigned int i = 0; i < src.length(); ++i) {
			ss << src[i];
			if (src[i] == '\n') {
				line++;
				ss << line << "  ";
			}
		}
		ss << std::endl;

		LOG_CORE_ERROR(ss.str());
	}

	void ShaderProgram::Use() {
		glUseProgram(mProgramId);
	}

	void ShaderProgram::Unuse() {
		glUseProgram(0);
	}
}