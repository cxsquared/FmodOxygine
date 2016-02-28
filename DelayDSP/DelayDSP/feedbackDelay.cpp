#include <stdio.h>
#include <string.h>
#include <iostream>
#include "fmod.hpp"

extern "C" {
	F_DECLSPEC F_DLLEXPORT FMOD_DSP_DESCRIPTION* F_STDCALL FMODGetDSPDescription();
}

const float CX2_DELAY_PARAM_TIME_MIN = 0.0f;
const float CX2_DELAY_PARAM_TIME_MAX = 1.0f;
const float CX2_DELAY_PARAM_TIME_DEFAULT = 0.5f;

enum {
	CX2_DELAY_PARAM_TIME = 0,
	CX2_DELAY_NUM_PARAMETERS
};

FMOD_RESULT F_CALLBACK CX2_Delay_dspcreate(FMOD_DSP_STATE *dsp_state);
FMOD_RESULT F_CALLBACK CX2_Delay_dsprelease(FMOD_DSP_STATE *dsp_state);
FMOD_RESULT F_CALLBACK CX2_Delay_dspreset(FMOD_DSP_STATE *dsp_state);
FMOD_RESULT F_CALLBACK CX2_Delay_dspprocess(FMOD_DSP_STATE *dsp_state, unsigned int length, const FMOD_DSP_BUFFER_ARRAY *inbufferarray, FMOD_DSP_BUFFER_ARRAY *outbufferarray, FMOD_BOOL inputsidle, FMOD_DSP_PROCESS_OPERATION op);
FMOD_RESULT F_CALLBACK CX2_Delay_dspsetparamfloat(FMOD_DSP_STATE *dsp_state, int index, float value);
FMOD_RESULT F_CALLBACK CX2_Delay_dspgetparamfloat(FMOD_DSP_STATE *dsp_state, int index, float *value, char *valuestr);
FMOD_RESULT F_CALLBACK CX2_Delay_shouldiprocess(FMOD_DSP_STATE *dsp_state, FMOD_BOOL inputsidle, unsigned int length, FMOD_CHANNELMASK inmask, int inchannels, FMOD_SPEAKERMODE speakermode);
FMOD_RESULT F_CALLBACK CX2_Delay_sys_register(FMOD_DSP_STATE *dsp_state);
FMOD_RESULT F_CALLBACK CX2_Delay_sys_deregister(FMOD_DSP_STATE *dsp_state);
FMOD_RESULT F_CALLBACK CX2_Delay_sys_mix(FMOD_DSP_STATE *dsp_state, int stage);

static bool CX2_Delay_Running = false;
static FMOD_DSP_PARAMETER_DESC p_time;

FMOD_DSP_PARAMETER_DESC *CX2_Delay_dspparam[CX2_DELAY_NUM_PARAMETERS] = 
{
	&p_time
};

FMOD_DSP_DESCRIPTION CX2_Delay_Desc =
{
	FMOD_PLUGIN_SDK_VERSION, // what sdk the plugin was built with
	"Cx2 Delay", // Identifier
	0x00000001, // Version
	1, // number of input buffers
	1, // number of output buffers
	CX2_Delay_dspcreate, // create method
	CX2_Delay_dsprelease, // release method
	CX2_Delay_dspreset, // reset method
	0, // read method (not using cause it's only for FMod EX support)
	CX2_Delay_dspprocess, // Process method
	0, // Set position
	CX2_DELAY_NUM_PARAMETERS, // Number of parameters
	CX2_Delay_dspparam, // Description of parameters
	CX2_Delay_dspsetparamfloat, // Set Float
	0, // set int
	0, // set bool
	0, // set data
	CX2_Delay_dspgetparamfloat, // Get float
	0, // Get int
	0, // get Bool
	0, // get data
	CX2_Delay_shouldiprocess, // make sure nothing is wrong before processing
	0, // user data
	CX2_Delay_sys_register, // called when loaded
	CX2_Delay_sys_deregister, // called when unloaded, clean up
	CX2_Delay_sys_mix // called before mixing ?
};

extern "C" {
	F_DECLSPEC F_DLLEXPORT FMOD_DSP_DESCRIPTION* F_STDCALL FMODGetDSPDescription() {
		FMOD_DSP_INIT_PARAMDESC_FLOAT(p_time, "Time", "ms", "Time in milliseconds.", CX2_DELAY_PARAM_TIME_MIN, CX2_DELAY_PARAM_TIME_MAX, CX2_DELAY_PARAM_TIME_DEFAULT);

		return &CX2_Delay_Desc;
	}
}

class CX2DelayState {
public:
	CX2DelayState();
	~CX2DelayState();

	void read(float *inbuffer, float *outbuffer, unsigned int length, int channels);
	void reset();
	void setTime(float);
	float time() const { return m_time; }
	void createBuffer(int sampleRate);

private:
	float m_time;
	float* delayBuffer;
	int delayIndex;
	unsigned int delayBufferSize; // in samples
};

CX2DelayState::CX2DelayState() {
	m_time = CX2_DELAY_PARAM_TIME_DEFAULT;
	reset();
}

