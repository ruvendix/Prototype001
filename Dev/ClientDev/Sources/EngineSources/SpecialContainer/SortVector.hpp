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
            return (priority < other.priority); // �켱������ ���� ���� ���� ������
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

        // �̹� �ִ� �켱������� ��ü
        if (foundIter != m_vecTask.cend())
        {
            // ������ �ִ� �켱������ ������ �����ϰ� ���ο� ������Ʈ�� �߰�
            foundIter->priority = (m_vecTask.back().priority) + 1;
        }

        m_vecTask.push_back(newTask);
        SortTasks();
    }

    // �켱���� ��ȯ
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

        // �� ã�Ҵ����� Ȯ��
        if ((pLhsTask == nullptr) ||
            (pRhsTask == nullptr))
        {
            return;
        }

        std::swap(pLhsTask->priority, pRhsTask->priority);
        SortTasks();
    }

    // �켱���� ���� �� �����
    void RemovePriority(int32 targetPriority)
    {
        // �켱������ removePriority�� ��� ����
        m_vecTask.erase(std::remove_if(m_vecTask.begin(), m_vecTask.end(),
            [=](const Task& task)
            {
                return (task.priority == targetPriority);
            }), m_vecTask.end());

        // �켱������ ū ��ҵ��� �켱������ �ϳ��� ����
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