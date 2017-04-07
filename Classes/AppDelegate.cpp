#include "AppDelegate.h"
#include "SimpleAudioEngine.h"
#include "SceneManager.h"

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
	return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
		glview = GLViewImpl::createWithRect("GameExample", Rect(0, 0, designResolutionSize.height, designResolutionSize.width));
#else
		glview = GLViewImpl::create("GameExample");
#endif
		director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
	director->setAnimationInterval(1.0 / 60);

    //FileUtils::getInstance()->addSearchPath("res");

	// Set the design resolution
	//glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
	glview->setDesignResolutionSize(1080, 1920, ResolutionPolicy::NO_BORDER);
	Size frameSize = glview->getFrameSize();

	register_all_packages();

	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("sound/backmusic1.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/jump1.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/down.wav");

	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/backmusic1.mp3", true);

	UserDefault::getInstance()->setBoolForKey("soundFlag", true);

	director->setAnimationInterval(1.0 / 60);
    // create a scene. it's an autorelease object
	SceneManager *sceneManager = new SceneManager();
	sceneManager->createMainScene();
	director->runWithScene(sceneManager->mainScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
	CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
	CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
