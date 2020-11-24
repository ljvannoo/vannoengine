#pragma once
#include <string>
namespace VannoEngine {
	class Event;

	class EventWrapper final
	{
	public: // Methods
		EventWrapper(double time, Event* message);
		~EventWrapper();

		std::string GetName();
		double GetTime() const { return mTime; }
		Event* GetEvent() { return mEvent; }

	private: // Methods

	private: // Variables
		double mTime;
		Event* mEvent;
	};
}
