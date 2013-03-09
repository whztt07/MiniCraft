/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2012 Torus Knot Software Ltd
Also see acknowledgements in Readme.html

You may use this sample code for anything you like, it is not covered by the
same license as the rest of the engine.
-----------------------------------------------------------------------------
*/

#include "DeferredShading.h"

#include "OgreConfigFile.h"
#include "OgreStringConverter.h"
#include "OgreException.h"

#include "OgreRoot.h"
#include "OgreRenderSystem.h"

#include "OgreEntity.h"
#include "OgreSubEntity.h"
#include "OgreRoot.h"

#include "OgreCompositor.h"
#include "OgreCompositorManager.h"
#include "OgreCompositorChain.h"
#include "OgreCompositorInstance.h"

#include "OgreLogManager.h"

#include "DeferredLightCP.h"
#include "SSAOLogic.h"
#include "GBufferSchemeHandler.h"
#include "NullSchemeHandler.h"


using namespace Ogre;

const Ogre::uint8 DeferredShadingSystem::PRE_GBUFFER_RENDER_QUEUE = Ogre::RENDER_QUEUE_1;
const Ogre::uint8 DeferredShadingSystem::POST_GBUFFER_RENDER_QUEUE = Ogre::RENDER_QUEUE_8;

DeferredShadingSystem::DeferredShadingSystem(
		Viewport *vp, SceneManager *sm,  Camera *cam
	):
	mViewport(vp), mSceneMgr(sm), mCamera(cam)
{
}

void DeferredShadingSystem::initialize()
{
	for(int i=0; i<DSM_COUNT; ++i)
		mInstance[i]=0;

	createResources();
	
	mActive = false;
	
	mSSAO = false;
	mCurrentMode = DSM_SHOWLIT;
	setActive(true);
}

DeferredShadingSystem::~DeferredShadingSystem()
{
	CompositorChain *chain = CompositorManager::getSingleton().getCompositorChain(mViewport);
	for(int i=0; i<DSM_COUNT; ++i)
		chain->_removeInstance(mInstance[i]);
	CompositorManager::getSingleton().removeCompositorChain(mViewport);

	Ogre::CompositorManager& compMgr = Ogre::CompositorManager::getSingleton();
	CompositorLogicMap::const_iterator itor = mCompositorLogics.begin();
	CompositorLogicMap::const_iterator end  = mCompositorLogics.end();
	while( itor != end )
	{
		compMgr.unregisterCompositorLogic( itor->first );
		delete itor->second;
		++itor;
	}
	mCompositorLogics.clear();
}

void DeferredShadingSystem::setMode(DSMode mode)
{
	assert( 0 <= mode && mode < DSM_COUNT);

	// prevent duplicate setups
	if (mCurrentMode == mode && mInstance[mode]->getEnabled()==mActive)
		return;

	for(int i=0; i<DSM_COUNT; ++i)
	{
		if(i == mode)
		{
			mInstance[i]->setEnabled(mActive);
		}
		else
		{
			mInstance[i]->setEnabled(false);
		}
	}

	mCurrentMode = mode;

	mSSAOInstance->setEnabled(mActive && mSSAO && mCurrentMode == DSM_SHOWLIT);
}

void DeferredShadingSystem::setSSAO(bool ssao)
{
	if (ssao != mSSAO) 
	{
		mSSAO = ssao;
		if (mActive && mCurrentMode == DSM_SHOWLIT)
		{
			mSSAOInstance->setEnabled(ssao);
		}
	}
}
	
bool DeferredShadingSystem::getSSAO() const
{
	return mSSAO;
}
void DeferredShadingSystem::setActive(bool active)
{
	if (mActive != active)
	{
		mActive = active;
		mGBufferInstance->setEnabled(active);

		// mCurrentMode could have changed with a prior call to setMode, so iterate all
		setMode(mCurrentMode);
	}
}

DeferredShadingSystem::DSMode DeferredShadingSystem::getMode(void) const
{
	return mCurrentMode;
}

void DeferredShadingSystem::createResources(void)
{
	CompositorManager &compMan = CompositorManager::getSingleton();

	//Hook up the compositor logic and scheme handlers.
	//This can theoretically happen in a loaded plugin, but in this case the demo contains the code.
	static bool firstTime = true;
	if (firstTime)
	{
		MaterialManager::getSingleton().addListener(new GBufferSchemeHandler, "GBuffer");
		MaterialManager::getSingleton().addListener(new NullSchemeHandler, "NoGBuffer");

		compMan.registerCustomCompositionPass("DeferredLight", new DeferredLightCompositionPass);

		firstTime = false;
	}

	mCompositorLogics["SSAOLogic"] = new SSAOLogic;
	compMan.registerCompositorLogic("SSAOLogic", mCompositorLogics["SSAOLogic"]);

	// Create the main GBuffer compositor
	mGBufferInstance = compMan.addCompositor(mViewport, "DeferredShading/GBuffer");
	
	// Create filters
	mInstance[DSM_SHOWLIT] = compMan.addCompositor(mViewport, "DeferredShading/ShowLit");
	mInstance[DSM_SHOWNORMALS] = compMan.addCompositor(mViewport, "DeferredShading/ShowNormals");
	mInstance[DSM_SHOWDSP] = compMan.addCompositor(mViewport, "DeferredShading/ShowDepthSpecular");
	mInstance[DSM_SHOWCOLOUR] = compMan.addCompositor(mViewport, "DeferredShading/ShowColour");

	mSSAOInstance =  compMan.addCompositor(mViewport, "DeferredShading/SSAO");
}


void DeferredShadingSystem::logCurrentMode(void)
{
	if (mActive==false)
	{
		LogManager::getSingleton().logMessage("No Compositor Enabled!");
		return;
	}

	CompositorInstance* ci = mInstance[mCurrentMode];
	assert(ci->getEnabled()==true);

	LogManager::getSingleton().logMessage("Current Mode: ");
	LogManager::getSingleton().logMessage(ci->getCompositor()->getName());
}