#include "cocos2d.h"
#include "Online.h"

USING_NS_CC;

//��ʼ���泡��
class startScene : public Scene,ClientSocket
{
public:
    static Scene* createScene();
    virtual bool init();
    virtual void onEnter() override;
    CREATE_FUNC(startScene);

    void musicControl(Ref* sender);

};
