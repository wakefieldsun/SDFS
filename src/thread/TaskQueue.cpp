/*
 * TaskQueue.cpp
 *
 *  Created on: 2013年7月22日
 *      Author: sun
 */

#include "TaskQueue.h"
#include "../common/common_define.h"

namespace sdfs {

const TaskQueue* TaskQueue::instance = new TaskQueue(TASKQUEUE_MAX_SIZE);


} /* namespace sdfs */
