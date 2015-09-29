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

#ifndef ANDROID_OS_EVENT_H_
#define ANDROID_OS_EVENT_H_

#include <stdlib.h>
#include <vector>
#include <algorithm>
#include "android/os/Utils.h"
#include "android/os/Delegate.h"

namespace android {
namespace os {

template<typename R = void, typename A1 = void, typename A2 = void, typename A3 = void, typename A4 = void>
class Event;

// 4 arguments
template<typename R, typename A1, typename A2, typename A3, typename A4>
class Event
{
public:
	Event() {}
	~Event() {}

    Event& operator+=(const Delegate<R, A1, A2, A3, A4>& delegate) {
    	mEventHandlers.push_back(delegate);
    	return *this;
    }

    Event& operator-=(const Delegate<R, A1, A2, A3, A4>& delegate) {
		typename std::vector< Delegate<R, A1, A2, A3, A4> >::iterator itr;
		itr = std::find(mEventHandlers.begin(), mEventHandlers.end(), delegate);
		if (itr != mEventHandlers.end()) {
			mEventHandlers.erase(itr);
		}
		return *this;
	}

    void operator()(A1 arg1, A2 arg2, A3 arg3, A4 arg4) const {
    	typename std::vector< Delegate<R, A1, A2, A3, A4> >::const_iterator itr;
		for (itr = mEventHandlers.begin(); itr != mEventHandlers.end(); ++itr) {
			(*itr)(arg1, arg2, arg3, arg4);
		}
	}

private:
    std::vector< Delegate<R, A1, A2, A3, A4> > mEventHandlers;

    NO_COPY_CTOR_AND_ASSIGNMENT_OPERATOR(Event)
};

template<typename R, typename A1, typename A2, typename A3, typename A4>
class Event<R (A1, A2, A3, A4)>
{
public:
	Event() {}
	~Event() {}

	Event& operator+=(const Delegate<R (A1, A2, A3, A4)>& delegate) {
		mEventHandlers.push_back(delegate);
		return *this;
	}

	Event& operator-=(const Delegate<R (A1, A2, A3, A4)>& delegate) {
		typename std::vector< Delegate<R (A1, A2, A3, A4)> >::iterator itr;
		itr = std::find(mEventHandlers.begin(), mEventHandlers.end(), delegate);
		if (itr != mEventHandlers.end()) {
			mEventHandlers.erase(itr);
		}
		return *this;
	}

	void operator()(A1 arg1, A2 arg2, A3 arg3, A4 arg4) const {
		typename std::vector< Delegate<R (A1, A2, A3, A4)> >::const_iterator itr;
		for (itr = mEventHandlers.begin(); itr != mEventHandlers.end(); ++itr) {
			(*itr)(arg1, arg2, arg3, arg4);
		}
	}

private:
	std::vector< Delegate<R (A1, A2, A3, A4)> > mEventHandlers;

	NO_COPY_CTOR_AND_ASSIGNMENT_OPERATOR(Event)
};

// 3 arguments
template<typename R, typename A1, typename A2, typename A3>
class Event<R, A1, A2, A3, void>
{
public:
	Event() {}
	~Event() {}

	Event& operator+=(const Delegate<R, A1, A2, A3>& delegate) {
		mEventHandlers.push_back(delegate);
		return *this;
	}

	Event& operator-=(const Delegate<R, A1, A2, A3>& delegate) {
		typename std::vector< Delegate<R, A1, A2, A3> >::iterator itr;
		itr = std::find(mEventHandlers.begin(), mEventHandlers.end(), delegate);
		if (itr != mEventHandlers.end()) {
			mEventHandlers.erase(itr);
		}
		return *this;
	}

	void operator()(A1 arg1, A2 arg2, A3 arg3) const {
		typename std::vector< Delegate<R, A1, A2, A3> >::const_iterator itr;
		for (itr = mEventHandlers.begin(); itr != mEventHandlers.end(); ++itr) {
			(*itr)(arg1, arg2, arg3);
		}
	}

private:
	std::vector< Delegate<R, A1, A2, A3> > mEventHandlers;

