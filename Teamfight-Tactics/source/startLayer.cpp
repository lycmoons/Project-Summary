#include "startLayer.h"
#include"helpScene.h"
#include"playerScene.h"
USING_NS_CC;

extern int BuildRoom;
extern int JoinRoom;
extern int GameinProgress;
extern int Isconnected;

extern rapidjson::Document document;
extern cocos2d::network::WebSocket* webSocket;

Layer* startLayer::createLayer() {
	return startLayer::create();  //ֱ�ӵ���create����
}


bool startLayer::init() {
	if (!Layer::init()) {   //�������ʼ������
		return false;
	}

	//�������
	auto Background = Sprite::create("startSceneBackground.png");
	Background->setPosition(955, 540);
	this->addChild(Background, 0);  //��ӵ��Ҽ�


	//�������ѡ���ͼƬ����
	/* ��ʼ��Ϸ */
	auto button11 = Sprite::create("startSceneButton11.png");
	auto button12 = Sprite::create("startSceneButton12.png");
	/* �������� */
	auto button21 = Sprite::create("startSceneButton21.png");
	auto button22 = Sprite::create("startSceneButton22.png");
	/* ���뷿�� */
	auto button31 = Sprite::create("startSceneButton31.png");
	auto button32 = Sprite::create("startSceneButton32.png");
	/* ���� */
	auto button41 = Sprite::create("startSceneButton41.png");
	auto button42 = Sprite::create("startSceneButton42.png");
	/* �˳� */
	auto button51 = Sprite::create("startSceneButton51.png");
	auto button52 = Sprite::create("startSceneButton52.png");

	//���ͼƬΪMenu��
	auto button1 = MenuItemSprite::create(button11, button12,
		CC_CALLBACK_1(startLayer::callbackofbutton1, this));
	auto button2 = MenuItemSprite::create(button21, button22,
		CC_CALLBACK_1(startLayer::callbackofbutton2, this));
	auto button3 = MenuItemSprite::create(button31, button32,
		CC_CALLBACK_1(startLayer::callbackofbutton3, this));
	auto button4 = MenuItemSprite::create(button41, button42,
		CC_CALLBACK_1(startLayer::callbackofbutton4, this));
	auto button5 = MenuItemSprite::create(button51, button52,
		CC_CALLBACK_1(startLayer::callbackofbutton5, this));

	auto startMenu = Menu::create(button1, button2, button3, button4, button5, NULL);  //һ�����˵�
	startMenu->alignItemsVerticallyWithPadding(15);  //���ô�ֱ����
	startMenu->setPosition(960, 400);  //�ڳ����еĲ���
	this->addChild(startMenu, 1);   //��ӵ��Ҽ�

	return true;
}


//������Ӧ

/* ��ʼ��Ϸ */
void startLayer::callbackofbutton1(Ref* pSender)   //����ģʽ
{
	auto scene = playerScene::createScene();
	Director::getInstance()->pushScene(scene);
}

/* �������� */
void startLayer::callbackofbutton2(Ref* pSender)   //��������
{
	if (Isconnected && !BuildRoom) {
		BuildRoom = 1;
		SendJSONstring(document);
		ClientSocket::onSend(document);
		auto createroomscene = createRoomScene::createScene();
		Director::getInstance()->pushScene(createroomscene);   //ѹջ
	}
	else if (!Isconnected) {
		auto label = Label::createWithTTF("Not connected to the server!", "fonts/arial.ttf", 100);
		label->setPosition(955, 700);
		this->addChild(label, 20);
		// ʹ�ö�ʱ�����ȣ�1���ִ�лص�����
		this->scheduleOnce([label](float dt) {
			// �ڻص��������Ƴ���ǩ
			label->removeFromParent();
			}, 1.0f, "remove_label");
	}
	else {
		auto label = Label::createWithTTF("Room Already Exists!", "fonts/arial.ttf", 100);
		label->setPosition(955, 700);
		this->addChild(label, 20);
		// ʹ�ö�ʱ�����ȣ�1���ִ�лص�����
		this->scheduleOnce([label](float dt) {
			// �ڻص��������Ƴ���ǩ
			label->removeFromParent();
			}, 1.0f, "remove_label");
	}
}

/* ���뷿�� */
void startLayer::callbackofbutton3(Ref* pSender)   //���뷿��
{
	if (Isconnected && BuildRoom) {
		JoinRoom = 1;
		SendJSONstring(document);
		ClientSocket::onSend(document);
		auto joinroomscene = joinRoomScene::createScene();
		Director::getInstance()->pushScene(joinroomscene);   //ѹջ
	}
	else if (!Isconnected) {
		auto label = Label::createWithTTF("Not connected to the server!", "fonts/arial.ttf", 100);
		label->setPosition(955, 700);
		this->addChild(label, 20);
		// ʹ�ö�ʱ�����ȣ�1���ִ�лص�����
		this->scheduleOnce([label](float dt) {
			// �ڻص��������Ƴ���ǩ
			label->removeFromParent();
			}, 1.0f, "remove_label");
	}
	else {
		auto label = Label::createWithTTF("The room has not been created!", "fonts/arial.ttf", 100);
		label->setPosition(955, 700);
		this->addChild(label, 20);
		// ʹ�ö�ʱ�����ȣ�1���ִ�лص�����
		this->scheduleOnce([label](float dt) {
			// �ڻص��������Ƴ���ǩ
			label->removeFromParent();
			}, 1.0f, "remove_label");
	}
}

/* ���� */
void startLayer::callbackofbutton4(Ref* pSender)   //����
{
	auto helpscene = helpScene::createScene();
	Director::getInstance()->pushScene(helpscene);   //ѹջ
}

/* �˳� */
void startLayer::callbackofbutton5(Ref* pSender)   //�˳�
{
	Director::getInstance()->end();   //�˳�
}