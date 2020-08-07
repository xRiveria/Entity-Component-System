#pragma once
#include "Component.h"

///==== Component Masks ====

///The core idea is that the world is always aware of which components a given entity has at any given time.
///Note that this implementation assumes a maximum of one component of each type on a given entity. Why would there be 2 movement components on a player?
///The world knows which components each system cares about as well, and keeps a "mask" which represents which components are currently on an entity.
///When the mask changes, the entity mask is checked against the system mask to see if the system should be processing that entity.
///This image may help to clear things up: https://miro.medium.com/max/500/1*h-BfPTXh9btUGp4GGtsr_Q.png
///In the above example, a system like the Movement system needs to act on entities that have Transform and Motion. Therefore, the system mask would be 110000. 
///Similarly, the system that deals with player input might need Motion and Joystick, in which case the system mask would be 010010.

namespace EntitySystem
{
    struct ComponentMask
    {
        unsigned int mask = 0;  //The mask itself.

        template <typename ComponentType>
        void AddComponent()
        {
            mask |= (1 << GetComponentFamily<ComponentType>()); //Bitwise exclusive/inclusive OR assignment.
        }

        template <typename ComponentType>
        void RemoveComponent()
        {
            mask &= ~(1 << GetComponentFamily<ComponentType>());
        }

        //Returns true if the system is now matched, but didn't used to be (based on "oldMask")
        bool IsNewMatch(ComponentMask oldMask, ComponentMask systemMask);

        //Returns true if the system is not matched, but used to be matched.
        bool IsNoLongerMatched(ComponentMask oldMask, ComponentMask systemMask);

        bool Matches(ComponentMask systemMask);
    };
}