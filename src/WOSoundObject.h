/* 
* @file SoundObject.h
* @author Tatiana Lipert
* @date 2024-02-20
* @brief class to overload the setPosition method so that a WO and irrKland sound 
* exist in the same location and move together 
* 
*/

#pragma once
#include <irrKlang.h>
#include "WO.h" 

using namespace irrklang;
using namespace Aftr; 

class WOSoundObject : public WO
{
public:
	WOMacroDeclaration(WOSoundObject, WO);  //WO macro 

	//use this to instantiate a WOSoundObject
	static WOSoundObject* New(const std::string& modelFileName, Vector scale, 
		MESH_SHADING_TYPE shadingType, ISound* sound); 

	virtual ~WOSoundObject() = default;

	/*
	* override the setPosition function so the sound emitter and WO stay in the same location
	*/
	virtual void setPosition(const Vector& position) override;
	
	/* 
	* copies of minimal irrKlang ISound capabilities for manipulating the sound object 
	* may be later updated to include more functions 
	*/
	ISoundSource* getSoundSource();
	void setIsPaused(bool paused = true);
	bool getIsPaused();

	void stop();
	ik_f32 getVolume();
	void setVolume(ik_f32 volume);
	void setMinDistance(ik_f32 min);
	void setMaxDistance(ik_f32 max);

protected:
	WOSoundObject(); 
	ISound* sound = nullptr; 

	virtual void onCreate(const std::string& modelFileName, Vector scale, 
				MESH_SHADING_TYPE shadingType, ISound* sound);
};