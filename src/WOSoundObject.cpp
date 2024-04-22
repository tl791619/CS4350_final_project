/*
* @file SoundObject.cpp
* @author Tatiana Lipert
* @date 2024-02-20
* @brief implementation of the WOSoundObject class 
*
*/

#include "WOSoundObject.h"
#include "irrKlang.h"


WOMacroDefinition(WOSoundObject, WO);
 
WOSoundObject* WOSoundObject::New(const std::string& modelFileName, Vector scale, 
	MESH_SHADING_TYPE shadingType, ISound* sound) 
{
	WOSoundObject* wo = new WOSoundObject();
	wo->onCreate(modelFileName, scale, shadingType, sound); 
	return wo; 
}

WOSoundObject::WOSoundObject() : WO(), IFace(this) {
	//nothing else to do here (for now)
}

void WOSoundObject::onCreate(const std::string& modelFileName, Vector scale,
	MESH_SHADING_TYPE shadingType, ISound* sound) 
{
	WO::onCreate(modelFileName, scale, shadingType);
	this->sound = sound;
}

void WOSoundObject::setPosition(const Vector& position) {
	WO::setPosition(position);
	sound->setPosition(vec3df(position.y, position.x, position.z));
}

//ISound function copies 
ISoundSource* WOSoundObject::getSoundSource() {
	return sound->getSoundSource();
}

void WOSoundObject::setIsPaused(bool paused) {
	sound->setIsPaused(paused);
}

bool WOSoundObject::getIsPaused() {
		return sound->getIsPaused();
}



void WOSoundObject::stop() {
	sound->stop();
}

ik_f32 WOSoundObject::getVolume() {
	return sound->getVolume(); 
}

void WOSoundObject::setVolume(ik_f32 volume) {
	sound->setVolume(volume);
}

void WOSoundObject::setMinDistance(ik_f32 min) {
	sound->setMinDistance(min);
}

void WOSoundObject::setMaxDistance(ik_f32 max) {
	sound->setMaxDistance(max);
}
