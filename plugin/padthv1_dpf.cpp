// padthv1_dpf.cpp
//
/****************************************************************************
   Copyright (C) 2023, AnClark Liu. All rights reserved.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along
   with this program; if not, write to the Free Software Foundation, Inc.,
   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*****************************************************************************/

#include "padthv1_dpf.h"
#include "padthv1_param.h"
#include "padthv1_config.h"

#include "DistrhoPluginUtils.hpp"

#include <QApplication>

//-------------------------------------------------------------------------
// padthv1_dpf - Constants.
//

// Parameter names - extracted from LV2 definition
static const char *ParamNames[padthv1::NUM_PARAMS] = {
	"GEN1 Sample1",
	"GEN1 Width 1",
	"GEN1 Scale 1",
	"GEN1 Nh 1",
	"GEN1 Apodizer 1",
	"GEN1 Detune 1",
	"GEN1 Glide 1",
	"GEN1 Sample2",
	"GEN1 Width 2",
	"GEN1 Scale 2",
	"GEN1 Nh 2",
	"GEN1 Apodizer 2",
	"GEN1 Detune 2",
	"GEN1 Glide 2",
	"GEN1 Balance",
	"GEN1 Phase",
	"GEN1 Ring Mod",
	"GEN1 Octave",
	"GEN1 Tuning",
	"GEN1 Env.Time",
	"DCF1 Enabled",
	"DCF1 Cutoff",
	"DCF1 Resonance",
	"DCF1 Type",
	"DCF1 Slope",
	"DCF1 Envelope",
	"DCF1 Attack",
	"DCF1 Decay",
	"DCF1 Sustain",
	"DCF1 Release",
	"LFO1 Enabled",
	"LFO1 Wave Shape",
	"LFO1 Wave Width",
	"LFO1 BPM",
	"LFO1 Rate",
	"LFO1 Sync",
	"LFO1 Sweep",
	"LFO1 Pitch",
	"LFO1 Balance",
	"LFO1 Ring Mod",
	"LFO1 Cutoff",
	"LFO1 Resonance",
	"LFO1 Panning",
	"LFO1 Volume",
	"LFO1 Attack",
	"LFO1 Decay",
	"LFO1 Sustain",
	"LFO1 Release",
	"DCA1 Volume",
	"DCA1 Attack",
	"DCA1 Decay",
	"DCA1 Sustain",
	"DCA1 Release",
	"OUT1 Stereo Width",
	"OUT1 Panning",
	"OUT1 FX Send",
	"OUT1 Volume",
	"DEF1 Pitchbend",
	"DEF1 Modwheel",
	"DEF1 Pressure",
	"DEF1 Velocity",
	"DEF1 Channel",
	"DEF1 Mono",
	"Chorus Wet",
	"Chorus Delay",
	"Chorus Feedback",
	"Chorus Rate",
	"Chorus Modulation",
	"Flanger Wet",
	"Flanger Delay",
	"Flanger Feedback",
	"Flanger Daft",
	"Phaser Wet",
	"Phaser Rate",
	"Phaser Feedback",
	"Phaser Depth",
	"Phaser Daft",
	"Delay Wet",
	"Delay Delay",
	"Delay Feedback",
	"Delay BPM",
	"Reverb Wet",
	"Reverb Room",
	"Reverb Damp",
	"Reverb Feedback",
	"Reverb Width",
	"Dynamic Compressor",
	"Dynamic Limiter",
	"Keyboard Low",
	"Keyboard High"
};


//-------------------------------------------------------------------------
// padthv1_dpf - Instantiation and cleanup.
//

QApplication *padthv1_dpf::g_qapp_instance = nullptr;
unsigned int  padthv1_dpf::g_qapp_refcount = 0;


padthv1_dpf::padthv1_dpf(double sample_rate): padthv1(2, float(sample_rate))
{
}


padthv1_dpf::~padthv1_dpf()
{
}


void padthv1_dpf::qapp_instantiate (void)
{
	if (qApp == nullptr && g_qapp_instance == nullptr) {
		static int s_argc = 1;
		static const char *s_argv[] = { PADTHV1_TITLE, nullptr };
	#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
		::_putenv_s("QT_NO_GLIB", "1"); // Avoid glib event-loop...
	#else
		::setenv("QT_NO_GLIB", "1", 1); // Avoid glib event-loop...
	#endif
		g_qapp_instance = new QApplication(s_argc, (char **) s_argv);
	}

	if (g_qapp_instance) ++g_qapp_refcount;
}


void padthv1_dpf::qapp_cleanup (void)
{
	if (g_qapp_instance && --g_qapp_refcount == 0) {
		delete g_qapp_instance;
		g_qapp_instance = nullptr;
	}
}


QApplication *padthv1_dpf::qapp_instance (void)
{
	return g_qapp_instance;
}


