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

void main() {
	gl_Position = projection * model * vec4(position, 0.0, 1.0);

	fragmentColor = vertexColor;
	fragmentPosition = position;
	
	fragmentUV = uv;
}

#BEGIN_FRAGMENT_SHADER
#version 430 core

in vec2 fragmentPosition;
in vec4 fragmentColor;
in vec2 fragmentUV;

layout(location = 0) out vec4 color;

uniform sampler2D tileset;
uniform vec2 tilesetSize;
uniform vec2 tileSize;
uniform float index;

void main() {
	float w = tilesetSize.x;
	float h = tilesetSize.y;

	// Normalize sprite size (0.0-1.0)
	float dx = tileSize.x / w;
	float dy = tileSize.y / h;

	// Figure out number of tile cols of sprite sheet
	float cols = w / tileSize.x;

	// From linear index to row/col pair
	float col = mod(index, cols);
	float row = floor(index / cols);

	vec2 uv = vec2(dx * fragmentUV.x + col * dx, 1.0 - dy - row * dy + dy * fragmentUV.y);
	gl_FragColor = texture2D(tileset, uv);
}
