#include "Settings.h"
#include "SimpleAudioEngine.h"
#include "GameLayer.h"

using namespace cocos2d;
using namespace std;

bool Settings::init()
{
	if(!Layer::init())
	{
		return false;
	}

	Vec2 origin=Director::getInstance()->getVisibleOrigin();
	Size visibleSize=Director::getInstance()->getVisibleSize();

	music=MenuItemImage::create(		
			"pic/yinyue1.png",
			"pic/yinyue1.png",
			CC_CALLBACK_1(Settings::menuMusicCallback,this)
			                   );
	music->setEnabled(true);
	music->setScale(1.7f);
	music->setAnchorPoint(Vec2(0,0));
	music->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2+100));

	sound=MenuItemImage::create(
			"pic/yinyue1.png",
			"pic/yinyue1.png",
			CC_CALLBACK_1(Settings::menuSoundCallback,this));
	sound->setEnabled(true);
	sound->setScale(1.7f);
	sound->setAnchorPoint(Vec2(0,0));
	sound->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2-350));

    if(UserDefault::getInstance()->getBoolForKey("musicFlag"))
	  {
	   music->setNormalSpriteFrame(SpriteFrame::create("pic/yinyue1.png",Rect(0,0,256,100)));
	  }else
	  {
	   music->setNormalSpriteFrame(SpriteFrame::create("pic/yinyue0.png",Rect(0,0,256,100)));
	  }

    if(UserDefault::getInstance()->getBoolForKey("soundFlag"))
	{
		  sound->setNormalSpriteFrame(SpriteFrame::create("pic/yinyue1.png",Rect(0,0,256,100)));
	}else
	{
		  sound->setNormalSpriteFrame(SpriteFrame::create("pic/yinyue0.png",Rect(0,0,256,100)));
	}

	back=MenuItemImage::create(
			"pic/back.png",
			"pic/back.png",
			CC_CALLBACK_1(Settings::menuBackCallback,this)
			);
	back->setEnabled(true);
	back->setScale(2.0f);
	back->setAnchorPoint(Vec2(0,0));
	back->setPosition(Vec2(origin.x+50,origin.y+300));

	Menu *menu=Menu::create(music,sound,back,NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu,10);


    Sprite *sprite=Sprite::create("pic/settings.png");
    sprite->setAnchorPoint(Vec2(0,0));
    sprite->setPosition(Vec2(origin.x,origin.y));
    this->addChild(sprite);

	return true;
}

void Settings::menuMusicCallback(Ref* ref)
{
   if(UserDefault::getInstance()->getBoolForKey("musicFlag"))
   {
	   music->setNormalSpriteFrame(SpriteFrame::create("pic/yinyue0.png",Rect(0,0,256,100)));
	   CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
	   UserDefault::getInstance()->setBoolForKey("musicFlag",false);
   }else
   {
	   music->setNormalSpriteFrame(SpriteFrame::create("pic/yinyue1.png",Rect(0,0,256,100)));
	   CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
	   UserDefault::getInstance()->setBoolForKey("musicFlag",true);
   }
}
void Settings::menuSoundCallback(Ref* ref)
{

	if(UserDefault::getInstance()->getBoolForKey("soundFlag"))
	{
		  sound->setNormalSpriteFrame(SpriteFrame::create("pic/yinyue0.png",Rect(0,0,256,100)));
		  UserDefault::getInstance()->setBoolForKey("soundFlag",false);
	}else
	{
		  sound->setNormalSpriteFrame(SpriteFrame::create("pic/yinyue1.png",Rect(0,0,256,100)));
		  UserDefault::getInstance()->setBoolForKey("soundFlag",true);
	}
}
void Settings::menuBackCallback(Ref* ref)
{
   sceneManager->gotoMainScene();
}
