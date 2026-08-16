/**
 * ToolBar for PROUI (Updated for Marlin ProUI v1.10.1)
 */

#include "../../../inc/MarlinConfigPre.h"

#if ALL(DWIN_LCD_PROUI, HAS_TOOLBAR)

  #include "toolbar.h"
  #include "toolbar_def.h"
  #include "menus.h"
  #include "dwinui.h"
  #include "dwin.h"

  ToolBar toolBar;

  uint8_t ToolBar::OptCount() {
    return count();
  }

  static void drawTBItem(int8_t pos, int8_t selected) {

    MenuItem *item = static_cast<MenuItem*>(Menu::items()[pos]);
    if (!item) return;

    const uint8_t tw    = 48;
    const uint8_t B_XPOS = 60;
    const uint8_t B_YPOS = 20;

    const uint8_t xoff =
      (DWIN_WIDTH - (B_XPOS * toolBar.OptCount() + tw)) / 2;

    const uint16_t xp = xoff + pos * B_XPOS;

    DWINUI::drawIcon(item->icon, xp, B_YPOS);
    DWINUI::drawString(xp + B_XPOS, B_YPOS + 1, item->caption);

    if (pos == selected) {
      DWINUI::drawBox(
        1,
        hmiData.colorCursor,
        { xp - 4, B_YPOS - 4, tw + 8, tw + 8 }
      );
    }
  }

  void drawToolBar() {

    if (currentMenu != &toolBar) {
      previousMenu = currentMenu;
      currentMenu  = &toolBar;
      resetMenu(currentMenu);
    }

    toolBar.menuTitle.draw();

    DWINUI::setColors(
      hmiData.colorText,
      hmiData.colorBackground,
      hmiData.colorStatusBg
    );

    dwinDrawRectangle(
      1,
      DWINUI::backColor,
      0,
      TITLE_HEIGHT,
      DWIN_WIDTH - 1,
      STATUS_Y - 1
    );

    for (uint8_t i = 0; i < toolBar.OptCount(); i++)
      drawTBItem(i, toolBar.selected);

    dwinUpdateLCD();
  }

  void ToolBar::onScroll(bool dir) {
    int8_t sel = selected;
    if (dir) sel++; else sel--;
    LIMIT(sel, 0, OptCount() - 1);

    if (sel != selected) {
      selected = sel;
      drawToolBar();
    }
  }

  void ToolBar::onClick() {
    MenuItem *item = static_cast<MenuItem*>(selectedItem());
    if (item && item->onClick)
      (*item->onClick)();
  }

  void exitToolBar() {
    if (previousMenu) {
      currentMenu = previousMenu;
      redrawMenu(true);
    }
  }

  void gotoToolBar() {
    setupToolBarItems();
    drawToolBar();
  }

#endif // ALL(DWIN_LCD_PROUI, HAS_TOOLBAR)
