#ifndef _WelcomeLayer_H__
#define _WelcomeLayer_H__
#include "cocos2d.h"								//引入cocos2d头文件
#include "SceneManager.h"							//引入SceneManager头文件
using namespace cocos2d;							//引入cocos2d命名空间
using namespace std;								//引入std命名空间
class WelcomeLayer:public cocos2d::Layer{			//WelcomeLayer类的声明
  public:
	clock_t startTime;                      		//时间戳的开始标志
	ParticleSystemQuad *psd;						//粒子系统的指针
	SceneManager* sceneManager;						//场景管理指针
	virtual bool init();							//初始化布景
	virtual void update(float dt);					//定时回调的方法
	void StartGame(Ref* ref);						//切换到游戏场景
	void Shezhi(Ref* ref);							//切换到设置场景
	void ExitGame(Ref* ref);						//退出游戏
	CREATE_FUNC(WelcomeLayer);						//定义的宏，创建指定对象并做好相应初始化及释放工作
};
#endif
