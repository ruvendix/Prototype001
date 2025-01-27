// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

class InputMappingContext
{
public:
	InputMappingContext();
	~InputMappingContext();

	void AddInputAction(const InputActionPtr& spInputAction);
	bool ProcessInputMappingContext();

	void SetId(int32 id) { m_id = id; }
	int32 GetId() const { return m_id; }

private:
	int32 m_id = 0;
	std::vector<InputActionPtr> m_vecInputAction;
};