/*
 * Copyright (C) 2012 Daniel Himmelein
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

#include "android/lang/String.h"
#include <stdio.h>

using namespace android::os;
using namespace android::util;

namespace android {
namespace lang {

sp<String::StringBuffer> String::sNullString = new String::StringBuffer();
sp<String::StringBuffer> String::sEmptyString = new String::StringBuffer("", 0);

String::String() {
	mString = sNullString;
}

String::String(const String& string) {
	mString = string.mString;
}

String::String(const char* string) {
	if (string != NULL) {
		size_t size = strlen(string);
		if (size > 0) {
			mString = new StringBuffer(string, size);
		} else {
			mString = sEmptyString;
		}
	} else {
		mString = sNullString;
	}
}

String::String(const char* string, size_t size) {
	if (string != NULL) {
		if (size > 0) {
			mString = new StringBuffer(string, size);
		} else {
			mString = sEmptyString;
		}
	} else {
		mString = sNullString;
	}
}

bool String::contains(const char* subString) const {
	if (c_str() && subString) {
		return strstr(c_str(), subString) != NULL;
	} else {
		return false;
	}
}

String& String::operator=(const char* string) {
	if (string != NULL) {
		size_t size = strlen(string);
		if (size > 0) {
			mString = new StringBuffer(string, size);
		} else {
			mString = sEmptyString;
		}
	} else {
		mString = sNullString;
	}
	return *this;
}

bool String::operator<(const char* string) const {
	if (mString->mData && string) {
		return strcmp(mString->mData, string) < 0;
	} else {
		return false;
	}
}

bool String::operator<(const String& string) const {
	if (mString->mData && string.mString->mData) {
		return strcmp(mString->mData, string.mString->mData) < 0;
	} else {
		return false;
	}
}

bool String::operator<=(const char* string) const {
	if (mString->mData && string) {
		return strcmp(mString->mData, string) <= 0;
	} else {
		return false;
	}
}

bool String::operator<=(const String& string) const {
	if (mString->mData && string.mString->mData) {
		return strcmp(mString->mData, string.mString->mData) <= 0;
	} else {
		return false;
	}
}

bool String::operator==(const char* string) const {
	if (mString->mData && string) {
		return strcmp(mString->mData, string) == 0;
	} else {
		if (mString->mData == NULL && string == NULL) {
			return true;
		} else {
			return false;
		}
	}
}

bool String::operator==(const String& string) const {
	if (mString->mData && string.mString->mData) {
		return strcmp(mString->mData, string.mString->mData) == 0;
	} else {
		if (mString->mData == NULL && string.mString->mData == NULL) {
			return true;
		} else {
			return false;
		}
	}
}

bool String::operator!=(const char* string) const {
	if (mString->mData && string) {
		return strcmp(mString->mData, string) != 0;
	} else {
		if (mString->mData == NULL && string == NULL) {
			return false;
		} else {
			return true;
		}
	}
}

bool String::operator!=(const String& string) const {
	if (mString->mData && string.mString->mData) {
		return strcmp(mString->mData, string.mString->mData) != 0;
	} else {
		if (mString->mData == NULL && string.mString->mData == NULL) {
			return false;
		} else {
			return true;
		}
	}
}

bool String::operator>=(const char* string) const {
	if (mString->mData && string) {
		return strcmp(mString->mData, string) >= 0;
	} else {
		return false;
	}
}

bool String::operator>=(const String& string) const {
	if (mString->mData && string.mString->mData) {
		return strcmp(mString->mData, string.mString->mData) >= 0;
	} else {
		return false;
	}
}

bool String::operator>(const char* string) const {
	if (mString->mData && string) {
		return strcmp(mString->mData, string) > 0;
	} else {
		return false;
	}
}

bool String::operator>(const String& string) const {
	if (mString->mData && string.mString->mData) {
		return strcmp(mString->mData, string.mString->mData) > 0;
	} else {
		return false;
	}
}

bool String::equalsIgnoreCase(const char* string) const {
	return toLowerCase() == String(string).toLowerCase();
}

bool String::equalsIgnoreCase(const String& string) const {
	return toLowerCase() == string.toLowerCase();
}

bool String::contains(const String& subString) const {
	return contains(subString.c_str());
}

bool String::startsWith(const char* prefix) const {
	size_t prefixSize = strlen(prefix);
	if (size() >= prefixSize) {
		return strncmp(c_str(), prefix, prefixSize) == 0;
	} else {
		return false;
	}
}

bool String::startsWith(const String& prefix) const {
	return startsWith(prefix.c_str());
}

bool String::endsWith(const char* suffix) const {
	size_t suffixSize = strlen(suffix);
	if (size() >= suffixSize) {
		return strncmp(c_str() + size() - suffixSize, suffix, suffixSize) == 0;
	} else {
		return false;
	}
}

bool String::endsWith(const String& suffix) const {
	return endsWith(suffix.c_str());
}

String String::substr(size_t beginIndex) const {
	if (beginIndex < size()) {
		return String(c_str() + beginIndex, size() - beginIndex);
	} else {
		return String();
	}
}

String String::substr(size_t beginIndex, size_t endIndex) const {
	if (beginIndex < size()) {
		return String(c_str() + beginIndex, endIndex - beginIndex);
	} else {
		return String();
	}
}

String String::toLowerCase() const {
	android::os::sp<StringBuffer> string = new StringBuffer(size());
	for (size_t i = 0; i < size(); i++) {
		string->mData[i] = tolower(mString->mData[i]);
	}
	String tmp(string);
	return tmp;
}

String String::toUpperCase() const {
	android::os::sp<StringBuffer> string = new StringBuffer(size());
	for (size_t i = 0; i < size(); i++) {
		string->mData[i] = toupper(mString->mData[i]);
	}
	String tmp(string);
	return tmp;
}

ssize_t String::indexOf(const char* string) const {
	const char* substr = strstr(c_str(), string);
	if (substr != NULL) {
		return substr - c_str();
	} else {
		return -1;
	}
}

ssize_t String::indexOf(const String& string) const {
	return indexOf(string.c_str());
}

ssize_t String::indexOf(const char* string, size_t fromIndex) const {
	const char* substr = strstr(c_str() + fromIndex, string);
	if (substr != NULL) {
		return substr - c_str();
	} else {
		return -1;
	}
}

ssize_t String::indexOf(const String& string, size_t fromIndex) const {
	return indexOf(string.c_str() + fromIndex);
}

String String::trim() const {
	String tmp(*this);
	size_t beginIndex;
	ssize_t endIndex;
	for (beginIndex = 0; beginIndex < tmp.size(); beginIndex++) {
		if (!isspace(tmp.mString->mData[beginIndex])) {
			break;
		}
	}
	for (endIndex = tmp.size() - 1; endIndex >= 0; endIndex--) {
		if (!isspace(tmp.mString->mData[endIndex])) {
			break;
		}
	}
	if (beginIndex == 0 && endIndex == (ssize_t)tmp.size() - 1) {
		return tmp;
	} else {
		if (beginIndex != tmp.size()) {
			tmp.mString = new StringBuffer(tmp.mString->mData + beginIndex, endIndex - beginIndex + 1);
		} else {
			tmp.mString = sEmptyString;
		}
		return tmp;
	}
}

String String::left(size_t n) const {
	String tmp(*this);
	n = (n > tmp.size()) ? tmp.size() : n;
	tmp.mString = new StringBuffer(tmp.mString->mData, n);
	return tmp;
}

String String::right(size_t n) const {
	String tmp(*this);
	n = (n > tmp.size()) ? tmp.size() : n;
	tmp.mString = new StringBuffer(tmp.mString->mData + tmp.size() - n , n);
	return tmp;
}

sp< List<String> > String::split(const char* separator) const {
	sp< List<String> > strings = new List<String>();
	ssize_t curIndex = 0;
	ssize_t prevCurIndex;
	while (curIndex >= 0 && (size_t)curIndex < size()) {
		prevCurIndex = curIndex;
		curIndex = indexOf(separator, curIndex);
		if (curIndex >= 0) {
			strings->push_back(substr(prevCurIndex, curIndex));
			curIndex += strlen(separator);
		} else {
			strings->push_back(substr(prevCurIndex, size()));
		}
	}
	return strings;
}

sp< List<String> > String::split(const String& separator) const {
	return split(separator.c_str());
}

String& String::append(const char* data, size_t size) {
	if (data != NULL && size > 0) {
		sp<StringBuffer> oldString = mString;
		mString = new StringBuffer(oldString->mData, oldString->mSize, data, size);
	}
	return *this;
}

String& String::appendFormatted(const char* format, ...) {
	va_list args;
	va_start(args, format);
	appendFormattedWithVarArgList(format, args);
	va_end(args);
	return *this;
}

String String::format(const char* format, ...) {
	va_list args;
	va_start(args, format);
	String string;
	string.appendFormattedWithVarArgList(format, args);
	va_end(args);
	return string;
}

String& String::appendFormattedWithVarArgList(const char* format, va_list args) {
	// see http://stackoverflow.com/questions/9937505/va-list-misbehavior-on-linux
	va_list copyOfArgs;
	va_copy(copyOfArgs, args);
	int size = vsnprintf(NULL, 0, format, copyOfArgs);
	va_end(copyOfArgs);

    if (size != 0) {
        sp<StringBuffer> oldString = mString;
        mString = new StringBuffer(oldString->mSize + size);
        memcpy(mString->mData, oldString->mData, oldString->mSize);
        vsnprintf(mString->mData + oldString->mSize, size + 1, format, args);
    }

    return *this;
}

String::StringBuffer::StringBuffer(size_t size) {
	mSize = size;
	if (size > 0) {
		mData = (char*) malloc(mSize + 1);
		mData[size] = '\0';
	} else {
		mData = NULL;
	}
}

String::StringBuffer::StringBuffer(const char* string, size_t size) {
	mSize = size;
	if (string != NULL) {
		mData = (char*) malloc(mSize + 1);
		memcpy(mData, string, size);
		mData[size] = '\0';
	} else {
		mData = NULL;
	}
}

String::StringBuffer::StringBuffer(const char* string1, size_t size1, const char* string2, size_t size2) {
	mSize = size1 + size2;
	mData = (char*) malloc(mSize + 1);
	memcpy(mData, string1, size1);
	memcpy(mData + size1, string2, size2);
	mData[size1 + size2] = '\0';
}

} /* namespace lang */
} /* namespace android */

bool operator<(const char* lhs, const android::lang::String& rhs) {
	return rhs < lhs;
}

bool operator<=(const char* lhs, const android::lang::String& rhs) {
	return rhs <= lhs;
}

bool operator==(const char* lhs, const android::lang::String& rhs) {
	return rhs == lhs;
}

bool operator!=(const char* lhs, const android::lang::String& rhs) {
	return rhs != lhs;
}

bool operator>=(const char* lhs, const android::lang::String& rhs) {
	return rhs >= lhs;
}

bool operator>(const char* lhs, const android::lang::String& rhs) {
	return rhs > lhs;
}
