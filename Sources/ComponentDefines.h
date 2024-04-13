#pragma once

#define ADD_COMPONENT(Actor, ComponentType) Actor->AddComponent<ComponentType, ComponentType##Id>()
#define REMOVE_COMPONENT(Actor, ComponentType) Actor->RemoveComponent<ComponentType, ComponentType##Id>()
#define GET_COMPONENT(Actor, ComponentType) Actor->GetComponent<ComponentType, ComponentType##Id>()
#define FIND_COMPONENTS(Actor, ComponentType, outFoundComponents) Actor->FindComponents<ComponentType, ComponentType##Id>(outFoundComponents)