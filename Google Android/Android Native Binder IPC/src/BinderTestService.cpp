#include "BinderTestService.h"
#include <binder/IPCThreadState.h>
#include <binder/IServiceManager.h>
#include <binder/Parcel.h>
#include <utils/Log.h>

namespace android {

void BinderTestService::instantiate()
{
    if(defaultServiceManager()->addService(String16("BinderTestService"), new BinderTestService()) != NO_ERROR)
    {
    	LOGE("BinderService error");
    }
}

BinderTestService::BinderTestService() {
	mAndroidSharedMemory = new MemoryHeapBase(4096, 0, "Android Shared Memory");
}

BinderTestService::~BinderTestService() {
}

int32_t BinderTestService::test1() {
	LOGI("BinderTestService::onTransact(TEST1) on thread %d starting...", pthread_self());
	sleep(4);
	LOGI("BinderTestService::onTransact(TEST1) on thread %d done", pthread_self());
	return 17;
}

int32_t BinderTestService::test2(int32_t value) {
	LOGI("BinderTestService::onTransact(TEST2) with value %d on thread %d starting", value, pthread_self());
	sleep(2);
	LOGI("BinderTestService::onTransact(TEST2) with value %d on thread %d done", value, pthread_self());
	return 42;
}

sp<IMemoryHeap> BinderTestService::test3(const String8& message) {
	LOGI("BinderTestService::onTransact(TEST3) on thread %d starting", pthread_self());
	strcpy((char*)mAndroidSharedMemory->base(), message.string());
	LOGI("BinderTestService::onTransact(TEST3) on thread %d done", pthread_self());
	return mAndroidSharedMemory;
}

} /* namespace android */

using namespace android;

int main(int argc, char** argv)
{
	// The Binder thread pool has some threads to concurrently handle IPC calls
    sp<ProcessState> processState(ProcessState::self());
    sp<IServiceManager> sm = defaultServiceManager();
    BinderTestService::instantiate();
    ProcessState::self()->startThreadPool();
    IPCThreadState::self()->joinThreadPool();
    return 0;
}
