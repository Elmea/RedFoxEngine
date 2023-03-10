#pragma once
#include<thread>
#include<vector>
#include <any>

namespace RedFoxEngine
{
    enum class ThreadType
    {
        LOAD,
        PHYSICS,
        CAMERA
    };

    struct Thread
    {
        ThreadType type;
        std::thread* thread;
    };

    class ThreadManager
    {
    public:

        void AddThread(ThreadType type, std::thread* thread)
        {
            Thread newOne;
            newOne.type = type;
            newOne.thread = thread;
            threads.push_back(newOne);
        }

        ThreadManager::~ThreadManager()
        {
            for (int i = 0; i < threads.size(); i++)
                threads[i].thread->join();

            for (int i = 0; i < threads.size(); i++)
                delete threads[i].thread;
        }

    private:
        std::vector<Thread> threads;
    };

    static ThreadManager threadManager;
}