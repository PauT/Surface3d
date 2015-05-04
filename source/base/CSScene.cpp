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
#include "CSScene.h"
NS_CS_BEGIN

Scene::~Scene()
{

}

/*create scene*/
Scene* Scene::create(ISceneManager* mgr)
{
	Scene *pRet = new Scene(0, mgr, 0);
	if(pRet && pRet->init())
	{
		return pRet;
	}

	return nullptr;
}

bool Scene::init()
{
	return true;
}

void Scene::render()
{

}

const core::aabbox3d<f32>& Scene::getBoundingBox() const
{
	return core::aabbox3d<f32>(-5,0,-5,5,1,5);
}


NS_CS_END