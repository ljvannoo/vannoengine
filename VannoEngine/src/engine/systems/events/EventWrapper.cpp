#include "EventWrapper.h"

#include "Event.h"

namespace VannoEngine {
	EventWrapper::EventWrapper(double time, Event* message) :
		mTime{ time },
		mEvent{ message }
	{ }

	EventWrapper::~EventWrapper() {
	}

	std::string EventWrapper::GetName()
	{
		return mEvent->GetName();
	}
}