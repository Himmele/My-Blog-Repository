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

#ifndef RUNNABLE_H_
#define RUNNABLE_H_

#include <stdint.h>

namespace android {
namespace os {

class Runnable
{
public:
	virtual ~Runnable() {}
	virtual void run() = 0;
};

} /* namespace os */
} /* namespace android */

#endif /*RUNNABLE_H_*/
