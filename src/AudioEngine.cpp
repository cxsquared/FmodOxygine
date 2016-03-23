// Based on the GDC talks from Guy Somberg
// Check them out on the GDC vault if you get the chance

#include "AudioEngine.h"


/*****************
* Implementation *
******************/
Implementation::Implementation() {
	mpStudioSystem = NULL;
	CAudioEngine::ErrorCheck(FMOD::Studio::System::create(&mpStudioSystem));
	CAudioEngine::ErrorCheck(mpStudioSystem->initialize(32, FMOD_STUDIO_INIT_LIVEUPDATE, FMOD_INIT_PROFILE_ENABLE, NULL));

	mpSystem = NULL;
	CAudioEngine::ErrorCheck(mpStudioSystem->getLowLevelSystem(&mpSystem));
}

Implementation::~Implementation() {
	CAudioEngine::ErrorCheck(mpStudioSystem->unloadAll());
	CAudioEngine::ErrorCheck(mpStudioSystem->release());
}

void Implementation::Update(float fTimeDeltaSeconds) {
	vector<ChannelMap::iterator> pStoppedChannels;
	for (auto it = mChannels.begin(), itEnd = mChannels.end(); it != itEnd; ++it)
	{
		it->second->Update(fTimeDeltaSeconds);
		if (it->second->meState == Channel::State::STOPPED)
		{
			pStoppedChannels.push_back(it);
		}
	}
	for (auto& it : pStoppedChannels)
	{
		mChannels.erase(it);
	}
	CAudioEngine::ErrorCheck(mpStudioSystem->update());
}

void Implementation::LoadSound(int nSoundId) {
	if (SoundIsLoaded(nSoundId)) {
		return;
	}

	auto tFoundIt = mSoundDefinitions.find(nSoundId);
	if (tFoundIt == mSoundDefinitions.end())
		return;

	FMOD_MODE eMode = FMOD_NONBLOCKING;
	eMode |= tFoundIt->second.bIs3d ? (FMOD_3D | FMOD_3D_INVERSETAPEREDROLLOFF) : FMOD_2D;
	eMode |= tFoundIt->second.bIsLooping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
	eMode |= tFoundIt->second.bIsStreaming ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;

	CAudioEngine::ErrorCheck(mpSystem->createSound(tFoundIt->second.mSoundName.c_str(), eMode, nullptr, &mSounds[nSoundId]));
	if (mSounds[nSoundId]) {
		mSounds[nSoundId]->set3DMinMaxDistance(tFoundIt->second.fMinDistance, tFoundIt->second.fMaxDistance);
	}
}

void Implementation::UnLoadSound(int nSoundId) {
    auto tSoundIt = mSounds.find(nSoundId);
    if (tSoundIt == mSounds.end())
        return;
    CAudioEngine::ErrorCheck(tSoundIt->second->release());
    mSounds.erase(tSoundIt);
    
    auto tFoundIt = mSoundDefinitions.find(nSoundId);
    if (tFoundIt == mSoundDefinitions.end())
        return;
    mSoundDefinitions.erase(tFoundIt);
}

bool Implementation::SoundIsLoaded(int nSoundId) {
	auto tFoundIt = mSounds.find(nSoundId);
    if (tFoundIt == mSounds.end()) {
        cout << "Loading sound not found " << mSoundDefinitions[nSoundId].mSoundName << endl;
		return false;
    }

	// Checking the state of the FMOD sound to see if it's loaded
	FMOD_OPENSTATE state;
	CAudioEngine::ErrorCheck(tFoundIt->second->getOpenState(&state, 0, 0, 0));
    cout << "Fmod loading state = " << state << endl;
	if (state == FMOD_OPENSTATE_READY)
		return true;

	return false;
}

Implementation* sgpImplementation = nullptr;

/****************
* Channel State *
*****************/