//-------------------------------------------------------------------------
// padthv1_dpf - methods impl.
//

void padthv1_dpf::updatePreset ( bool /*bDirty*/ )
{
	// NOTICE: No need to tell DPF about preset changes, since DPF knows it
	//         when parameter changes from UI side.
	//         Also "synthesizer -> plug-in" access is not essential in DPF.
	//         See: padthv1widget_dpf::updateParam.
}


void padthv1_dpf::updateParam ( padthv1::ParamIndex index )
{
	// NOTICE: No need to tell DPF about param changes. Reason mentioned above.
	(void) index;
}


void padthv1_dpf::updateParams (void)
{
	// NOTICE: No need to tell DPF about param changes. Reason mentioned above.
}


void padthv1_dpf::updateTuning (void)
{
	// NOTICE: DPF does not support manual tuning. Will not implement it for now.
}


void padthv1_dpf::activate (void)
{
	padthv1::reset();
}


void padthv1_dpf::deactivate (void)
{
	padthv1::reset();
}


void padthv1_dpf::run(const float **inputs, float **outputs, uint32_t nframes, const MidiEvent* midiEvents, uint32_t midiEventCount)
{
    for (AudioMidiSyncHelper amsh(outputs, nframes, midiEvents, midiEventCount); amsh.nextEvent();)
    {
        for (uint32_t i = 0; i < amsh.midiEventCount; ++i)
        {
            const MidiEvent &ev(amsh.midiEvents[i]);
            padthv1::process_midi((uint8_t *)ev.data, ev.size);
        }

        padthv1::process((float **)inputs, amsh.outputs, amsh.frames);
    }
}



//-------------------------------------------------------------------------
// PadthV1Plugin - DPF plugin interface.
//

START_NAMESPACE_DISTRHO

PadthV1Plugin::PadthV1Plugin()
	: Plugin(padthv1::NUM_PARAMS, 0, 0) // parameters, programs, states
{
	padthv1_dpf::qapp_instantiate();
}


PadthV1Plugin::~PadthV1Plugin()
{
	padthv1_dpf::qapp_cleanup();
}


padthv1_dpf* PadthV1Plugin::getSynthesizer()
{
	DISTRHO_SAFE_ASSERT(fSynthesizer != nullptr)

	return &(*fSynthesizer);	// Unique pointer -> standard pointer
}


void PadthV1Plugin::initParameter(uint32_t index, Parameter& parameter)
{
	DISTRHO_SAFE_ASSERT(fSynthesizer != nullptr)

	padthv1::ParamIndex currentParam = (padthv1::ParamIndex)index;

	parameter.hints = kParameterIsAutomatable;

	parameter.name = ParamNames[index];
	parameter.shortName = padthv1_param::paramName(currentParam);
	parameter.symbol = padthv1_param::paramName(currentParam);
	parameter.ranges.def = padthv1_param::paramDefaultValue(currentParam);
	parameter.ranges.min = padthv1_param::paramMinValue(currentParam);
	parameter.ranges.max = padthv1_param::paramMaxValue(currentParam);

	if (padthv1_param::paramBool(currentParam)) {
		parameter.hints |= kParameterIsBoolean;
	}
	else if (padthv1_param::paramInt(currentParam)) {
		parameter.hints |= kParameterIsInteger;
	}

	// Apply default parameter values for padthv1 explicitly,
	// since DPF cannot apply it automatically
	fSynthesizer->setParamValue(currentParam, parameter.ranges.def);
}


float PadthV1Plugin::getParameterValue(uint32_t index) const
{
	DISTRHO_SAFE_ASSERT(fSynthesizer != nullptr)

	return fSynthesizer->paramValue((padthv1::ParamIndex)index);
}


void PadthV1Plugin::setParameterValue(uint32_t index, float value)
{
	DISTRHO_SAFE_ASSERT(fSynthesizer != nullptr)

	fSynthesizer->setParamValue((padthv1::ParamIndex)index, value);
}


void PadthV1Plugin::activate()
{
	DISTRHO_SAFE_ASSERT(fSynthesizer != nullptr)

	fSynthesizer->activate();
}


void PadthV1Plugin::run(const float** inputs, float** outputs, uint32_t frames, const MidiEvent* midiEvents, uint32_t midiEventCount) 
{
	DISTRHO_SAFE_ASSERT(fSynthesizer != nullptr)

	fSynthesizer->run(inputs, outputs, frames, midiEvents, midiEventCount);
}


void PadthV1Plugin::sampleRateChanged(double newSampleRate)
{
	DISTRHO_SAFE_ASSERT(fSynthesizer != nullptr)

	fSynthesizer->setSampleRate(newSampleRate);
}


Plugin* createPlugin()
{
	d_stderr2(">> Creating plugin...");
	return new PadthV1Plugin();
}

END_NAMESPACE_DISTRHO

// end of padthv1_dpf.cpp
