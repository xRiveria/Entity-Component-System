#pragma once
#include "Component.h"

/*
 * ComponentMasks are used by systems as a way of denoting which components an
 * entity needs for the system to run on it. For example, a system that updates
 * position based on velocity might have a mask that included both the Position
 * component and the Velocity component.
 *
 * For a more detailed explanation:
 * https://medium.com/@savas/nomad-game-engine-part-6-the-world-303d305f55cb
 */