	NO_COPY_CTOR_AND_ASSIGNMENT_OPERATOR(Event)
};

template<typename R, typename A1, typename A2, typename A3>
class Event<R (A1, A2, A3), void>
{
public:
	Event() {}
	~Event() {}

	Event& operator+=(const Delegate<R (A1, A2, A3)>& delegate) {
		mEventHandlers.push_back(delegate);
		return *this;
	}

	Event& operator-=(const Delegate<R (A1, A2, A3)>& delegate) {
		typename std::vector< Delegate<R (A1, A2, A3)> >::iterator itr;
		itr = std::find(mEventHandlers.begin(), mEventHandlers.end(), delegate);
		if (itr != mEventHandlers.end()) {
			mEventHandlers.erase(itr);
		}
		return *this;
	}

	void operator()(A1 arg1, A2 arg2, A3 arg3) const {
		typename std::vector< Delegate<R (A1, A2, A3)> >::const_iterator itr;
		for (itr = mEventHandlers.begin(); itr != mEventHandlers.end(); ++itr) {
			(*itr)(arg1, arg2, arg3);
		}
	}

private:
	std::vector< Delegate<R (A1, A2, A3)> > mEventHandlers;

	NO_COPY_CTOR_AND_ASSIGNMENT_OPERATOR(Event)
};

// 2 arguments
template<typename R, typename A1, typename A2>
class Event<R, A1, A2, void, void>
{
public:
	Event() {}
	~Event() {}

	Event& operator+=(const Delegate<R, A1, A2>& delegate) {
		mEventHandlers.push_back(delegate);
		return *this;
	}

	Event& operator-=(const Delegate<R, A1, A2>& delegate) {
		typename std::vector< Delegate<R, A1, A2> >::iterator itr;
		itr = std::find(mEventHandlers.begin(), mEventHandlers.end(), delegate);
		if (itr != mEventHandlers.end()) {
			mEventHandlers.erase(itr);
		}
		return *this;
	}

	void operator()(A1 arg1, A2 arg2) const {
		typename std::vector< Delegate<R, A1, A2> >::const_iterator itr;
		for (itr = mEventHandlers.begin(); itr != mEventHandlers.end(); ++itr) {
			(*itr)(arg1, arg2);
		}
	}

private:
	std::vector< Delegate<R, A1, A2> > mEventHandlers;

	NO_COPY_CTOR_AND_ASSIGNMENT_OPERATOR(Event)
};

template<typename R, typename A1, typename A2>
class Event<R (A1, A2), void, void>
{
public:
	Event() {}
	~Event() {}

	Event& operator+=(const Delegate<R (A1, A2)>& delegate) {
		mEventHandlers.push_back(delegate);
		return *this;
	}

	Event& operator-=(const Delegate<R (A1, A2)>& delegate) {
		typename std::vector< Delegate<R (A1, A2)> >::iterator itr;
		itr = std::find(mEventHandlers.begin(), mEventHandlers.end(), delegate);
		if (itr != mEventHandlers.end()) {
			mEventHandlers.erase(itr);
		}
		return *this;
	}

	void operator()(A1 arg1, A2 arg2) const {
		typename std::vector< Delegate<R (A1, A2)> >::const_iterator itr;
		for (itr = mEventHandlers.begin(); itr != mEventHandlers.end(); ++itr) {
			(*itr)(arg1, arg2);
		}
	}

private:
	std::vector< Delegate<R (A1, A2)> > mEventHandlers;

	NO_COPY_CTOR_AND_ASSIGNMENT_OPERATOR(Event)
};

// 1 argument
template<typename R, typename A1>
class Event<R, A1, void, void, void>
{
public:
	Event() {}
	~Event() {}

