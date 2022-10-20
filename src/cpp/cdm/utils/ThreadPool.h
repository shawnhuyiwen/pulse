/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include "cdm/CommonDefs.h"

#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <queue>
#include <atomic>
#include <condition_variable>

template<typename T>
class MultiThreadedVectorProcessor
{
public:
  MultiThreadedVectorProcessor(const std::vector<T*>& v) : m_Vector(v)
  {
    m_NextIdx.store(0);
    m_NumComplete.store(0);
    m_Stop = false;
  }
  virtual ~MultiThreadedVectorProcessor()
  {
    Stop();
  }

  void Start(size_t numThreads)
  {
    Stop();
    m_NextIdx.store(m_Vector.size());
    m_NumComplete.store(0);
    for (size_t i = 0; i < numThreads; i++)
      m_Threads.push_back(std::thread(&MultiThreadedVectorProcessor::Run, this));
  }

  void Stop()
  {
    m_Stop = true;
    for (std::thread& t : m_Threads)
      t.join();
    m_Threads.clear();
    m_Stop = false;
  }

  void ProcessVectorContents()
  {
    m_NumComplete.store(0);
    m_NextIdx.store(0);
    // Wait for all items in vector to be processed
    while (m_NumComplete.load(std::memory_order_consume) < m_Vector.size());
  }

  void Run()
  {
    while (!m_Stop)
    {
      if (m_NextIdx.load(std::memory_order_consume) >= m_Vector.size())
      {
        continue;
      }
      T* m = nullptr;
      m_Mutex.lock();
      size_t idx = m_NextIdx.load();
      if (idx < m_Vector.size())
      {
        m = m_Vector[idx];
        m_NextIdx++;
      }
      m_Mutex.unlock();
      if (m != nullptr)
      {
        Work(m);
        m_NumComplete++;
      }
    }
    m_NumComplete.store(m_Vector.size());
  }

  virtual void Work(T*) = 0;
protected:

  std::atomic<size_t>        m_NextIdx;
  std::atomic<size_t>        m_NumComplete;
  const std::vector<T*>&     m_Vector;


  bool                       m_Stop;
  std::mutex                 m_Mutex;
  std::vector<std::thread>   m_Threads;
};

/*
*** assigns indices to threads instead of first-come-first-serve
*** keeps threads alive as long as possible
*** will need to update constructor calls with numThreads and don't need to call start anymore
template<typename T>
class MultiThreadedVectorProcessor
{
public:
  MultiThreadedVectorProcessor(const std::vector<T*>& v, size_t numThreads = 1) : m_Vector(v)
  {
    m_NumThreads = numThreads;
    m_Stop = false;
    SplitVector();

    StartThreads();
  }
  virtual ~MultiThreadedVectorProcessor()
  {
    Stop();
  }

  void Stop()
  {
    m_Stop = true;
    for(std::thread& t : m_Threads)
      t.join();
    m_Threads.clear();
    m_Stop = false;
  }

  void SetNumberOfThreads(size_t numThreads)
  {
    if(m_NumThreads != numThreads)
    {
      Stop();

      m_NumThreads = numThreads;
      SplitVector();
      StartThreads();
    }
  }

  void ProcessVectorContents()
  {
    // Ensure the proper number of threads are running
    if(m_Threads.size() != m_NumThreads)
    {
      size_t nThreads = m_NumThreads;
      m_NumThreads = std::numeric_limits<std::size_t>::max();
      SetNumberOfThreads(nThreads);
    }

    // Elements have been added/removed from vector
    if(m_Vector.size() != m_VectorSize)
      SplitVector();
  
    // Tell threads to process
    m_NumComplete = 0;
    for( auto p : m_Process )
    {
      p = true;
    }

    // Wait for all threads to process
    while (m_NumComplete < m_Vector.size())
      std::this_thread::sleep_for(std::chrono::nanoseconds(1));
  }

  void Run(size_t id)
  {
    while(true)
    {
      if(m_Stop)
        return;

      if(m_Process[id])
      {
        auto& indices = m_IndexAssignments[id];
        for(size_t idx = indices.first; idx < indices.second; ++idx)
        {
          T* m = m_Vector[idx];
          if( m != nullptr )
          {
            Work(m);
          }
        }
        m_Mutex.lock();
        m_Process[id] = false;
        m_NumComplete += indices.second - indices.first;
        m_Mutex.unlock();
      }
      else
      {
        std::this_thread::sleep_for(std::chrono::nanoseconds(1));
      }
    }
  }

  virtual void Work(T*) = 0;
protected:

  const std::vector<T*>&                  m_Vector;
  size_t                                  m_VectorSize;
  std::vector<std::pair<size_t, size_t>>  m_IndexAssignments;

  size_t                                  m_NumThreads;
  std::vector<std::thread>                m_Threads;

  bool                                    m_Stop;
  size_t                                  m_NumComplete;
  std::vector<bool>                       m_Process;
  std::mutex                              m_Mutex;
  

  void StartThreads()
  {
    m_Process = std::vector<bool>(m_NumThreads, false);
    for(size_t i = 0; i < m_NumThreads; ++i)
    {
      m_Threads.push_back(std::thread(&MultiThreadedVectorProcessor::Run, this, i));
    }
  }

  void SplitVector()
  {
    m_IndexAssignments.clear();
    m_VectorSize = m_Vector.size();

    size_t length = m_Vector.size() / m_NumThreads;
    size_t remain = m_Vector.size() % m_NumThreads;

    size_t begin = 0;
    size_t end = 0;

    for (size_t i = 0; i < std::min(m_NumThreads, m_Vector.size()); ++i)
    {
      end += (remain > 0) ? (length + !!(remain--)) : length;

      m_IndexAssignments.push_back(std::make_pair(begin, end));

      begin = end;
    }
  }
};
*/

