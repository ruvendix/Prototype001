// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "ThreadPool.h"

#include "Tls.h"

RxThreadPool::RxThreadPool()
{
	m_hardwareConcurrencyThreadCount = std::thread::hardware_concurrency();

	for (int i = 0; i < m_hardwareConcurrencyThreadCount; ++i)
	{
		m_threads.push_back(std::thread(
			[this]()
			{
				StartupTls();
				ProcessWorkerThread();
				CleanupTls();
			}));
	}
}

RxThreadPool::~RxThreadPool()
{
	Cleanup();
}

// 어떤 쓰레드가 일을 할지는 모름... (쓰레드 순서대로 일을 하는 게 아님!)
void RxThreadPool::ProcessWorkerThread()
{
	while (true)
	{
		TaskFuncType taskFuncObj = nullptr;

		// Critical section
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_conditionVar.wait(lock, [this]() { return ((m_taskQueue.empty() == false) || (m_bAtomicAllStop == true)); });

			// 중단 시그널이 왔으면 탈출 (락은 자동 해제)
			if ((m_bAtomicAllStop == true) &&
				(m_taskQueue.empty() == true))
			{
				return;
			}

			taskFuncObj = std::move(m_taskQueue.front());
			m_taskQueue.pop();
		}

		taskFuncObj();
	}
}

void RxThreadPool::StartupTls()
{
	std::hash<std::thread::id> threadIdHash;

	// Critical section
	{
		std::lock_guard<std::mutex> lock(m_mutex); // Tls도 크리티컬 섹션에 넣어야 함...
		uint32 hashValue = static_cast<uint32>(threadIdHash(std::this_thread::get_id()));
		//printf("ThreadId: %u\n", hashValue);
		m_threadIds.push_back(hashValue);
	}
}

void RxThreadPool::CleanupTls()
{

}

/*
멀티쓰레드 위주로 처리되는 프로그램은 일반적인 프로그램과
흐름이 다르므로 쓰레드들이 일을 마칠 때까지 대기해야 할 수도 있음...
요 쓰레드풀에서는 일하는 중인 모든 쓰레드들을 강제로
중단시키는 것도 가능하므로 m_bAllStop과 분리함
*/
void RxThreadPool::Join()
{
	for (std::thread& refThread : m_threads)
	{
		if (refThread.joinable() == true)
		{
			refThread.join();
		}
	}

	::OutputDebugStringA("모든 서브 쓰레드 종료\n");
}

void RxThreadPool::Cleanup()
{
	// 이걸로 종료 신호를 줌
	m_bAtomicAllStop = true;
	m_conditionVar.notify_all();
}