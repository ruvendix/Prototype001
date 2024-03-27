#pragma once

#include "SystemBase.h"
#include "UiBase.h"

class UiSystem : public SystemBase
{
public:
	UiSystem();
	virtual ~UiSystem();

	virtual bool Update()  override;
	virtual void Render()  override;
	virtual void Cleanup() override;

public:
	template <typename TUi>
	TUi::Ptr CreateUi(const std::string& strUiName)
	{
		typename TUi::Ptr spNewUi = std::make_shared<TUi>();
		spNewUi->Startup();

		m_uiStorage.emplace(strUiName, spNewUi);
		return spNewUi;
	}

private:
	std::unordered_map<std::string, UiBase::Ptr> m_uiStorage;
};