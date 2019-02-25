#include "Sound.h"

SoundManager* SoundManager::instance = 0;

SoundManager::SoundManager()
{
	soundFile load;
	soundLoad("Sound/bgm.wav", "mpegvideo", "bgm", &load);
}

bool SoundManager::soundLoad(String filePath, String fileType, String fileName, soundFile* returnFile) // preloading sound files
{
	MCIERROR error;
	String sound;
	sound = "open \"" + filePath + "\" type " + fileType + " alias " + fileName; // giving the .wav files aliases
	error = mciSendString(sound.c_str(), NULL, 0, NULL);
	if (error == 0)
	{
		(*returnFile).status = 0;
		(*returnFile).filePath = filePath;
		(*returnFile).fileType = fileType;
		(*returnFile).fileName = fileName;
		/*returnFile->status = 0;
		returnFile->filePath = filePath;
		returnFile->fileType = fileType;
		returnFile->fileName = fileName;*/
		sound = " setaudio " + fileName + " volume to " + String("50"); // default volume is half (50)
		error = mciSendString(sound.c_str(), NULL, 0, NULL);
		return true;
	}
	else
	{
		(*returnFile).status = error;
		return false;
	}
}

bool SoundManager::playSound(String fileName)
{
	MCIERROR error;
	String sound = " setaudio " + fileName + " volume to " + volume;
	error = mciSendString(sound.c_str(), NULL, 0, NULL);
	sound = "play " + fileName + " from 0"; // from 0 so it doesnt have to wait for the audio to end 
	error = mciSendString(sound.c_str(), NULL, 0, NULL);
	
	return error ? false : true;
}

bool SoundManager::loopSound(String fileName)
{
	MCIERROR error;
	String sound = " setaudio " + fileName + " volume to " + volume;
	error = mciSendString(sound.c_str(), NULL, 0, NULL);
	sound = "play " + fileName + " repeat";
	error = mciSendString(sound.c_str(), NULL, 0, NULL);
	
	return error ? false : true;
}

bool SoundManager::stopSound(String fileName)
{
	MCIERROR error;
	String sound = " setaudio " + fileName + " volume to " + volume;
	error = mciSendString(sound.c_str(), NULL, 0, NULL);
	sound = "stop " + fileName;
	error = mciSendString(sound.c_str(), NULL, 0, NULL);
	
	return error ? false : true;
}

SoundManager::~SoundManager()
{
}

SoundManager * SoundManager::GetInstance()
{
	if (instance == nullptr)
		instance = new SoundManager();
	return instance;
}

void SoundManager::setVolume(String val)
{
	volume = val;
}

