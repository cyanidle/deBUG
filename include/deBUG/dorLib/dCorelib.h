#pragma once
#include <memory>
#include <thread>
#include <mutex>
#include <queue>
#include <deque>
#include <vector>
#include "dLinearAlg.h"
#include "dVariant.h"
class Worker
{
public:
	Worker();
	void run();
	void stop();
	void pause();
	void spin();
};

struct JobStatus
{
	std::string repr;
	int code;
};


struct dArgs
{
	dArgs():empty(true) {};
	dArgs(dVariant val) {
		append(val);
	}
	dArgs(int val) {
		append(val);
	}
	dArgs(float val) {
		append(val);
	}
	void append(dVariant val) {
		list.push_back(val);
	}
	void append(float val) {
		f_list.push_back(val);
	}
	void append(int val) {
		i_list.push_back(val);
	}
	std::deque<dVariant> list;
	std::deque<float> f_list;
	std::deque<int> i_list;
	bool empty = false;
	uint16_t numArgs;
};

struct dTask
{
	dArgs args;
	virtual void operator()(dArgs* args) = 0;
	bool threadSafe;
	~dTask() {
	}
};

class dJob
{
public:
	dJob() {};
	void init(dTask* task) {
		append(task);
	}
	void append(dTask* task) {
		m_tasks.emplace_back(task);
		++m_tasksLeft;
	}
	JobStatus& getStatus() { return m_status; };
	void setStatus(const JobStatus& newStatus) {
		m_status = newStatus;
	}
	void run_safe() {
		;
	}
	void run() {
		;
	}
	void finish() {
		;
	}
	std::unique_ptr<dTask>& nextTask() {
		m_tasks.pop_back();
		if (!(--m_tasksLeft))
			finish();
		else
			return m_tasks.front();
	}
protected:
	JobStatus m_status;
	std::deque<std::unique_ptr<dTask>> m_tasks;
	uint32_t m_tasksLeft;
};



class dThreadPool
{
public:
	dThreadPool* init();
	void run();
	void stop();
	void appendJob(dJob newJob);
private:
	~dThreadPool() noexcept;
	dThreadPool(const uint32_t num_threads);
	const uint32_t m_num_threads;
	bool m_should_terminate = false;           // Tells threads to stop looking for jobs
	std::mutex m_queue_mutex;                  // Prevents data races to the job queue
	std::condition_variable m_mutex_condition; // Allows threads to wait on new jobs or termination 
	std::vector<Worker*> m_workers;
	std::queue<dJob> m_jobs;
};