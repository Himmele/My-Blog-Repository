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

#ifndef ANDROID_OS_THREADPOOLEXECUTOR_H_
#define ANDROID_OS_THREADPOOLEXECUTOR_H_

#include "android/os/Executor.h"
#include "android/os/Thread.h"
#include "android/os/LinkedBlockingQueue.h"

namespace android {
namespace os {

class ThreadPoolExecutor :
	public Executor
{
public:
	ThreadPoolExecutor(uint32_t threadPoolSize);
	virtual ~ThreadPoolExecutor();

	virtual void execute(const sp<Runnable>& runnable);
	virtual bool cancel(const sp<Runnable>& runnable);

private:
	class WorkerThread : public Thread
	{
	public:
		WorkerThread() : mQueue(NULL) {}
		virtual ~WorkerThread() {}
		virtual void run();

	private:
		void setQueue(LinkedBlockingQueue< sp<Runnable> >& queue);

		LinkedBlockingQueue< sp<Runnable> >* mQueue;

		friend class ThreadPoolExecutor;
	};

	void start();
	void shutdown();

	const uint32_t THREAD_POOL_SIZE;
	sp<WorkerThread>* mWorkerThreads;
	LinkedBlockingQueue< sp<Runnable> > mQueue;

	NO_COPY_CTOR_AND_ASSIGNMENT_OPERATOR(ThreadPoolExecutor)
};

} /* namespace os */
} /* namespace android */

#endif /* ANDROID_OS_THREADPOOLEXECUTOR_H_ */
