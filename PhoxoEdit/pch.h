#pragma once

//-------------------------------
// Windows / MFC macros
//-------------------------------
#define WIN32_LEAN_AND_MEAN                  // Exclude rarely-used Windows headers
#define VC_EXTRALEAN                         // Exclude rarely-used stuff from Windows headers
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS   // Some CString constructors will be explicit
#define _AFX_NO_MFC_CONTROLS_IN_DIALOGS      // Remove support for MFC controls in dialogs
#define _AFX_ALL_WARNINGS                    // Turn off MFC's hiding of common warnings
#define _ATL_NO_HOSTING                      // Disable ActiveX / OCX hosting support

#include <SDKDDKVer.h>  // Define the highest available Windows platform

//-------------------------------
// BCG library
//-------------------------------
#include "BCGCBProInc.h"

//-------------------------------
// PhoXo Core & UIStone (lowest-level libraries)
//-------------------------------
#include "../3rd/phoxo-core-lib/src/phoxo_core.h"
#define  PHOXO_LANGUAGE_BASE_FOLDER  LR"(language\edit\)"
#include "../3rd/UIStone/UIStone.h"

#include "canvas/canvas_all.h"

using FCColor = phoxo::Color;
using FCImage = phoxo::Image;
using GColor = Gdiplus::Color;

using std::unique_ptr, std::make_unique;
using std::shared_ptr, std::make_shared;
using phoxo::GPointF;
using phoxo::Canvas;
using phoxo::Layer;

//-----------------------------------------------------------------------------
#include "phoxo_shared.h"

#include "base/app_define.h"
#include "base/fixed_aspect_ratio.h"
#include "base/runtime_context.h"
#include "base/app_config.h"

#include "tool_base.h"

#include "resource.h"       // main symbols
#pragma comment (lib, "../modules/image/bin/image.lib")
