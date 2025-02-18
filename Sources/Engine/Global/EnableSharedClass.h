// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

// 이 경우는 유일무이한 다중 상속
class EnableSharedClass : public ICoreLoop, public std::enable_shared_from_this<EnableSharedClass>
{
public:
	EnableSharedClass() = default;
	virtual ~EnableSharedClass() = default;

public:
	template <typename TObject>
	std::shared_ptr<TObject> SharedFromThisExactType()
	{
		return (std::dynamic_pointer_cast<TObject>(shared_from_this()));
	}
};