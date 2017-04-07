#include "WelcomeLayer.h"
#include "SceneManager.h"
#include "time.h"
using namespace cocos2d;
using namespace std;
bool WelcomeLayer::init(){			//布景的初始化方法
	if(!Layer::init())				//父Layer的初始化方法
		return false;				//如果没有初始化，返回false
	Vec2 origin=Director::getInstance()->getVisibleOrigin();//获得可见区域圆点
	Size visibleSize=Director::getInstance()->getVisibleSize();//获得可见区域大小
    Sprite* sprite=Sprite::create("pic/first_background.png");//创建背景精灵
    sprite->setAnchorPoint(Vec2(0,0));			//设置锚点
    sprite->setPosition(Vec2(origin.x,origin.y));//设置位置
    this->addChild(sprite);						//将背景精灵添加进布景
    psd=ParticleSystemQuad::create("welcome.plist");//创建自定义粒子系统
    psd->retain();							//保持引用
    psd->setBlendAdditive(true);		//设置混合方式
    psd->setPosition(Vec2(origin.x+300,origin.y+visibleSize.height/2-100));//设置粒子系统位置
    sprite->addChild(psd,10);							//将粒子系统添加进布景
	MenuItemImage* menuStartButton=MenuItemImage::create(	//创建开始按钮
			"pic/start_game.png",						//默认图片
			"pic/start_game.png",						//点击后显示图片
			CC_CALLBACK_1(WelcomeLayer::StartGame,this));	//点击开始菜单回调方法
	menuStartButton->setEnabled(true);//设置显示状态为true
	menuStartButton->setScale(2.0f);	//设置缩放状态
	menuStartButton->setPosition(Vec2(origin.x+300,origin.y+visibleSize.height/2+400)); //设置位置
	MenuItemImage* menuShezhi=MenuItemImage::create(	//创建设置菜单
				"pic/shezhi.png",			//默认图片
				"pic/shezhi.png",			//点击后显示图片
				CC_CALLBACK_1(WelcomeLayer::Shezhi,this));	//点击菜单回调方法
	menuShezhi->setEnabled(true);	//设置显示状态为true
	menuShezhi->setScale(2.0f);		//设置缩放状态
	menuShezhi->setPosition(Vec2(origin.x+300,origin.y+visibleSize.height/2+150));//设置位置
	MenuItemImage* menuExitGame=MenuItemImage::create( //创建退出菜单
					"pic/exit.png",   //默认图片
					"pic/exit.png",		//点击后显示图片
					CC_CALLBACK_1(WelcomeLayer::ExitGame,this));//点击后回调方法
	menuExitGame->setEnabled(true); //设置显示状态为true
	menuExitGame->setScale(2.0f);	//设置缩放状态
	menuExitGame->setPosition(Vec2(origin.x+300,origin.y+visibleSize.height/2-100));//设置位置
	Menu* menu=Menu::create(menuStartButton,menuShezhi,menuExitGame,NULL);//将菜单添加进菜单组
	menu->setPosition(Vec2::ZERO); //设置位置
	this->addChild(menu,20);	//添加进此布景
	startTime=clock();			//时间戳的开始
	scheduleUpdate();			//调用update方法
	return true;
}
void WelcomeLayer::StartGame(Ref* ref){	//去游戏界面场景方法
	sceneManager->gotoGameScene();		//调用切换到游戏场景方法
}
void WelcomeLayer::Shezhi(Ref* ref){	//去设置界面场景方法
//	if(!sceneManager->settingsScene)	//如果没有创建设置场景对象
//		sceneManager->createSettingsScene();//创建设置场景对象
	sceneManager->gotoSettingsScene();	//切换到设置场景对象
}
void WelcomeLayer::update(float dt){	//update回调方法
	Vec2 origin=Director::getInstance()->getVisibleOrigin();//获得可见区域圆点
	Size visibleSize=Director::getInstance()->getVisibleSize();//获得可见区域大小
	clock_t endTime=clock();			//设置新时间戳
	double temp=(endTime-startTime)/(double)CLOCKS_PER_SEC;	//两个时间戳之间的时间间隔，单位为s
	if(temp>9){
		psd->setPosition(Vec2(origin.x+300,origin.y+400)); //当9秒时更新位置
		startTime=endTime;	//将第二个时间戳赋给第一个时间戳
	}else if(temp>6){
		psd->setPosition(Vec2(origin.x+visibleSize.width-300,origin.y+visibleSize.height-300));//当6秒时更新位置
	}else if(temp>3){
		psd->setPosition(Vec2(origin.x+visibleSize.width-300,origin.y+300));//当3秒时更新位置
	}
}
void WelcomeLayer::ExitGame(Ref* ref){//退出游戏方法
	Director::getInstance()->end();  //结束游戏
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		exit(0);
	#endif
}