/*
*** MultiThreadedVectorProcessor implementation using std::futures
*** assigns indices to threads instead of first-come-first-serve
*** spawns std::asyncs each time instead of full threads
*** will need to update constructor calls with numThreads and don't need to call start anymore
template<typename T>
class MultiThreadedVectorProcessor
{
public:
  MultiThreadedVectorProcessor(const std::vector<T*>& v, size_t numThreads = 1) : m_Vector(v)
  {
    m_NumThreads = numThreads;
  }
  virtual ~MultiThreadedVectorProcessor()
  {
  }

  void SetNumberOfThreads(size_t numThreads)
  {
    m_NumThreads = numThreads;
  }

  void ProcessVectorContents()
  {
    const std::vector<std::pair<size_t, size_t>> threadSplit = SplitVector(m_Vector, m_NumThreads);

    std::vector<std::future<void>> futures;
    for(auto indices: threadSplit)
      futures.emplace_back(std::async(std::launch::async, [=](){ Run(indices); }));

    for(auto& future: futures)
      future.get();
  }

  void Run(std::pair<size_t, size_t> indices)
  {
    for(size_t idx = indices.first; idx < indices.second; ++idx)
    {
      T* m = m_Vector[idx];
      if( m != nullptr )
      {
        Work(m);
      }
    }
  }

  virtual void Work(T*) = 0;
protected:

  const std::vector<T*>&     m_Vector;
  size_t                     m_NumThreads;

  std::vector<std::pair<size_t,size_t>> SplitVector(const std::vector<T*>& vec, size_t n)
  {
    std::vector<std::pair<size_t, size_t>> outVec;

    size_t length = vec.size() / n;
    size_t remain = vec.size() % n;

    size_t begin = 0;
    size_t end = 0;

    for (size_t i = 0; i < std::min(n, vec.size()); ++i)
    {
      end += (remain > 0) ? (length + !!(remain--)) : length;

      outVec.push_back(std::make_pair(begin, end));

      begin = end;
    }

    return outVec;
  }
};
*/


