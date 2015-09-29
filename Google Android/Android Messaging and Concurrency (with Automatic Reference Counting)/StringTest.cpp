#include "android/lang/String.h"
#include <stdio.h>
#include <assert.h>

using namespace android::os;
using namespace android::lang;
using namespace android::util;

int main() {
	String s1("Hello");
	printf("%s\n", s1.c_str());
	s1 += " World";
	printf("%s\n", s1.c_str());
	s1 = s1 + "!";
	printf("%s\n", s1.c_str());
	s1 = "Test";
	printf("%s\n", s1.c_str());

	String s2 = String::format("%d", 123);
	printf("%s\n", s2.c_str());
	s2 = s2.appendFormatted("%s%d", "456", 789);
	printf("%s\n", s2.c_str());

	String s3 = s2;
	printf("%s\n", s3.c_str());
	s2 = NULL;
	if (!s2.isNull()) {
		printf("%s\n", s2.c_str());
	}
	printf("%s\n", s3.c_str());

	if (s3 == "123456789") {
		printf("OK\n");
	} else {
		printf("Error\n");
	}

	String s4("    1234    ");
	assert(s4.size() == 12);
	assert(s4 == "    1234    ");
	s4 = s4.trim();
	assert(s4.size() == 4);
	assert(s4 == "1234");

	s4 = "\r\n";
	assert(s4.size() == 2);
	assert(s4 == "\r\n");
	s4 = s4.trim();
	assert(s4.size() == 0);
	assert(s4 == "");

	String s5;
	assert(s5.isNull());
	assert(s5.isEmpty());

	String s6("Test");
	assert(s6.startsWith("Te"));
	assert(!s6.startsWith("abc"));
	assert(s6.substr(1) == "est");
	assert(s6.substr(1, 3) == "es");
	assert(s6.indexOf("es") == 1);

	String s7("Test");
	assert(s7.endsWith("st"));

	assert(s7.left(2) == "Te");
	assert(s7.right(2) == "st");
	assert(s7.right(20) == "Test");

	String s8("abc\n\n\ndef\r\nyxz");
	sp< List<String> > strings = s8.split("\n");
	List<String>::iterator itr = strings->begin();
	while (itr != strings->end()) {
		printf("%s\n", itr->trim().c_str());
		++itr;
	}

	return 0;
}
