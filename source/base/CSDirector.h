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


/**
	This class is base class for the new project.
	Default Set is:


*/

#ifndef	__SDIRECTOR_H__
#define __SDIRECTOR_H__

#include "platform/CSPlatformMacros.h"
#include "platform/CSApplication.h"

#include <irrlicht.h>
USING_NS_IRR;


NS_CS_BEGIN
class CS_DLL Director : public IReferenceCounted
{
public:
	Director(void);
	virtual ~Director();

	/** returns a shared instance of the director */
	static Director* getInstance();
	virtual bool init();

	/**  main loop */
	virtual void mainLoop() = 0;

	/** Stops the animation. Nothing will be drawn. The main loop won't be triggered anymore.
     If you don't want to pause your animation call [pause] instead.
     */
    virtual void stopMainLoop() = 0;

    /** The main loop is triggered again.
     Call this function only if [stopAnimation] was called earlier
     @warning Don't call this function to start the main loop. To run the main loop call runWithScene
     */
    virtual void startMainLoop() = 0;

	/** Set the FPS value. */
	virtual void setFPS(int FPS) = 0;

	inline bool isNextDeltaTimeZero() { return _nextDeltaTimeZero; }
	void setNextDeltaTimeZero(bool nextDeltaTimeZero);

protected:
	/* purge data */
	void purgeDirector();
	bool _purgeDirectorInNextLoop; // this flag will be set to true in end()
	

	//global device
	CS_SYNTHESIZE(IrrlichtDevice*, _gDevice, IrrDevice);
	//global scene mgr
	CS_SYNTHESIZE(ISceneManager*, _gSceneMgr, IrrSceneManager);
	//global gui environment
	CS_SYNTHESIZE(IGUIEnvironment*, _gGUIEnv, IrrGUIEnvironment);

	/* whether or not the next delta time will be zero */
	bool _nextDeltaTimeZero;

	/* _animationInterval = 1 / FPS */
	double _animationInterval;
	double _oldAnimationInterval;

};


/** 
 @brief DisplayLinkDirector is a Director that synchronizes timers with the refresh rate of the display.
 
 Features and Limitations:
  - Scheduled timers & drawing are synchronizes with the refresh rate of the display
  - Only supports animation intervals of 1/60 1/30 & 1/15
 
 @since v0.8.2
 */
class DisplayLinkDirector : public Director
{
public:
    DisplayLinkDirector() 
        : _invalid(false)
    {}
    virtual ~DisplayLinkDirector(){}

    //
    // Overrides
    //
    virtual void mainLoop() override;
	virtual void setFPS(int FPS) override;
    virtual void stopMainLoop() override;
    virtual void startMainLoop() override;

protected:
    bool _invalid;
};

NS_CS_END

#endif