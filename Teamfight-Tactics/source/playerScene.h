#pragma once
#include"cocos2d.h"
#include"preparationSeat.h"
#include"chessboardSeat.h"
#include"storeLayer.h"
#include"playerroleLayer.h"
#include "cocos/audio/include/AudioEngine.h"
#include "settingLayer.h"
USING_NS_CC;
extern Vector<Hero*> allMyHeroes;//�ҷ�����Ӣ��
extern Vector<Hero*> allEnemyHeroes;//�з�����Ӣ��
extern int fight;
extern int enemyType;

class playerScene : public Scene
{
public:
    playerroleLayer* myrole;   //�ҷ�ССӢ��
    playerroleLayer* enemyrole1;  //�˻�1
    playerroleLayer* enemyrole2;  //�˻�2

    Sprite* attacktram;  //�����ͼ��
    Sprite* healtram;    //�����ͼ��

	storeLayer* store;  //�̵�

    // ������
    Sprite* progress0;
    ProgressTimer* progress1;
    Sequence* to1;
    Sequence* gameprogress;

    // ����ʱ��
    float totalTime = 20.0f;
    float currentTime = 0.0f;

    // ����б�
    MenuItemSprite* seeEnemy1;
    MenuItemSprite* seeEnemy2;
    MenuItemSprite* goback;

    // ���ý���
    settingLayer* setting;

	static playerScene* createScene();
	virtual bool init();
	CREATE_FUNC(playerScene);
	void ShowHeroes(int IsMine);
	void CoverHeroes(int IsMine);

    Sprite* equip_for_blood;
    Sprite* equip_for_power;
    Sprite* equip_for_magic;


    // ����Ҫ��ʱ���ֶ�������ʱ��
    void startattack() {
        schedule(CC_SCHEDULE_SELECTOR(playerScene::attack), 2.0f);
    }

    // ��������
    void attack(float dt)
    {
        bool attacked = 0;

        //�ҷ���������
        for (auto myHero : allMyHeroes)
        {
            if (!myHero->isDead()&& myHero->isInBoard()) {
                Hero* target;
                bool targetFind = 0;
                long long nearst_distance = 99999999999;
                for (auto enemyHero : allEnemyHeroes) {
                    if (!enemyHero->isDead() && enemyHero->isInBoard()) {
                        targetFind = 1;
                        long long x_distance = myHero->getPosition().x - enemyHero->getPosition().x;
                        long long y_distance = myHero->getPosition().y - enemyHero->getPosition().y;
                        long long distance = pow(x_distance, 2) + pow(y_distance, 2);
                        if (distance < nearst_distance) {
                            target = enemyHero;
                            nearst_distance = distance;
                        }
                    }
                }
                if (targetFind) {
                    myHero->attack(target);
                    attacked = 1;
                }
            }
        }
        //�з���������
        for (auto enemyHero : allEnemyHeroes)
        {
            if (!enemyHero->isDead()&&enemyHero->isInBoard()) {
                Hero* target;
                bool targetFind = 0;
                long long nearst_distance = 99999999999;
                for (auto myHero : allMyHeroes) {
                    if (!myHero->isDead()&&myHero->isInBoard()) {
                        targetFind = 1;
                        long long x_distance = myHero->getPosition().x - enemyHero->getPosition().x;
                        long long y_distance = myHero->getPosition().y - enemyHero->getPosition().y;
                        long long distance = pow(x_distance, 2) + pow(y_distance, 2);
                        if (distance < nearst_distance) {
                            target = myHero;
                            nearst_distance = distance;
                        }
                    }
                }

                if (targetFind) {
                    enemyHero->attack(target);
                    attacked = 1;
                }

            }
        }
        if (!attacked) {
            unschedule(CC_SCHEDULE_SELECTOR(playerScene::attack));
            fight = 0;

            //ССӢ�۵Ĺ��������Զ���Ѫ
            int countUnDead = 0;
            for (Hero* hero : allEnemyHeroes) {
                if (hero->isInBoard() && !hero->isDead()) {
                    countUnDead++;
                }
            }
            if (countUnDead == 0) {
                if (enemyType == 1)
                {
                    myrole->attack(enemyrole1);
                }
                else {
                    myrole->attack(enemyrole2);
                }
            }
            countUnDead = 0;
            for (Hero* hero : allMyHeroes) {
                if (hero->isInBoard() && !hero->isDead()) {
                    countUnDead++;
                }
            }
            if (countUnDead == 0) {
                if (enemyType == 1)
                {
                    enemyrole1->attack(myrole);
                }
                else {
                    enemyrole2->attack(myrole);
                }
            }
            schedule(CC_SCHEDULE_SELECTOR(playerScene::startGame), 1.0f);
        }
    }

