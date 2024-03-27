/*
<내용 요약>
시스템과 관련된 매크로 모음
*/

#pragma once

#define GET_SYSTEM(SystemType) SystemManager::I()->GetSystem<SystemType, SystemType##Id>()