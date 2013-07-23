/*
 * TaskQueue.h
 *
 *  Created on: 2013年7月22日
 *      Author: sun
 */

#ifndef TASKQUEUE_H_
#define TASKQUEUE_H_

#include <queue>
#include <vector>
#include "Mutex.h"

namespace sdfs {

typedef struct Task
{
	unsigned int	taskid;
	int				sockfd;
	int				type;
	int				weight;
	Task()
	{
		this->sockfd = -1;
		this->taskid = 0;
		this->type = -1;
		this->weight = 0;
	}
	Task(const Task &task)
	{
		this->sockfd = task.sockfd;
		this->taskid = task.taskid;
		this->type = task.type;
		this->weight = task.weight;
	}
}Task;

class TaskQueue {
private:
	TaskQueue(int maxsize)
	{
		m_nMaxSize = maxsize;
		m_queue = new std::queue<Task>;
	}
public:

	static TaskQueue* GetInstance()
	{
		return (TaskQueue *)instance;
	}

	virtual ~TaskQueue()
	{
		delete m_queue;
	}

	void Add(Task& task)
	{
		m_mutex.Lock();
		m_queue->push(task);
		m_mutex.Unlock();
	}

	Task* GetTask()
	{
		m_mutex.Lock();
		if(m_queue->size()< 1)
		{
			m_mutex.Unlock();
			return NULL;
		}
		Task t = m_queue->front();
		m_queue->pop();
		m_mutex.Unlock();
		return &t;
	}

	bool IsEmpty()
	{
		bool rst;
		m_mutex.Lock();
		rst =  m_queue->empty();
		m_mutex.Unlock();
		return rst;
	}
	bool IsFull()
	{
		bool rst;
		m_mutex.Lock();
		rst =  m_queue->size() == m_nMaxSize ? true : false;
		m_mutex.Unlock();
		return rst;
	}

	int GetSize()
	{
		int rst;
		m_mutex.Lock();
		rst =  m_queue->size();
		m_mutex.Unlock();
		return rst;
	}
private:
	int		m_nMaxSize;
	std::queue<Task>*	m_queue;
	Mutex	m_mutex;
	static const TaskQueue *instance;
};

} /* namespace sdfs */
#endif /* TASKQUEUE_H_ */
