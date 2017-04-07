#include "SceneManager.h"								//����SceneManagerͷ�ļ�
#include "WelcomeLayer.h"								//����WelcomeLayerͷ�ļ�
#include "GameLayer.h" 									//����GameLayerͷ�ļ�
#include "Settings.h"									//����Settingsͷ�ļ�
using namespace cocos2d;								//����cocos2d�����ռ�
void SceneManager::createGameScene(){					//������Ϸ�����ķ���
    gameScene = Scene::create();						//������Ϸ��������
    auto layer = GameLayer::create();					//����һ����ӭ��������
    layer->sceneManager=this;							//��������sceneManagerָ��ָ��˶���
    gameScene->addChild(layer);							//����ӭ������ӵ�������
}
void SceneManager::createMainScene(){					//���������泡���ķ���
	mainScene=Scene::create();							//���������泡��
	auto layer=WelcomeLayer::create();					//����WelcomeLayer��������
	layer->sceneManager=this;							//��������sceneManagerָ��ָ��˶���
	mainScene->addChild(layer);							//�������沼����ӽ�������
}
void SceneManager::createSettingsScene(){				//�������ý��泡������
	settingsScene=Scene::create();						//�������ý��泡������
	auto layer=Settings::create();						//�������ý��沼������
	//layer->retain();									//���ִ˲�������
	layer->sceneManager=this;							//��������sceneManagerָ��ָ��˶���
	settingsScene->addChild(layer);						//�����ý��沼����ӽ�������
}
void SceneManager::gotoGameScene(){						//�л�����Ϸ�����ķ���
    auto director=Director::getInstance();				//��õ���ʵ��
    createGameScene();									//������Ϸ����
    director->replaceScene(gameScene);					//�л�����Ϸ����
}
void SceneManager::gotoMainScene(){						//�л���������ķ���
	auto director=Director::getInstance();				//��õ���ʵ��
	createMainScene();									//���������泡��
	director->replaceScene(mainScene);					//�л��������泡��
}
void SceneManager::gotoSettingsScene(){					//�л������ý��泡���ķ���
	auto director=Director::getInstance();				//��õ���ʵ��
	createSettingsScene();
	director->replaceScene(settingsScene);				//�л������ý��泡��
}
