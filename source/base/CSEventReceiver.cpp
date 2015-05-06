/****************************************************************************
Copyright (c) 2014 PauT

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
#include "CSEventReceiver.h"
#include "base/CSScene.h"
NS_CS_BEGIN

// singleton stuff
static EventReceiver *s_SharedEventReceiver = nullptr;

EventReceiver::EventReceiver()
{

}
EventReceiver::~EventReceiver()
{
	_eventReceiverList.clear();
}

/** returns a shared instance of the director */
EventReceiver* EventReceiver::getInstance()
{
	if (!s_SharedEventReceiver)
	{
		s_SharedEventReceiver = new (std::nothrow) EventReceiver();
		//CCASSERT(s_SharedDirector, "FATAL: Not enough memory");
		s_SharedEventReceiver->init();
	}

	return s_SharedEventReceiver;
}


void EventReceiver::init()
{

}

void EventReceiver::registerNewRevicer(EventReceiverDelegate* reciver)
{
	_eventReceiverList.push_back(reciver);
}

bool EventReceiver::OnEvent(const SEvent& event)
{
	for (auto& eventReceiver : _eventReceiverList)
	{
		eventReceiver->OnEvent(event);
	}
	return true;
}



NS_CS_END