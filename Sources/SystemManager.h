/*
<내용 요약>
시스템들의 관리자.
초기화와 정리의 순서를 편하게 설정할 수 있음.
*/

#pragma once
#include "SystemBase.h"
#include "SystemIdCounter.h"

class SystemManager : public IDefault
{
public:
	DEFINE_SINGLETON(SystemManager);

public:
	SystemManager();
	~SystemManager();

	virtual void Startup() override;
	virtual bool Update()  override;
	virtual void Render()  override;
	virtual void Cleanup() override;

public:
	template <typename TSystem>
	void AddSystem()
	{
		std::is_convertible_v<TSystem, SystemBase>;

		TSystem* pNewSystem = new TSystem;
		pNewSystem->Startup();

		m_systems.emplace_back(pNewSystem);
	}

	template <typename TSystem, typename TSystemId>
	TSystem* GetSystem()
	{
		std::is_convertible_v<TSystem, SystemBase>;

		if (m_systems.empty() == true)
		{
			return nullptr;
		}

		if (m_systems.size() <= TSystemId::s_id)
		{
			return nullptr;
		}

		TSystem* pSystem = dynamic_cast<TSystem*>(m_systems[TSystemId::s_id]);
		assert(pSystem != nullptr);

		return pSystem;
	}

private:
	std::vector<SystemBase*> m_systems;
};