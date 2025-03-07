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

// � �����尡 ���� ������ ��... (������ ������� ���� �ϴ� �� �ƴ�!)
void RxThreadPool::ProcessWorkerThread()
{
	while (true)
	{
		TaskFuncType taskFuncObj = nullptr;

		// Critical section
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_conditionVar.wait(lock, [this]() { return ((m_taskQueue.empty() == false) || (m_bAtomicAllStop == true)); });

			// �ߴ� �ñ׳��� ������ Ż�� (���� �ڵ� ����)
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
		std::lock_guard<std::mutex> lock(m_mutex); // Tls�� ũ��Ƽ�� ���ǿ� �־�� ��...
		uint32 hashValue = static_cast<uint32>(threadIdHash(std::this_thread::get_id()));
		//printf("ThreadId: %u\n", hashValue);
		m_threadIds.push_back(hashValue);
	}
}

void RxThreadPool::CleanupTls()
{

}

/*
��Ƽ������ ���ַ� ó���Ǵ� ���α׷��� �Ϲ����� ���α׷���
�帧�� �ٸ��Ƿ� ��������� ���� ��ĥ ������ ����ؾ� �� ���� ����...
�� ������Ǯ������ ���ϴ� ���� ��� ��������� ������
�ߴܽ�Ű�� �͵� �����ϹǷ� m_bAllStop�� �и���
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

	::OutputDebugStringA("��� ���� ������ ����\n");
}

void RxThreadPool::Cleanup()
{
	// �̰ɷ� ���� ��ȣ�� ��
	m_bAtomicAllStop = true;
	m_conditionVar.notify_all();
}