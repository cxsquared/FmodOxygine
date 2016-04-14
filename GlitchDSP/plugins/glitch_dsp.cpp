#ifdef WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "fmod.hpp"
#include <random>
#include <ctime>

#define FMOD_GAIN_USEPROCESSCALLBACK            /* FMOD plugins have 2 methods of processing data.  
1. via a 'read' callback which is compatible with FMOD Ex but limited in functionality, or
2. via a 'process' callback which exposes more functionality, like masks and query before process early out logic. */

extern "C" {
	F_DECLSPEC F_DLLEXPORT FMOD_DSP_DESCRIPTION* F_STDCALL FMODGetDSPDescription();
}

const float CX2_SOUNDTRACK_PARAM_TIME_MIN = 0.01f;
const float CX2_SOUNDTRACK_PARAM_TIME_MAX = 2.0f;
const float CX2_SOUNDTRACK_PARAM_TIME_DEFAULT = 1.0f;

const float CX2_SOUNDTRACK_PARAM_FEEDBACK_MIN = 0;
const float CX2_SOUNDTRACK_PARAM_FEEDBACK_MAX = 100;
const float CX2_SOUNDTRACK_PARAM_FEEDBACK_DEFAULT = 50;

const float CX2_SOUNDTRACK_PARAM_RESONANCE_MIN = 0;
const float CX2_SOUNDTRACK_PARAM_RESONANCE_MAX = 100;
const float CX2_SOUNDTRACK_PARAM_RESONANCE_DEFAULT = 50;

enum
{
	CX2_SOUNDTRACK_PARAM_TIME = 0,
    CX2_SOUNDTRACK_PARAM_FEEDBACK = 1,
    CX2_SOUNDTRACK_PARAM_RESONANCE = 2,
	CX2_SOUNDTRACK_NUM_PARAMETERS
};

#define DECIBELS_TO_LINEAR(__dbval__)  ((__dbval__ <= FMOD_GAIN_PARAM_GAIN_MIN) ? 0.0f : powf(10.0f, __dbval__ / 20.0f))
#define LINEAR_TO_DECIBELS(__linval__) ((__linval__ <= 0.0f) ? FMOD_GAIN_PARAM_GAIN_MIN : 20.0f * log10f((float)__linval__))

FMOD_RESULT F_CALLBACK FMOD_Gain_dspcreate(FMOD_DSP_STATE *dsp_state);
FMOD_RESULT F_CALLBACK FMOD_Gain_dsprelease(FMOD_DSP_STATE *dsp_state);
FMOD_RESULT F_CALLBACK FMOD_Gain_dspreset(FMOD_DSP_STATE *dsp_state);
#ifdef FMOD_GAIN_USEPROCESSCALLBACK
FMOD_RESULT F_CALLBACK FMOD_Gain_dspprocess(FMOD_DSP_STATE *dsp_state, unsigned int length, const FMOD_DSP_BUFFER_ARRAY *inbufferarray, FMOD_DSP_BUFFER_ARRAY *outbufferarray, FMOD_BOOL inputsidle, FMOD_DSP_PROCESS_OPERATION op);
#else
FMOD_RESULT F_CALLBACK FMOD_Gain_dspread(FMOD_DSP_STATE *dsp_state, float *inbuffer, float *outbuffer, unsigned int length, int inchannels, int *outchannels);
#endif
FMOD_RESULT F_CALLBACK FMOD_Gain_dspsetparamfloat(FMOD_DSP_STATE *dsp_state, int index, float value);
FMOD_RESULT F_CALLBACK FMOD_Gain_dspsetparamint(FMOD_DSP_STATE *dsp_state, int index, int value);
FMOD_RESULT F_CALLBACK FMOD_Gain_dspsetparambool(FMOD_DSP_STATE *dsp_state, int index, FMOD_BOOL value);
FMOD_RESULT F_CALLBACK FMOD_Gain_dspsetparamdata(FMOD_DSP_STATE *dsp_state, int index, void *data, unsigned int length);
FMOD_RESULT F_CALLBACK FMOD_Gain_dspgetparamfloat(FMOD_DSP_STATE *dsp_state, int index, float *value, char *valuestr);
FMOD_RESULT F_CALLBACK FMOD_Gain_dspgetparamint(FMOD_DSP_STATE *dsp_state, int index, int *value, char *valuestr);
FMOD_RESULT F_CALLBACK FMOD_Gain_dspgetparambool(FMOD_DSP_STATE *dsp_state, int index, FMOD_BOOL *value, char *valuestr);
FMOD_RESULT F_CALLBACK FMOD_Gain_dspgetparamdata(FMOD_DSP_STATE *dsp_state, int index, void **value, unsigned int *length, char *valuestr);
FMOD_RESULT F_CALLBACK FMOD_Gain_shouldiprocess(FMOD_DSP_STATE *dsp_state, FMOD_BOOL inputsidle, unsigned int length, FMOD_CHANNELMASK inmask, int inchannels, FMOD_SPEAKERMODE speakermode);
FMOD_RESULT F_CALLBACK FMOD_Gain_sys_register(FMOD_DSP_STATE *dsp_state);
FMOD_RESULT F_CALLBACK FMOD_Gain_sys_deregister(FMOD_DSP_STATE *dsp_state);
FMOD_RESULT F_CALLBACK FMOD_Gain_sys_mix(FMOD_DSP_STATE *dsp_state, int stage);

