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


//MazeEventCallback::MazeEventCallback(ISound* sound) {
//	soundEffect = sound; 
//}

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
				//PxVec3 position = contactPoints[j].position;
				//soundEffect->setPosition(vec3df(position.y, position.x, position.z)); 

				cout << "CONTACT DETECTED" << endl; 

				//contactPoints[j].impulse;
				
			}
		}
	}
}