#pragma once

#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit
#define _AFX_NO_MFC_CONTROLS_IN_DIALOGS         // remove support for MFC controls in dialogs
#define _AFX_ALL_WARNINGS       // turns off MFC's hiding of some common and often safely ignored warning messages

#include "BCGCBProInc.h"

#include "phoxo-core-lib/src/phoxo_core.h"

#define PHOXO_LANGUAGE_BASE_FOLDER LR"(language\edit\)";
#include "UIStone/UIStone.h"

using FCColor = phoxo::Color;
using FCImage = phoxo::Image;
using CStringRef = const CString&;

using phoxo::GPointF;
using std::unique_ptr, std::make_unique;
using std::shared_ptr, std::make_shared;

//-----------------------------------------------------------------------------
#include "base/app_define.h"
#include "app_config.h"

#include "canvas/canvas.h"

using phoxo::Canvas;
using phoxo::Layer;

#include "phoxo_shared.h"
#include "resource.h"       // main symbols

#pragma comment (lib, "../modules/image/bin/image.lib")