Implementation::Channel::Channel(Implementation& tImplemenation, int nSoundId, const SoundDefinition& tSoundDefinition, const FmodVector3& vPosition, float vVolumedB): mImplemenation(tImplemenation) {
	mSoundId = nSoundId;
	
	// Getting info from Sound Definition
	mfVolumedB = vVolumedB;
	mfSoundVolume = CAudioEngine::dbToVolume(vVolumedB);

	mvPosition = vPosition;
}

void Implementation::Channel::Update(float fTimeDeltaSeconds) {
	switch (meState)
	{
	case Implementation::Channel::State::INITIALIZE:
	case Implementation::Channel::State::DEVIRTUALIZE:
	case Implementation::Channel::State::TOPLAY: {
        cout << "Starting to play..." << mImplemenation.mSoundDefinitions[mSoundId].mSoundName << endl;
		if (mbStopRequested) {
			meState = State::STOPPING;
			return;
		}

		if (ShouldBeVirtual(true)) {
			if (IsOneShot()) {
				meState = State::STOPPING;
			}
			else {
				meState = State::VIRTUAL;
			}
			return;
		}

		if (!mImplemenation.SoundIsLoaded(mSoundId)) {
			mImplemenation.LoadSound(mSoundId);
			meState = State::LOADING;
			return;
		}

		mpChannel = nullptr;

		auto tSoundIt = mImplemenation.mSounds.find(mSoundId);
		if (tSoundIt != mImplemenation.mSounds.end()) {
			mImplemenation.mpSystem->playSound(tSoundIt->second, nullptr, true, &mpChannel);
		}

		if (mpChannel) {
            cout << "Playing state success " << mSoundId << endl;
			if (meState == State::DEVIRTUALIZE) {
				mVirtualizedFader.StartFade(mfVolumedB, CAudioFader::SILENCE_dB, CAudioFader::VIRTUALIZE_FADE_TIME);
			}

			meState = State::PLAYING;

			FMOD_VECTOR position = CAudioEngine::VectorToFmod(mvPosition);
			mpChannel->set3DAttributes(&position, nullptr);
			mpChannel->setVolume(CAudioEngine::dbToVolume(GetVolumedB()));
			mpChannel->setPaused(false);
		}
		else {
            cout << "Tried to play sound " << mSoundId << " but playSound didn't work." << endl;
			meState = State::STOPPING;
		}
	}
		break;
	case Implementation::Channel::State::LOADING:
        cout << "Loading sound..." << mImplemenation.mSoundDefinitions[mSoundId].mSoundName << endl;
		if (mImplemenation.SoundIsLoaded(mSoundId)) {
			meState = State::TOPLAY;
		}
		break;
	case Implementation::Channel::State::PLAYING:
        cout << "Playing..." << mImplemenation.mSoundDefinitions[mSoundId].mSoundName << endl;
		mVirtualizedFader.Update(fTimeDeltaSeconds);
		UpdateChannelParameters();

		if (!IsPlaying() || mbStopRequested) {
			meState = State::STOPPING;
			return;
		}

		if (ShouldBeVirtual(false)) {
			mVirtualizedFader.StartFade(mfVolumedB, CAudioFader::SILENCE_dB, CAudioFader::VIRTUALIZE_FADE_TIME);
			meState = State::VIRTUALIZING;
		}
		break;
	case Implementation::Channel::State::STOPPING:
        cout << "Stopping..." << mImplemenation.mSoundDefinitions[mSoundId].mSoundName << endl;
		mStopFader.Update(fTimeDeltaSeconds);
		UpdateChannelParameters();
		if (mStopFader.IsFinished()) {
            
			mpChannel->stop();
		}
		if (!IsPlaying()) {
			meState = State::STOPPED;
			return;
		}
		break;
	case Implementation::Channel::State::STOPPED:
        cout << "Stopped..." << mImplemenation.mSoundDefinitions[mSoundId].mSoundName << endl;
		break;
	case Implementation::Channel::State::VIRTUALIZING:
        cout << "Virtualizing..." << mImplemenation.mSoundDefinitions[mSoundId].mSoundName << endl;
		mVirtualizedFader.Update(fTimeDeltaSeconds);
		UpdateChannelParameters();
		if (!ShouldBeVirtual(false)) {
			mVirtualizedFader.StartFade(mfVolumedB, CAudioFader::SILENCE_dB, CAudioFader::VIRTUALIZE_FADE_TIME);
			meState = State::PLAYING;
			break;
		}
		if (mVirtualizedFader.IsFinished()) {
			mpChannel->stop();
			meState = State::VIRTUAL;
		}
		break;
	case Implementation::Channel::State::VIRTUAL:
        cout << "Sound is virtual..." << mImplemenation.mSoundDefinitions[mSoundId].mSoundName << endl;
		if (mbStopRequested) {
			meState = State::STOPPING;
		}
		else if (!ShouldBeVirtual(false)) {
			meState = State::DEVIRTUALIZE;
		}
		break;
	}
}

