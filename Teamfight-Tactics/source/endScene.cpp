#include "endScene.h"
endScene* endScene::createScene(int c) {
	endScene* scene = new endScene;
	if (scene) {  //��������ɹ�
		scene->autorelease();
		Sprite* background;
		// ����c��Ϊ��������ͬ�ĳ���ͼ
		switch (c) {
			case 0:  //ʧ��
				background = Sprite::create("endbackground0.png");
				scene->addChild(background, 0);
				background->setPosition(955, 540);
				break;
			case 1:  //ʤ��
				background = Sprite::create("endbackground1.png");
				scene->addChild(background, 0);
				background->setPosition(955, 540);
				break;
			case 2:  //ƽ��
				background = Sprite::create("endbackground2.png");
				scene->addChild(background, 0);
				background->setPosition(955, 540);
				break;
		}

		// ����һ��menu  ���ڵ���˳�������Ϸ��
		auto button1 = Sprite::create("exitmenu1.png");
		auto button2 = Sprite::create("exitmenu2.png");
		auto button = MenuItemSprite::create(button1, button2,
			CC_CALLBACK_1(endScene::exitCallBack, scene));
		auto menu = Menu::create(button, NULL);
		menu->setPosition(960, 400);  //�ڳ����еĲ���
		scene->addChild(menu, 1);   //��ӵ��Ҽ�
		return scene;
	}

	CC_SAFE_DELETE(scene);
	return nullptr;
}

void endScene::exitCallBack(Ref* pSender) {
	Director::getInstance()->end();   //�˳�
}