	Event& operator+=(const Delegate<R, A1>& delegate) {
		mEventHandlers.push_back(delegate);
		return *this;
	}

	Event& operator-=(const Delegate<R, A1>& delegate) {
		typename std::vector< Delegate<R, A1> >::iterator itr;
		itr = std::find(mEventHandlers.begin(), mEventHandlers.end(), delegate);
		if (itr != mEventHandlers.end()) {
			mEventHandlers.erase(itr);
		}
		return *this;
	}

	void operator()(A1 arg1) const {
		typename std::vector< Delegate<R, A1> >::const_iterator itr;
		for (itr = mEventHandlers.begin(); itr != mEventHandlers.end(); ++itr) {
			(*itr)(arg1);
		}
	}

private:
	std::vector< Delegate<R, A1> > mEventHandlers;

	NO_COPY_CTOR_AND_ASSIGNMENT_OPERATOR(Event)
};

template<typename R, typename A1>
class Event<R (A1), void, void, void>
{
public:
	Event() {}
	~Event() {}

	Event& operator+=(const Delegate<R (A1)>& delegate) {
		mEventHandlers.push_back(delegate);
		return *this;
	}

	Event& operator-=(const Delegate<R (A1)>& delegate) {
		typename std::vector< Delegate<R (A1)> >::iterator itr;
		itr = std::find(mEventHandlers.begin(), mEventHandlers.end(), delegate);
		if (itr != mEventHandlers.end()) {
			mEventHandlers.erase(itr);
		}
		return *this;
	}

	void operator()(A1 arg1) const {
		typename std::vector< Delegate<R (A1)> >::const_iterator itr;
		for (itr = mEventHandlers.begin(); itr != mEventHandlers.end(); ++itr) {
			(*itr)(arg1);
		}
	}

private:
	std::vector< Delegate<R (A1)> > mEventHandlers;

	NO_COPY_CTOR_AND_ASSIGNMENT_OPERATOR(Event)
};

// no arguments
template<typename R>
class Event<R, void, void, void, void>
{
public:
	Event() {}
	~Event() {}

	Event& operator+=(const Delegate<R>& delegate) {
		mEventHandlers.push_back(delegate);
		return *this;
	}

	Event& operator-=(const Delegate<R>& delegate) {
		typename std::vector< Delegate<R> >::iterator itr;
		itr = std::find(mEventHandlers.begin(), mEventHandlers.end(), delegate);
		if (itr != mEventHandlers.end()) {
			mEventHandlers.erase(itr);
		}
		return *this;
	}

	void operator()() const {
		typename std::vector< Delegate<R> >::const_iterator itr;
		for (itr = mEventHandlers.begin(); itr != mEventHandlers.end(); ++itr) {
			(*itr)();
		}
	}

private:
	std::vector< Delegate<R> > mEventHandlers;

	NO_COPY_CTOR_AND_ASSIGNMENT_OPERATOR(Event)
};

template<typename R>
class Event<R (void), void, void, void>
{
public:
	Event() {}
	~Event() {}

	Event& operator+=(const Delegate<R>& delegate) {
		mEventHandlers.push_back(delegate);
		return *this;
	}

	Event& operator-=(const Delegate<R>& delegate) {
		typename std::vector< Delegate<R> >::iterator itr;
		itr = std::find(mEventHandlers.begin(), mEventHandlers.end(), delegate);
		if (itr != mEventHandlers.end()) {
			mEventHandlers.erase(itr);
		}
		return *this;
	}

	void operator()() const {
		typename std::vector< Delegate<R> >::const_iterator itr;
		for (itr = mEventHandlers.begin(); itr != mEventHandlers.end(); ++itr) {
			(*itr)();
		}
	}

private:
	std::vector< Delegate<R> > mEventHandlers;

	NO_COPY_CTOR_AND_ASSIGNMENT_OPERATOR(Event)
};

} /* namespace os */
} /* namespace android */

#endif /* ANDROID_OS_EVENT_H_ */