void Implementation::Channel::UpdateChannelParameters() {
	// Only update if the channel is active
	if (mpChannel) {
        //Faders
        if (mVirtualizedFader.IsRunning()) {
            mfVolumedB = mVirtualizedFader.GetCurrentVolume();
        }
        if (mStopFader.IsRunning()) {
            mfVolumedB = mStopFader.GetCurrentVolume();
        }
		FMOD_VECTOR vVel;
		vVel.x = 0;
		vVel.y = 0;
		vVel.z = 0;
        FMOD_VECTOR vPos = CAudioEngine::VectorToFmod(mvPosition);
		mpChannel->set3DAttributes(&vPos, &vVel);
		mpChannel->setVolume(CAudioEngine::dbToVolume(mfVolumedB));
	}
}

bool Implementation::Channel::ShouldBeVirtual(bool bAllowOneShotVirtuals) const {
	// TODO: Write code to check if things should be virtualized
	return false;
}

bool Implementation::Channel::IsPlaying() const {
    bool bIsPlaying;
    mpChannel->isPlaying(&bIsPlaying);

	return bIsPlaying;
}

bool Implementation::Channel::IsOneShot() const {
	auto tSoundIt = mImplemenation.mSoundDefinitions.find(mSoundId);
	if (tSoundIt == mImplemenation.mSoundDefinitions.end())
		return NULL; // TODO: Return actual error

	return !tSoundIt->second.bIsLooping;
}

float Implementation::Channel::GetVolumedB() const {
	return mfVolumedB;
}

/**************
* Audio Fader *
***************/
void CAudioFader::StartFade(float fStartVolumedB, float fEndVolumedB, float fSeconds) {
	mfSecondsLeft = fSeconds;
	mfStartTime = fSeconds;
	mfStartVolumedB = fStartVolumedB;
	mfTargetVolumedB = fEndVolumedB;

	mbRunning = true;
    mbStarted = true;
    
    cout << "Starting fade " << fSeconds << " start volume " << fStartVolumedB << " end volume " << fEndVolumedB << endl;
}

bool CAudioFader::IsFinished() {
    if (mbStarted && !mbRunning) {
        mbStarted = false;
        return true;
    }
    
	return false;
}

void CAudioFader::Update(float fTimeDeltaSeconds) {
	if (mbRunning) {
		mfSecondsLeft -= fTimeDeltaSeconds;
        cout << "Fade seconds left " << mfSecondsLeft << " delta time " << fTimeDeltaSeconds << " current volume " << GetCurrentVolume() << endl;

		if (mfSecondsLeft < 0) {
			mbRunning = false;
		}
	}
}

float CAudioFader::GetCurrentVolume() {
	return lerp(mfSecondsLeft / mfStartTime, mfStartVolumedB, mfTargetVolumedB);
}

float CAudioFader::lerp(float fTime, float fEnd, float fStart) {
	return (1 - fTime)*fStart + fTime*fEnd;
}

