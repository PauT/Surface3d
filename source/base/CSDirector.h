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
#include "base/CSScene.h"
#include "CSLibrary/CSVector.h"

#include <irrlicht.h>
USING_NS_IRR;


NS_CS_BEGIN
class CS_DLL Director : public IReferenceCounted, 
	public EventReceiverDelegate
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

	/* show fps */
	void setDisplayStats(bool displayStats);

	/* set screen size*/
	void setScreenSize(int width, int height);

	/* set full screen*/
	void setFullScreen(bool isSetFullscreen);

	/* is Editor mode*/
	CS_SYNTHESIZE(bool, _gEditMode, IsEditMode);

	// Scene Management

    /** Enters the Director's main loop with the given Scene.
     * Call it to run only your FIRST scene.
     * Don't call it if there is already a running scene.
     *
     * It will call pushScene: and then it will call startAnimation
     */
    void runWithScene(Scene *scene);

    /** Suspends the execution of the running scene, pushing it on the stack of suspended scenes.
     * The new scene will be executed.
     * Try to avoid big stacks of pushed scenes to reduce memory allocation. 
     * ONLY call it if there is a running scene.
     */
    void pushScene(Scene *scene);

    /** Pops out a scene from the stack.
     * This scene will replace the running one.
     * The running scene will be deleted. If there are no more scenes in the stack the execution is terminated.
     * ONLY call it if there is a running scene.
     */
    void popScene();

    /** Pops out all scenes from the stack until the root scene in the queue.
     * This scene will replace the running one.
     * Internally it will call `popToSceneStackLevel(1)`
     */
    void popToRootScene();

    /** Pops out all scenes from the stack until it reaches `level`.
     If level is 0, it will end the director.
     If level is 1, it will pop all scenes until it reaches to root scene.
     If level is <= than the current stack level, it won't do anything.
     */
 	void popToSceneStackLevel(int level);

    /** Replaces the running scene with a new one. The running scene is terminated.
     * ONLY call it if there is a running scene.
     */
    void replaceScene(Scene *scene);


	virtual bool OnEvent(const SEvent& event);
protected:
	/* purge data */
	void purgeDirector();
	bool _purgeDirectorInNextLoop; // this flag will be set to true in end()
	
	void setNextScene();


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

	/* The running scene */
    Scene *_runningScene;
    
    /* will be the next 'runningScene' in the next frame
     nextScene is a weak reference. */
    Scene *_nextScene;
    
    /* If true, then "old" scene will receive the cleanup message */
    bool _sendCleanupToScene;

    /* scheduled scenes */
    Vector<Scene*> _scenesStack;


	/*last fps optmize */
	int lastFPS;
	/* stat label */
	IGUIStaticText* statsText;
	/*switch of the stats show*/
	bool _displayStats;
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