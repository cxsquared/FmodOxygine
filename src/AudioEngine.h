#ifndef _AUDIO_ENGINE_H_
#define _AUDIO_ENGINE_H_

#include "fmod_studio.hpp"
#include "fmod.hpp"
#include <string>
#include <map>
#include <vector>
#include <math.h>
#include <iostream>
#include <memory>

using namespace std;

struct FmodVector3 {
	float x;
	float y;
	float z;
};

struct SoundDefinition {
    string mSoundName;
    float fDefaultVolumedB;
    float fMinDistance;
    float fMaxDistance;
    bool bIs3d;
    bool bIsLooping;
    bool bIsStreaming;
};

class CAudioFader {
public:
    static constexpr float SILENCE_dB = -40.0f;
    static constexpr float VIRTUALIZE_FADE_TIME = 0.125f;
    
    void StartFade(float fStartVolumedB, float fEndVolumedB, float fSeconds);
    bool IsFinished();
    void Update(float fTimeDeltaSeconds);
    
    float GetCurrentVolume();
private:
    float mfSecondsLeft;
    float mfStartTime;
    float mfStartVolumedB;
    float mfTargetVolumedB;
    bool mbRunning;
    float lerp(float fTime, float fStart, float fEnd);
};

struct Implementation {
	Implementation();
	~Implementation();

	void Update(float fTimeDeltaSeconds);
	void LoadSound(int nSoundId);
    void UnLoadSound(int nSoundId);
	bool SoundIsLoaded(int nSoundId);
	
	FMOD::Studio::System* mpStudioSystem;
	FMOD::System* mpSystem;

	struct Channel {
        Channel(Implementation& tImplemenation, int nSoundId, const SoundDefinition& sSoundDefinition, const FmodVector3& vPosition, float vVolumedB);

		enum class State {
			INITIALIZE,
			TOPLAY,
			LOADING,
			STOPPING,
			STOPPED,
			VIRTUALIZING,
			VIRTUAL,
			DEVIRTUALIZE,
			PLAYING
		};

		Implementation& mImplemenation;
		FMOD::Channel* mpChannel = nullptr;
		int mSoundId;
		FmodVector3 mvPosition;
		float mfVolumedB = 0.0f;
		float mfSoundVolume = 0.0f;
		State meState = State::INITIALIZE;
		bool mbStopRequested = false;
		CAudioFader mStopFader;
		CAudioFader mVirtualizedFader;

		void Update(float fTimeDeltaSeconds);
		void UpdateChannelParameters();
		bool ShouldBeVirtual(bool bAllowOneShotVirtuals) const;
		bool IsPlaying() const;
		bool IsOneShot() const;
		float GetVolumedB() const;
	};

	struct Sound {
		SoundDefinition& mSoundDefinition;
		FMOD::Sound* mpSound;
	};

	unsigned int mnNextChannelId = 0;

	typedef map<int, SoundDefinition> SoundDefinitionMap;
    typedef map<int, FMOD::Sound*> SoundMap;
	typedef map<int, unique_ptr<Implementation::Channel>> ChannelMap;
	typedef map<string, FMOD::Studio::EventInstance*> EventMap;
	typedef map<string, FMOD::Studio::Bank*> BankMap;
	BankMap mBanks;
	EventMap mEvents;
	SoundMap mSounds;
    SoundDefinitionMap mSoundDefinitions;
	ChannelMap mChannels;
};

class CAudioEngine {
public:
	static void Init();
	static void Update(float fTimeDeltaSeconds);
	static void Shutdown();
	static int ErrorCheck(FMOD_RESULT result);

	// Low Level Sounds
	int RegisterSound(const SoundDefinition& tSoundDefinition, int nSoundId, bool bLoad = true);
	void UnregisterSound(int nSoundId);
	void Set3dListenerAndOrientation(const FmodVector3& vPosition, const FmodVector3& vLook, const FmodVector3& vUp);
	int PlaySound(int nSoundId, const FmodVector3& vPos = FmodVector3{ 0, 0, 0 }, float fVolumedB = 0.0f);
	void StopChannel(int nChannelId, float fFadeTimeSeconds = 0.0f);
	void StopAllChannels();
	void SetChannel3dPosition(int nChannelId, const FmodVector3& vPosition);
	void SetChannelVolume(int nChannelId, float fVolumedB);
	bool IsPlaying(int nChannelId) const;

	// Studio Event Sounds
	void LoadBank(const std::string& strBankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags);
	void LoadEvent(const std::string& strEventName);
	void PlayEvent(const string &strEventName);
	void StopEvent(const string &strEventName, bool bImmediate = false);
	void GetEventParameter(const string &strEventName, const string &strEventParameter, float* parameter);
	void SetEventParameter(const string &strEventName, const string &strParameterName, float fValue);
	bool IsEventPlaying(const string &strEventName) const;

	// Helpers
	static float dbToVolume(float dB);
	static float VolumeTodB(float volume);
	static FMOD_VECTOR VectorToFmod(const FmodVector3& vPosition);
};

#endif