#include "startScene.h"
#include "startLayer.h"
#include "cocos/audio/include/AudioEngine.h"

USING_NS_CC;


Scene* startScene::createScene() {
	return startScene::create();   //ֱ�ӵ���create����
}

bool startScene::init() {
	if (!Scene::init()) {  //�������ʼ������
		return false;
	}


	//connectToServer();



	// ��������
	int audioId = AudioEngine::play2d("welcome.mp3", -1);
	AudioEngine::setVolume(audioId, 0.05);

	
	auto item1 = cocos2d::MenuItemImage::create(
		"musicControl11.png", "musicControl12.png", CC_CALLBACK_1(startScene::musicControl, this));
	auto item2 = cocos2d::MenuItemImage::create(
		"musicControl21.png", "musicControl22.png", CC_CALLBACK_1(startScene::musicControl, this));
	auto toggleItem = cocos2d::MenuItemToggle::createWithCallback(CC_CALLBACK_1(startScene::musicControl, this), item1, item2, nullptr);
	toggleItem->setSelectedIndex(0);   // ���ó�ʼ��ť��
	auto menu = cocos2d::Menu::create(toggleItem, nullptr);
	menu->setPosition(1260, 700);
	menu->setScale(0.5);
	addChild(menu, 2);
	




	auto startlayer = startLayer::createLayer();  //��ȡ��ʼ���泡���ĹҼ�
	addChild(startlayer, 1);   //��ӹҼ���Ⱦ
	return true;
}

void startScene::musicControl(Ref* sender) {
	// ����˵����л��¼�
	auto toggleItem = dynamic_cast<cocos2d::MenuItemToggle*>(sender);
	if (toggleItem) {
		int selectedIndex = toggleItem->getSelectedIndex();
		if (selectedIndex == 0) {
			int audioId = AudioEngine::play2d("welcome.mp3", -1);
			AudioEngine::setVolume(audioId, 0.05);
		}
		else {
			AudioEngine::stopAll();
		}
	}
}

void startScene::onEnter() {
	Scene::onEnter();
	connectToServer();
}

