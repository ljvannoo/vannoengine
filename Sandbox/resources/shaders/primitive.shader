/************************************************************************
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

File Name:		shader.frag
Purpose:		Basic fragment shader
Language:		C++
Platform:		Windows 10, Microsoft Visual Studio 2019

Project:		CS529 - Final Project
Author:			Lukas VanNoord, lukas.vannoord, 60001020
Creation Date:	2020-Oct-08
*************************************************************************/
#BEGIN_VERTEX_SHADER
#version 430 core

layout(location = 0) in vec2 position;

out vec4 fragmentColor;
out vec2 fragmentPosition;

uniform mat4 model;
uniform mat4 projection;
uniform vec4 color;

void main() {
	gl_Position = projection * model * vec4(position, 0.0, 1.0);

	fragmentColor = color;
	fragmentPosition = position;
}

#BEGIN_FRAGMENT_SHADER
#version 430 core

in vec2 fragmentPosition;
in vec4 fragmentColor;

layout(location = 0) out vec4 color;

void main() {
	gl_FragColor = fragmentColor;
}
