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
#include "ThreadCondition.h"
#include "../common/Log.h"

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
		m_waiting = true;
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
		m_cond.Lock();
		m_queue->push(task);
		m_cond.Signal();
		m_cond.Unlock();
	}
	/**
	 * GetTask will block, if no task in the queue, the method will
	 * be waked up by Add
	 */
	int GetTask(Task &task)
	{
		m_cond.Lock();
		Log::Debug("m_queue->size: %d", m_queue->size());
		while(m_queue->size()< 1 && m_waiting)
		{
			int err = m_cond.Wait();
			Log::Debug("waked...");
			if(err != 0)
			{
				Log::Error("File: "__FILE__", line: %d, " \
				"call pthread_join, errno: %d, info: %s",
				__LINE__, err, STRERROR(err));
				m_cond.Unlock();
				return err;
			}
		}
		if(!m_waiting)
		{
			m_cond.Unlock();
			return -1;
		}
		task = m_queue->front();
		m_queue->pop();
		m_cond.Unlock();
		return 0;
	}

	int StopWaiting()
	{
		m_waiting = false;
		return m_cond.Broadcast();
	}

	bool IsEmpty()
	{
		bool rst;
		m_cond.Lock();
		rst =  m_queue->empty();
		m_cond.Unlock();
		return rst;
	}
	bool IsFull()
	{
		bool rst;
		m_cond.Lock();
		rst =  m_queue->size() == m_nMaxSize ? true : false;
		m_cond.Unlock();
		return rst;
	}

	int GetSize()
	{
		int rst;
		m_cond.Lock();
		rst =  m_queue->size();
		m_cond.Unlock();
		return rst;
	}
private:
	unsigned int		m_nMaxSize;
	std::queue<Task>*	m_queue;
	CThreadCondition	m_cond;
	static const TaskQueue *instance;
	bool				m_waiting;
};

} /* namespace sdfs */
#endif /* TASKQUEUE_H_ */