    //ͨ�����fightȫ�ֱ�����ʵ����Ϣ������ս�����ڵĽ���
    void startGame(float dt) {  
        if (fight == 0) {  //�˳�ս�����ֵ���Ϣ����
            unschedule(CC_SCHEDULE_SELECTOR(playerScene::startGame));  //ֹͣ���ѭ���ص�
            static int count = 0;  //����������ʼ�����̵���ظ�ˢ��
            if (count)
            {
                store->gold += 2;  //����ˢ������ѵ�
                store->refresh();   //�µĻ��ڻ�ˢ���̵�

                if (store->level < 5)
                {
                    store->gold += 4;  //�������
                    store->upgrade();
                }

                //ÿһ�ֵĶ��⽱��
                store->gold += 2;

                /*��ȡ�̵귵������Ϣ*/
                // ÿ��10����ҿ��Է���һ����ҵ���Ϣ
                store->gold += store->gold / 10;
                store->updateUI();
            }
            count++;

            seeEnemy1->setEnabled(true);
            seeEnemy2->setEnabled(true);

            //�����ҷ�Ӣ��
            ShowHeroes(0);

            //��ȥ�з�Ӣ��
            CoverHeroes(enemyType);


            // �ı���һ����Ҫ��ս���˻�
            if (enemyType == 1) {
                enemyType = 2;
            }
            else {
                enemyType = 1;
            }


            //��������ʼ
            progress0->setVisible(true);
            progress1->setVisible(true);
            // ���ö�ʱ������Ƶ�ʣ����������ÿ֡���£�
            this->schedule([this](float dt) {
                currentTime += dt;
            // ���㵱ǰ���ȣ��ٷֱȣ�
            float progressPercentage = (currentTime / totalTime) * 100.0f;
            // ���½�����
            progress1->setPercentage(progressPercentage);
            // �ж��Ƿ�ﵽ��ʱʱ�䣬����ﵽ��ȡ����ʱ���������������


            if (currentTime >= totalTime) {  //��Ϣʱ�����
                this->unschedule("progress");
                currentTime = 0.0f;   //����
                fight = 1;
                progress0->setVisible(false);
                progress1->setVisible(false);
                schedule(CC_SCHEDULE_SELECTOR(playerScene::startGame), 1.0f);
            }
                }, "progress");
        }
        else {

            unschedule(CC_SCHEDULE_SELECTOR(playerScene::startGame));  //ֹͣ���ص�

            //��ʾ�з�Ӣ��
            ShowHeroes(enemyType);
            ShowHeroes(0);
            store->setVisible(true);

            // ���ù۲찴���Ľ���
            goback->setVisible(false);
            seeEnemy1->setEnabled(false);
            seeEnemy2->setEnabled(false);

            //��ʼ�Դ�
            startattack();
        }
    }

    void seeEnemy1_callback(Ref* pSender);  //�۲��˻�1������
    void seeEnemy2_callback(Ref* pSender);  //�۲��˻�2������
    void goback_callback(Ref* pSender);     //����
    void enemyMove(float dt);    //�˻����ƶ�

    //��⹥���
    void detect_attack_in(float dt) {
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
        if (tag1 && tag2 && tag4) {  //�ɹ����ɹ����
            this->unschedule(CC_SCHEDULE_SELECTOR(playerScene::detect_attack_in));
            //��ʾͼ�ꡢ������Ч
            attacktram->setVisible(true);
            int audioId = AudioEngine::play2d("trammel.mp3");
            AudioEngine::setVolume(audioId, 1.0f);
            this->schedule(CC_SCHEDULE_SELECTOR(playerScene::detect_attack_out), 0.01f);
        }
    }
    void detect_attack_out(float dt) {
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
        if ((!tag1) || (!tag2) || (!tag4)) {  //�ɹ��˳������
            this->unschedule(CC_SCHEDULE_SELECTOR(playerScene::detect_attack_out));
            //��ȥͼ��
            attacktram->setVisible(false);
            this->schedule(CC_SCHEDULE_SELECTOR(playerScene::detect_attack_in), 0.01f);
        }
    }

    //���ָ��
    void detect_heal_in(float dt) {
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
        if (tag3 && tag5 && tag6) {  //�ɹ����ɻָ��
            this->unschedule(CC_SCHEDULE_SELECTOR(playerScene::detect_heal_in));
            //��ʾͼ�ꡢ������Ч
            healtram->setVisible(true);
            int audioId = AudioEngine::play2d("trammel.mp3");
            AudioEngine::setVolume(audioId, 1.0f);
            this->schedule(CC_SCHEDULE_SELECTOR(playerScene::detect_heal_out), 0.01f);
        }
    }
    void detect_heal_out(float dt) {
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
        if ((!tag3) || (!tag5) || (!tag6)) {  //�ɹ��˳��ָ��
            this->unschedule(CC_SCHEDULE_SELECTOR(playerScene::detect_heal_out));
            //��ȥͼ��
            healtram->setVisible(false);
            this->schedule(CC_SCHEDULE_SELECTOR(playerScene::detect_heal_in), 0.01f);
        }
    }

    // ���ð�����Ӧ
    void settingCallBack(Ref* sender);

    // ������������ԭλ����
    void putback_equip_for_blood() {
        equip_for_blood->setScale(0.2);
        equip_for_blood->setPosition(200, 750);
        equip_for_blood->setVisible(true);
    }
    void putback_equip_for_power(){
        equip_for_power->setScale(0.2);
        equip_for_power->setPosition(200, 650);
        equip_for_power->setVisible(true);
    }
    void putback_equip_for_magic() {  
        equip_for_magic->setScale(0.2);
        equip_for_magic->setPosition(200, 550);
        equip_for_magic->setVisible(true);
    }
};

