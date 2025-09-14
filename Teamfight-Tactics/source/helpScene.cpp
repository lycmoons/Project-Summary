#include "helpScene.h"
#include"helpLayer.h"
USING_NS_CC;

Scene* helpScene::createScene() {
	return helpScene::create();
}

bool helpScene::init() {
	if (!Scene::init()) {  //�������ʼ������
		return false;
	}

	//���Ҽ��ҵ�������
	auto layer = helpLayer::createLayer();  
	this->addChild(layer);
	return true;
}