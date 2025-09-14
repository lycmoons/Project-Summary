#pragma once
#include"cocos2d.h"
#include <iostream>
#include "cocos/network/WebSocket.h"
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include "json.h"
#include "cocos/audio/include/AudioEngine.h"

using namespace cocos2d;
using namespace cocos2d::network;

USING_NS_CC;


class ClientSocket :public cocos2d::network::WebSocket::Delegate
{
public:
	// ���ӵ� WebSocket ������
	void connectToServer();

	//���ͷ�����Ϣ
	static void onSend(rapidjson::Document& document);

	//���Ͷ�ս��Ϣ
	static void onSendGamein();

	// WebSocket ���ӳɹ��ص�
	void onOpen(cocos2d::network::WebSocket* ws);

	// WebSocket �յ���Ϣ�ص�
	void onMessage(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::Data& data);

	// WebSocket ���ӹرջص�
	void onClose(cocos2d::network::WebSocket* ws);

	// WebSocket ����ص�
	void onError(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::ErrorCode& error);

	static void closeWebSocket();

	// ���ʵ���ʱ������ WebSocket ��Դ
	~ClientSocket();

};
















/*
#include "cocos2d.h"
#include <string.h>
#include "network/WebSocket.h"
#include "json/document.h"

USING_NS_CC;

using namespace std;
using namespace network;

class SocketService :public network::WebSocket::Delegate
{
public:

	static SocketService* socket;

	static SocketService* getInstance();

	static void delteInstance();
public:
	~SocketService();

	void initSocket();

	void sendMessage(string str);

	string getMessage();

	void setMessage(string s);

	void setPrivatePassword(string privateStr);

	string getPrivatePassword();

	void setPlayerType(string type);

	string  getPlayerType();

	void setAccount(string name);

	string  getAccount();


	virtual void onOpen(WebSocket* ws);

	virtual void onMessage(WebSocket* ws, const WebSocket::Data& data);

	virtual void onClose(WebSocket* ws);

	virtual void onError(WebSocket* ws, const WebSocket::ErrorCode& error);
private:
	WebSocket* client;

	string receiveMessage;

	bool ifReceviceMessage;

	string privatePassword;

	string playerType;

	string account;

};
*/
