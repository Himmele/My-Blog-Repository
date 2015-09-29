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

#include "android/net/SocketAddress.h"
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>

namespace android {
namespace net {

SocketAddress::SocketAddress() :
	mValid(false) {
	memset(&mSocketAddress, 0, sizeof(mSocketAddress));
}

SocketAddress::SocketAddress(uint16_t port) :
	mValid(true) {
	memset(&mSocketAddress, 0, sizeof(mSocketAddress));
	mSocketAddress.sin_family = AF_INET;
	mSocketAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	mSocketAddress.sin_port = htons(port);
}

SocketAddress::SocketAddress(const char* host, uint16_t port) :
	mValid(false) {
	struct addrinfo hints, *result;
	int status;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	status = getaddrinfo(host, NULL, &hints, &result);
	mValid = (status == 0);

	if (mValid) {
		memset(&mSocketAddress, 0, sizeof(mSocketAddress));
		mSocketAddress.sin_family = AF_INET;
		mSocketAddress.sin_addr.s_addr = ((struct sockaddr_in *)(result->ai_addr))->sin_addr.s_addr;
		mSocketAddress.sin_port = htons(port);
	}

	freeaddrinfo(result);
}

const char* SocketAddress::getHostName() const {
	static char host[NI_MAXHOST];
	getnameinfo((sockaddr*)&mSocketAddress, sizeof(mSocketAddress), host, sizeof(host), NULL, 0, 0);
	return host;
}

} /* namespace net */
} /* namespace android */
