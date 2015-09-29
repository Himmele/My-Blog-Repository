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

#ifndef ANDROID_NET_SOCKETADDRESS_H_
#define ANDROID_NET_SOCKETADDRESS_H_

#include "android/os/Ref.h"
#include <netinet/in.h>

namespace android {
namespace net {

class SocketAddress :
	public android::os::Ref
{
public:
	SocketAddress();
	SocketAddress(uint16_t port);
	SocketAddress(const char* host, uint16_t port);
	virtual ~SocketAddress() {}
	const char* getHostName() const;
	uint16_t getPort() const { return ntohs(mSocketAddress.sin_port); }
	bool isValid() const { return mValid; }

private:
	struct sockaddr_in mSocketAddress;
	bool mValid;

	friend class ServerSocket;
	friend class Socket;
	friend class DatagramSocket;

	NO_COPY_CTOR_AND_ASSIGNMENT_OPERATOR(SocketAddress)
};

} /* namespace net */
} /* namespace android */

#endif /*ANDROID_NET_SOCKETADDRESS_H_*/
