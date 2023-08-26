// padthv1_dpfui.h
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

#ifndef __padthv1_dpfui_h
#define __padthv1_dpfui_h

#include "padthv1widget_dpf.h"

#include "DistrhoUI.hpp"

// Forward decls.
class padthv1_dpf;

// Constants.
static const int BASE_UI_WIDTH = 1100;
static const int BASE_UI_HEIGHT = 510;

// -----------------------------------------------------------------------------------------------------------
// PadthV1PluginUI - DPF Plugin UI interface.

START_NAMESPACE_DISTRHO

class PadthV1PluginUI : public UI {

	std::unique_ptr<padthv1widget_dpf> fWidget;
	WId fWinId;
	WId fParent;

	//ResizeHandle fResizeHandle;

	// ----------------------------------------------------------------------------------------------------------------

public:
	PadthV1PluginUI();
	~PadthV1PluginUI();

protected:
	// ----------------------------------------------------------------------------------------------------------------
	// DSP/Plugin Callbacks

	void parameterChanged(uint32_t index, float value) override;
	//void programLoaded(uint32_t index) override;
	//void stateChanged(const char* key, const char* value) override;

	// ----------------------------------------------------------------------------------------------------------------
	// External window overrides

	void focus() override;
	uintptr_t getNativeWindowHandle() const noexcept override;
	void sizeChanged(uint width, uint height) override;
	void titleChanged(const char* const title) override;
	void transientParentWindowChanged(const uintptr_t winId) override;
	void visibilityChanged(const bool visible) override;
	void uiIdle() override;

private:
	// ----------------------------------------------------------------------------------------------------------------
	// Internal Procedures

	void _initParameterProperties();

	DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PadthV1PluginUI)
};

// -----------------------------------------------------------------------------------------------------------

END_NAMESPACE_DISTRHO

//-------------------------------------------------------------------------
// padthv1_dpfui - Synth engine accessor: decl.
//

class padthv1_dpfui : public padthv1_ui
{
public:

	// Constructor.
	padthv1_dpfui(padthv1_dpf *pSynth, DISTRHO::PadthV1PluginUI *pluginUiInterface);

	// Accessors.
	void write_function(padthv1::ParamIndex index, float fValue) const;

private:
	DISTRHO::PadthV1PluginUI *m_plugin_ui;
};

#endif// __padthv1_dpfui_h

// end of padthv1_dpfui.h
