#pragma once

#define ZMQ_STATIC

#include "zmq.hpp"
#include <zmq_addon.hpp>

#include <string>
#include <iostream>
#include <chrono>
#include <thread>

namespace Protocol {

	using namespace zmq;

	enum TYPE {
		SERVER,
		CLIENT
	};

	template <class T>
	class PSEUDO_IPC {
	public:

		~PSEUDO_IPC() {
			sock.close();
		}

		bool Init(const char *adress, TYPE type) {
			if (!sock.connected()) {
				if (type == TYPE::SERVER) {
					ctx = zmq::context_t(4);
					sock = zmq::socket_t(ctx, zmq::socket_type::pub);
					sock.bind(adress);
				}
				else {
					ctx = zmq::context_t();
					sock = zmq::socket_t(ctx, zmq::socket_type::sub);
					sock.connect(adress);
					sock.setsockopt(ZMQ_SUBSCRIBE, "", 0);
				}
			}

			return sock.connected();
		}

		bool send(T *data) {
			if (sock.connected()) {
				send_packet = zmq::message_t((char*)(data), sizeof(T));
				sock.send(send_packet, zmq::send_flags::dontwait);
				return true;
			}
			else {
				return false;
			}
		}

		bool send(string str) {
			if (sock.connected()) {
				send_packet = zmq::message_t(str);
				sock.send(send_packet, zmq::send_flags::dontwait);
				return true;
			}
			else {
				return false;
			}
		}

		bool recive_packet(T  *data) {
			if (sock.connected() && sock.recv(recv_packet, zmq::recv_flags::dontwait)) {
				*data = static_cast<T*>(recv_packet.data());
				return true;
			}
			else {
				return false;
			}
		}

		bool recive_message(string  &msg = string()) {
			if (sock.connected() && sock.recv(recv_packet, zmq::recv_flags::dontwait)) {
				msg = std::string(static_cast<char*>(recv_packet.data()), recv_packet.size());
				return true;
			}
			else {
				return false;
			}
		}

		void close() {
			if (sock.connected()) {
				sock.close();
			}
		}

	private:
		context_t ctx;
		socket_t sock;
		message_t recv_packet;
		message_t send_packet;
	};
}