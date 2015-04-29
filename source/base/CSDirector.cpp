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
#include "CSDirector.h"
#include <irrlicht.h>
USING_NS_IRR;

NS_CS_BEGIN

// singleton stuff
static DisplayLinkDirector *s_SharedDirector = nullptr;

Director::Director()
{
}

Director::~Director()
{
	_gDevice->drop();
}

/** returns a shared instance of the director */
Director* Director::getInstance()
{
	if (!s_SharedDirector)
	{
		s_SharedDirector = new (std::nothrow) DisplayLinkDirector();
		//CCASSERT(s_SharedDirector, "FATAL: Not enough memory");
		s_SharedDirector->init();
	}

	return s_SharedDirector;
}

bool Director::init()
{
	_gDevice = createDevice( video::EDT_OPENGL, dimension2d<u32>(480, 320), 16,
		false, false, false, 0);

	getIrrDevice()->setWindowCaption(L"hello surface 3D");
	setIrrSceneManager(getIrrDevice()->getSceneManager());
	setIrrGUIEnvironment(getIrrDevice()->getGUIEnvironment());
	getIrrSceneManager()->addCameraSceneNode(0, vector3df(0,30,-40), vector3df(0,5,0));

	return true;
}


NS_CS_END