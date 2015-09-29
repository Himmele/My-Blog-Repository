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

#include "android/os/Utils.h"
#include "android/os/Delegate.h"
#include "android/util/List.h"

namespace android {
namespace os {

template<typename R = void, typename A1 = void, typename A2 = void, typename A3 = void, typename A4 = void>
class Event;

// 4 arguments
template<typename R, typename A1, typename A2, typename A3, typename A4>
class Event
{
public:
	typedef Delegate<R, A1, A2, A3, A4> DelegateType;

	Event() { mEventHandlers = new android::util::List< DelegateType >(); }
	~Event() { }

    Event& operator+=(const DelegateType& delegate) {
    	mEventHandlers->push_back(delegate);
    	return *this;
    }

    Event& operator-=(const DelegateType& delegate) {
		typename android::util::List< DelegateType >::iterator itr;
		for (itr = mEventHandlers->begin(); itr != mEventHandlers->end(); ++itr) {
			if (*itr == delegate) {
				itr = mEventHandlers->erase(itr);
				break;
			}
		}
		return *this;
	}

    void operator()(A1 arg1, A2 arg2, A3 arg3, A4 arg4) const {
    	typename android::util::List< DelegateType >::const_iterator itr;
		for (itr = mEventHandlers.getConstPointer()->begin(); itr != mEventHandlers.getConstPointer()->end(); ++itr) {
			(*itr)(arg1, arg2, arg3, arg4);
		}
	}

private:
    android::os::sp< android::util::List< DelegateType > > mEventHandlers;

    NO_COPY_CTOR_AND_ASSIGNMENT_OPERATOR(Event)
};

template<typename R, typename A1, typename A2, typename A3, typename A4>
class Event<R (A1, A2, A3, A4)>
{
public:
	typedef Delegate<R (A1, A2, A3, A4)> DelegateType;

	Event() { mEventHandlers = new android::util::List< DelegateType >(); }
	~Event() { }


	Event& operator+=(const DelegateType& delegate) {
		mEventHandlers->push_back(delegate);
		return *this;
	}

	Event& operator-=(const DelegateType& delegate) {
		typename android::util::List< DelegateType >::iterator itr;
		for (itr = mEventHandlers->begin(); itr != mEventHandlers->end(); ++itr) {
			if (*itr == delegate) {
				itr = mEventHandlers->erase(itr);
				break;
			}
		}
		return *this;
	}

	void operator()(A1 arg1, A2 arg2, A3 arg3, A4 arg4) const {
		typename android::util::List< DelegateType >::const_iterator itr;
		for (itr = mEventHandlers.getConstPointer()->begin(); itr != mEventHandlers.getConstPointer()->end(); ++itr) {
			(*itr)(arg1, arg2, arg3, arg4);
		}
	}

private:
	android::os::sp< android::util::List< DelegateType > > mEventHandlers;

	NO_COPY_CTOR_AND_ASSIGNMENT_OPERATOR(Event)
};

// 3 arguments
template<typename R, typename A1, typename A2, typename A3>
class Event<R, A1, A2, A3, void>
{
public:
	typedef Delegate<R, A1, A2, A3> DelegateType;

	Event() { mEventHandlers = new android::util::List< DelegateType >(); }
	~Event() { }

	Event& operator+=(const DelegateType& delegate) {
		mEventHandlers->push_back(delegate);
		return *this;
	}

	Event& operator-=(const DelegateType& delegate) {
		typename android::util::List< DelegateType >::iterator itr;
		for (itr = mEventHandlers->begin(); itr != mEventHandlers->end(); ++itr) {
			if (*itr == delegate) {
				itr = mEventHandlers->erase(itr);
				break;
			}
		}
		return *this;
	}

	void operator()(A1 arg1, A2 arg2, A3 arg3) const {
		typename android::util::List< DelegateType >::const_iterator itr;
		for (itr = mEventHandlers.getConstPointer()->begin(); itr != mEventHandlers.getConstPointer()->end(); ++itr) {
			(*itr)(arg1, arg2, arg3);
		}
	}

private:
	android::os::sp< android::util::List< DelegateType > > mEventHandlers;

