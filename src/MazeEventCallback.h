/* 
* @file "MazeEventCallback.h" 
* @author Tatiana Lipert 
* @date 2024-07-07
* @brief inheritted PxSimulationEventCallback for onContact() event callback 
*/

#pragma once 

#include "GLView.h"
#include <irrKlang.h>
#include "PxPhysicsAPI.h" 

using namespace physx; 
using namespace Aftr; 
using namespace irrklang; 

class MazeEventCallback : public PxSimulationEventCallback
{
public:
	//constructor defined to take member variable of a sound effect 
	//make sure that the sound is defined BEFORE passing it to the event callback 
	//MazeEventCallback(ISound* sound); 

	//this is the only funciton that we care about
	void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs);

	//unused simulation event calback functions (disable them) 
	void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) { PX_UNUSED(constraints); PX_UNUSED(count); }
	void onWake(PxActor** actors, PxU32 count) { PX_UNUSED(actors); PX_UNUSED(count); }
	void onSleep(PxActor** actors, PxU32 count) { PX_UNUSED(actors); PX_UNUSED(count); }
	void onTrigger(PxTriggerPair* pairs, PxU32 count) { PX_UNUSED(pairs); PX_UNUSED(count); }
	void onAdvance(const PxRigidBody* const*, const PxTransform*, const PxU32) {}

protected: 
	ISound* soundEffect = nullptr; 
	
};
