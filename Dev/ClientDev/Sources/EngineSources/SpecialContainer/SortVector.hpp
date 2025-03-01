// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

template <typename TObject>
class SortVector
{
    struct Task
    {
        int32 priority = -1;
        TObject obj;

        bool operator < (const Task& other) const
        {
            return (priority < other.priority); // 우선순위가 낮은 것이 먼저 오도록
        }
    };

public:
    using TaskIterator = typename std::vector<Task>::iterator;
    using TaskConstIterator = typename std::vector<Task>::const_iterator;

public:
    TaskIterator Begin()
    {
        return (m_vecTask.begin());
    }

    TaskConstIterator Begin() const
    {
        return (m_vecTask.begin());
    }

    TaskIterator End()
    {
        return (m_vecTask.end());
    }

    TaskConstIterator End() const
    {
        return (m_vecTask.end());
    }

    void AddObject(const TObject& obj)
    {
        Task newTask;
        newTask.obj = obj;

        if (m_vecTask.empty() == true)
        {
            newTask.priority = 0;
        }
        else
        {
            newTask.priority = (m_vecTask.back().priority) + 1;
        }

        m_vecTask.push_back(newTask);
    }

    void AddObject(const TObject& obj, int32 priority)
    {
        Task newTask;
        newTask.obj = obj;
        newTask.priority = priority;

        auto foundIter = std::find_if(m_vecTask.begin(), m_vecTask.end(),
            [=](const Task& other)
            {
                return (other.priority == priority);
            });

        // 이미 있는 우선순위라면 교체
        if (foundIter != m_vecTask.cend())
        {
            // 기존에 있던 우선순위를 끝으로 변경하고 새로운 오브젝트를 추가
            foundIter->priority = (m_vecTask.back().priority) + 1;
        }

        m_vecTask.push_back(newTask);
        SortTasks();
    }

    // 우선순위 교환
    void SwapPriority(int32 lhsPriority, int32 rhsPriority)
    {
        Task* pLhsTask = nullptr;
        Task* pRhsTask = nullptr;

        for (Task& refTask : m_vecTask)
        {
            if (refTask.priority == lhsPriority)
            {
                pLhsTask = &refTask;
            }
            else if (refTask.priority == rhsPriority)
            {
                pRhsTask = &refTask;
            }

            if ((pLhsTask != nullptr) &&
                (pRhsTask != nullptr))
            {
                break;
            }
        }

        // 못 찾았는지도 확인
        if ((pLhsTask == nullptr) ||
            (pRhsTask == nullptr))
        {
            return;
        }

        std::swap(pLhsTask->priority, pRhsTask->priority);
        SortTasks();
    }

    // 우선순위 삭제 후 땡기기
    void RemovePriority(int32 targetPriority)
    {
        // 우선순위가 removePriority인 요소 제거
        m_vecTask.erase(std::remove_if(m_vecTask.begin(), m_vecTask.end(),
            [=](const Task& task)
            {
                return (task.priority == targetPriority);
            }), m_vecTask.end());

        // 우선순위가 큰 요소들의 우선순위를 하나씩 감소
        for (auto& task : m_vecTask)
        {
            if (task.priority > targetPriority)
            {
                task.priority--;
            }
        }
    }

    void Cleanup()
    {
        m_vecTask.clear();
    }

private:
    void SortTasks()
    {
        std::sort(m_vecTask.begin(), m_vecTask.end());
    }

private:
    std::vector<Task> m_vecTask;
};