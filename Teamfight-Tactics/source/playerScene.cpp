#include "playerScene.h"
#include"heroSprite.h"
#include<vector>
#include"endScene.h"

USING_NS_CC;

#define STORETAG   462

std::vector<Sprite*>MySeat;  //��СΪ14
/*
  0����ս��ϯ0
  1����ս��ϯ1
  2����ս��ϯ2
  3����ս��ϯ3
  4����ս��ϯ4

  5������սϯ0
  6������սϯ1
  7������սϯ2
  8������սϯ3
  9������սϯ4
  10������սϯ5
  11������սϯ6
  12������սϯ7
  13������սϯ8
*/

int enemyType = 1;  // ��Եĵ�һ���˻�Ϊenemyrole1

chessboardSeat seat1(1);   //����λ��
preparationSeat seat2(1);   //��սϯλ��

chessboardSeat seat3(0);   //�з�����λ��
preparationSeat seat4(0);   //�з���սϯλ��

extern Vector<Hero*> allMyHeroes;//�ҷ�����Ӣ��
extern Vector<Hero*> allEnemyHeroes;//�з�����Ӣ��

extern int fight;  //ȫ��ս��

playerScene* playerScene::createScene() {
	return playerScene::create();
}

bool playerScene::init() {

	if (!Scene::create()) {
		return 0;
	}





	//��������
	auto background = Sprite::create("playerSceneBackground.png");
	this->addChild(background, 0);
	background->setPosition(955, 540);





	//����ͼƬ��ʼ��
	equip_for_blood = Sprite::create("equip_for_blood.png");    // �������Ѫ��666
	equip_for_power = Sprite::create("equip_for_power.png");    // ���ӹ�����100
	equip_for_magic = Sprite::create("equip_for_magic.png");    // ����ÿ���չ�����10
	
	equip_for_blood->setScale(0.2);
	equip_for_power->setScale(0.2);
	equip_for_magic->setScale(0.2);

	equip_for_blood->setPosition(200, 750);
	equip_for_power->setPosition(200, 650);
	equip_for_magic->setPosition(200, 550);

	this->addChild(equip_for_blood, 4);
	this->addChild(equip_for_power, 4);
	this->addChild(equip_for_magic, 4);





	//�ͼ���ʼ��
	attacktram = Sprite::create("attacktrammel.png");
	healtram = Sprite::create("healtrammel.png");

	attacktram->setPosition(172 + 70, 900 + 50);
	attacktram->setScale(0.5);
	healtram->setPosition(172 + 70, 900 + 50);
	healtram->setScale(0.6);

	this->addChild(attacktram, 4);
	this->addChild(healtram, 4);
	attacktram->setVisible(false);
	healtram->setVisible(false);





	//���ð����Ĳ���
	setting = settingLayer::createLayer();  // ���ý����ʼ��
	setting->setPosition(0, 0);
	this->addChild(setting, 10);
	setting->setVisible(false);

	auto settingbutton1 = Sprite::create("setting1.png");
	auto settingbutton2 = Sprite::create("setting2.png");
	auto settingbutton = MenuItemSprite::create(settingbutton1, settingbutton2,
		CC_CALLBACK_1(playerScene::settingCallBack, this));
	auto menu = Menu::create(settingbutton, NULL);
	menu->setPosition(940,520);
	this->addChild(menu, 4);
	menu->setScale(0.1);




	
	// ��������
	AudioEngine::stopAll();
	int audioId = AudioEngine::play2d("background_music.mp3", -1);
	AudioEngine::setVolume(audioId, 0.05);





	// �ҷ��ĳ�ʼӢ��
	auto hero1 = Hero::createHero(1, Vec2(seat1.seats[0].x + 70, seat1.seats[0].y + 50), 1);
	seat1.seats[0].state = 1;
	this->addChild(hero1, 2);
	hero1->enterBoard();
	hero1->setIndex(0);





	/* װ�����ƶ�������ͽ */
	auto equipmentListener1 = EventListenerTouchOneByOne::create();
	equipmentListener1->setSwallowTouches(true);
	//װ������ж�
	equipmentListener1->onTouchBegan = [this](Touch* t, Event* e)->bool {
		Vec2 myclick = t->getLocation();
		myclick = equip_for_blood->convertToNodeSpace(myclick);
		Rect spriteBoundingBox = Rect(0, 0, equip_for_blood->getContentSize().width, equip_for_blood->getContentSize().height);
		if (spriteBoundingBox.containsPoint(myclick)) {
			return true;
		}
		return false;
	};
	//װ�����ƶ�
	equipmentListener1->onTouchMoved = [this](Touch* t, Event* e)->void {
		equip_for_blood->setPosition(equip_for_blood->getPosition() + t->getDelta());
	};
	//Ӣ�۷���
	equipmentListener1->onTouchEnded = [this](Touch* t, Event* e)->void {
		int find = 0;
		for (Hero* hero : allMyHeroes) {
			Vec2 clickEnd = t->getLocation();
			clickEnd = hero->convertToNodeSpace(clickEnd);
			Rect spriteBoundingBox = Rect(0, 0, hero->getContentSize().width, hero->getContentSize().height);
			if (spriteBoundingBox.containsPoint(clickEnd)) {  // װ���ɹ�
				// Ч��
				hero->setMaxBlood(hero->getMaxBlood() + 666);
				hero->setBlood(hero->getBlood() + 666);
				hero->equip_blood = 1;
				find = 1;

				// װ������ʧЧ������С����ʧ����Ч��
				auto scale = ScaleTo::create(0.2f, 0.01f);
				auto disappear = CallFunc::create([this]() {
					equip_for_blood->setVisible(false);
				});
				auto music = CallFunc::create([]() {
					int audioId = AudioEngine::play2d("equip.mp3");
				    AudioEngine::setVolume(audioId, 1.0f);
				});
				auto sequence = Sequence::create(scale, disappear, music, nullptr);
				equip_for_blood->runAction(sequence);

				break;
			}
		}
		if (find == 0) {  // û��װ���ɹ�������ԭλ
			putback_equip_for_blood();
		}
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(equipmentListener1, this);






	/* װ�����ƶ������� */
	auto equipmentListener2 = EventListenerTouchOneByOne::create();
	equipmentListener2->setSwallowTouches(true);
	//װ������ж�
	equipmentListener2->onTouchBegan = [this](Touch* t, Event* e)->bool {
		Vec2 myclick = t->getLocation();
		myclick = equip_for_power->convertToNodeSpace(myclick);
		Rect spriteBoundingBox = Rect(0, 0, equip_for_power->getContentSize().width, equip_for_power->getContentSize().height);
		if (spriteBoundingBox.containsPoint(myclick)) {
			return true;
		}
		return false;
	};
	//װ�����ƶ�
	equipmentListener2->onTouchMoved = [this](Touch* t, Event* e)->void {
		equip_for_power->setPosition(equip_for_power->getPosition() + t->getDelta());
	};
	//Ӣ�۷���
	equipmentListener2->onTouchEnded = [this](Touch* t, Event* e)->void {
		int find = 0;
		for (Hero* hero : allMyHeroes) {
			Vec2 clickEnd = t->getLocation();
			clickEnd = hero->convertToNodeSpace(clickEnd);
			Rect spriteBoundingBox = Rect(0, 0, hero->getContentSize().width, hero->getContentSize().height);
			if (spriteBoundingBox.containsPoint(clickEnd)) {  // װ���ɹ�
				// Ч��
				hero->setPower(hero->getPower() + 100);
				hero->equip_power = 1;
				find = 1;

				// װ������ʧЧ������С����ʧ����Ч��
				auto scale = ScaleTo::create(0.2f, 0.01f);
				auto disappear = CallFunc::create([this]() {
					equip_for_power->setVisible(false);
				});
				auto music = CallFunc::create([]() {
					int audioId = AudioEngine::play2d("equip.mp3");
				    AudioEngine::setVolume(audioId, 1.0f);
				});
				auto sequence = Sequence::create(scale, disappear, music, nullptr);
				equip_for_power->runAction(sequence);

				break;
			}
		}
		if (find == 0) {  // û��װ���ɹ�������ԭλ
			putback_equip_for_power();
		}
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(equipmentListener2, this);






	/* װ�����ƶ������൶ */
	auto equipmentListener3 = EventListenerTouchOneByOne::create();
	equipmentListener3->setSwallowTouches(true);
	//װ������ж�
	equipmentListener3->onTouchBegan = [this](Touch* t, Event* e)->bool {
		Vec2 myclick = t->getLocation();
		myclick = equip_for_magic->convertToNodeSpace(myclick);
		Rect spriteBoundingBox = Rect(0, 0, equip_for_magic->getContentSize().width, equip_for_magic->getContentSize().height);
		if (spriteBoundingBox.containsPoint(myclick)) {
			return true;
		}
		return false;
	};
	//װ�����ƶ�
	equipmentListener3->onTouchMoved = [this](Touch* t, Event* e)->void {
		equip_for_magic->setPosition(equip_for_magic->getPosition() + t->getDelta());
	};
	//Ӣ�۷���
	equipmentListener3->onTouchEnded = [this](Touch* t, Event* e)->void {
		int find = 0;
		for (Hero* hero : allMyHeroes) {
			Vec2 clickEnd = t->getLocation();
			clickEnd = hero->convertToNodeSpace(clickEnd);
			Rect spriteBoundingBox = Rect(0, 0, hero->getContentSize().width, hero->getContentSize().height);
			if (spriteBoundingBox.containsPoint(clickEnd)) {  // װ���ɹ�
				// Ч��
				hero->setEachMagic(hero->getEachMagic() + 10);
				hero->equip_magic = 1;
				find = 1;

				// װ������ʧЧ������С����ʧ����Ч��
				auto scale = ScaleTo::create(0.2f, 0.01f);
				auto disappear = CallFunc::create([this]() {
					equip_for_magic->setVisible(false);
				});
				auto music = CallFunc::create([]() {
					int audioId = AudioEngine::play2d("equip.mp3");
				    AudioEngine::setVolume(audioId, 1.0f);
				});
				auto sequence = Sequence::create(scale, disappear, music, nullptr);
				equip_for_magic->runAction(sequence);

				break;
			}
		}
		if (find == 0) {  // û��װ���ɹ�������ԭλ
			putback_equip_for_magic();
		}
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(equipmentListener3, this);






	/*Ӣ��λ���ƶ�*/
	auto changePosListener = EventListenerTouchOneByOne::create();
	//�¼���û
	changePosListener->setSwallowTouches(true);   
	//Ӣ�۵���ж�
	changePosListener->onTouchBegan = [this](Touch* t, Event* e)->bool {
		//��ȡ�����λ��

		//ս������ֻ�ܶԱ�սϯ��Ӣ�۵����Ч
		if (fight) {
			for (Hero* hero : allMyHeroes) {
				if (!hero->isInBoard()) {  //����ս��ϯ��
					Vec2 myclick = t->getLocation();
					myclick = hero->convertToNodeSpace(myclick);
					Rect spriteBoundingBox = Rect(0, 0, hero->getContentSize().width, hero->getContentSize().height);
					if (spriteBoundingBox.containsPoint(myclick)) {
						hero->SetClicked(1);
						return true;
					}
				}
			}
			return false;  //��û�е������սϯ���κ�Ӣ��
		}
		else {  //��ս�����ڶ����м���Ӣ�۵������Ч
			for (Hero* hero : allMyHeroes) {
				Vec2 myclick = t->getLocation();
				myclick = hero->convertToNodeSpace(myclick);
				Rect spriteBoundingBox = Rect(0, 0, hero->getContentSize().width, hero->getContentSize().height);
				if (spriteBoundingBox.containsPoint(myclick)) {
					hero->SetClicked(1);
					return true;
				}
			}
			return false;  //��û�е�����κ�Ӣ��
		}

	};
	//Ӣ�۵��ƶ�
	changePosListener->onTouchMoved = [](Touch* t, Event* e)->void {
		for (Hero* hero : allMyHeroes) {
			if (hero->IsClicked()) {
				hero->setPosition(hero->getPosition() + t->getDelta());
				break;
			}
		}
	};
	//Ӣ�۷���
	changePosListener->onTouchEnded = [this](Touch* t, Event* e)->void {
		if (fight) {  //������ֻ�����Ƿ����ڱ�սϯ��
			int i;
			for (i = 5; i < 14; i++) {
				Vec2 clickEnd = t->getLocation();
				clickEnd = MySeat[i]->convertToNodeSpace(clickEnd);
				Rect spriteBoundingBox = Rect(0, 0, MySeat[i]->getContentSize().width, MySeat[i]->getContentSize().height);
				if (spriteBoundingBox.containsPoint(clickEnd)) {
					int index = i - 5;  //��սϯλ���±�
					
					//�жϸ�λ���Ƿ�Ϊ��
					if (seat2.seats[index].state == 0) {  //��λ��
						for (Hero* hero : allMyHeroes) {
							if (hero->IsClicked()) {  //�ҵ��������Ӣ��
								seat2.seats[hero->GetIndex()].Removesprite();  //ԭλ����Ϊ��
								hero->setIndex(index);  //Ӣ�۴洢���±�
								hero->setPosition(seat2.seats[index].x + 70, seat2.seats[index].y + 50);
								seat2.seats[index].Setsprite();
								hero->SetClicked(0);  //���õ��
								break;
							}
						}
					}
					else {   //�ǿ�λ��
						for (Hero* hero1 : allMyHeroes) {
							if (hero1->IsClicked()) {  //�ҵ��������Ӣ��
								for (Hero* hero2 : allMyHeroes) {
									if (!hero2->isInBoard() && hero2->GetIndex() == index) {
										//����λ��
										hero2->setIndex(hero1->GetIndex());
										hero1->setIndex(index);
										hero1->setPosition(seat2.seats[hero1->GetIndex()].x + 70, seat2.seats[hero1->GetIndex()].y + 50);
										hero2->setPosition(seat2.seats[hero2->GetIndex()].x + 70, seat2.seats[hero2->GetIndex()].y + 50);
										hero1->SetClicked(0);
										break;
									}
								}

								break;
							}
						}
					}

					break;  //�˳�����������սϯλ��
				}
			}
			if (i == 14) {  //�ص�ԭ��
				for (Hero* hero : allMyHeroes) {
					if (hero->IsClicked()) {
						hero->setPosition(seat2.seats[hero->GetIndex()].x + 70, seat2.seats[hero->GetIndex()].y + 50);
						hero->SetClicked(0);
						break;
					}
				}
			}
		}
		else {   //�����㿼�����е�λ
			int i;
			for (i = 0; i < 14; i++) {
				Vec2 clickEnd = t->getLocation();
				clickEnd = MySeat[i]->convertToNodeSpace(clickEnd);
				Rect spriteBoundingBox = Rect(0, 0, MySeat[i]->getContentSize().width, MySeat[i]->getContentSize().height);
				if (spriteBoundingBox.containsPoint(clickEnd)) {  //ͣ������ĳ��λ��
					if (i >= 5) {  //ͣ����λ���Ǳ�սϯ
						int index = i - 5;  //��սϯλ���±�

						//�жϸ�λ���Ƿ�Ϊ��
						if (seat2.seats[index].state == 0) {  //��λ��
							for (Hero* hero : allMyHeroes) {
								if (hero->IsClicked()) {  //�ҵ��������Ӣ��
									if (hero->isInBoard()) {  //ԭλ����ս��ϯ
										seat1.seats[hero->GetIndex()].Removesprite();  //ԭλ����Ϊ��
									}
									else {   //ԭλ���ڱ�սϯ
										seat2.seats[hero->GetIndex()].Removesprite();  //ԭλ����Ϊ��
									}
									
									hero->setIndex(index);  //Ӣ�۴洢���±�
									hero->setPosition(seat2.seats[index].x + 70, seat2.seats[index].y + 50);
									seat2.seats[index].Setsprite();
									hero->exitBoard();  //�ص���սϯ
									hero->SetClicked(0);  //���õ��
									break;
								}
							}
						}
						else {   //�ǿ�λ��
							for (Hero* hero1 : allMyHeroes) {
								if (hero1->IsClicked()) {  //�ҵ��������Ӣ��
									for (Hero* hero2 : allMyHeroes) {
										if (!hero2->isInBoard() && hero2->GetIndex() == index) {
											//����λ��
											hero2->setIndex(hero1->GetIndex());
											hero1->setIndex(index);
											hero1->setPosition(seat2.seats[hero1->GetIndex()].x + 70, seat2.seats[hero1->GetIndex()].y + 50);
											if (!hero1->isInBoard())
											{
												hero2->setPosition(seat2.seats[hero2->GetIndex()].x + 70, seat2.seats[hero2->GetIndex()].y + 50);
											}
											else {
												hero2->setPosition(seat1.seats[hero2->GetIndex()].x + 70, seat1.seats[hero2->GetIndex()].y + 50);
											}
											hero1->SetClicked(0);
											hero2->SetBoard(hero1->isInBoard());
											hero1->exitBoard();
											break;
										}
									}

									break;
								}
							}
						}
					}
					else {   //ͣ����λ����ս��ϯ
						int index = i;
						if (seat1.seats[index].state == 0) {  //��λ��
							int hero_in_board = 0;
							for (Hero* hero : allMyHeroes) {
								if (hero->isInBoard()) {
									hero_in_board++;
								}
							}
							for (Hero* hero : allMyHeroes) {
								if (hero->IsClicked()) {  //�ҵ��������Ӣ��
									if (hero->isInBoard()) {  //ԭλ����ս��ϯ
										seat1.seats[hero->GetIndex()].Removesprite();  //ԭλ����Ϊ��
										hero->setIndex(index);  //Ӣ�۴洢���±�
										hero->setPosition(seat1.seats[index].x + 70, seat1.seats[index].y + 50);
										seat1.seats[index].Setsprite();
										hero->enterBoard();  //��ս��
										hero->SetClicked(0);  //���õ��
									}
									else {   //ԭλ���ڱ�սϯ
										if (hero_in_board >= store->level)  //��������
										{
											hero->setPosition(seat2.seats[hero->GetIndex()].x + 70, seat2.seats[hero->GetIndex()].y + 50);
										}
										else {
											seat2.seats[hero->GetIndex()].Removesprite();  //ԭλ����Ϊ��
											hero->setIndex(index);  //Ӣ�۴洢���±�
											hero->setPosition(seat1.seats[index].x + 70, seat1.seats[index].y + 50);
											seat1.seats[index].Setsprite();
											hero->enterBoard();  //��ս��
											hero->SetClicked(0);  //���õ��
										}
									}
									break;
								}
							}
						}
						else {   //�ǿ�λ��
							for (Hero* hero1 : allMyHeroes) {
								if (hero1->IsClicked()) {  //�ҵ��������Ӣ��
									for (Hero* hero2 : allMyHeroes) {
										if (hero2->isInBoard() && hero2->GetIndex() == index) {
											//����λ��
											hero2->setIndex(hero1->GetIndex());
											hero1->setIndex(index);
											hero1->setPosition(seat1.seats[hero1->GetIndex()].x + 70, seat1.seats[hero1->GetIndex()].y + 50);
											if(!hero1->isInBoard())
											{
												hero2->setPosition(seat2.seats[hero2->GetIndex()].x + 70, seat2.seats[hero2->GetIndex()].y + 50);
											}
											else {
												hero2->setPosition(seat1.seats[hero2->GetIndex()].x + 70, seat1.seats[hero2->GetIndex()].y + 50);
											}
											hero1->SetClicked(0);
											hero2->SetBoard(hero1->isInBoard());
											hero1->enterBoard();
											break;
										}
									}

									break;
								}
							}
						}
					}

					break;  //�˳�����������սϯλ��
				}
			}
			if (i == 14) {  //�ص�ԭ��
				for (Hero* hero : allMyHeroes) {
					if (hero->IsClicked()) {
						if(!hero->isInBoard())
						{
							hero->setPosition(seat2.seats[hero->GetIndex()].x + 70, seat2.seats[hero->GetIndex()].y + 50);
						}
						else {
							hero->setPosition(seat1.seats[hero->GetIndex()].x + 70, seat1.seats[hero->GetIndex()].y + 50);
						}
						hero->SetClicked(0);
						break;
					}
				}
			}
		}
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(changePosListener, this);





	//ÿһ֡����Ӣ�۵�Ѫ��������
	this->schedule([this](float dt) {
		//�����ҷ�Ӣ��Ѫ��������
		for (Hero* hero : allMyHeroes) {
			hero->updatebloodandmagic();
		}
        //���µз�Ӣ�۵�Ѫ��������
	    for (Hero* hero : allEnemyHeroes) {
		    hero->updatebloodandmagic();
	    }
	}, "updateherobar");





	//ÿһ֡����ССӢ�۵�Ѫ������Ϸ���˳�
	this->schedule([this](float dt) {
		float mypercentage = myrole->cur_blood / myrole->max_blood * 100.0f;
	    myrole->healthBar->setPercentage(mypercentage);
		if (enemyType == 2)
		{
			float enemypercentage = enemyrole1->cur_blood / enemyrole1->max_blood * 100.0f;
			enemyrole1->healthBar->setPercentage(enemypercentage);
			if (enemypercentage == 0) {
				enemyrole1->setVisible(false);
				seeEnemy1->setEnabled(false);
			}
		}
		else {
			float enemypercentage = enemyrole2->cur_blood / enemyrole2->max_blood * 100.0f;
			enemyrole2->healthBar->setPercentage(enemypercentage);
			if (enemypercentage == 0) {
				enemyrole2->setVisible(false);
				seeEnemy2->setEnabled(false);
			}
		}

		if (myrole->cur_blood == 0 && (enemyrole1->cur_blood != 0 || enemyrole2->cur_blood != 0)) {  //�ҷ�ʧ��
			auto delayAction = DelayTime::create(1.0f); // 1���ӵ��ӳ�ʱ��
			auto callback = CallFunc::create([]() {
				auto endscene = endScene::createScene(0);
			    Director::getInstance()->replaceScene(endscene);
				AudioEngine::stopAll();
				int audioId = AudioEngine::play2d("failure.mp3");
				AudioEngine::setVolume(audioId, 1.0f);
			});
			auto sequence = Sequence::create(delayAction, callback, nullptr);
			this->runAction(sequence);
		}
		if (myrole->cur_blood != 0 && enemyrole1->cur_blood == 0 && enemyrole2->cur_blood == 0) {  //�з�ʧ��
			auto delayAction = DelayTime::create(1.0f); // 1���ӵ��ӳ�ʱ��
			auto callback = CallFunc::create([]() {
				auto endscene = endScene::createScene(1);
			    Director::getInstance()->replaceScene(endscene);
				AudioEngine::stopAll();
				int audioId = AudioEngine::play2d("victory.mp3");
				AudioEngine::setVolume(audioId, 1.0f);
			});
			auto sequence = Sequence::create(delayAction, callback, nullptr);
			this->runAction(sequence);
		}
		if (myrole->cur_blood == 0 && enemyrole1->cur_blood == 0 && enemyrole2->cur_blood == 0) {  //ƽ��
			auto delayAction = DelayTime::create(1.0f); // 1���ӵ��ӳ�ʱ��
			auto callback = CallFunc::create([]() {
				auto endscene = endScene::createScene(2);
			    Director::getInstance()->replaceScene(endscene);
			});
			auto sequence = Sequence::create(delayAction, callback, nullptr);
			this->runAction(sequence);
		}
	}, "updaterolebar");






	//����������
	progress0 = Sprite::create("progress0.png");
	progress1 = ProgressTimer::create(Sprite::create("progress1.png"));
	progress0->setPosition(955, 1000);
	progress1->setPosition(955, 1000);
	this->addChild(progress0, 5);
	this->addChild(progress1, 6);
	progress1->setType(ProgressTimer::Type::BAR);
	progress1->setMidpoint(Vec2(0, 0.5)); // ������
	progress1->setBarChangeRate(Vec2(1, 0)); // ��ˮƽ����ı�
	





	//��Ϸ��ѭ���Ļص�����
	this->schedule(CC_SCHEDULE_SELECTOR(playerScene::startGame), 1.0f);





	/*�ҷ�λ��ͼ�����ʾ(����ʾ�ҷ�)*/
	//λ��ͼ���ʼ��
	for (int i = 0; i < 14; i++) {
		MySeat.push_back(Sprite::create("seats.png"));
	}
	//ͼ����ʾ
	for (int i = 0; i < 5; i++) {
		MySeat[i]->setScale(0.25);
		MySeat[i]->setPosition(seat1.seats[i].x + 70, seat1.seats[i].y + 50);
		this->addChild(MySeat[i], 1);
	}
	for (int i = 0; i < 9; i++) {
		MySeat[i + 5]->setScale(0.25);
		MySeat[i + 5]->setPosition(seat2.seats[i].x + 70, seat2.seats[i].y + 50);
		this->addChild(MySeat[i + 5], 1);
	}





	/* ����� */
	/*
	
	  �������type 1��2��4   //ÿ5����з���Ѫ100
	  �ظ����type 3��5��6   //ÿ5����ҷ���Ѫ100
	
	*/
	/* ����� */
	this->schedule(CC_SCHEDULE_SELECTOR(playerScene::detect_attack_in), 0.01f);
	this->schedule([this](float dt) {
		//�ҷ������Ч��
		int tag1 = 0;
	    int tag2 = 0;
	    int tag4 = 0;
	    for (Hero* hero : allMyHeroes) {
			if (hero->isInBoard() && hero->getType() == 1) {
				tag1 = 1;
			}
			else if (hero->isInBoard() && hero->getType() == 2) {
		    	tag2 = 1;
		    }
		    else if (hero->isInBoard() && hero->getType() == 4) {
			    tag4 = 1;
		    }
	    }
		if (tag1 && tag2 && tag4)
		{
			if (fight) {  // ս������
				for (Hero* hero : allEnemyHeroes) {
					if (hero->isInBoard() && !hero->isDead()) {
						//�����ͼ�����š����Ч
						auto scale1 = ScaleTo::create(0.5f, 0.8f);
						auto scale2 = ScaleTo::create(0.5f, 0.5f);
						auto sequence = Sequence::create(scale1, scale2, nullptr);
						attacktram->runAction(sequence);
						int audioId = AudioEngine::play2d("trammel.mp3");
						AudioEngine::setVolume(audioId, 1.0f);
						
						// ��Ѫ
						hero->decreaseHealth(200);
					}
				}
			}
		}

		//�з������Ч��
		tag1 = 0;
		tag2 = 0;
		tag4 = 0;
		for (Hero* hero : allEnemyHeroes) {
			if (hero->isInBoard() && hero->getType() == 1) {
				tag1 = 1;
			}
			else if (hero->isInBoard() && hero->getType() == 2) {
				tag2 = 1;
			}
			else if (hero->isInBoard() && hero->getType() == 4) {
				tag4 = 1;
			}
		}
		if (tag1 && tag2 && tag4)
		{
			if (fight) {  // ս������
				for (Hero* hero : allMyHeroes) {
					if (hero->isInBoard() && !hero->isDead()) {
						hero->decreaseHealth(200);
					}
				}
			}
		}

	}, 5.0f, "attacktrammel");

	/* �ָ�� */
	this->schedule(CC_SCHEDULE_SELECTOR(playerScene::detect_heal_in), 0.01f);
	this->schedule([this](float dt) {
		//�ҷ��ָ��Ч��
		int tag3 = 0;
	    int tag5 = 0;
	    int tag6 = 0;
	    for (Hero* hero : allMyHeroes) {
		    if (hero->isInBoard() && hero->getType() == 3) {
			    tag3 = 1;
		    }
		    else if (hero->isInBoard() && hero->getType() == 5) {
			    tag5 = 1;
		    }
		    else if (hero->isInBoard() && hero->getType() == 6) {
			    tag6 = 1;
		    }
	    }
		if (tag3 && tag5 && tag6)
		{
			if (fight) {  // ս������
				for (Hero* hero : allMyHeroes) {
					if (hero->isInBoard() && !hero->isDead()) {
						//�����ͼ�����š����Ч
						auto scale1 = ScaleTo::create(0.5f, 0.9f);
						auto scale2 = ScaleTo::create(0.5f, 0.6f);
						auto sequence = Sequence::create(scale1, scale2, nullptr);
						healtram->runAction(sequence);
						int audioId = AudioEngine::play2d("trammel.mp3");
						AudioEngine::setVolume(audioId, 1.0f);

						//��Ѫ
						hero->setBlood((hero->getBlood() + 200) > hero->getMaxBlood() ? hero->getMaxBlood() : (hero->getBlood() + 200));
					}
				}
			}
		}

		//�з��ָ��Ч��
		tag3 = 0;
		tag5 = 0;
		tag6 = 0;
		for (Hero* hero : allEnemyHeroes) {
			if (hero->isInBoard() && hero->getType() == 3) {
				tag3 = 1;
			}
			else if (hero->isInBoard() && hero->getType() == 5) {
				tag5 = 1;
			}
			else if (hero->isInBoard() && hero->getType() == 6) {
				tag6 = 1;
			}
		}
		if (tag3 && tag5 && tag6)
		{
			if (fight) {  // ս������
				for (Hero* hero : allEnemyHeroes) {
					if (hero->isInBoard() && !hero->isDead()) {
						hero->setBlood((hero->getBlood() + 200) > hero->getMaxBlood() ? hero->getMaxBlood() : (hero->getBlood() + 200));
					}
				}
			}
		}

	}, 5.0f, "healtrammel");






	//Ӣ����������ͨ��������Ҽ���ʵ��
	auto sellheroListener = EventListenerMouse::create();
	sellheroListener->onMouseDown = [this](EventMouse* event)->void {
		if (event->getMouseButton() == EventMouse::MouseButton::BUTTON_RIGHT) {
			//��ս������ֻ������սϯ�ϵ�Ӣ��
			if (fight) {
				for (Hero* hero : allMyHeroes) {
					if (!hero->isInBoard()) {  //����ս��ϯ��
						Vec2 touchLocation = Vec2(event->getCursorX(), event->getCursorY());
						touchLocation = hero->convertToNodeSpace(touchLocation);
						Rect spriteBoundingBox = Rect(0, 0, hero->getContentSize().width, hero->getContentSize().height);
						if (spriteBoundingBox.containsPoint(touchLocation)) {
							//�̵��Ǯ��Ҫ��ʾˢ��
							store->gold += (store->herocost[hero->GetheroType()]) * pow(3, (hero->getLevel()) - 1);
							store->updateUI();

							// װ������
							if (hero->equip_blood) {
								putback_equip_for_blood();
							}
							if (hero->equip_power) {
								putback_equip_for_power();
							}
							if (hero->equip_magic) {
								putback_equip_for_magic();
							}

							//Ӣ����ʧ
							seat2.seats[hero->GetIndex()].Removesprite();
							allMyHeroes.eraseObject(hero);
							hero->removeFromParent();
							break;
						}
					}
				}
			}
			else {  //��ս�����ڣ����Ե������Ӣ��
				for (Hero* hero : allMyHeroes) {
					Vec2 touchLocation = Vec2(event->getCursorX(), event->getCursorY());
					touchLocation = hero->convertToNodeSpace(touchLocation);
					Rect spriteBoundingBox = Rect(0, 0, hero->getContentSize().width, hero->getContentSize().height);
					
					if (spriteBoundingBox.containsPoint(touchLocation)) {
						//�̵��Ǯ��Ҫ��ʾˢ��
						store->gold += (store->herocost[hero->GetheroType()]) * pow(3, (hero->getLevel()) - 1);
						store->updateUI();

						// װ������
						if (hero->equip_blood) {
							putback_equip_for_blood();
						}
						if (hero->equip_power) {
							putback_equip_for_power();
						}
						if (hero->equip_magic) {
							putback_equip_for_magic();
						}

						//Ӣ����ʧ
						if (hero->isInBoard()) {
							seat1.seats[hero->GetIndex()].Removesprite();
						}
						else {
							seat2.seats[hero->GetIndex()].Removesprite();
						}
						allMyHeroes.eraseObject(hero);
						hero->removeFromParent();
						break;
					}
				}
			}
		}
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(sellheroListener, this);






	//ССӢ��
	myrole = playerroleLayer::createLayer(0);
	this->addChild(myrole, 3, ROLETAG);
	enemyrole1= playerroleLayer::createLayer(1);
	this->addChild(enemyrole1, 3);
	enemyrole2 = playerroleLayer::createLayer(2);
	this->addChild(enemyrole2, 3);






	// Ӣ���б���
	/* �˻�1 */
	auto enemy11 = Sprite::create("enemy11.png");
	auto enemy12 = Sprite::create("enemy12.png");
	auto enemy13 = Sprite::create("enemy13.png");
	seeEnemy1 = MenuItemSprite::create(enemy11, enemy12, enemy13,
		CC_CALLBACK_1(playerScene::seeEnemy1_callback, this));
	auto see_enemy1 = Menu::create(seeEnemy1, NULL);
	this->addChild(see_enemy1, 4);
	see_enemy1->setScale(0.7);
	see_enemy1->setPosition(1400, 700);
	
	/* �˻�2 */
	auto enemy21 = Sprite::create("enemy21.png");
	auto enemy22 = Sprite::create("enemy22.png");
	auto enemy23 = Sprite::create("enemy23.png");
	seeEnemy2 = MenuItemSprite::create(enemy21, enemy22, enemy23,
		CC_CALLBACK_1(playerScene::seeEnemy2_callback, this));
	auto see_enemy2 = Menu::create(seeEnemy2, NULL);
	this->addChild(see_enemy2, 4);
	see_enemy2->setScale(0.7);
	see_enemy2->setPosition(1400, 600);

	/* ���ذ��� */
	auto goback1 = Sprite::create("goback1.png");
	auto goback2 = Sprite::create("goback2.png");
	goback = MenuItemSprite::create(goback1, goback2,
		CC_CALLBACK_1(playerScene::goback_callback, this));
	auto go_back = Menu::create(goback, NULL);
	this->addChild(go_back, 4);
	go_back->setScale(0.7);
	go_back->setPosition(1400, 500);
	goback->setVisible(false);






	//�̵꣬��Scene�е�˽�г�Ա
	store = storeLayer::createLayer();   
	this->addChild(store, 4);






	//ССӢ�۵��ƶ�
	auto roleMoveListener = EventListenerTouchOneByOne::create();
	roleMoveListener->setSwallowTouches(true);    //�¼���û
	roleMoveListener->onTouchBegan = [background, this](Touch* t, Event* ec)-> bool {
		Vec2 pos = background->convertTouchToNodeSpace(t);
		if (pos.x > 323 && pos.x < 1247 && pos.y>327 && pos.y < 839)  //ССӢ���ƶ���Χ
		{
			myrole->cur_position = Vec2(pos.x + 70, pos.y + 50);
			auto role = this->myrole->getChildByTag(ROLETAG);
			role->stopAllActions();
			role->runAction(MoveTo::create(0.3, Vec2(pos.x + 55.25, pos.y + 46.875)));
			return true;   //�¼����ٴ��ݸ�����������
		}
		else {
			return false;  //�¼����Լ�������
		}
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(roleMoveListener, this);
	




	// �����鶯�ԣ��˻�ССӢ�۵�����ƶ���
	this->schedule(CC_SCHEDULE_SELECTOR(playerScene::enemyMove), 10.0f); // 10���ʱ����
	//���з�Ӣ�۵��ƶ�
	this->schedule([this](float dt) {
		enemyrole1->getChildByTag(ROLETAG)->stopAllActions();
	enemyrole1->getChildByTag(ROLETAG)->runAction(MoveTo::create(0.3f, Vec2(enemyrole1->cur_position.x - 14.75, enemyrole1->cur_position.y - 3.125)));
	enemyrole2->getChildByTag(ROLETAG)->stopAllActions();
	enemyrole2->getChildByTag(ROLETAG)->runAction(MoveTo::create(0.3f, Vec2(enemyrole2->cur_position.x - 14.75, enemyrole2->cur_position.y - 3.125)));
		}, 0.3, "enemymove");
	



	
	return 1;
}

/*

  ����ģʽ���˶�ս�������˻�����ƣ�
  ͨ����playerScene�е������з�ССӢ������ʾ�����˻�
  ����AllEnemyHeroes�ǲ���ˢ�µ�
  ��ս���ó���ս
  ÿһ��һ���˻��ֿ�
  �϶����������˻�������ȫʤ��
  ����ʧ��

  ���������˻���Ӣ�۵�������ɷ�����һ����
  ��һ���������ɵ�Ӣ�����ȼ�

*/

//��ʾ����Ӣ��
void playerScene::ShowHeroes(int tag) {
	if (tag == 0) {  //��ʾ�ҷ�
		for (Hero* hero : allMyHeroes) {
			if (hero->isInBoard()) {  //��ս����
				hero->setPosition(seat1.seats[hero->GetIndex()].x + 70, seat1.seats[hero->GetIndex()].y + 50);
			}
			else {  //�ڱ�սϯ��
				hero->setPosition(seat2.seats[hero->GetIndex()].x + 70, seat2.seats[hero->GetIndex()].y + 50);
			}
			hero->setBlood(hero->getMaxBlood());   //���ó�ʼѪ������Ѫ
			hero->setMagic(hero->getStartMagic());  //���ó�ʼ����
			hero->setColor(Color3B::WHITE);  //�ָ���ɫ���������ı���ɫ��
			hero->setVisible(true);   //������ʾ
			if (hero->isDead()) {   //����Die��־
				hero->setDie(0);
			}
		}
	}
	else{  //����enemyrole1��Ӣ��

		//ɾ��ԭ����Ӣ��
		Vector<Hero*>heroToDelete;
		for (Hero* hero : allEnemyHeroes) {
			heroToDelete.pushBack(hero);
		}
		for (Hero* hero : heroToDelete) {
			allEnemyHeroes.eraseObject(hero);
			hero->removeFromParent();
		}

		//����seat3��stateΪ0
		for (int i = 0; i < 5; i++) {
			seat3.seats[i].state = 0;
		}

		//�����ҷ�ս���ϵ�Ӣ�ۣ�������λ�á��Ǽ���������ɵз�Ӣ�۷���Vector��
		//position�������⣬ֻҪ��index��inBoardֵ��Ծͺã�Ȼ�����ò��ɼ�����ʾ��������棩
		//��Ӧ��λ��seat3��stateҪ����Ϊ1
		int enemyPriority1[5] = { 1,4,5,3,6 };  //�������Ӣ�����͵����ȼ�
		int enemyPriority2[5] = { 4,6,1,3,5 };
		int i = 0;  //ѭ������
		for (Hero* myhero : allMyHeroes) {
			if (myhero->isInBoard()) {
				Hero* enemy;
				if (tag == 1)
				{
					enemy = Hero::createHero(enemyPriority1[i], Vec2(0, 0), 0);
				}
				else if (tag == 2) {
					enemy = Hero::createHero(enemyPriority2[i], Vec2(0, 0), 0);
				}
				enemy->setIndex(myhero->GetIndex());    //�����±�
				enemy->enterBoard();    //��ս��
				this->addChild(enemy, 2);   //���볡��
				enemy->setVisible(false);   //���ò��ɼ�
				seat3.seats[myhero->GetIndex()].state = 1;   //λ���趨
				if (myhero->getLevel() >= 2)
				{
					enemy->upgrade(myhero->getLevel());  //����Ϊ��ͬ���Ǽ�
				}
				i++;
			}
		}

		//ͨ��Vector��������ʾ�з�Ӣ��
		for (Hero* hero : allEnemyHeroes) {
			if (hero->isInBoard()) {  //��ս����
				hero->setPosition(seat3.seats[hero->GetIndex()].x + 70, seat3.seats[hero->GetIndex()].y + 50);
			}
			else {  //�ڱ�սϯ��
				hero->setPosition(seat4.seats[hero->GetIndex()].x + 70, seat4.seats[hero->GetIndex()].y + 50);
			}
			hero->setBlood(hero->getMaxBlood());
			hero->setMagic(hero->getStartMagic());
			hero->setColor(Color3B::WHITE);
			hero->setVisible(true);
			if (hero->isDead()) {
				hero->setDie(0);
			}
		}
	}
	Hero::mergeHeroes();
}



//��������Ӣ��
void playerScene::CoverHeroes(int tag) {
	if (tag == 0) {
		for (Hero* hero : allMyHeroes) {
			hero->setVisible(false);
		}
	}
	else {
		for (Hero* hero : allEnemyHeroes) {
			hero->setVisible(false);
		}
	}
}


//�۲��˻�1������
void playerScene::seeEnemy1_callback(Ref* pSender) {
	seeEnemy1->setEnabled(false);  //����
	seeEnemy2->setEnabled(false);  //����
	goback->setVisible(true);  //���ذ���

	// ��ʾ�ҷ�Ӣ�۳�ȥ���ҷ��̵곷ȥ����ʾ�з�1
	CoverHeroes(0);
	store->setVisible(false);
	ShowHeroes(1);
}


//�۲��˻�2������
void playerScene::seeEnemy2_callback(Ref* pSender) {
	seeEnemy1->setEnabled(false);  //����
	seeEnemy2->setEnabled(false);  //����
	goback->setVisible(true);  //���ذ���

	// ��ʾ�ҷ�Ӣ�۳�ȥ���ҷ��̵곷ȥ����ʾ�з�1
	CoverHeroes(0);
	store->setVisible(false);
	ShowHeroes(2);
}


// ����
void playerScene::goback_callback(Ref* pSender) {
	goback->setVisible(false);
	seeEnemy1->setEnabled(true);
	seeEnemy2->setEnabled(true);

	//��ȥ�з�Ӣ�ۣ���ʾ�ҷ�Ӣ�ۺ��̵�
	CoverHeroes(1);
	ShowHeroes(0);
	store->setVisible(true);
}


// �˻�������ƶ�
void playerScene::enemyMove(float dt) {
	int x = rand() % 925 + 323;
	int y = rand() % 513 + 327;
	/* �˻�1��λ�����ݱ仯 */
	enemyrole1->cur_position = Vec2(x + 70, y + 50);
	
	x = rand() % 925 + 323;
	y = rand() % 513 + 327;
	/* �˻�2��λ�����ݱ仯 */
	enemyrole2->cur_position = Vec2(x + 70, y + 50);
}


// ���ð�����Ӧ
void playerScene::settingCallBack(Ref* sender) {
	setting->setVisible(true);
}