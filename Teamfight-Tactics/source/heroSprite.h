#pragma once
#include"cocos2d.h"
USING_NS_CC;
#define ALL_TYPE 6


class Hero :public Sprite {
private:
    float blood;//Ѫ��
    float max_blood;
    int each_magic;//ÿ����������
    int start_magic;//��ʼmagic
    int magic;//����
    int level;//�Ǽ�(1-3)
    int camp;//1��ʾ�ҷ���Ӫ��0��ʾ�з���Ӫ
    int heroType;//Ӣ������(1-ALL_TYPE)
    int weapon;//������ʽ(1ΪԶ�̣�0Ϊ��ս��
    int power;//�չ��˺�
    int skill;//�����˺�

    

    bool inBoard = 0;//����Ƿ�λ�������ϣ�1��ʾ�������ϣ�0��ʾ�ڱ�սϯ��
    int seatIndex = -1;   //seat���±�
    int Die = 0;   //1��ʾ����
    int click = 0;   //���ڵ���ж�
    ProgressTimer* healthBar;//Ѫ����
    ProgressTimer* magicBar;//������
public:

    // ���Ƿ���װ��
    int equip_blood = 0;
    int equip_magic = 0;
    int equip_power = 0;

    Hero();
    // ������ͬ���͵�Ӣ��
    static Hero* createHero(int heroType, const Vec2& position, int camp);
    // �ƶ�����
    void move(Vec2 destination);
    // �ϳɷ���
    static void mergeHeroes();

    // Getter �� Setter ����
    int getLevel() const;
    void upgrade(int newLevel);
    int getType()const;


    // ��������
    void enterBoard();

    // �˳�����
    void exitBoard();

    // �Ƿ�������
    bool isInBoard() const;

    //��ȡ��ս����Զ��
    int getWeapon() const;


    // ��������������ָ��Ŀ��
    void attack(Hero* target);

    // ����Ѫ������
    void decreaseHealth(int amount);

    // ��������
    void die();

    // ��ȡ��Ӫ
    int getCamp() const;

    // ��ȡѪ��
    float getBlood();

    //����λ���±�
    void setIndex(int index);


    // ��ȡӢ������
    int GetheroType() {
        return heroType;
    }

    int getLevel() {
        return level;
    }

    //��ȡλ��
    int GetIndex() {
        return seatIndex;
    }

    //�ж��Ƿ񱻵��
    int IsClicked() {
        return click;
    }

    //���õ��
    void SetClicked(int c) {
        click = c;
    }

    //����Borad����ֵ
    void SetBoard(int b) {
        inBoard = b;
    }

    int isDead() {
        return Die;
    }

    void setDie(int d) {
        Die = d;
    }

    float getMaxBlood() {
        return max_blood;
    }

    void setBlood(float b) {
        blood = b;
    }

    void setMagic(int m) {
        magic = m;
    }

    void updatebloodandmagic();

    int getStartMagic() {
        return start_magic;
    }

    int getMagic() {
        return magic;
    }

    int getEachMagic() {
        return each_magic;
    }

    int getPower() {
        return power;
    }

    int getSkill() {
        return skill;
    }

    void setMaxBlood(int m) {
        max_blood = m;
    }

    void setPower(int m) {
        power = m;
    }

    void setEachMagic(int m) {
        each_magic = m;
    }
};