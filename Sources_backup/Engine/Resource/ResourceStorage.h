#pragma once

// �� ��ü�� ����
extern PersistantResourcePtr g_spNullResource;

template <typename TPersistantResource>
class ResourceStorage
{
public:
	PersistantResourcePtr CreateResource(const std::string& strResKey)
	{
		PersistantResourcePtr spRes = std::make_shared<TPersistantResource>();
		spRes->SetTag(strResKey);

		// �����ϴ� �������� ������
		const auto& retIter = m_mapRes.insert(std::make_pair(strResKey, spRes));
		ASSERT_LOG(retIter.second == true);

		return spRes;
	}

	const PersistantResource* SearchResource(const std::string& strResKey) const
	{
		const PersistantResourcePtr& spResource = SearchSharedResource(strResKey);
		return (spResource.get());
	}

	const PersistantResourcePtr& SearchSharedResource(const std::string& strResKey) const
	{
		const auto& foundIter = m_mapRes.find(strResKey);
		if (foundIter == m_mapRes.cend())
		{
			return g_spNullResource;
		}

		return (foundIter->second);
	}

	PersistantResourcePtr LoadResource(const std::string& strResKey)
	{
		if (SearchResource(strResKey) != nullptr)
		{
			DETAIL_ERROR_LOG(EErrorCode::ExistedResource);
			return nullptr;
		}

		PersistantResourcePtr spRes = std::make_shared<TPersistantResource>();
		spRes->SetResourceRelativePath(strResKey);
		spRes->LoadResource(strResKey);
		const auto& retIter = m_mapRes.insert(std::make_pair(strResKey, spRes));
		ASSERT_LOG(retIter.second == true);

		return spRes;
	}

private:
	// key���� ���ҽ� ��� �Ǵ� ���ҽ� �±װ� ��
	std::unordered_map<std::string, PersistantResourcePtr> m_mapRes;
};