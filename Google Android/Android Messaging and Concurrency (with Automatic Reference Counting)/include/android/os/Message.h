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
#include "android/os/Ref.h"
#include "android/os/Bundle.h"

namespace android {
namespace os {

class Runnable;
class Handler;
class Message;

struct MessagePool
{
	MessagePool();
	~MessagePool();

	uint32_t MAX_SIZE;
	sp<Message> mHeadMessage;
	uint32_t mSize;
	Lock mLock;
};


class Message :
	public Ref,
	public Ref::Destroyer {
public:
	virtual ~Message();
    static sp<Message> obtain();
    static sp<Message> obtain(const sp<Handler>& handler);
    static sp<Message> obtain(const sp<Handler>& handler, int32_t what);
    static sp<Message> obtain(const sp<Handler>& handler, int32_t what, int32_t arg1, int32_t arg2);
    static sp<Message> obtain(const sp<Handler>& handler, const sp<Runnable>& callback);

    uint64_t getExecTimestamp() const;
    void setHandler(const sp<Handler>& handler);
    sp<Handler> getHandler() const;
    sp<Runnable> getCallback() const;
    void setData(sp<Bundle> data);
    sp<Bundle> getData() const;
    bool sendToTarget();
    sp<Message> dup() const;

    virtual void destroy(Ref* ref);

    int32_t what;
    int32_t arg1;
    int32_t arg2;
    void* obj;

protected:
    Message();
    static sp<Message> obtain(const Message* message);
    void clear();

private:
    uint64_t mExecTimestamp; // nanoseconds
    sp<Handler> mHandler;
    sp<Runnable> mCallback;
    sp<Bundle> mData;
    sp<Message> mNextMessage;
	static MessagePool sMessagePool;

    friend class MessageQueue;
    friend class Looper;
    friend class Handler;
    friend class MessagePool;
};

} /* namespace os */
} /* namespace android */

#endif /* ANDROID_OS_MESSAGE_H_ */
