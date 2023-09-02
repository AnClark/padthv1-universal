// padthv1_dpfui.cpp
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

#include "padthv1_dpfui.h"
#include "padthv1_dpf.h"

#include <QWindow>
#include <QApplication>
#include <QMainWindow>

//-------------------------------------------------------------------------
// PadthV1PluginUI - DPF Plugin UI interface.
//

START_NAMESPACE_DISTRHO

PadthV1PluginUI::PadthV1PluginUI()
	: UI(), fWidget(nullptr)
{
	// Print out DPF standalone mode state.
	const bool standalone = isStandalone();
	d_stdout("isStandalone %d", (int)standalone);

	// padthv1 UI requires directly accessing synth instance.
	// This is discouraged by DPF, but padthv1 do need this due to its designation.
	PadthV1Plugin *fDspInstance = (PadthV1Plugin*) UI::getPluginInstancePointer();

	// Load background window.
	// Showing UI within background window can prevent unexpected "ghost shadows" when resizing.
	fWindow = std::make_unique<QMainWindow>();

	// Load padthv1 main UI part.
	fWidget = std::make_unique<padthv1widget_dpf>(fDspInstance->getSynthesizer(), this);

	// Set minimum size
	m_widgetSize = fWidget->sizeHint();
	m_widgetSize.setHeight(m_widgetSize.height() * getScaleFactor());
	m_widgetSize.setWidth(m_widgetSize.width() * getScaleFactor());
	fWidget->setMinimumSize(m_widgetSize);
	fWindow->setMinimumSize(m_widgetSize);

	// Embed padthv1 UI part into background window
	fWidget->setParent(&*fWindow);

	// Explicitly set window position to avoid misplace on some platforms (especially Windows)
	fWidget->move(0, 0);

	// Embed plug-in UI into host window.
	fParent = (WId) getParentWindowHandle();
	fWinId = fWindow->winId();	// Must require WinID first, otherwise plug-in will crash!
#ifdef DISTRHO_PLUGIN_TARGET_LV2
	fWinId_Widget = fWidget->winId();	// On LV2, we need to require fWidget's WinId as well 
#endif
	if (fParent)
	{
		fWindow->windowHandle()->setParent(QWindow::fromWinId(fParent));
	}

	// Explicitly show UI. This is required when using external UI mode of DPF.
	fWindow->show();
}

PadthV1PluginUI::~PadthV1PluginUI()
{
	// NOTICE: Now fWidget is already managed by unique pointer. No need to clean up manually.
}

/* --------------------------------------------------------------------------------------------------------
* DSP/Plugin Callbacks */

/**
	A parameter has changed on the plugin side.
	This is called by the host to inform the UI about parameter changes.
*/
void PadthV1PluginUI::parameterChanged(uint32_t index, float value)
{
	DISTRHO_SAFE_ASSERT_RETURN(fWidget != nullptr,);
	fWidget->setUIParamValue(padthv1::ParamIndex(index), value);
}

/* --------------------------------------------------------------------------------------------------------
* External Window overrides */

void PadthV1PluginUI::focus()
{
	d_stdout("focus");

	DISTRHO_SAFE_ASSERT_RETURN(fWidget != nullptr,);
	fWidget->setFocus();
}

uintptr_t PadthV1PluginUI::getNativeWindowHandle() const noexcept
{
	return (uintptr_t)fWidget->windowHandle()->winId();
}

void PadthV1PluginUI::sizeChanged(uint width, uint height)
{
	UI::sizeChanged(width, height);

	if (fWindow != nullptr && fWidget != nullptr) {
		fWindow->resize(width, height);
 		fWidget->resize(width, height);
	}
}

void PadthV1PluginUI::titleChanged(const char* const title)
{
	d_stdout("titleChanged %s", title);

	DISTRHO_SAFE_ASSERT_RETURN(fWidget != nullptr,);
	fWidget->setWindowTitle(QString(title));
}

void PadthV1PluginUI::transientParentWindowChanged(const uintptr_t winId)
{
	d_stdout("transientParentWindowChanged %lu", winId);

	DISTRHO_SAFE_ASSERT_RETURN(fWidget != nullptr,);
	// NOTICE: Seems not implemented by Qt
}

void PadthV1PluginUI::visibilityChanged(const bool visible)
{
	d_stdout("visibilityChanged %d", visible);

	DISTRHO_SAFE_ASSERT_RETURN(fWindow != nullptr,);

	if (visible)
	{
		fWindow->show();
		fWindow->raise();
		fWindow->activateWindow();
	}
	else
		fWindow->hide();
}

void PadthV1PluginUI::uiIdle()
{
	// d_stdout("uiIdle");

	if (fWidget != nullptr)
	{
		QApplication::processEvents();
		return;
	}
}

/* ------------------------------------------------------------------------------------------------------------
 * UI entry point, called by DPF to create a new UI instance. */

UI* createUI()
{
	return new PadthV1PluginUI();
}

END_NAMESPACE_DISTRHO


//-------------------------------------------------------------------------
// padthv1_dpfui - Synth engine accessor: impl.
//

padthv1_dpfui::padthv1_dpfui(padthv1_dpf *pSynth, DISTRHO::PadthV1PluginUI *pluginUiInterface)
	: padthv1_ui(pSynth, true), m_plugin_ui(pluginUiInterface)
{
}

void padthv1_dpfui::write_function(padthv1::ParamIndex index, float fValue) const
{
	m_plugin_ui->setParameterValue(index, fValue);
}

// end of padthv1_dpfui.cpp