bool CAudioFader::IsRunning() {
    return mbRunning;
}

/***************
* Audio Engine *
****************/
void CAudioEngine::Init() {
	sgpImplementation = new Implementation;
}

void CAudioEngine::Update(float fTimeDeltaSeconds) {
	sgpImplementation->Update(fTimeDeltaSeconds);
}

int CAudioEngine::RegisterSound(const SoundDefinition &tSoundDefinition, int nSoundId, bool bLoad) {
    // Check if sound id is already used
    auto tSoundIt = sgpImplementation->mSoundDefinitions.find(nSoundId);
    if (tSoundIt != sgpImplementation->mSoundDefinitions.end())
        return -1;
    
    // Set Sound definition
    sgpImplementation->mSoundDefinitions[nSoundId] = tSoundDefinition;
    
    if (bLoad){
        // Load sound
        sgpImplementation->LoadSound(nSoundId);
    }
    
    return nSoundId;
}

int CAudioEngine::PlaySound(int nSoundId, const FmodVector3& vPos, float fVolumedB)
{
	int nChannelId = sgpImplementation->mnNextChannelId++;
	auto tFoundIt = sgpImplementation->mSoundDefinitions.find(nSoundId);
	if (tFoundIt == sgpImplementation->mSoundDefinitions.end())
	{
		return nChannelId;
	}
    
        cout << "Playing sound " << tFoundIt->second.mSoundName << endl;
	
    sgpImplementation->mChannels[nChannelId] = std::unique_ptr<Implementation::Channel>(new Implementation::Channel(*sgpImplementation, nSoundId, tFoundIt->second, vPos, fVolumedB));

	return nChannelId;
}

void CAudioEngine::StopChannel(int nChannelId, float fFadeTimeSeconds) {
    auto tFoundIt = sgpImplementation->mChannels.find(nChannelId);
    if (tFoundIt == sgpImplementation->mChannels.end())
        return;
    
	if (fFadeTimeSeconds <= 0.0f) {
        cout << "Hard stopping sound " << endl;
		CAudioEngine::ErrorCheck(tFoundIt->second->mpChannel->stop());
	}
	else {
        cout << "Starting fade out " << endl;
		tFoundIt->second->mbStopRequested = true;
		tFoundIt->second->mStopFader.StartFade(tFoundIt->second->mfVolumedB, CAudioFader::SILENCE_dB, fFadeTimeSeconds);
	}
}

void CAudioEngine::SetChannel3dPosition(int nChannelId, const FmodVector3& vPosition)
{
	auto tFoundIt = sgpImplementation->mChannels.find(nChannelId);
	if (tFoundIt == sgpImplementation->mChannels.end())
		return;

	tFoundIt->second->mvPosition = vPosition;
}

void CAudioEngine::SetChannelVolume(int nChannelId, float fVolumedB)
{
	auto tFoundIt = sgpImplementation->mChannels.find(nChannelId);
	if (tFoundIt == sgpImplementation->mChannels.end())
		return;

	tFoundIt->second->mfVolumedB = fVolumedB;
}

void CAudioEngine::LoadBank(const std::string& strBankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags) {
	auto tFoundIt = sgpImplementation->mBanks.find(strBankName);
	if (tFoundIt != sgpImplementation->mBanks.end())
		return;
	FMOD::Studio::Bank* pBank;
	CAudioEngine::ErrorCheck(sgpImplementation->mpStudioSystem->loadBankFile(strBankName.c_str(), flags, &pBank));
	if (pBank) {
		sgpImplementation->mBanks[strBankName] = pBank;
	}
}

