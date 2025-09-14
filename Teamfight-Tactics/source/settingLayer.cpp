#include "settingLayer.h"
#include "cocos/audio/include/AudioEngine.h"
#include"endScene.h"

settingLayer* settingLayer::createLayer() {
	return settingLayer::create();
}


bool settingLayer::init() {
	if (!Layer::init()) {
		return false;
	}



	//����˵���ְ�
	auto Background = Sprite::create("settingbackground.png");
	Background->setPosition(955, 540);
	this->addChild(Background, 0);




	// ��������
	auto item1 = cocos2d::MenuItemImage::create(
		"musicControl11.png", "musicControl12.png", CC_CALLBACK_1(settingLayer::musicControl, this));
	auto item2 = cocos2d::MenuItemImage::create(
		"musicControl21.png", "musicControl22.png", CC_CALLBACK_1(settingLayer::musicControl, this));
	auto toggleItem = cocos2d::MenuItemToggle::createWithCallback(CC_CALLBACK_1(settingLayer::musicControl, this), item1, item2, nullptr);
	toggleItem->setSelectedIndex(0);   // ���ó�ʼ��ť��
	auto menu = cocos2d::Menu::create(toggleItem, nullptr);
	menu->setPosition(670, 450);
	menu->setScale(0.7);
	this->addChild(menu, 1);





	// ����
	auto return1 = Sprite::create("helpLayerreturn1.png");
	auto return2 = Sprite::create("helpLayerreturn2.png");
	auto returnButton = MenuItemSprite::create(return1, return2,
		CC_CALLBACK_1(settingLayer::callbackofreturn, this));
	returnButton->setPosition(150, 980);
	auto menu2 = Menu::create(returnButton, NULL);
	menu2->setPosition(0, 0);
	this->addChild(menu2, 1);




	//�˳���Ͷ����
	auto button1 = Sprite::create("exitmenu1.png");
	auto button2 = Sprite::create("exitmenu2.png");
	auto button = MenuItemSprite::create(button1, button2,
		CC_CALLBACK_1(settingLayer::exitCallBack, this));
	auto menu3 = Menu::create(button, NULL);
	menu3->setPosition(960, 400);  //�ڳ����еĲ���
	this->addChild(menu3, 1);   //��ӵ��Ҽ�



	return true;
}

void settingLayer::musicControl(Ref* sender) {
	// ����˵����л��¼�
	auto toggleItem = dynamic_cast<cocos2d::MenuItemToggle*>(sender);
	if (toggleItem) {
		int selectedIndex = toggleItem->getSelectedIndex();
		if (selectedIndex == 0) {
			int audioId = AudioEngine::play2d("background_music.mp3", -1);
			AudioEngine::setVolume(audioId, 0.05);
		}
		else {
			AudioEngine::stopAll();
		}
	}
}

void settingLayer::callbackofreturn(Ref* sender) {
	this->setVisible(false);   // �����ùҼ���Ϊ���ɼ�
}

void settingLayer::exitCallBack(Ref* sender) {
	auto callback = CallFunc::create([]() {
		auto endscene = endScene::createScene(0);
	Director::getInstance()->replaceScene(endscene);
	AudioEngine::stopAll();
	int audioId = AudioEngine::play2d("failure.mp3");
	AudioEngine::setVolume(audioId, 1.0f);
		});
	auto sequence = Sequence::create(callback, nullptr);
	this->runAction(sequence);
}