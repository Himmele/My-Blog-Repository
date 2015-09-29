#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include "android/os/Looper.h"
#include "android/os/Handler.h"
#include "android/os/Thread.h"
#include "android/os/Message.h"
#include "android/os/Closure.h"
#include "android/os/AsyncTask.h"
#include "android/os/Semaphore.h"

using namespace android::os;

class Handler3;
class Handler4;
Looper* sLooper1 = NULL;
Looper* sLooper2 = NULL;
sp<Handler> sHandler1 = NULL;
sp<Handler> sHandler2 = NULL;
sp<Handler3> sHandler3 = NULL;
sp<Handler4> sHandler4 = NULL;
Semaphore sSemaphore1(0);
Semaphore sSemaphore2(0);

class Params :
	public Ref
{
public:
	static const uint32_t ARRAY_SIZE = 32;

	Params() {
		for (uint32_t i = 0; i < ARRAY_SIZE; i++) {
			values[i] = i;
		}
	}

	uint32_t values[ARRAY_SIZE];
};

class AsyncTask1 : public AsyncTask<sp<Params>, int32_t, int32_t>
{
public:
	virtual void onPreExecute() {
		printf("AsyncTask1::onPreExecute on thread %d\n", (int32_t) pthread_self());
	}

	virtual int32_t doInBackground(sp<Params> params) {
		printf("AsyncTask1::doInBackground on thread %d with params %d\n", (int32_t) pthread_self(), params->values[0]);
		Thread::sleep(250);
		uint32_t sum = 0;
		uint32_t i;
		for (i = 0; i < Params::ARRAY_SIZE / 2; i++) {
			sum += params->values[i];
		}
		publishProgress(sum);
		Thread::sleep(250);
		for (; i < Params::ARRAY_SIZE; i++) {
			sum += params->values[i];
		}
		return sum;
	}

	virtual void onProgressUpdate(int32_t value) {
		printf("AsyncTask1::onProgressUpdate on thread %d with value %d\n", (int32_t) pthread_self(), value);
	}

	virtual void onPostExecute(int32_t result) {
		printf("AsyncTask1::onPostExecute on thread %d with result %d\n", (int32_t) pthread_self(), result);
	}

	virtual void onCancelled() {
		printf("AsyncTask1::onCancelled on thread %d\n", (int32_t) pthread_self());
	}
};

class AsyncTask2 : public AsyncTask<int32_t, int32_t, int32_t>
{
public:
	virtual void onPreExecute() {
		printf("AsyncTask2::onPreExecute on thread %d\n", (int32_t) pthread_self());
	}

	virtual int32_t doInBackground(int32_t param) {
		printf("AsyncTask2::doInBackground on thread %d with params %d\n", (int32_t) pthread_self(), param);
		Thread::sleep(100);
		return 42;
	}

	virtual void onPostExecute(int32_t result) {
		printf("AsyncTask2::onPostExecute on thread %d with result %d\n", (int32_t) pthread_self(), result);
	}

	virtual void onCancelled() {
		printf("AsyncTask2::onCancelled on thread %d\n", (int32_t) pthread_self());
	}
};

class Handler1 : public Handler
{
public:
	static const uint32_t MSG_START_ASYNC_TASKS = 1;
	static const uint32_t MSG_PRINT_INFO = 2;

	virtual void handleMessage(const sp<Message>& message) {
		switch (message->what) {
		case MSG_START_ASYNC_TASKS: {
			mAsyncTask1 = new AsyncTask1();
			mAsyncTask1->execute(new Params());
			mAsyncTask2 = new AsyncTask2();
			mAsyncTask2->executeOnExecutor(AsyncTaskBase::THREAD_POOL_EXECUTOR, 1234567);
			mAsyncTask3 = new AsyncTask2();
			mAsyncTask3->executeOnExecutor(AsyncTaskBase::THREAD_POOL_EXECUTOR, 7654321);
			break;
		}
		case MSG_PRINT_INFO:
			printf("Handler1::handleMessage %p with ID %d by Looper %p\n",
					message.getPointer(), message->what, Looper::myLooper());
			break;
		default:
			printf("Handler1::handleMessage: Oops, this message ID is not valid\n");
			break;
		}
	}

private:
	sp<AsyncTask1> mAsyncTask1;
	sp<AsyncTask2> mAsyncTask2;
	sp<AsyncTask2> mAsyncTask3;
};

class Handler2 : public Handler
{
public:
	virtual void handleMessage(const sp<Message>& message) {
		printf("Handler2::handleMessage %p with ID %d by Looper %p\n",
				message.getPointer(), message->what, Looper::myLooper());
	}
};

class Handler3 : public Handler
{
public:
	virtual void handleMessage(const sp<Message>& message) {
		printf("Handler3::handleMessage %p with ID %d by Looper %p\n",
				message.getPointer(), message->what, Looper::myLooper());
	}

	void test() {
		sendMessage(obtainMessage(3));
	}
};

class Handler4 : public Handler
{
public:
	Handler4(Looper& looper) : Handler(looper) {}

	virtual void handleMessage(const sp<Message>& message) {
		printf("Handler4::handleMessage %p with ID %d by Looper %p\n",
				message.getPointer(), message->what, Looper::myLooper());
	}

	void test() {
		sendMessageDelayed(obtainMessage(4), 1000);
	}
};

class Thread1 : public Thread
{
public:
	virtual void run() {
		Looper::prepare();
		sLooper1 = Looper::myLooper();
		sHandler1 = new Handler1();
		sHandler3 = new Handler3();
		printf("Looper 1 uses thread %d\n", (int32_t) pthread_self());
		sSemaphore1.signal();
		Looper::loop();
	}
};

class Thread2 : public Thread
{
public:
	virtual void run() {
		Looper::prepare();
		sLooper2 = Looper::myLooper();
		sHandler2 = new Handler2();
		printf("Looper 2 uses thread %d\n", (int32_t) pthread_self());
		sSemaphore2.signal();
		Looper::loop();
	}
};

class Runnable1 : public Runnable
{
public:
	virtual void run() {
		printf("Runnable1::run by Looper %p\n", Looper::myLooper());
		assert(false);
	}
};

class Closure1
{
public:
	void test(int32_t value) {
		printf("Closure1::test with value %d by Looper %p\n",
				value, Looper::myLooper());
	}
};

sp<Thread1> sThread1 = new Thread1();
sp<Thread2> sThread2 = new Thread2();
Closure1 sClosure1;

int main() {
	sThread1->start();
	sThread2->start();

	sSemaphore1.wait();
	sSemaphore2.wait();

	sHandler4 = new Handler4(*sLooper2);

	sp<Message> msg = sHandler1->obtainMessage(Handler1::MSG_PRINT_INFO);
	while (true) {
		sHandler1->sendMessage(msg);
		sHandler1->sendMessage(msg->dup());

		sHandler1->obtainMessage(Handler1::MSG_START_ASYNC_TASKS)->sendToTarget();

		sp<Runnable1> runnable1 = new Runnable1();
		sHandler2->postDelayed(runnable1, 100);
		sHandler2->removeCallbacks(runnable1);
		sHandler2->postDelayed(newRunnable(sClosure1, &Closure1::test, 42), 500);

		sHandler3->test();
		sHandler4->test();

		Thread::sleep(2000);
	}

	sLooper1->quit();
	sThread1->join();
	sLooper2->quit();
	sThread2->join();

	return 0;
}