static bool FMOD_Gain_Running = false;
static FMOD_DSP_PARAMETER_DESC p_time;
static FMOD_DSP_PARAMETER_DESC p_feedback;
static FMOD_DSP_PARAMETER_DESC p_resonance;

FMOD_DSP_PARAMETER_DESC *FMOD_Gain_dspparam[CX2_SOUNDTRACK_NUM_PARAMETERS] =
{
	&p_time,
    &p_feedback,
    &p_resonance
};

FMOD_DSP_DESCRIPTION FMOD_Gain_Desc =
{
	FMOD_PLUGIN_SDK_VERSION,
	"CX2 Glitch Soundtrack",    // name
	0x00000003,     // plug-in version
	1,              // number of input buffers to process
	1,              // number of output buffers to process
	FMOD_Gain_dspcreate,
	FMOD_Gain_dsprelease,
	FMOD_Gain_dspreset,
#ifndef FMOD_GAIN_USEPROCESSCALLBACK
	FMOD_Gain_dspread,
#else
	0,
#endif
#ifdef FMOD_GAIN_USEPROCESSCALLBACK
	FMOD_Gain_dspprocess,
#else
	0,
#endif
	0,
	CX2_SOUNDTRACK_NUM_PARAMETERS,
	FMOD_Gain_dspparam,
	FMOD_Gain_dspsetparamfloat,
	0, // FMOD_Gain_dspsetparamint,
	0, //FMOD_Gain_dspsetparambool,
	0, // FMOD_Gain_dspsetparamdata,
	FMOD_Gain_dspgetparamfloat,
	0, // FMOD_Gain_dspgetparamint,
	0, // FMOD_Gain_dspgetparambool,
	0, // FMOD_Gain_dspgetparamdata,
	FMOD_Gain_shouldiprocess,
	0,                                      // userdata
	FMOD_Gain_sys_register,
	FMOD_Gain_sys_deregister,
	FMOD_Gain_sys_mix
};

