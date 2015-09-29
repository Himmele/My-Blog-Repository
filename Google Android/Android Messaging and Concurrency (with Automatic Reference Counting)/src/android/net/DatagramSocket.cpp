/*
 * Copyright (C) 2012 Daniel Himmelein
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "android/net/DatagramSocket.h"
#include "android/net/SocketAddress.h"
#include <unistd.h>
#include <sys/socket.h>

using namespace android::os;

namespace android {
namespace net {

DatagramSocket::DatagramSocket() :
	mSocketId(-1),
	mIsBound(false),
	mIsClosed(false) {
	mSocketId = socket(AF_INET, SOCK_DGRAM, 0);
}

DatagramSocket::DatagramSocket(uint16_t port) :
	mSocketId(-1),
	mIsBound(false),
	mIsClosed(false) {
	mSocketId = socket(AF_INET, SOCK_DGRAM, 0);
	bind(port);
}

DatagramSocket::DatagramSocket(const char* host, uint16_t port) :
	mSocketId(-1),
	mIsBound(false),
	mIsClosed(false) {
	mSocketId = socket(AF_INET, SOCK_DGRAM, 0);
	bind(host, port);
}

DatagramSocket::~DatagramSocket() {
	close();
}

bool DatagramSocket::bind(uint16_t port) {
	sp<SocketAddress> socketAddress = new SocketAddress(port);
	mIsBound = ::bind(mSocketId, (struct sockaddr*)&socketAddress->mSocketAddress, sizeof(socketAddress->mSocketAddress)) == 0;
	return mIsBound;
}

bool DatagramSocket::bind(const char* host, uint16_t port) {
	sp<SocketAddress> socketAddress = new SocketAddress(host, port);
	mIsBound = ::bind(mSocketId, (struct sockaddr*)&socketAddress->mSocketAddress, sizeof(socketAddress->mSocketAddress)) == 0;
	return mIsBound;
}

ssize_t DatagramSocket::recv(uint8_t* data, size_t size) {
	return ::recvfrom(mSocketId, reinterpret_cast<char*>(data), size, 0, NULL, 0);
}

ssize_t DatagramSocket::recv(uint8_t* data, size_t size, android::os::sp<SocketAddress>& sender) {
	socklen_t socketSize = sizeof(sender->mSocketAddress);
	ssize_t result = ::recvfrom(mSocketId, reinterpret_cast<char*>(data), size, 0, (struct sockaddr*)&sender->mSocketAddress, &socketSize);
	sender->mValid = true;
	return result;
}

bool DatagramSocket::send(const void* data, size_t size, const android::os::sp<SocketAddress>& receiver) {
	socklen_t socketSize = sizeof(receiver->mSocketAddress);
	return (size_t)::sendto(mSocketId, reinterpret_cast<const char*>(data), size, 0, (struct sockaddr*)&receiver->mSocketAddress, socketSize) == size;
}

void DatagramSocket::close() {
	mIsClosed = true;
	mIsBound = false;
	if (mSocketId >= 0) {
		::shutdown(mSocketId, SHUT_RDWR);
		::close(mSocketId);
		mSocketId = -1;
	}
}

} /* namespace net */
} /* namespace android */
