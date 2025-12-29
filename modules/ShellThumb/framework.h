#pragma once

#ifndef STRICT
#define STRICT
#endif

#include "targetver.h"

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit
#define SHARED_HANDLERS
#define ATL_NO_ASSERT_ON_DESTROY_NONEXISTENT_WINDOW

#include "resource.h"
#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>

#include <atlstr.h>
#include <atltypes.h>
using namespace ATL;

#include "phoxo-core-lib/src/phoxo_core.h"
#include "UIStone/UIStone.h"

#include "../image/image.h"
#pragma comment (lib, "../image/bin/image.lib")

namespace eff = phoxo::effect;
using phoxo::Utils;
using phoxo::ImageFileExtParser;
using phoxo::ImageFormat;
using phoxo::ImageHandler;

using FCColor = phoxo::Color;
using FCImage = phoxo::Image;
