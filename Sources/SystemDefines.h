#pragma once

#define GET_SYSTEM(SystemType) SystemManager::I()->GetSystem<SystemType, SystemType##Id>()