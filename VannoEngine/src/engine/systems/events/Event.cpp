#include "Event.h"

#include "Message.h"

namespace VannoEngine {
	Event::Event(double time, Message* message) :
		mTime{ time },
		mMessage{ message }
	{ }

	Event::~Event() {
		delete mMessage;
	}

	std::string Event::GetName()
	{
		return mMessage->GetName();
	}
}