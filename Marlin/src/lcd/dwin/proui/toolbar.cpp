/**
 * toolBar for ProUI
 * Author: Miguel A. Risco-Castillo (MRISCOC)
 * version: 3.2.0
 * Date: 2023/09/12
 */

#include "../../../inc/MarlinConfig.h"

#if HAS_TOOLBAR

#include "dwin.h"
#include "toolbar.h"
#include "toolbar_def.h"

uint8_t TBGetCount() {
  return COUNT(TBItemA);
}

// Draw toolbar as a horizontal icon row at the bottom of the main screen.
// Icons are spaced evenly across the screen width.
// The hmiData.TBopt[] array maps each slot (0..TBMaxOpt-1) to an index in TBItemA[].
// Slot value 0 means "disabled" (no icon drawn).
void drawToolBar() {
  const uint8_t count = TBGetCount();
  if (count <= 1) return; // Only "disabled" entry exists, nothing to draw

  // Draw opaque toolbar background strip — must be drawn before icons
  dwinDrawRectangle(1, hmiData.colorBackground, 0, TBYPOS, DWIN_WIDTH - 1, STATUS_Y - 1);
  // Draw a thin separator line above the toolbar
  dwinDrawLine(hmiData.colorSplitLine, 0, TBYPOS, DWIN_WIDTH - 1, TBYPOS);

  // Calculate horizontal spacing for TBMaxOpt slots
  const uint16_t iconSpacing = (DWIN_WIDTH - 2 * B_XPOS) / TBMaxOpt;

  for (uint8_t i = 0; i < TBMaxOpt; i++) {
    const uint8_t itemIdx = hmiData.TBopt[i];
    if (itemIdx == 0 || itemIdx >= count) continue; // Skip disabled or out-of-range slots

    const uint16_t x = B_XPOS + i * iconSpacing;
    DWINUI::drawIcon(TBItemA[itemIdx].icon, x, B_YPOS);
  }
}

// Execute the toolbar action for a given slot index (0..TBMaxOpt-1)
// Returns true if an action was triggered.
bool toolbarClick(uint8_t slot) {
  if (slot >= TBMaxOpt) return false;
  const uint8_t itemIdx = hmiData.TBopt[slot];
  if (itemIdx == 0 || itemIdx >= TBGetCount()) return false;
  auto cb = TBItemA[itemIdx].onClick;
  if (cb) {
    cb();
    return true;
  }
  return false;
}

#endif // HAS_TOOLBAR
