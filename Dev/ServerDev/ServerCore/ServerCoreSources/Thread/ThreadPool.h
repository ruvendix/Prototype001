// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class RxThreadPool
{
public:
	RxThreadPool();
	~RxThreadPool();

	// �� ���Դ�!
	template <typename TTaskFunc, typename... Args>
	std::future<std::invoke_result_t<TTaskFunc, Args&&...>> AddTask(TTaskFunc taskFunc, Args&&... args)
	{
		using ReturnType = std::invoke_result_t<TTaskFunc, Args&&...>;
		using PackagedTask = std::packaged_task<ReturnType(void)>;

		/*
		std::bind()�� �Լ� ��ü�� ���� �� ���ڵ��� ������ �Լ��� bind�ϹǷ�
		bind�� �Լ� ��ü�� �����ϴ� �ʿ����� �Ű������� void�� ���� �� ����.
		��ȯ���� �ʿ��ϴٸ� �����ϴ� �ʿ��� ��ȯ ������ ����ϸ��
		*/
		auto bindTask = std::bind(taskFunc, std::forward<Args>(args)...);
		std::shared_ptr<PackagedTask> spTask = std::make_shared<PackagedTask>(bindTask);

		// task�� local���� �����ϹǷ� queue�� ������Ű���� heap���� �Ѱܾ���
		TaskFuncType taskFuncObj = [spTask]() { (*spTask)(); };

		// Critical section
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			m_taskQueue.push(taskFuncObj);
		}

		m_conditionVar.notify_one();
		return (spTask->get_future());
	}

	// ������ ���� �Լ�
	void ProcessWorkerThread();

	void StartupTls();
	void CleanupTls();

	void Join();
	void Cleanup();

	void ForceAllStop() { m_bAtomicAllStop = false; }

private:
	using TaskFuncType = std::function<void(void)>;

private:
	int m_hardwareConcurrencyThreadCount = 0;
	std::vector<std::thread> m_threads;

	std::mutex m_mutex; // Lock-based
	std::condition_variable m_conditionVar;

	std::queue<TaskFuncType> m_taskQueue;
	std::atomic<bool> m_bAtomicAllStop;

	// ������ Id ������
	std::vector<uint32> m_threadIds;
};