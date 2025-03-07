#pragma once

// �� ��ü�� ����
extern PersistantResourcePtr g_spNullResource;

template <typename TPersistantResource>
class ResourceStorage
{
public:
	PersistantResourcePtr CreateResource(const std::string& strResKey)
	{
		PersistantResourcePtr spRes = FindPersistantSharedResource(strResKey);
		if (spRes != nullptr)
		{
			return spRes;
		}

		spRes = std::make_shared<TPersistantResource>();
		spRes->SetTag(strResKey);

		// �����ϴ� �������� ������
		auto insertedIter = m_mapRes.insert(std::make_pair(strResKey, spRes));
		ASSERT_LOG(insertedIter.second == true);

		return spRes;
	}

	const PersistantResource* FindPersistantResource(const std::string& strResKey) const
	{
		const PersistantResourcePtr& spResource = FindPersistantSharedResource(strResKey);
		return (spResource.get());
	}

	const PersistantResourcePtr& FindPersistantSharedResource(const std::string& strResKey) const
	{
		auto foundIter = m_mapRes.find(strResKey);
		if (foundIter == m_mapRes.cend())
		{
			return g_spNullResource;
		}

		return (foundIter->second);
	}

	PersistantResourcePtr LoadResource(const std::string& strResKey)
	{
		const PersistantResourcePtr& spFoundRes = FindPersistantSharedResource(strResKey);		
		if (spFoundRes != nullptr)
		{
			return spFoundRes;
		}

		PersistantResourcePtr spRes = std::make_shared<TPersistantResource>();
		spRes->SetResourceRelativePath(strResKey);
		spRes->LoadResource(strResKey);

		auto insertedIter = m_mapRes.insert(std::make_pair(strResKey, spRes));
		ASSERT_LOG(insertedIter.second == true);

		return spRes;
	}

private:
	// key���� ���ҽ� ��� �Ǵ� ���ҽ� �±װ� ��
	std::unordered_map<std::string, PersistantResourcePtr> m_mapRes;
};