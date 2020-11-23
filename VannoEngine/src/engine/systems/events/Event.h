#pragma once
#include <string>
namespace VannoEngine {
	class Message;

	class Event final
	{
	public: // Methods
		Event(double time, Message* message);
		~Event();

		std::string GetName();
		double GetTime() const { return mTime; }
		Message* GetMsg() { return mMessage; }

	private: // Methods

	private: // Variables
		double mTime;
		Message* mMessage;
	};
}