extern "C"
{

	F_DECLSPEC F_DLLEXPORT FMOD_DSP_DESCRIPTION* F_STDCALL FMODGetDSPDescription()
	{
		FMOD_DSP_INIT_PARAMDESC_FLOAT(p_time, "Time", "ms", "Time in milliseconds.", CX2_SOUNDTRACK_PARAM_TIME_MIN, CX2_SOUNDTRACK_PARAM_TIME_MAX, CX2_SOUNDTRACK_PARAM_TIME_DEFAULT);
        FMOD_DSP_INIT_PARAMDESC_FLOAT(p_feedback, "Feedback", "%", "Feedback percent.", CX2_SOUNDTRACK_PARAM_FEEDBACK_MIN, CX2_SOUNDTRACK_PARAM_FEEDBACK_MAX, CX2_SOUNDTRACK_PARAM_FEEDBACK_DEFAULT);
        FMOD_DSP_INIT_PARAMDESC_FLOAT(p_resonance, "Resonance", "%", "Resonance percent", CX2_SOUNDTRACK_PARAM_RESONANCE_MIN, CX2_SOUNDTRACK_PARAM_RESONANCE_MAX, CX2_SOUNDTRACK_PARAM_RESONANCE_DEFAULT);

		return &FMOD_Gain_Desc;
	}

}

class FMODGainState
{
public:
	FMODGainState();
	~FMODGainState();

	void init();
	void read(float *inbuffer, float *outbuffer, unsigned int length, int channels);
	void reset();
	void setDelayTime(float);
    void setFeedback (float);
    void setResonance (float);
	float delayTime() const { return m_delay_time; }
    float feedback() const { return m_feedback; }
    float resonance() const { return m_resonance; }
	static int sampleRate;

private:
	float m_delay_time;
    float* m_delay_buffer = nullptr;
    float* m_reson_buffer = nullptr;
    int m_delay_buffer_size;
    float m_max_delay_sec;
    int m_currentDelayIndex;
	int m_readDelayIndex;
	int m_pitch_delay;
    
    float m_feedback;
    float m_resonance;
};

int FMODGainState::sampleRate = 44100;

FMODGainState::FMODGainState()
{
	init();
}

FMODGainState::~FMODGainState() {
	delete m_delay_buffer;
}

void FMODGainState::init() {
	srand(time(0));

	m_max_delay_sec = CX2_SOUNDTRACK_PARAM_TIME_MAX;
	m_currentDelayIndex = 0;
	m_readDelayIndex = 0;
    m_delay_buffer_size = (int)(m_max_delay_sec * FMODGainState::sampleRate);
	if (m_delay_buffer == nullptr) {
		m_delay_buffer = new float[m_delay_buffer_size]();
	}
    if (m_reson_buffer == nullptr){
        m_reson_buffer = new float[m_delay_buffer_size]();
    }
	reset();
}

void FMODGainState::read(float *inbuffer, float *outbuffer, unsigned int length, int channels)
{
    float f = m_feedback;
    float q = m_resonance;

    float fb = q + q/(1.0 - f);
    
    float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    
    unsigned int samples = length * channels;
    while (samples--) {
        //Saturation ?
        //float r = 0.5;
        float inSamp = *inbuffer;
        if (inSamp > r) {
            inSamp = r + (inSamp-r)/ (1+((inSamp-r)/(1-r)) * (1+((inSamp-r)/(1-r))));
        } else if (inSamp > 1) {
            inSamp = (r+1)/2;
        }
        
		// Check delay index
		if (m_currentDelayIndex > m_delay_buffer_size - 1) {
			m_currentDelayIndex = 0;
		}

		if (m_readDelayIndex < 0) {
			m_readDelayIndex = m_delay_buffer_size - 1;
		}
        
        int delayIndex = 0;
        // Delay
        if ((m_currentDelayIndex + (m_delay_time * FMODGainState::sampleRate)) > m_delay_buffer_size){
            delayIndex = (int)((m_currentDelayIndex + (m_delay_time * FMODGainState::sampleRate)) - m_delay_buffer_size);
        } else {
            delayIndex = (int)(m_currentDelayIndex + (m_delay_time * FMODGainState::sampleRate));
        }
        
        //Resonace ?
        m_reson_buffer[delayIndex] = m_reson_buffer[delayIndex] + f * (inSamp - m_reson_buffer[delayIndex] + fb * (m_reson_buffer[delayIndex] - m_delay_buffer[delayIndex]));
        m_delay_buffer[delayIndex] += m_delay_buffer[delayIndex] + f * (m_reson_buffer[delayIndex] - m_delay_buffer[delayIndex]);
        
        //m_delay_buffer[delayIndex] *= .5f;
        
        m_delay_buffer[delayIndex] *= fb;
		
		if (m_pitch_delay % 2 == 0) {
			*outbuffer++ = *inbuffer++ + m_delay_buffer[m_readDelayIndex--] + m_reson_buffer[m_currentDelayIndex++];
		}
		else {
			*outbuffer++ = m_reson_buffer[m_currentDelayIndex++];
		}

		//*outbuffer++ = *inbuffer++ + m_delay_buffer[m_readDelayIndex];
		//m_readDelayIndex += 2;
		m_pitch_delay++;
    }
}

