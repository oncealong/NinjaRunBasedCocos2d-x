#ifndef __SceneManager_H__
#define __SceneManager_H__
#include "cocos2d.h"						//����cocos2dͷ�ļ�
using namespace cocos2d;					//����cocos2d�����ռ�
class SceneManager{							//���ڴ�����������
public:
    void createGameScene();					//������Ϸ��������
    void createMainScene();					//���������泡������
    void gotoGameScene();					//�л�����Ϸ��������
    void gotoMainScene();					//�л��������淽��
    void createSettingsScene();				//�������ý��泡������
    void gotoSettingsScene();				//�л������ý��淽��

public:
    Scene* mainScene;						//ָ�������泡�������ָ��
    Scene* gameScene;						//ָ����Ϸ���������ָ��
    Scene* settingsScene;					//ָ�����ó��������ָ��
};
#endif
