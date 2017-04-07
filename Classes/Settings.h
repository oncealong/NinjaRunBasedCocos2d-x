#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "cocos2d.h"
#include "SceneManager.h"
#include "GameLayer.h"

using namespace cocos2d;
using namespace std;

class Settings : public cocos2d::Layer
{
public:
	virtual bool init();

public:
	MenuItemImage *music;
	MenuItemImage *sound;
	MenuItemImage *back;
    bool musicFlag;
	bool soundFlag;

	void menuMusicCallback(Ref* ref);
	void menuSoundCallback(Ref* ref);
	void menuBackCallback(Ref* ref);

	SceneManager *sceneManager;

   CREATE_FUNC(Settings);
};

#endif
