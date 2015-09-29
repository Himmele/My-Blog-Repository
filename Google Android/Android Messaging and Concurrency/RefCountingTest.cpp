#include <stdio.h>
#include "android/os/Ref.h"

using namespace android::os;

class RefTest : public Ref {
public:
	RefTest(int32_t id) : mID(id) {
		printf("RefTest ctor: %d\n", mID);
	}

	virtual ~RefTest() {
		printf("RefTest dtor: %d\n", mID);
	}

	int32_t id() const {
		return mID;
	}

private:
	int32_t mID;
};

int main() {
	sp<RefTest> ref1 = new RefTest(1);

	{
	sp<RefTest> ref2 = new RefTest(2);
	}

	wp<RefTest> ref3 = ref1;
	sp<RefTest> ref31 = ref3.toStrongRef();
	if (ref31 == NULL) {
		printf("RefTest object is already destroyed\n");
	} else {
		printf("RefTest object %d is still around\n", ref31->id());
		ref31->printRefs();
	}
	ref31 = NULL;

	ref1 = NULL;
	ref31 = ref3.toStrongRef();
	if (ref31 == NULL) {
		printf("RefTest object is already destroyed\n");
	} else {
		printf("RefTest object %d is still around\n", ref31->id());
		ref31->printRefs();
	}

	wp<RefTest> ref4 = new RefTest(4);
	wp<RefTest> ref5 = ref4;

	return 0;
}
