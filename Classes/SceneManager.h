#ifndef __SceneManager_H__
#define __SceneManager_H__
#include "cocos2d.h"						//引入cocos2d头文件
using namespace cocos2d;					//引入cocos2d命名空间
class SceneManager{							//用于创建场景的类
public:
    void createGameScene();					//创建游戏场景方法
    void createMainScene();					//创建主界面场景方法
    void gotoGameScene();					//切换到游戏场景方法
    void gotoMainScene();					//切换到主界面方法
    void createSettingsScene();				//创建设置界面场景方法
    void gotoSettingsScene();				//切换到设置界面方法

public:
    Scene* mainScene;						//指向主界面场景对象的指针
    Scene* gameScene;						//指向游戏场景对象的指针
    Scene* settingsScene;					//指向设置场景对象的指针
};
#endif
