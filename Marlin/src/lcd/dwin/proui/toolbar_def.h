/**
 * ToolBar definitions for PROUI (Updated for Marlin ProUI v1.10.1)
 */

#pragma once

#include "../../../inc/MarlinConfigPre.h"

#if ALL(DWIN_LCD_PROUI, HAS_TOOLBAR)

  #include "menus.h"
  #include "toolbar.h"

  // Handlers for toolbar items
  inline void onTBHomeOffset() {
    exitToolBar();
  }

  inline void onTBPIDNozzle() {
    exitToolBar();
  }

  inline void onTBAdvancedSettings() {
    exitToolBar();
  }

  // Setup toolbar items using the new menu engine
  inline void setupToolBarItems() {
    const int8_t totalitems = 3;
    menuItemsPrepare(totalitems);
    toolBar.menuTitle.setCaption(GET_TEXT_F(MSG_TOOLBAR_SETUP));

    menuItemAdd(ICON_HomeOffset, GET_TEXT_F(MSG_SET_HOME_OFFSETS), onDrawMenuItem, onTBHomeOffset);
    menuItemAdd(ICON_PIDNozzle, GET_TEXT_F(MSG_PID_FOR_NOZZLE), onDrawMenuItem, onTBPIDNozzle);
    menuItemAdd(ICON_AdvSet, GET_TEXT_F(MSG_ADVANCED_SETTINGS), onDrawMenuItem, onTBAdvancedSettings);
  }

#endif // ALL(DWIN_LCD_PROUI, HAS_TOOLBAR)