/*
*** MultiThreadedVectorProcessor implementation using std::condition_variables
*** assigns indices to threads instead of first-come-first-serve
*** keeps threads alive as long as possible
*** will need to update constructor calls with numThreads and don't need to call start anymore
template<typename T>
class MultiThreadedVectorProcessor
{
public:
  MultiThreadedVectorProcessor(const std::vector<T*>& v, size_t numThreads = 1) : m_Vector(v)
  {
    m_NumThreads = numThreads;
    m_Stop = false;

    // Assign vector indices to threads and start up threads
    SplitVector();
    StartThreads();
  }
  virtual ~MultiThreadedVectorProcessor()
  {
    Stop();
  }

  void Stop()
  {
    m_Stop = true;
    for(size_t i = 0; i < m_NumThreads; ++i)
    {
      m_CondVar[i].notify_all();
      m_Threads[i].join();
    }      
    m_Threads.clear();
    m_Stop = false;
  }

  void SetNumberOfThreads(size_t numThreads)
  {
    if(m_NumThreads != numThreads)
    {
      Stop();

      m_NumThreads = numThreads;
      SplitVector();
      StartThreads();
    }
  }

  void ProcessVectorContents()
  {
    // Ensure the proper number of threads are running
    if(m_Threads.size() != m_NumThreads)
    {
      size_t nThreads = m_NumThreads;
      m_NumThreads = std::numeric_limits<std::size_t>::max();
      SetNumberOfThreads(nThreads);
    }

    // Elements have been added/removed from vector
    if(m_Vector.size() != m_VectorSize)
      SplitVector();
  
    // Tell threads to process
    for(size_t i = 0; i < m_Process.size(); ++i)
    {
      std::lock_guard<std::mutex> guard(m_Mutex[i]);
      m_Process[i] = true;
      m_CondVar[i].notify_all();
    }

    // Wait for all threads to process
    for(size_t i = 0; i < m_Process.size(); ++i)
    {
      std::unique_lock<std::mutex> L{m_Mutex[i]};
      m_CondVar[i].wait(L, [&]()
      {
        // Acquire lock only if we've processed
        return !m_Process[i];
      });
    }
  }

  void Run(size_t id)
  {
    while(true)
    {
      // Wait until told to process or told to stop
      std::unique_lock<std::mutex> L{m_Mutex[id]};
      m_CondVar[id].wait(L, [&]()
      {
        // Acquire lock only if we've stopped or have something to process
        return m_Stop || m_Process[id];
      });

      if(m_Stop)
        return;

      if(m_Process[id])
      {
        auto& indices = m_IndexAssignments[id];
        for(size_t idx = indices.first; idx < indices.second; ++idx)
        {
          T* m = m_Vector[idx];
          if( m != nullptr )
          {
            Work(m);
          }
        }
        m_Process[id] = false;
        m_CondVar[id].notify_all();
      }
    }
  }

  virtual void Work(T*) = 0;
protected:

  const std::vector<T*>&                  m_Vector;
  size_t                                  m_VectorSize;
  std::vector<std::pair<size_t, size_t>>  m_IndexAssignments;

  size_t                                  m_NumThreads;
  std::vector<std::thread>                m_Threads;

  std::vector<bool>                       m_Process;
  std::vector<std::mutex>                 m_Mutex;
  std::vector<std::condition_variable>    m_CondVar;
  bool                                    m_Stop;
  

  void StartThreads()
  {
    m_Process = std::vector<bool>(m_NumThreads, false);
    m_Mutex = std::vector<std::mutex>(m_NumThreads);
    m_CondVar = std::vector<std::condition_variable>(m_NumThreads);
    for(size_t i = 0; i < m_NumThreads; ++i)
    {
      m_Threads.push_back(std::thread(&MultiThreadedVectorProcessor::Run, this, i));
    }
  }

  void SplitVector()
  {
    m_IndexAssignments.clear();
    m_VectorSize = m_Vector.size();

    size_t length = m_Vector.size() / m_NumThreads;
    size_t remain = m_Vector.size() % m_NumThreads;

    size_t begin = 0;
    size_t end = 0;

    for (size_t i = 0; i < std::min(m_NumThreads, m_Vector.size()); ++i)
    {
      end += (remain > 0) ? (length + !!(remain--)) : length;

      m_IndexAssignments.push_back(std::make_pair(begin, end));

      begin = end;
    }
  }
};
*/


// Original code https://github.com/progschj/ThreadPool
class ThreadPool {
public:
  ThreadPool(size_t = 0);
  template<class F, class... Args>
  auto enqueue(F&& f, Args&&... args)
    ->std::future<typename std::result_of<F(Args...)>::type>;
  size_t workerCount() const { return workers.size(); }
  ~ThreadPool();
private:
  // need to keep track of threads so we can join them
  std::vector< std::thread > workers;
  // the task queue
  std::queue< std::function<void()> > tasks;

  // synchronization
  std::mutex queue_mutex;
  std::condition_variable condition;
  bool stop;
};

// the constructor just launches some amount of workers
inline ThreadPool::ThreadPool(size_t threads)
  : stop(false)
{
  if (threads == 0)
    threads = std::thread::hardware_concurrency();
  for (size_t i = 0; i < threads; ++i)
    workers.emplace_back(
      [this]
  {
    for (;;)
    {
      std::function<void()> task;

      {
        std::unique_lock<std::mutex> lock(this->queue_mutex);
        this->condition.wait(lock,
          [this] { return this->stop || !this->tasks.empty(); });
        if (this->stop && this->tasks.empty())
          return;
        task = std::move(this->tasks.front());
        this->tasks.pop();
      }

      task();
    }
  }
  );
}

// add new work item to the pool
template<class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args)
-> std::future<typename std::result_of<F(Args...)>::type>
{
  using return_type = typename std::result_of<F(Args...)>::type;

  auto task = std::make_shared< std::packaged_task<return_type()> >(
    std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );

  std::future<return_type> res = task->get_future();
  {
    std::unique_lock<std::mutex> lock(queue_mutex);

    // don't allow enqueueing after stopping the pool
    if (stop)
      throw std::runtime_error("enqueue on stopped ThreadPool");

    tasks.emplace([task]() { (*task)(); });
  }
  condition.notify_one();
  return res;
}

// the destructor joins all threads
inline ThreadPool::~ThreadPool()
{
  {
    std::unique_lock<std::mutex> lock(queue_mutex);
    stop = true;
  }
  condition.notify_all();
  for (std::thread& worker : workers)
    worker.join();
}