void CAudioEngine::LoadEvent(const std::string& strEventName) {
	auto tFoundit = sgpImplementation->mEvents.find(strEventName);
	if (tFoundit != sgpImplementation->mEvents.end())
		return;
	FMOD::Studio::EventDescription* pEventDescription = NULL;
	CAudioEngine::ErrorCheck(sgpImplementation->mpStudioSystem->getEvent(strEventName.c_str(), &pEventDescription));
	if (pEventDescription){
		FMOD::Studio::EventInstance* pEventInstance = NULL;
		CAudioEngine::ErrorCheck(pEventDescription->createInstance(&pEventInstance));
		if (pEventInstance){
			sgpImplementation->mEvents[strEventName] = pEventInstance;
		}
	}	
}

void CAudioEngine::PlayEvent(const string &strEventName) {
	auto tFoundit = sgpImplementation->mEvents.find(strEventName);
	if (tFoundit == sgpImplementation->mEvents.end()){
		LoadEvent(strEventName);
		tFoundit = sgpImplementation->mEvents.find(strEventName);
		if (tFoundit == sgpImplementation->mEvents.end())
			return;
	}
	tFoundit->second->start();
    //cout << "Playing event " << strEventName << endl;
}

void CAudioEngine::StopEvent(const string &strEventName, bool bImmediate) {
	auto tFoundIt = sgpImplementation->mEvents.find(strEventName);
	if (tFoundIt == sgpImplementation->mEvents.end())
		return;
	FMOD_STUDIO_STOP_MODE eMode;
	eMode = bImmediate ? FMOD_STUDIO_STOP_IMMEDIATE : FMOD_STUDIO_STOP_ALLOWFADEOUT;
	CAudioEngine::ErrorCheck(tFoundIt->second->stop(eMode));
}

bool CAudioEngine::IsEventPlaying(const string &strEventName) const {
	auto tFoundIt = sgpImplementation->mEvents.find(strEventName);
	if (tFoundIt == sgpImplementation->mEvents.end())
		return false;

	FMOD_STUDIO_PLAYBACK_STATE* state = NULL;
	if (tFoundIt->second->getPlaybackState(state) == FMOD_STUDIO_PLAYBACK_PLAYING) {
		return true;
	}
	return false;
}

void CAudioEngine::GetEventParameter(const string &strEventName, const string &strParameterName, float* parameter) {
	auto tFoundIt = sgpImplementation->mEvents.find(strEventName);
	if (tFoundIt == sgpImplementation->mEvents.end())
		return;
	FMOD::Studio::ParameterInstance* pParameter = NULL;
	CAudioEngine::ErrorCheck(tFoundIt->second->getParameter(strParameterName.c_str(), &pParameter));
	CAudioEngine::ErrorCheck(pParameter->getValue(parameter));
}

void CAudioEngine::SetEventParameter(const string &strEventName, const string &strParameterName, float fValue) {
	auto tFoundIt = sgpImplementation->mEvents.find(strEventName);
	if (tFoundIt == sgpImplementation->mEvents.end())
		return;
	FMOD::Studio::ParameterInstance* pParameter = NULL;
	CAudioEngine::ErrorCheck(tFoundIt->second->getParameter(strParameterName.c_str(), &pParameter));
	CAudioEngine::ErrorCheck(pParameter->setValue(fValue));
    //cout << "Event " << strEventName << " parameter " << strParameterName << " set to " << fValue << endl;
}

FMOD_VECTOR CAudioEngine::VectorToFmod(const FmodVector3& vPosition){
	FMOD_VECTOR fVec;
	fVec.x = vPosition.x;
	fVec.y = vPosition.y;
	fVec.z = vPosition.z;
	return fVec;
}

int CAudioEngine::ErrorCheck(FMOD_RESULT result) {
	if (result != FMOD_OK){
		cout << "FMOD ERROR " << result << endl;
		return 1;
	}
	// cout << "FMOD all good" << endl;
	return 0;
}

float  CAudioEngine::dbToVolume(float dB)
{
	return powf(10.0f, 0.05f * dB);
}

float  CAudioEngine::VolumeTodB(float volume)
{
	return 20.0f * log10f(volume);
}

void CAudioEngine::Shutdown() {
	delete sgpImplementation;
}