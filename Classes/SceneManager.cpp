#include "SceneManager.h"								//引入SceneManager头文件
#include "WelcomeLayer.h"								//引入WelcomeLayer头文件
#include "GameLayer.h" 									//引入GameLayer头文件
#include "Settings.h"									//引入Settings头文件
using namespace cocos2d;								//引入cocos2d命名空间
void SceneManager::createGameScene(){					//创建游戏场景的方法
    gameScene = Scene::create();						//创建游戏场景对象
    auto layer = GameLayer::create();					//创建一个欢迎布景对象
    layer->sceneManager=this;							//将布景中sceneManager指针指向此对象
    gameScene->addChild(layer);							//将欢迎布景添加到场景中
}
void SceneManager::createMainScene(){					//创建主界面场景的方法
	mainScene=Scene::create();							//创建主界面场景
	auto layer=WelcomeLayer::create();					//创建WelcomeLayer布景对象
	layer->sceneManager=this;							//将布景中sceneManager指针指向此对象
	mainScene->addChild(layer);							//将主界面布景添加进场景中
}
void SceneManager::createSettingsScene(){				//创建设置界面场景方法
	settingsScene=Scene::create();						//创建设置界面场景对象
	auto layer=Settings::create();						//创建设置界面布景对象
	//layer->retain();									//保持此布景对象
	layer->sceneManager=this;							//将布景中sceneManager指针指向此对象
	settingsScene->addChild(layer);						//将设置界面布景添加进场景中
}
void SceneManager::gotoGameScene(){						//切换到游戏场景的方法
    auto director=Director::getInstance();				//获得导演实例
    createGameScene();									//创建游戏场景
    director->replaceScene(gameScene);					//切换到游戏场景
}
void SceneManager::gotoMainScene(){						//切换到主界面的方法
	auto director=Director::getInstance();				//获得导演实例
	createMainScene();									//创建主界面场景
	director->replaceScene(mainScene);					//切换到主界面场景
}
void SceneManager::gotoSettingsScene(){					//切换到设置界面场景的犯法
	auto director=Director::getInstance();				//获得导演实例
	createSettingsScene();
	director->replaceScene(settingsScene);				//切换到设置界面场景
}
