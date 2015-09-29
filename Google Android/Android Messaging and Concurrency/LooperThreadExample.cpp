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
#include "android/os/LooperThread.h"

using namespace android::os;

class ExampleHandler : public Handler
{
public:
	virtual void handleMessage(Message& message) {
		printf("ExampleHandler::handleMessage with ID %d by Looper 0x%x\n",
				message.what, Looper::myLooper());
	}
};

class Test
{
public:
	void test(int32_t value) {
		printf("Test::test() called with value %d\n", value);
	}
};

static LooperThread<ExampleHandler> sLooperThread;
static Test sTest;

int main() {
	sLooperThread.start();
	Handler* handler = sLooperThread.getHandler();

	handler->obtainMessage(1234).sendToTarget();
	handler->postDelayed(newRunnable(sTest, &Test::test, 42), 1000);

	Thread::sleep(2000);

	sLooperThread.getLooper()->quit();
	sLooperThread.join();

	return 0;
}
