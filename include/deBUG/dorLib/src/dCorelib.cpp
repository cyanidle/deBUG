#include "dCorelib.h"

Worker::Worker()
{
}

void Worker::run()
{
}

void Worker::stop()
{
}

void Worker::pause()
{
}

void Worker::spin()
{
}


dThreadPool* dThreadPool::init() {
    const uint32_t num_threads = std::thread::hardware_concurrency(); // Max # of threads the system supports
    auto pool = new dThreadPool(num_threads);
    m_workers.resize(num_threads);
    for (uint32_t i = 0; i < num_threads; i++) {
        m_workers.at(i) = new Worker();
    }
    return pool;
}


void dThreadPool::run()
{
}


void dThreadPool::stop()
{
}


void dThreadPool::appendJob(dJob newJob)
{
}

dThreadPool::dThreadPool(uint32_t num_workers) : m_num_threads(num_workers)
{

}

dThreadPool::~dThreadPool() noexcept{
    for (uint32_t i = 0; i < m_num_threads; i++) {
        free(m_workers.at(i));
    }
}

