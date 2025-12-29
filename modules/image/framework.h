#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>

#include <atltypes.h>
#include <atlstr.h>
#include "phoxo-core-lib/src/phoxo_core.h"
#include "phoxo-core-lib/optional/svg_drawer.h"
using FCColor = phoxo::Color;
using FCImage = phoxo::Image;
using phoxo::ImageFileExtParser;
using phoxo::ImageFormat;
#include "UIStone/base/global.h"

#include <libheif/heif.h>
