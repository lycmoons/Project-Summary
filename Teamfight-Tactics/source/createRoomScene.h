#pragma once
#include "cocos2d.h"
#include "Online.h"
#include <iostream>
#include "cocos/network/WebSocket.h"

USING_NS_CC;

/*
typedef websocketpp::server<websocketpp::config::asio> server;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

// pull out the type of messages sent by our config
typedef server::message_ptr message_ptr;

*/


class createRoomScene : public Scene
{
public:
	//��ʼ������
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(createRoomScene);

	//���ذ�������Ӧ
	//void callbackofreturn(Ref* pSender);

};
