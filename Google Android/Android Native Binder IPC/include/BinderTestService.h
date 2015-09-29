#ifndef BINDERTESTSERVICE_H_
#define BINDERTESTSERVICE_H_

#include <IBinderTestService.h>

namespace android {

class BinderTestService :
	public BnBinderTestService
{
public:
	BinderTestService();
	virtual ~BinderTestService();

	static void instantiate();

	virtual int32_t test1();
	virtual int32_t test2(int32_t value);
	virtual sp<IMemoryHeap> test3(const String8& message);

private:
	sp<MemoryHeapBase> mAndroidSharedMemory;
};

} /* namespace android */

#endif /* BINDERTESTSERVICE_H_ */
