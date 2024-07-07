/*
* @file "MazeEventCallback.cpp"
* @author Tatiana Lipert
* @date 2024-07-07
* @brief inheritted PxSimulationEventCallback for onContact() event callback
*/

#include "MazeEventCallback.h"
#include <vector>

using namespace irrklang; 
using namespace std; 


MazeEventCallback::MazeEventCallback(ISoundEngine* engine, ISoundSource* source) {
	soundEngine = engine; 
	soundEffect = source; 
}

void MazeEventCallback::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
{
	PX_UNUSED((pairHeader));
	vector<PxContactPairPoint> contactPoints;

	for (PxU32 i = 0; i < nbPairs; i++) 
	{
		PxU32 contactCount = pairs[i].contactCount; 
		if (contactCount > 0) 
		{
			contactPoints.resize(contactCount); 
			pairs[i].extractContacts(&contactPoints[0], contactCount);

			for (PxU32 j = 0; j < contactCount; j++) 
			{
				//cout << "CONTACT DETECTED" << endl;

				//play a 3D sound from the collision contact point 

				PxVec3 position = contactPoints[j].position;
				soundEngine->play3D(soundEffect, vec3df(position.z, position.x, position.y));
				
			}
		}
	}
}