	NO_COPY_CTOR_AND_ASSIGNMENT_OPERATOR(Event)
};

template<typename R, typename A1, typename A2, typename A3>
class Event<R (A1, A2, A3), void>
{
public:
	typedef Delegate<R (A1, A2, A3)> DelegateType;

	Event() { mEventHandlers = new android::util::List< DelegateType >(); }
	~Event() { }

	Event& operator+=(const DelegateType& delegate) {
		mEventHandlers->push_back(delegate);
		return *this;
	}

	Event& operator-=(const DelegateType& delegate) {
		typename android::util::List< DelegateType >::iterator itr;
		for (itr = mEventHandlers->begin(); itr != mEventHandlers->end(); ++itr) {
			if (*itr == delegate) {
				itr = mEventHandlers->erase(itr);
				break;
			}
		}
		return *this;
	}

	void operator()(A1 arg1, A2 arg2, A3 arg3) const {
		typename android::util::List< DelegateType >::const_iterator itr;
		for (itr = mEventHandlers.getConstPointer()->begin(); itr != mEventHandlers.getConstPointer()->end(); ++itr) {
			(*itr)(arg1, arg2, arg3);
		}
	}

private:
	android::os::sp< android::util::List< DelegateType > > mEventHandlers;

	NO_COPY_CTOR_AND_ASSIGNMENT_OPERATOR(Event)
};

// 2 arguments
template<typename R, typename A1, typename A2>
class Event<R, A1, A2, void, void>
{
public:
	typedef Delegate<R, A1, A2> DelegateType;

	Event() { mEventHandlers = new android::util::List< DelegateType >(); }
	~Event() {}

	Event& operator+=(const DelegateType& delegate) {
		mEventHandlers->push_back(delegate);
		return *this;
	}

	Event& operator-=(const DelegateType& delegate) {
		typename android::util::List< DelegateType >::iterator itr;
		for (itr = mEventHandlers->begin(); itr != mEventHandlers->end(); ++itr) {
			if (*itr == delegate) {
				itr = mEventHandlers->erase(itr);
				break;
			}
		}
		return *this;
	}

	void operator()(A1 arg1, A2 arg2) const {
		typename android::util::List< DelegateType >::const_iterator itr;
		for (itr = mEventHandlers.getConstPointer()->begin(); itr != mEventHandlers.getConstPointer()->end(); ++itr) {
			(*itr)(arg1, arg2);
		}
	}

private:
	android::os::sp< android::util::List< DelegateType > > mEventHandlers;

	NO_COPY_CTOR_AND_ASSIGNMENT_OPERATOR(Event)
};

template<typename R, typename A1, typename A2>
class Event<R (A1, A2), void, void>
{
public:
	typedef Delegate<R (A1, A2)> DelegateType;

	Event() { mEventHandlers = new android::util::List< DelegateType >(); }
	~Event() { }

	Event& operator+=(const DelegateType& delegate) {
		mEventHandlers->push_back(delegate);
		return *this;
	}

	Event& operator-=(const DelegateType& delegate) {
		typename android::util::List< DelegateType >::iterator itr;
		for (itr = mEventHandlers->begin(); itr != mEventHandlers->end(); ++itr) {
			if (*itr == delegate) {
				itr = mEventHandlers->erase(itr);
				break;
			}
		}
		return *this;
	}

	void operator()(A1 arg1, A2 arg2) const {
		typename android::util::List< DelegateType >::const_iterator itr;
		for (itr = mEventHandlers.getConstPointer()->begin(); itr != mEventHandlers.getConstPointer()->end(); ++itr) {
			(*itr)(arg1, arg2);
		}
	}

private:
	android::os::sp< android::util::List< DelegateType > > mEventHandlers;

	NO_COPY_CTOR_AND_ASSIGNMENT_OPERATOR(Event)
};

// 1 argument
template<typename R, typename A1>
class Event<R, A1, void, void, void>
{
public:
	typedef Delegate<R, A1> DelegateType;

	Event() { mEventHandlers = new android::util::List< DelegateType >(); }
	~Event() { }

	Event& operator+=(const DelegateType& delegate) {
		mEventHandlers->push_back(delegate);
		return *this;
	}

