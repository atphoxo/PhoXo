#pragma once

#include "canvas.h"
#include "mfc_scroll_view_anchor_restore.h"

using phoxo::ViewportContext;
using phoxo::ScrollViewAnchorRestore;

// basic commands
#include "commands/set_canvas_size.h"
#include "commands/move_layer.h"
#include "commands/mfc_set_doc_modified.h"
#include "commands/modify_layer_image.h"

// composite commands
#include "commands/flip_canvas.h"
#include "commands/rotate_canvas.h"
