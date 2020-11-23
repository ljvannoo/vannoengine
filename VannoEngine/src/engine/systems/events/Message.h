#pragma once

#include <string>

namespace VannoEngine {
	class Message
	{
	public: // Variables

	public: // Methods
		Message(std::string name);
		virtual ~Message();

		std::string GetName() const { return mName; }
	private:
		std::string mName;
	};
}