	Event& operator-=(const DelegateType& delegate) {
		typename android::util::List< DelegateType >::iterator itr;
		for (itr = mEventHandlers->begin(); itr != mEventHandlers->end(); ++itr) {
			if (*itr == delegate) {
				itr = mEventHandlers->erase(itr);
				break;
			}
		}
		return *this;
	}

	void operator()(A1 arg1) const {
		typename android::util::List< DelegateType >::const_iterator itr;
		for (itr = mEventHandlers.getConstPointer()->begin(); itr != mEventHandlers.getConstPointer()->end(); ++itr) {
			(*itr)(arg1);
		}
	}

private:
	android::os::sp< android::util::List< DelegateType > > mEventHandlers;

	NO_COPY_CTOR_AND_ASSIGNMENT_OPERATOR(Event)
};

template<typename R, typename A1>
class Event<R (A1), void, void, void>
{
public:
	typedef Delegate<R (A1)> DelegateType;

	Event() { mEventHandlers = new android::util::List< DelegateType >(); }
	~Event() { }

	Event& operator+=(const DelegateType& delegate) {
		mEventHandlers->push_back(delegate);
		return *this;
	}

	Event& operator-=(const DelegateType& delegate) {
		typename android::util::List< DelegateType >::iterator itr;
		for (itr = mEventHandlers->begin(); itr != mEventHandlers->end(); ++itr) {
			if (*itr == delegate) {
				itr = mEventHandlers->erase(itr);
				break;
			}
		}
		return *this;
	}

	void operator()(A1 arg1) const {
		typename android::util::List< DelegateType >::const_iterator itr;
		for (itr = mEventHandlers.getConstPointer()->begin(); itr != mEventHandlers.getConstPointer()->end(); ++itr) {
			(*itr)(arg1);
		}
	}

private:
	android::os::sp< android::util::List< DelegateType > > mEventHandlers;

	NO_COPY_CTOR_AND_ASSIGNMENT_OPERATOR(Event)
};

// no arguments
template<typename R>
class Event<R, void, void, void, void>
{
public:
	typedef Delegate<R> DelegateType;

	Event() { mEventHandlers = new android::util::List< DelegateType >(); }
	~Event() { }

	Event& operator+=(const DelegateType& delegate) {
		mEventHandlers->push_back(delegate);
		return *this;
	}

	Event& operator-=(const DelegateType& delegate) {
		typename android::util::List< DelegateType >::iterator itr;
		for (itr = mEventHandlers->begin(); itr != mEventHandlers->end(); ++itr) {
			if (*itr == delegate) {
				itr = mEventHandlers->erase(itr);
				break;
			}
		}
		return *this;
	}

	void operator()() const {
		typename android::util::List< DelegateType >::const_iterator itr;
		for (itr = mEventHandlers.getConstPointer()->begin(); itr != mEventHandlers.getConstPointer()->end(); ++itr) {
			(*itr)();
		}
	}

private:
	android::os::sp< android::util::List< DelegateType > > mEventHandlers;

	NO_COPY_CTOR_AND_ASSIGNMENT_OPERATOR(Event)
};

template<typename R>
class Event<R (void), void, void, void>
{
public:
	typedef Delegate<R> DelegateType;

	Event() { mEventHandlers = new android::util::List< DelegateType >(); }
	~Event() { }

	Event& operator+=(const DelegateType& delegate) {
		mEventHandlers->push_back(delegate);
		return *this;
	}

	Event& operator-=(const DelegateType& delegate) {
		typename android::util::List< DelegateType >::iterator itr;
		for (itr = mEventHandlers->begin(); itr != mEventHandlers->end(); ++itr) {
			if (*itr == delegate) {
				itr = mEventHandlers->erase(itr);
				break;
			}
		}
		return *this;
	}

	void operator()() const {
		typename android::util::List< DelegateType >::const_iterator itr;
		for (itr = mEventHandlers.getConstPointer()->begin(); itr != mEventHandlers.getConstPointer()->end(); ++itr) {
			(*itr)();
		}
	}

private:
	android::os::sp< android::util::List< DelegateType > > mEventHandlers;

	NO_COPY_CTOR_AND_ASSIGNMENT_OPERATOR(Event)
};

} /* namespace os */
} /* namespace android */

#endif /* ANDROID_OS_EVENT_H_ */
