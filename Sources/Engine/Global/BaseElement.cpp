// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "BaseElement.h"

void BaseElement::Startup()
{

}

bool BaseElement::Update(float deltaSeconds)
{
	return true;
}

// Update�� ����� ����� �� �ʿ���
bool BaseElement::PostUpdate(float deltaSeconds)
{
	return true;
}

void BaseElement::Cleanup()
{

}

void BaseElement::SaveToFileStream()
{

}

void BaseElement::LoadFromFileStream()
{

}