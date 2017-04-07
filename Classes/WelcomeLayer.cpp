#include "WelcomeLayer.h"
#include "SceneManager.h"
#include "time.h"
using namespace cocos2d;
using namespace std;
bool WelcomeLayer::init(){			//�����ĳ�ʼ������
	if(!Layer::init())				//��Layer�ĳ�ʼ������
		return false;				//���û�г�ʼ��������false
	Vec2 origin=Director::getInstance()->getVisibleOrigin();//��ÿɼ�����Բ��
	Size visibleSize=Director::getInstance()->getVisibleSize();//��ÿɼ������С
    Sprite* sprite=Sprite::create("pic/first_background.png");//������������
    sprite->setAnchorPoint(Vec2(0,0));			//����ê��
    sprite->setPosition(Vec2(origin.x,origin.y));//����λ��
    this->addChild(sprite);						//������������ӽ�����
    psd=ParticleSystemQuad::create("welcome.plist");//�����Զ�������ϵͳ
    psd->retain();							//��������
    psd->setBlendAdditive(true);		//���û�Ϸ�ʽ
    psd->setPosition(Vec2(origin.x+300,origin.y+visibleSize.height/2-100));//��������ϵͳλ��
    sprite->addChild(psd,10);							//������ϵͳ��ӽ�����
	MenuItemImage* menuStartButton=MenuItemImage::create(	//������ʼ��ť
			"pic/start_game.png",						//Ĭ��ͼƬ
			"pic/start_game.png",						//�������ʾͼƬ
			CC_CALLBACK_1(WelcomeLayer::StartGame,this));	//�����ʼ�˵��ص�����
	menuStartButton->setEnabled(true);//������ʾ״̬Ϊtrue
	menuStartButton->setScale(2.0f);	//��������״̬
	menuStartButton->setPosition(Vec2(origin.x+300,origin.y+visibleSize.height/2+400)); //����λ��
	MenuItemImage* menuShezhi=MenuItemImage::create(	//�������ò˵�
				"pic/shezhi.png",			//Ĭ��ͼƬ
				"pic/shezhi.png",			//�������ʾͼƬ
				CC_CALLBACK_1(WelcomeLayer::Shezhi,this));	//����˵��ص�����
	menuShezhi->setEnabled(true);	//������ʾ״̬Ϊtrue
	menuShezhi->setScale(2.0f);		//��������״̬
	menuShezhi->setPosition(Vec2(origin.x+300,origin.y+visibleSize.height/2+150));//����λ��
	MenuItemImage* menuExitGame=MenuItemImage::create( //�����˳��˵�
					"pic/exit.png",   //Ĭ��ͼƬ
					"pic/exit.png",		//�������ʾͼƬ
					CC_CALLBACK_1(WelcomeLayer::ExitGame,this));//�����ص�����
	menuExitGame->setEnabled(true); //������ʾ״̬Ϊtrue
	menuExitGame->setScale(2.0f);	//��������״̬
	menuExitGame->setPosition(Vec2(origin.x+300,origin.y+visibleSize.height/2-100));//����λ��
	Menu* menu=Menu::create(menuStartButton,menuShezhi,menuExitGame,NULL);//���˵���ӽ��˵���
	menu->setPosition(Vec2::ZERO); //����λ��
	this->addChild(menu,20);	//��ӽ��˲���
	startTime=clock();			//ʱ����Ŀ�ʼ
	scheduleUpdate();			//����update����
	return true;
}
void WelcomeLayer::StartGame(Ref* ref){	//ȥ��Ϸ���泡������
	sceneManager->gotoGameScene();		//�����л�����Ϸ��������
}
void WelcomeLayer::Shezhi(Ref* ref){	//ȥ���ý��泡������
//	if(!sceneManager->settingsScene)	//���û�д������ó�������
//		sceneManager->createSettingsScene();//�������ó�������
	sceneManager->gotoSettingsScene();	//�л������ó�������
}
void WelcomeLayer::update(float dt){	//update�ص�����
	Vec2 origin=Director::getInstance()->getVisibleOrigin();//��ÿɼ�����Բ��
	Size visibleSize=Director::getInstance()->getVisibleSize();//��ÿɼ������С
	clock_t endTime=clock();			//������ʱ���
	double temp=(endTime-startTime)/(double)CLOCKS_PER_SEC;	//����ʱ���֮���ʱ��������λΪs
	if(temp>9){
		psd->setPosition(Vec2(origin.x+300,origin.y+400)); //��9��ʱ����λ��
		startTime=endTime;	//���ڶ���ʱ���������һ��ʱ���
	}else if(temp>6){
		psd->setPosition(Vec2(origin.x+visibleSize.width-300,origin.y+visibleSize.height-300));//��6��ʱ����λ��
	}else if(temp>3){
		psd->setPosition(Vec2(origin.x+visibleSize.width-300,origin.y+300));//��3��ʱ����λ��
	}
}
void WelcomeLayer::ExitGame(Ref* ref){//�˳���Ϸ����
	Director::getInstance()->end();  //������Ϸ
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		exit(0);
	#endif
}
