/**
 * toolBar for PRO UI
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

// Horizontal position of each toolbar slot
static uint16_t tbSlotX(uint8_t slot) {
  const uint16_t iconSpacing = (DWIN_WIDTH - 2 * B_XPOS) / TBMaxOpt;
  return B_XPOS + slot * iconSpacing;
}

// Draw toolbar as a horizontal icon row at the bottom of the main screen (visual overlay)
void drawToolBar() {
  const uint8_t count = TBGetCount();
  if (count == 0) return;

  // Draw opaque toolbar background strip
  dwinDrawRectangle(1, hmiData.colorBackground, 0, TBYPOS, DWIN_WIDTH - 1, STATUS_Y - 1);
  // Separator line
  dwinDrawLine(hmiData.colorSplitLine, 0, TBYPOS, DWIN_WIDTH - 1, TBYPOS);

  for (uint8_t i = 0; i < TBMaxOpt; i++) {
    const uint8_t itemIdx = hmiData.TBopt[i];
    if (itemIdx == 0 || itemIdx >= count) continue;
    DWINUI::drawIcon(TBItemA[itemIdx].icon, tbSlotX(i), B_YPOS);
  }
}

// Draw a selection highlight frame around a toolbar slot (like main menu button frame)
void drawToolBarHighlight(uint8_t selectedSlot) {
  if (selectedSlot >= TBMaxOpt) return;
  const uint16_t x = tbSlotX(selectedSlot);
  // 1px frame outline like main menu buttons, using colorHighlight
  dwinDrawBox(0, hmiData.colorHighlight, x - 3, B_YPOS - 3, 26, 26);
}

// Execute the toolbar action for a given slot index
bool toolbarClick(uint8_t slot) {
  if (slot >= TBMaxOpt) return false;
  const uint8_t itemIdx = hmiData.TBopt[slot];
  if (itemIdx == 0 || itemIdx >= TBGetCount()) return false;
  auto cb = TBItemA[itemIdx].onClick;
  if (cb) { cb(); return true; }
  return false;
}

#endif // HAS_TOOLBAR
