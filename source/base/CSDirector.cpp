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

void Director::purgeDirector()
{

}

bool Director::init()
{
	_gDevice = createDevice( video::EDT_OPENGL, dimension2d<u32>(480, 320), 16,
		false, false, false, 0);

	getIrrDevice()->setWindowCaption(L"hello surface 3D");
	setIrrSceneManager(getIrrDevice()->getSceneManager());
	setIrrGUIEnvironment(getIrrDevice()->getGUIEnvironment());
	getIrrSceneManager()->addCameraSceneNode(0, vector3df(0,30,-40), vector3df(0,5,0));

	_purgeDirectorInNextLoop = false;

	setFPS(60);

	return true;
}

void Director::setNextDeltaTimeZero(bool nextDeltaTimeZero)
{
	_nextDeltaTimeZero = nextDeltaTimeZero;
}

/***************************************************
* implementation of DisplayLinkDirector
**************************************************/

// should we implement 4 types of director ??
// I think DisplayLinkDirector is enough
// so we now only support DisplayLinkDirector
void DisplayLinkDirector::startMainLoop()
{
	//if (gettimeofday(_lastUpdate, nullptr) != 0)
	//{
	//	CCLOG("cocos2d: DisplayLinkDirector: Error on gettimeofday");
	//}

	_invalid = false;

//#ifndef WP8_SHADER_COMPILER
	Application::getInstance()->setAnimationInterval(_animationInterval);
//#endif

	// fix issue #3509, skip one fps to avoid incorrect time calculation.
	setNextDeltaTimeZero(true);
}

void DisplayLinkDirector::mainLoop()
{
	if (_purgeDirectorInNextLoop)
	{
		_purgeDirectorInNextLoop = false;
		purgeDirector();
	}
	else if (! _invalid)
	{
		getIrrDevice()->getVideoDriver()->beginScene(true, true, SColor(255,100,101,140));
		getIrrSceneManager()->drawAll();
		getIrrGUIEnvironment()->drawAll();
		getIrrDevice()->getVideoDriver()->endScene();
		// release the objects
		//PoolManager::getInstance()->getCurrentPool()->clear();

		int fps = getIrrDevice()->getVideoDriver()->getFPS();

		//if (lastFPS != fps)
		{
			core::stringw str = L"hello world [";
			str += getIrrDevice()->getVideoDriver()->getName();
			str += "] FPS:";
			str += fps;

			getIrrDevice()->setWindowCaption(str.c_str());
			//lastFPS = fps;
		}

	}
}

void DisplayLinkDirector::stopMainLoop()
{
	_invalid = true;
}

void DisplayLinkDirector::setFPS(int FPS)
{
	_animationInterval = 1.0 / FPS;
	if (! _invalid)
	{
		stopMainLoop();
		startMainLoop();
	}    
}
NS_CS_END