void FMODGainState::reset()
{
	m_currentDelayIndex = 0;
    m_readDelayIndex = 0;
}

void FMODGainState::setDelayTime(float delay)
{
	m_delay_time = delay;
}

void FMODGainState::setFeedback(float fb)
{
    m_feedback = fb/100.0f;
    
    if (m_feedback < 0){
        m_feedback = 0;
    } else if (m_feedback > 1) {
        m_feedback = 1;
    }
}

void FMODGainState::setResonance(float r)
{
    m_resonance = r/100.0f;
    
    if (m_resonance < 0){
        m_resonance = 0;
    } else if (m_resonance > 1) {
        m_resonance = 1;
    }
}

FMOD_RESULT F_CALLBACK FMOD_Gain_dspcreate(FMOD_DSP_STATE *dsp_state)
{
	dsp_state->plugindata = (FMODGainState *)FMOD_DSP_STATE_MEMALLOC(dsp_state, sizeof(FMODGainState), FMOD_MEMORY_NORMAL, "FMODGainState");
	if (!dsp_state->plugindata)
	{
		return FMOD_ERR_MEMORY;
	}
	int sr;
	dsp_state->callbacks->getsamplerate(dsp_state, &sr);
	FMODGainState::sampleRate = sr;

	FMODGainState *state = (FMODGainState *)dsp_state->plugindata;
	state->init();
	return FMOD_OK;
}

FMOD_RESULT F_CALLBACK FMOD_Gain_dsprelease(FMOD_DSP_STATE *dsp_state)
{
	FMODGainState *state = (FMODGainState *)dsp_state->plugindata;
	FMOD_DSP_STATE_MEMFREE(dsp_state, state, FMOD_MEMORY_NORMAL, "FMODGainState");
	return FMOD_OK;
}

#ifdef FMOD_GAIN_USEPROCESSCALLBACK

FMOD_RESULT F_CALLBACK FMOD_Gain_dspprocess(FMOD_DSP_STATE *dsp_state, unsigned int length, const FMOD_DSP_BUFFER_ARRAY *inbufferarray, FMOD_DSP_BUFFER_ARRAY *outbufferarray, FMOD_BOOL /*inputsidle*/, FMOD_DSP_PROCESS_OPERATION op)
{
	FMODGainState *state = (FMODGainState *)dsp_state->plugindata;

	if (op == FMOD_DSP_PROCESS_QUERY)
	{
		if (outbufferarray && inbufferarray)
		{
			outbufferarray[0].bufferchannelmask[0] = inbufferarray[0].bufferchannelmask[0];
			outbufferarray[0].buffernumchannels[0] = inbufferarray[0].buffernumchannels[0];
			outbufferarray[0].speakermode = inbufferarray[0].speakermode;
		}
	}
	else
	{
		state->read(inbufferarray[0].buffers[0], outbufferarray[0].buffers[0], length, inbufferarray[0].buffernumchannels[0]); // input and output channels count match for this effect
	}

	return FMOD_OK;
}

