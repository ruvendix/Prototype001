// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

#include "InputEnums.h"

using InputActionHandler = std::function<void()>;
using InputActionPtr = std::shared_ptr<class InputAction>;
using InputMappingContextPtr = std::shared_ptr<class InputMappingContext>;

using KeyboardDevicePtr = std::shared_ptr<class KeyboardDevice>;
using MouseDevicePtr = std::shared_ptr<class MouseDevice>;

using InputActionValueModifierBitset = EnumBitset<EInputActionValueModifierType, TO_NUM(EInputActionValueModifierType::Count)>;