#pragma once

// 널 객체를 만듬
extern PersistantResourcePtr g_spNullResource;

template <typename TPersistantResource>
class ResourceStorage
{
public:
	PersistantResourcePtr CreateResource(const std::string& strResKey)
	{
		PersistantResourcePtr spRes = std::make_shared<TPersistantResource>();
		spRes->SetTag(strResKey);

		// 생성하는 순간부터 관리함
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
	// key값은 리소스 경로 또는 리소스 태그가 들어감
	std::unordered_map<std::string, PersistantResourcePtr> m_mapRes;
};