#include "android/os/Bundle.h"
#include "android/lang/String.h"
#include <stdio.h>
#include <assert.h>

using namespace android::os;
using namespace android::util;
using namespace android::lang;

class Test :
	public Ref
{
public:
	Test(uint32_t value) : mValue(value) { }
	virtual ~Test() { }
	uint32_t getValue() { return mValue; }

private:
	uint32_t mValue;
};

int main() {
	sp<Bundle> bundle = new Bundle();
	bundle->putBool("1", true);
	assert(bundle->getBool("1", false));
	bool value;
	assert(bundle->fillBool("1", &value));

	bundle->putInt32("2", 12345);
	assert(bundle->getInt32("2", 0) == 12345);
	assert(bundle->getInt64("2", 0) == 0);

	bundle->putObject("3", new Test(17));
	sp<Test> test = bundle->getObject<Test>("3");
	assert(test->getValue() == 17);
	assert(bundle->fillObject("3", &test));

	bundle->putString("4", "Test");
	String string = bundle->getString("4");
	assert(string == "Test");
	assert(bundle->fillString("4", &string));

	return 0;
}
