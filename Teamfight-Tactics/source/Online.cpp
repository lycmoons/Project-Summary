#include "cocos2d.h"
#include "Online.h"
#include <iostream>
#include "cocos/network/WebSocket.h"
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include"endScene.h"

using namespace cocos2d;
using namespace cocos2d::network;

USING_NS_CC;

cocos2d::network::WebSocket* webSocket = nullptr;
rapidjson::Document document;
rapidjson::Document documentGamein;

int BuildRoom = 0;
int JoinRoom = 0;
int GameinProgress = 0;
int Isconnected = 0;
int Gamein = 0;

// ���ӵ� WebSocket ������
void ClientSocket::connectToServer() {
	webSocket = new cocos2d::network::WebSocket();

	// ʹ�� init_by_url ������ʼ�� WebSocket

	if (webSocket->init(*this, "ws://100.81.180.38:9002"))
	{
		Isconnected = 1;
		CCLOG("Connected to server");
		// �����Ѿ�����
	}

}

//��������
void ClientSocket::onSend(rapidjson::Document& document) {
	// �� RapidJSON �ĵ�תΪ�ַ���
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	document.Accept(writer);
	std::string message = buffer.GetString();
	webSocket->send(message);
}

void ClientSocket::onSendGamein() {
	// �� RapidJSON �ĵ�תΪ�ַ���
	rapidjson::StringBuffer buffer1;
	rapidjson::Writer<rapidjson::StringBuffer> writer1(buffer1);
	document.Accept(writer1);
	std::string message1 = buffer1.GetString();
	webSocket->send(message1);
}

// WebSocket ���ӳɹ��ص�
void ClientSocket::onOpen(cocos2d::network::WebSocket* ws) {
	CCLOG("WebSocket ���ӳɹ�");
}

// WebSocket �յ���Ϣ�ص�
void ClientSocket::onMessage(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::Data& data) {

	// ������յ����ı���Ϣ
	std::string message = std::string(data.bytes);
	if (message == "start") {
		GameinProgress = 2;
	}
	else if (message == "win") {
		auto endscene = endScene::createScene(1);
		Director::getInstance()->pushScene(endscene);
		AudioEngine::stopAll();
		int audioId = AudioEngine::play2d("victory.mp3");
		AudioEngine::setVolume(audioId, 1.0f);
	}
	else if (Gamein == 0) {
		ReceiveJSONstring(message);
	}
	else {
		ReceiveJSONstringGamein(message);
	}
}

// WebSocket ���ӹرջص�
void ClientSocket::onClose(cocos2d::network::WebSocket* ws) {
	//CCLOG("WebSocket ���ӹر�");
	closeWebSocket();
}

// WebSocket ����ص�
void ClientSocket::onError(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::ErrorCode& error) {
	//CCLOG("WebSocket ������: %d", static_cast<int>(error));
	Isconnected = 0;
	closeWebSocket();
}

void ClientSocket::closeWebSocket() {
	if (webSocket) {
		webSocket->close();
		webSocket = nullptr;
	}
}


ClientSocket::~ClientSocket() {
	if (webSocket) {
		CCLOG("Connected to server");
		webSocket->close();
		webSocket = nullptr;
	}
}