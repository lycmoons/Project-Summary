#pragma once
#include "cocos2d.h"
USING_NS_CC;
class endScene :public Scene
{
public:
	static endScene* createScene(int c);  //0����ʧ��  1����ʤ��  2����ƽ��
	void exitCallBack(Ref* pSender);   //�˳���Ϸ
};

