#include <iostream>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <websocketpp/client.hpp>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <set>
#include <vector>
using namespace std;

typedef websocketpp::server<websocketpp::config::asio> server_;

// ����һ���Զ�����Ϣ������
class MyHandler : public websocketpp::server<websocketpp::config::asio>::message_handler {
public:
    // ���յ���Ϣʱ���ô˻ص�����
    void on_message(websocketpp::connection_hdl hdl, server_::message_ptr msg) {
        // ����JSON��Ϣ
        rapidjson::Document document;
        document.Parse(msg->get_payload().c_str());
        // �������Ƿ�ɹ�
        if (!document.IsObject()) {
            std::cerr << "�޷�����JSON��Ϣ" << std::endl;
            return;
        }

        if (document.HasMember("GameinProgress") && document["GameinProgress"].IsInt()
            && document.HasMember("Gamein") && document["Gamein"].IsInt()) {
            int Type1 = document["GameinProgress"].GetInt();
            int Type2 = document["Gamein"].GetInt();
            if (Type2 == 0) {
                // ��������
                if (document.HasMember("BuildRoom") && document["BuildRoom"].IsInt()) {
                    for (auto it : connections_) {
                        try {
                            server.send(it, msg->get_payload(), websocketpp::frame::opcode::text);
                        }
                        catch (const exception& e) {
                            cout << "Error sending message: " << e.what() << endl;
                        }
                    }
                }
                // ���뷿��
                if (document.HasMember("JoinRoom") && document["JoinRoom"].IsInt()) {
                    //int Type = document["JoinRoom"].GetInt();
                    for (auto it : connections_) {
                        try {
                            server.send(it, msg->get_payload(), websocketpp::frame::opcode::text);
                        }
                        catch (const exception& e) {
                            cout << "Error sending message: " << e.what() << endl;
                        }
                    }
                }
                //��ʼ��ս
                if (document.HasMember("GameinProgress") && document["GameinProgress"].IsInt()) {
                    int Type = document["GameinProgress"].GetInt();
                    if (Type == 1) {
                        for (auto it : connections_) {
                            try {
                                server.send(it, "start", websocketpp::frame::opcode::text);
                            }
                            catch (const exception& e) {
                                cout << "Error sending message: " << e.what() << endl;
                            }
                        }
                    }
                }
            }
            else {//��ս��Ϣ
                for (auto it = connections_.begin(); it != connections_.end(); ++it) {
                    try {
                        if (it->lock() != hdl.lock()) {
                            server.send(it->lock(), msg->get_payload(), websocketpp::frame::opcode::text);
                        }
                    }
                    catch (const exception& e) {
                        cout << "Error sending message: " << e.what() << endl;
                    }
                }
                if (connections_.size() == 1) {
                    try {
                        server.send(hdl, "win", websocketpp::frame::opcode::text);
                    }
                    catch (const exception& e) {
                        cout << "Error sending message: " << e.what() << endl;
                    }
                }
            }
        }
    }

    // �����µ����ӽ���ʱ����
    void on_open(websocketpp::connection_hdl hdl) {
        cout << "Client connected!" << endl;

        // �������Ӽ������Ӽ���
        connections_.insert(hdl);

        //// ����������ﵽ��������ʼ��Ϣ����
        if (connections_.size() == 2) {
            std::cout << "Two clients connected. Start exchanging messages." << std::endl;
        }
    }
    // �����ӹر�ʱ����
    void on_close(websocketpp::connection_hdl hdl) {
        cout << "Client disconnected!" << endl;

        // �����Ӽ������Ƴ��رյ�����
        connections_.erase(hdl);
    }
    /*void broadcast(const string& message) {
        for (auto it = connections_.begin(); it != connections_.end(); ++it) {
            try {
                server.send(*it, message, websocketpp::frame::opcode::text);
            }
            catch (const exception& e) {
                cout << "Error sending message: " << e.what() << endl;
            }
        }
    }*/

    // WebSocket++������ʵ��
    websocketpp::server<websocketpp::config::asio> server;
    set<websocketpp::connection_hdl, owner_less<websocketpp::connection_hdl>> connections_;

};


int main() {
    try {
        MyHandler handler;
        boost::asio::io_service io_service;
        boost::asio::ip::tcp::resolver resolver(io_service);
        boost::asio::ip::tcp::resolver::query query(boost::asio::ip::host_name(), "");

        boost::asio::ip::tcp::resolver::iterator endpoints = resolver.resolve(query);
        boost::asio::ip::tcp::resolver::iterator end;

        while (endpoints != end) {
            boost::asio::ip::tcp::endpoint endpoint = *endpoints++;
            std::cout << "IP Address: " << endpoint.address().to_string() << std::endl;
        }

        // ����WebSocket++������
        handler.server.init_asio();


        handler.server.set_access_channels(websocketpp::log::alevel::all);
        handler.server.clear_access_channels(websocketpp::log::alevel::frame_payload);

        handler.server.set_message_handler(websocketpp::lib::bind(
            &MyHandler::on_message, &handler, websocketpp::lib::placeholders::_1, websocketpp::lib::placeholders::_2
        ));
        handler.server.set_open_handler(bind(&MyHandler::on_open, &handler, std::placeholders::_1));
        handler.server.set_close_handler(bind(&MyHandler::on_close, &handler, std::placeholders::_1));


        // �����ڶ˿�9002
        boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v6(), 9002);
        handler.server.listen(endpoint);

        // ��������������ѭ��
        handler.server.start_accept();

        // ����ASIO io_service����ѭ��
        handler.server.run();
    }
    catch (websocketpp::exception const& e) {
        std::cerr << "����" << e.what() << std::endl;
    }
    catch (...) {
        std::cerr << "δ֪�쳣" << std::endl;
    }

    return 0;
}







