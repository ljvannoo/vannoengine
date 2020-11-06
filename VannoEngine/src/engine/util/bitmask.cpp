#include "bitmask.h"

#include "engine/core/Log.h"
namespace VannoEngine {
	void SetBit(unsigned int& mask, int position) {
		mask |= 1 << position;
	}

	void ClearBit(unsigned int& mask, int position) {
		mask &= ~(1 << position);
	}

	void ToggleBit(unsigned int& mask, int position) {
		mask ^= (1 << position);
	}

	bool IsSet(unsigned int mask, int position) {
		bool result = mask & (1 << position);
		//LOG_CORE_DEBUG("(A) Mask: {0}, Position: {1}, Set? {2}", mask, position, result);
		//position = 0;
		//unsigned int test = SetBit(0, position);
		//bool result = test & (1 << position);
		//LOG_CORE_DEBUG("(B) Mask: {0}, Position: {1}, Set? {2}", test, position, result);
		//return true;
		return result;
	}
}