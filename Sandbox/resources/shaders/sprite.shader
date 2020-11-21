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
layout(location = 1) in vec4 vertexColor;
layout(location = 2) in vec2 uv;

out vec4 fragmentColor;
out vec2 fragmentPosition;
out vec2 fragmentUV;

uniform mat4 model;
uniform mat4 projection;
uniform int flipHorizontal = 0;

void main() {
	gl_Position = projection * model * vec4(position, 0.0, 1.0);

	fragmentColor = vertexColor;
	fragmentPosition = position;
	
	if(flipHorizontal != 0) {
		fragmentUV = vec2(1.0-uv.x, uv.y);
	}
	else {
		fragmentUV = uv;
	}
}

#BEGIN_FRAGMENT_SHADER
#version 430 core

in vec2 fragmentPosition;
in vec4 fragmentColor;
in vec2 fragmentUV;

layout(location = 0) out vec4 color;

uniform sampler2D spriteSheet;
uniform vec2 spriteSheetSize;
uniform vec2 spriteSize;
uniform int index;
uniform int debugMode = 0;

void main() {
	float w = spriteSheetSize.x;
	float h = spriteSheetSize.y;

	// Normalize sprite size (0.0-1.0)
	float dx = spriteSize.x / w;
	float dy = spriteSize.y / h;

	// Figure out number of tile cols of sprite sheet
	int cols = int(w / spriteSize.x);

	// From linear index to row/col pair
	int col = index % cols;
	int row = index / cols;

	vec2 uv = vec2(dx * fragmentUV.x + col * dx, 1.0 - dy - row * dy + dy * fragmentUV.y);
	if(debugMode == 0) {
		gl_FragColor = texture2D(spriteSheet, uv);
	} else {
		gl_FragColor = fragmentColor;
	}
}
