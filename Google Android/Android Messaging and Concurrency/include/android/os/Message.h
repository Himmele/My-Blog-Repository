/*
 * Copyright (C) 2011 Daniel Himmelein
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

#ifndef ANDROID_OS_MESSAGE_H_
#define ANDROID_OS_MESSAGE_H_

#include <stdint.h>
#include "android/os/Lock.h"

namespace android {
namespace os {

class Runnable;
class Handler;

class Message {
public:
	virtual ~Message() {}
    static Message& obtain();
    static Message& obtain(Handler& handler);
    static Message& obtain(Handler& handler, int32_t what);
    static Message& obtain(Handler& handler, int32_t what, int32_t arg1, int32_t arg2);
    static Message& obtain(Handler& handler, Runnable& callback);

    /* you must not touch a Message object after calling recycle */
    virtual void recycle();
    void clear();

    uint64_t getExecTimestamp() const;
    void setHandler(Handler& handler);
    Handler* getHandler();
    Runnable* getCallback();
    bool sendToTarget();

    int32_t what;
    int32_t arg1;
    int32_t arg2;
    void* obj;

protected:
    Message();

private:
	static const uint32_t MAX_MESSAGE_POOL_SIZE = 10;

    uint64_t mExecTimestamp; // nanoseconds
    Handler* mHandler;
    Runnable* mCallback;
    Message* mNextMessage;
	static Message* sMessagePool;
	static uint32_t sMessagePoolSize;
	static Lock sMessagePoolLock;

    friend class MessageQueue;
    friend class Looper;
    friend class Handler;
};

} /* namespace os */
} /* namespace android */

#endif /* ANDROID_OS_MESSAGE_H_ */
