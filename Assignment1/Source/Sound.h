#pragma once
#include <Windows.h>
#include <vector>
#include <string>
typedef std::string String;

// playing sound using Media Control Interface(MCI)
struct soundFile
{
	String filePath;
	String fileType;
	String fileName;
	int status;
	soundFile()
		: filePath(""), fileType(""), fileName(""), status(0) {}
};

typedef std::vector<soundFile> SF_Vector;

class SoundManager {
public:
	static SoundManager* GetInstance();
	bool playSound(String fileName); // play sound effects
	bool loopSound(String fileName); // loop sound
	bool stopSound(String fileName); // Stop playing sound
	void setVolume(String val);
private:
	SoundManager();
	bool soundLoad(String filePath, String fileType, String fileName, soundFile* returnFile);
	~SoundManager();
	SF_Vector Files;
	String volume;
	static SoundManager *instance;
};