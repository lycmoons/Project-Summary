#pragma once
#include"cocos2d.h"
USING_NS_CC;

class helpLayer : public Layer
{
public:
	//��ʼ��
	static Layer* createLayer();
	virtual bool init();
	CREATE_FUNC(helpLayer);

	//���ذ�������Ӧ
	void callbackofreturn(Ref* pSender);
};

