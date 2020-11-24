#pragma once

#include <string>

namespace VannoEngine {
	class Event
	{
	public: // Variables

	public: // Methods
		Event(std::string name);
		virtual ~Event();

		std::string GetName() const { return mName; }
	private:
		std::string mName;
	};
}
