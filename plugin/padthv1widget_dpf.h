// padthv1widget_dpf.h
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

#ifndef __padthv1widget_dpf_h
#define __padthv1widget_dpf_h

#include "padthv1widget.h"

// Forward decls.
class padthv1_dpf;
class padthv1_dpfui;
namespace DISTRHO {
	class PadthV1PluginUI;
}

//-------------------------------------------------------------------------
// padthv1widget_dpf - decl.
//

class padthv1widget_dpf : public padthv1widget
{
public:

	// Constructor.
	padthv1widget_dpf(padthv1_dpf *pSynth, DISTRHO::PadthV1PluginUI *pPluginUiInterface);

	// Destructor.
	~padthv1widget_dpf();

	// Param method. (Host -> UI)
	void setUIParamValue(padthv1::ParamIndex paramIndex, float value);

protected:

	// Synth engine accessor.
	padthv1_ui *ui_instance() const;

	// Param method. (UI -> Host)
	void updateParam(padthv1::ParamIndex index, float fValue) const;

	// Close event handler.
	void closeEvent(QCloseEvent *pCloseEvent);

private:

	// Instance variables.
	padthv1_dpfui *m_pSynthUi;  // synth engine accessor
};

#endif  // __padthv1widget_dpf_h

// end of padthv1widget_dpf.h