CX2DelayState::~CX2DelayState() {
	delete(delayBuffer);
}

void CX2DelayState::read(float *inbuffer, float *outbuffer, unsigned int length, int channels) {
	int i = 0; // input index
	int j = 0; // delay index

	int delay = 1000;

	unsigned int samples = length * channels;
	while (i < samples) {

		if (delay >= samples) {
			delay -= samples * m_time;
		}

		outbuffer[i] = inbuffer[i] + inbuffer[delay++];

		i++;
	}
}

void CX2DelayState::reset() {
	// reset stuff
}

void CX2DelayState::setTime(float time) {
	m_time = time;
}

void CX2DelayState::createBuffer(int sampleRate) {
	delayBufferSize = sampleRate*4;
	delayBuffer = new float[delayBufferSize];
}

// FMOD Callbacks
FMOD_RESULT F_CALLBACK CX2_Delay_dspcreate(FMOD_DSP_STATE *dsp_state) {
	dsp_state->plugindata = (CX2DelayState *)FMOD_DSP_STATE_MEMALLOC(dsp_state, sizeof(CX2DelayState), FMOD_MEMORY_NORMAL, "CX2DelayState");
	if (!dsp_state->plugindata) {
		int rate;
		FMOD_DSP_STATE_GETSAMPLERATE(dsp_state, &rate);
		CX2DelayState *state = (CX2DelayState *)dsp_state->plugindata;
		state->createBuffer(rate);
		return FMOD_ERR_MEMORY;
	}

	return FMOD_OK;
}

FMOD_RESULT F_CALLBACK CX2_Delay_dsprelease(FMOD_DSP_STATE *dsp_state) {
	CX2DelayState *state = (CX2DelayState *)dsp_state->plugindata;
	FMOD_DSP_STATE_MEMFREE(dsp_state, state, FMOD_MEMORY_NORMAL, "CX2DelayState");
	return FMOD_OK;
}

FMOD_RESULT F_CALLBACK CX2_Delay_dspprocess(FMOD_DSP_STATE *dsp_state, unsigned int length, const FMOD_DSP_BUFFER_ARRAY *inbufferarray, FMOD_DSP_BUFFER_ARRAY *outbufferarray, FMOD_BOOL /*inputsidle*/, FMOD_DSP_PROCESS_OPERATION op)
{
	CX2DelayState *state = (CX2DelayState *)dsp_state->plugindata;

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

FMOD_RESULT F_CALLBACK CX2_Delay_dspreset(FMOD_DSP_STATE *dsp_state)
{
	CX2DelayState *state = (CX2DelayState *)dsp_state->plugindata;
	state->reset();
	return FMOD_OK;
}

FMOD_RESULT F_CALLBACK CX2_Delay_dspsetparamfloat(FMOD_DSP_STATE *dsp_state, int index, float value) {
	CX2DelayState *state = (CX2DelayState *)dsp_state->plugindata;

	switch (index)
	{
	case CX2_DELAY_PARAM_TIME:
		state->setTime(value);
		return FMOD_OK;
	}

	return FMOD_ERR_INVALID_PARAM;
}

FMOD_RESULT F_CALLBACK CX2_Delay_dspgetparamfloat(FMOD_DSP_STATE *dsp_state, int index, float *value, char *valuestr) {
	CX2DelayState *state = (CX2DelayState *)dsp_state->plugindata;

	switch (index)
	{
	case CX2_DELAY_PARAM_TIME:
		*value = state->time();
		if (valuestr) sprintf(valuestr, "%.1f ms", state->time());
		return FMOD_OK;
	}

	return FMOD_ERR_INVALID_PARAM;
}

FMOD_RESULT F_CALLBACK CX2_Delay_shouldiprocess(FMOD_DSP_STATE * /*dsp_state*/, FMOD_BOOL inputsidle, unsigned int /*length*/, FMOD_CHANNELMASK /*inmask*/, int /*inchannels*/, FMOD_SPEAKERMODE /*speakermode*/)
{
	if (inputsidle)
	{
		return FMOD_ERR_DSP_DONTPROCESS;
	}

	return FMOD_OK;
}


FMOD_RESULT F_CALLBACK CX2_Delay_sys_register(FMOD_DSP_STATE * /*dsp_state*/)
{
	CX2_Delay_Running = true;
	// called once for this type of dsp being loaded or registered (it is not per instance)
	return FMOD_OK;
}

FMOD_RESULT F_CALLBACK CX2_Delay_sys_deregister(FMOD_DSP_STATE * /*dsp_state*/)
{
	CX2_Delay_Running = false;
	// called once for this type of dsp being unloaded or de-registered (it is not per instance)
	return FMOD_OK;
}

FMOD_RESULT F_CALLBACK CX2_Delay_sys_mix(FMOD_DSP_STATE * /*dsp_state*/, int /*stage*/)
{
	// stage == 0 , before all dsps are processed/mixed, this callback is called once for this type.
	// stage == 1 , after all dsps are processed/mixed, this callback is called once for this type.
	return FMOD_OK;
}