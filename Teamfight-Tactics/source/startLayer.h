#pragma once
#include "cocos2d.h"
#include "Online.h"
#include "createRoomScene.h"
#include "joinRoomScene.h"
#include "networkScene.h"
USING_NS_CC;

class startLayer : public Layer
{
public:
	//��ʼ��
	static Layer* createLayer();
	virtual bool init();
	CREATE_FUNC(startLayer);

	//������Ӧ
	void callbackofbutton1(Ref* pSender);   //��ʼ��Ϸ
	void callbackofbutton2(Ref* pSender);   //��������
	void callbackofbutton3(Ref* pSender);   //���뷿��
	void callbackofbutton4(Ref* pSender);   //����
	void callbackofbutton5(Ref* pSender);   //�˳�
};

