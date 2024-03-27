/*
<내용 요약>
컴포넌트와 관련된 매크로 모음
*/

#pragma once

#define ADD_COMPONENT(Actor, ComponentType)    Actor->AddComponent<ComponentType, ComponentType##Id>()
#define REMOVE_COMPONENT(Actor, ComponentType) Actor->RemoveComponent<ComponentType, ComponentType##Id>()
#define GET_COMPONENT(Actor, ComponentType)    Actor->GetComponent<ComponentType, ComponentType##Id>()