#include "helpLayer.h"
USING_NS_CC;

Layer* helpLayer::createLayer() {
	return helpLayer::create();
}


bool helpLayer::init() {
	if (!Layer::init()) {  //�������ʼ������
		return false;
	}

	//����˵���ְ�
	auto Background = Sprite::create("helpLayerBackground.png");
	Background->setPosition(955, 540);
	this->addChild(Background, 0);

	//���ذ���
	auto return1 = Sprite::create("helpLayerreturn1.png");
	auto return2 = Sprite::create("helpLayerreturn2.png");

	auto returnButton = MenuItemSprite::create(return1, return2,
		CC_CALLBACK_1(helpLayer::callbackofreturn, this));

	returnButton->setPosition(150, 980);

	auto menu = Menu::create(returnButton, NULL);
	menu->setPosition(0, 0);
	this->addChild(menu, 1);

	return true;
}


//���ذ�����Ӧ
void helpLayer::callbackofreturn(Ref* pSender) {
	Director::getInstance()->popScene();   //��ʼ�����ջ
}