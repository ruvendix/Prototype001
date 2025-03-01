// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class RxThreadPool
{
public:
	RxThreadPool();
	~RxThreadPool();

	// 일 들어왔다!
	template <typename TTaskFunc, typename... Args>
	std::future<std::invoke_result_t<TTaskFunc, Args&&...>> AddTask(TTaskFunc taskFunc, Args&&... args)
	{
		using ReturnType = std::invoke_result_t<TTaskFunc, Args&&...>;
		using PackagedTask = std::packaged_task<ReturnType(void)>;

		/*
		std::bind()로 함수 객체를 만들 때 인자들을 전달한 함수에 bind하므로
		bind된 함수 객체를 저장하는 쪽에서는 매개변수를 void로 정할 수 있음.
		반환값이 필요하다면 저장하는 쪽에서 반환 형식을 명시하면됨
		*/
		auto bindTask = std::bind(taskFunc, std::forward<Args>(args)...);
		std::shared_ptr<PackagedTask> spTask = std::make_shared<PackagedTask>(bindTask);

		// task는 local에서 생성하므로 queue에 유지시키려면 heap으로 넘겨야함
		TaskFuncType taskFuncObj = [spTask]() { (*spTask)(); };

		// Critical section
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			m_taskQueue.push(taskFuncObj);
		}

		m_conditionVar.notify_one();
		return (spTask->get_future());
	}

	// 쓰레드 전용 함수
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

	// 쓰레드 Id 관리용
	std::vector<uint32> m_threadIds;
};