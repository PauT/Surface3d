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
	_gDevice = createDevice( video::EDT_OPENGL, dimension2d<u32>(1024, 768), 16,
		false, false, false, 0);

	getIrrDevice()->setWindowCaption(L"hello surface 3D");
	setIrrSceneManager(getIrrDevice()->getSceneManager());
	setIrrGUIEnvironment(getIrrDevice()->getGUIEnvironment());
	getIrrSceneManager()->addCameraSceneNode(0, vector3df(0,30,-40), vector3df(0,5,0));


	_purgeDirectorInNextLoop = false;

	setFPS(60);

	setDisplayStats(true);

	lastFPS = 0;


	_runningScene = nullptr;
	_nextScene = nullptr;
	_scenesStack.reserve(15);

	return true;
}

void Director::purgeDirector()
{

}


void Director::setNextDeltaTimeZero(bool nextDeltaTimeZero)
{
	_nextDeltaTimeZero = nextDeltaTimeZero;
}


void Director::setDisplayStats(bool displayStats)
{
	if(displayStats)
	{
		statsText = getIrrGUIEnvironment()->addStaticText(L" ", rect<s32>(10,10,100,50), false);
	} else 
	{
		statsText->remove();
	}

	_displayStats = displayStats;
}


/* set screen size*/
void Director::setScreenSize(int width, int height)
{
	//getIrrDevice()->setResizable()
}

/* set full screen*/
void Director::setFullScreen(bool isSetFullscreen)
{
	//if(getIrrDevice())
	//	getIrrDevice()->switchToFullScreen(isSetFullscreen);
}


// scene management

void Director::runWithScene(Scene *scene)
{
	_IRR_DEBUG_BREAK_IF(scene == nullptr);
	_IRR_DEBUG_BREAK_IF(_runningScene != nullptr);

	pushScene(scene);
	startMainLoop();
}

void Director::replaceScene(Scene *scene)
{
	//CCASSERT(_runningScene, "Use runWithScene: instead to start the director");
	_IRR_DEBUG_BREAK_IF(scene == nullptr);

	if (_runningScene == nullptr) {
		runWithScene(scene);
		return;
	}

	if (scene == _nextScene)
		return;

	if (_nextScene)
	{
		/*if (_nextScene->isRunning())
		{
			_nextScene->onExit();
		}
		_nextScene->cleanup();*/
		_nextScene->drop();
		_nextScene = nullptr;
	}

	ssize_t index = _scenesStack.size();

	_sendCleanupToScene = true;
	_scenesStack.replace(index - 1, scene);
	_nextScene = scene;
}

void Director::pushScene(Scene *scene)
{
	_IRR_DEBUG_BREAK_IF(scene == nullptr);

	_sendCleanupToScene = false;

	_scenesStack.pushBack(scene);
	_nextScene = scene;
}

void Director::popScene(void)
{
	_IRR_DEBUG_BREAK_IF(_runningScene == nullptr);

	_scenesStack.popBack();
	ssize_t c = _scenesStack.size();

	if (c == 0)
	{
		//end();
		_purgeDirectorInNextLoop = true;
	}
	else
	{
		_sendCleanupToScene = true;
		_nextScene = _scenesStack.at(c - 1);
	}
}

void Director::popToRootScene(void)
{
	popToSceneStackLevel(1);
}

void Director::popToSceneStackLevel(int level)
{
	_IRR_DEBUG_BREAK_IF(_runningScene == nullptr);
	ssize_t c = _scenesStack.size();

	// level 0? -> end
	if (level == 0)
	{
		//end();
		_purgeDirectorInNextLoop = true;
		return;
	}

	// current level or lower -> nothing
	if (level >= c)
		return;

	auto fisrtOnStackScene = _scenesStack.back();
	if (fisrtOnStackScene == _runningScene)
	{
		_scenesStack.popBack();
		--c;
	}

	// pop stack until reaching desired level
	while (c > level)
	{
		auto current = _scenesStack.back();

		/*if (current->isRunning())
		{
			current->onExit();
		}

		current->cleanup();*/
		_scenesStack.popBack();
		--c;
	}

	_nextScene = _scenesStack.back();

	// cleanup running scene
	_sendCleanupToScene = true;
}


void Director::setNextScene()
{
	/*bool runningIsTransition = dynamic_cast<TransitionScene*>(_runningScene) != nullptr;
	bool newIsTransition = dynamic_cast<TransitionScene*>(_nextScene) != nullptr;

	// If it is not a transition, call onExit/cleanup
	if (! newIsTransition)
	{
		if (_runningScene)
		{
			_runningScene->onExitTransitionDidStart();
			_runningScene->onExit();
		}

		// issue #709. the root node (scene) should receive the cleanup message too
		// otherwise it might be leaked.
		if (_sendCleanupToScene && _runningScene)
		{
			_runningScene->cleanup();
		}
	}*/

	if (_runningScene)
	{
		_runningScene->drop();
	}
	_runningScene = _nextScene;
	_nextScene->grab();
	_nextScene = nullptr;

	/*if ((! runningIsTransition) && _runningScene)
	{
		_runningScene->onEnter();
		_runningScene->onEnterTransitionDidFinish();
	}*/
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

		/* to avoid flickr, nextScene MUST be here: after tick and before draw.
		* FIXME: Which bug is this one. It seems that it can't be reproduced with v0.9
		*/
		if (_nextScene)
		{
			setNextScene();
		}
		getIrrDevice()->getVideoDriver()->beginScene(true, true, SColor(255,100,101,140));
		getIrrSceneManager()->drawAll();
		getIrrGUIEnvironment()->drawAll();
		getIrrDevice()->getVideoDriver()->endScene();
		// release the objects
		//PoolManager::getInstance()->getCurrentPool()->clear();

		int fps = getIrrDevice()->getVideoDriver()->getFPS();

		if (_displayStats && lastFPS != fps)
		{
			core::stringw str = L"FPS:";
			str += fps;
			statsText->setText(str.c_str());
			lastFPS = fps;
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