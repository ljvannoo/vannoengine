#pragma once

#define BIT_COLLISION_TOP		0
#define BIT_COLLISION_RIGHT		1
#define BIT_COLLISION_BOTTOM	2
#define BIT_COLLISION_LEFT		3

namespace VannoEngine {
	extern void SetBit(unsigned int& mask, int position);

	extern void ClearBit(unsigned int& mask, int position);

	extern void ToggleBit(unsigned int& mask, int position);

	extern bool IsSet(unsigned int mask, int position);
}