#else

FMOD_RESULT F_CALLBACK FMOD_Gain_dspread(FMOD_DSP_STATE *dsp_state, float *inbuffer, float *outbuffer, unsigned int length, int inchannels, int * /*outchannels*/)
{
	FMODGainState *state = (FMODGainState *)dsp_state->plugindata;
	state->read(inbuffer, outbuffer, length, inchannels); // input and output channels count match for this effect
	return FMOD_OK;
}

#endif

FMOD_RESULT F_CALLBACK FMOD_Gain_dspreset(FMOD_DSP_STATE *dsp_state)
{
	FMODGainState *state = (FMODGainState *)dsp_state->plugindata;
	state->reset();
	return FMOD_OK;
}

FMOD_RESULT F_CALLBACK FMOD_Gain_dspsetparamfloat(FMOD_DSP_STATE *dsp_state, int index, float value)
{
	FMODGainState *state = (FMODGainState *)dsp_state->plugindata;

	switch (index)
	{
        case CX2_SOUNDTRACK_PARAM_TIME:
            state->setDelayTime(value);
            return FMOD_OK;
        case CX2_SOUNDTRACK_PARAM_FEEDBACK:
            state->setFeedback(value);
            return FMOD_OK;
        case CX2_SOUNDTRACK_PARAM_RESONANCE:
            state->setResonance(value);
            return FMOD_OK;
	}

	return FMOD_ERR_INVALID_PARAM;
}

FMOD_RESULT F_CALLBACK FMOD_Gain_dspgetparamfloat(FMOD_DSP_STATE *dsp_state, int index, float *value, char *valuestr)
{
	FMODGainState *state = (FMODGainState *)dsp_state->plugindata;

	switch (index)
	{
        case CX2_SOUNDTRACK_PARAM_TIME:
            *value = state->delayTime();
            if (valuestr) sprintf(valuestr, "%.1f", state->delayTime());
            return FMOD_OK;
        case CX2_SOUNDTRACK_PARAM_FEEDBACK:
            *value = state->feedback();
            if (valuestr) sprintf(valuestr, "%.1f", state->feedback());
            return FMOD_OK;
        case CX2_SOUNDTRACK_PARAM_RESONANCE:
            *value = state->resonance();
            if (valuestr) sprintf(valuestr, "%.1f", state->resonance());
            return FMOD_OK;
	}

	return FMOD_ERR_INVALID_PARAM;
}

FMOD_RESULT F_CALLBACK FMOD_Gain_shouldiprocess(FMOD_DSP_STATE * /*dsp_state*/, FMOD_BOOL inputsidle, unsigned int /*length*/, FMOD_CHANNELMASK /*inmask*/, int /*inchannels*/, FMOD_SPEAKERMODE /*speakermode*/)
{
	if (inputsidle)
	{
		return FMOD_ERR_DSP_DONTPROCESS;
	}

	return FMOD_OK;
}


FMOD_RESULT F_CALLBACK FMOD_Gain_sys_register(FMOD_DSP_STATE * /*dsp_state*/)
{
	FMOD_Gain_Running = true;
	// called once for this type of dsp being loaded or registered (it is not per instance)
	return FMOD_OK;
}

FMOD_RESULT F_CALLBACK FMOD_Gain_sys_deregister(FMOD_DSP_STATE * /*dsp_state*/)
{
	FMOD_Gain_Running = false;
	// called once for this type of dsp being unloaded or de-registered (it is not per instance)
	return FMOD_OK;
}

FMOD_RESULT F_CALLBACK FMOD_Gain_sys_mix(FMOD_DSP_STATE * /*dsp_state*/, int /*stage*/)
{
	// stage == 0 , before all dsps are processed/mixed, this callback is called once for this type.
	// stage == 1 , after all dsps are processed/mixed, this callback is called once for this type.
	return FMOD_OK;
}
