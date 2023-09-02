// padthv1widget_dpf.cpp
//
/****************************************************************************
   Copyright (C) 2012-2022, rncbc aka Rui Nuno Capela.
   Copyright (C) 2023, AnClark Liu.
   All rights reserved.

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

#include "padthv1widget_dpf.h"

#include "padthv1_dpf.h"
#include "padthv1_dpfui.h"

#include <QApplication>
#include <QFileInfo>
#include <QDir>

#include "padthv1widget_palette.h"

#include <QCloseEvent>

#include <QStyleFactory>

#ifndef CONFIG_LIBDIR
#if defined(__x86_64__)
#define CONFIG_LIBDIR CONFIG_PREFIX "/lib64"
#else
#define CONFIG_LIBDIR CONFIG_PREFIX "/lib"
#endif
#endif

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#define CONFIG_PLUGINSDIR CONFIG_LIBDIR "/qt5/plugins"
#else
#define CONFIG_PLUGINSDIR CONFIG_LIBDIR "/qt6/plugins"
#endif


//-------------------------------------------------------------------------
// padthv1widget_lv2 - impl.
//

padthv1widget_dpf::padthv1widget_dpf ( padthv1_dpf *pSynth, DISTRHO::PadthV1PluginUI *pPluginUiInterface )
	: padthv1widget()
{
	// Check whether under a dedicated application instance...
	QApplication *pApp = padthv1_dpf::qapp_instance();
	if (pApp) {
		// Special style paths...
		if (QDir(CONFIG_PLUGINSDIR).exists())
			pApp->addLibraryPath(CONFIG_PLUGINSDIR);
	}

	// Custom color/style themes...
	padthv1_config *pConfig = padthv1_config::getInstance();
	if (pConfig) {
		if (!pConfig->sCustomColorTheme.isEmpty()) {
			QPalette pal;
			if (padthv1widget_palette::namedPalette(
					pConfig, pConfig->sCustomColorTheme, pal))
				padthv1widget::setPalette(pal);
		}
		if (!pConfig->sCustomStyleTheme.isEmpty()) {
			padthv1widget::setStyle(
				QStyleFactory::create(pConfig->sCustomStyleTheme));
		}
	}

	// Initialize (user) interface stuff...
	m_pSynthUi = new padthv1_dpfui(pSynth, pPluginUiInterface);

	// Initialise preset stuff...
	clearPreset();

	// Initial update, always...
	updateSample();

	//resetParamValues();
	resetParamKnobs();

	// May initialize the scheduler/work notifier.
	openSchedNotifier();
}


// Destructor.
padthv1widget_dpf::~padthv1widget_dpf (void)
{
	delete m_pSynthUi;
}


// Synth engine accessor.
padthv1_ui *padthv1widget_dpf::ui_instance (void) const
{
	return m_pSynthUi;
}

// Close event handler.
void padthv1widget_dpf::closeEvent ( QCloseEvent *pCloseEvent )
{
	padthv1widget::closeEvent(pCloseEvent);
}

// Param method: Host -> UI.
// Render host's parameter values on UI. This is called by DPF UI's paramChanged() method.
void padthv1widget_dpf::setUIParamValue(padthv1::ParamIndex paramIndex, float value)
{
	this->setParamValue(paramIndex, value);
}

// Param method: UI -> Host.
// This method sets host's param values from UI side.
void padthv1widget_dpf::updateParam (
	padthv1::ParamIndex index, float fValue ) const
{
	m_pSynthUi->setParamValue(index, fValue);
	m_pSynthUi->write_function(index, fValue);
}

// State method: Host -> UI.
// Render new sample config values on UI. This is called by DPF UI's stateChanged() method.
void padthv1widget_dpf::refreshUISample()
{
	updateSample();
}

// end of padthv1widget_dpf.cpp
