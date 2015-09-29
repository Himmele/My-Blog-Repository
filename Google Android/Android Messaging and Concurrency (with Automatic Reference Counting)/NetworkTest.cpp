#include <stdio.h>
#include "android/os/Thread.h"
#include "android/net/ServerSocket.h"
#include "android/net/Socket.h"
#include "android/net/DatagramSocket.h"
#include "android/net/SocketAddress.h"
#include "android/lang/String.h"

using namespace android::os;
using namespace android::net;
using namespace android::lang;

class ServerSocketThread : public Thread
{
public:
	virtual void run() {
		mServerSocket = new ServerSocket();
		mServerSocket->setReuseAddress(true);
		if (!mServerSocket->bind(1234)) {
			printf("TCP-Server: Cannot bind to port 1234\n");
			return;
		}
		while (!isInterrupted() && !mServerSocket->isClosed()) {
			printf("TCP-Server: Waiting for clients...\n");
			sp<Socket> socket = mServerSocket->accept();
			if (socket != NULL) {
				const uint32_t size = 16;
				char data[size];
				socket->readFully((uint8_t*)data, size - 1);
				data[size - 1] = '\0';
				printf("TCP-Server: Received message %s from client\n", data);
				socket->write(data, size);
				socket->close();
			}
		}
		mServerSocket->close();
	}

	void shutdown() {
		mServerSocket->close();
	}

private:
	sp<ServerSocket> mServerSocket;
};

class DatagramServerThread : public Thread
{
public:
	virtual void run() {
		mDatagramSocket = new DatagramSocket();
		if (!mDatagramSocket->bind(1234)) {
			printf("UDP-Server: Cannot bind to port 1234\n");
			return;
		}
		sp<SocketAddress> senderAddress = new SocketAddress();
		while (!isInterrupted() && !mDatagramSocket->isClosed()) {
			const uint32_t size = 16;
			char data[size];
			mDatagramSocket->recv((uint8_t*)data, size - 1, senderAddress);
			data[size - 1] = '\0';
			printf("UDP-Server: Received message %s from client %s:%d\n", data, senderAddress->getHostName(), senderAddress->getPort());
			mDatagramSocket->send(data, size - 1, senderAddress);
			mDatagramSocket->close();
		}
		mDatagramSocket->close();
	}

	void shutdown() {
		mDatagramSocket->close();
	}

private:
	sp<DatagramSocket> mDatagramSocket;
};

int main() {
	// TCP client/server test
	sp<ServerSocketThread> serverSocketThread = new ServerSocketThread();
	serverSocketThread->start();
	Thread::sleep(1000);

	sp<Socket> socket = new Socket();
	printf("TCP-Client: Connecting to port 1234\n");
	if (socket->connect("localhost", 1234)) {
		String string("123456789ABCDEF");
		socket->write(string.c_str(), string.size());
		int32_t size = 16;
		char data[size];
		data[size - 1] = '\0';
		size = socket->readFully((uint8_t*)data, size - 1);
		printf("TCP-Client: Received message %s from server\n", data);
	} else {
		printf("Cannot connect to port 1234\n");
	}
	socket->close();
	serverSocketThread->interrupt();
	serverSocketThread->shutdown();
	serverSocketThread->join();

	printf("\n\n");

	// UDP client/server test
	sp<DatagramServerThread> datagramServerThread = new DatagramServerThread();
	datagramServerThread->start();
	Thread::sleep(1000);

	sp<DatagramSocket> udpClientSocket = new DatagramSocket();
	String string("123456789ABCDEF");
	sp<SocketAddress> serverAddress = new SocketAddress("127.0.0.1", 1234);
	udpClientSocket->send(string.c_str(), string.size(), serverAddress);
	int32_t size = 16;
	char data[size];
	sp<SocketAddress> senderAddress = new SocketAddress();
	udpClientSocket->recv((uint8_t*)data, size - 1, senderAddress);
	data[size - 1] = '\0';
	printf("UDP-Client: Received message %s from server %s:%d\n", data, senderAddress->getHostName(), senderAddress->getPort());
	udpClientSocket->close();
	datagramServerThread->interrupt();
	datagramServerThread->shutdown();
	datagramServerThread->join();

	return 0;
}
