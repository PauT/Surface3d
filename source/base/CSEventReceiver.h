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


#ifndef	__SEVENT_RECEIVER_H__
#define __SEVENT_RECEIVER_H__

#include "platform/CSPlatformMacros.h"
#include "CSLibrary/CSVector.h"
#include <irrlicht.h>
USING_NS_IRR;


NS_CS_BEGIN


class EventReceiverDelegate
{
public:
	virtual bool OnEvent(const SEvent& event) = 0;
};

class CS_DLL EventReceiver : public IEventReceiver
{
public:
	EventReceiver(void);
	virtual ~EventReceiver();

	/** returns a shared instance of the director */
	static EventReceiver* getInstance();

	void init();

	void registerNewRevicer(EventReceiverDelegate* receiver);

	virtual bool OnEvent(const SEvent& event);

private:
	std::vector<EventReceiverDelegate *> _eventReceiverList;
};


NS_CS_END


#endif