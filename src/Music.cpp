#include "Music.h"

int rainChannel = -1;

string Music::parseNoun(vector<string> options, ScreenState * state)
{
	if (options.size() > 0) {
		if (options[0] == "set" && options.size() > 2) {
			if (options[1] == "music") {
				SoundManager::setEventParam("event:/Music/MusicTrack", "Intensity", stof(options[2]));
				return "Setting music intensity to " + options[2];
			}
			else if (options[1] == "rain" && rainChannel != -1) {
				SoundManager::setChannelVolume(rainChannel, stof(options[2]));
				return "Setting rain volume to " + options[2] + "dB";
			}
		}
		else if (options[0] == "play") {
			if (options[1] == "music" && SoundManager::isEventPlaying("event:/Music/MusicTrack") == false) {
				SoundManager::playEvent("event:/Music/MusicTrack");
				return "Playing music";
			}
			else if (options[1] == "rain") {
				rainChannel = SoundManager::playSound(0);
				cout << "Rain channel " << rainChannel << endl;
				return "Playing rain";
			}
		}
		else if (options[0] == "stop") {
			if (options[1] == "music") {
				SoundManager::stopEvent("event:/Music/MusicTrack");
				return "Stopping music";
			}
			else if (options[1] == "rain" && rainChannel != -1) {
                if (options.size() > 2){
                    SoundManager::stopChannel(rainChannel, stoi(options[2]));
                } else{
                    SoundManager::stopChannel(rainChannel);
                }
				rainChannel = -1;
				return "Stopping rain";
			}
		}
	}

	//TODO: Figure out how to allow commands to send multiple lines to screen instead of just one.
	return "The *smusic*e commands takes arguments that include: music set music *intensity*, music set rain *volume*, music play *rain/music*, music stop *rain/music* ";
}
