/**
 * toolBar for PRO UI
 * Author: Miguel A. Risco-Castillo (MRISCOC)
 * version: 3.2.0
 * Date: 2023/09/12
 */

#pragma once

#include "../../../inc/MarlinConfig.h"

#include "dwin.h"
#include "toolbar.h"

// Custom macros to handle language keys and literal strings seamlessly
#define TB_ITEM(I,L,V...) {I, GET_TEXT_F(L), V}
#define TB_ITEM_STR(I,S,V...) {I, F(S), V}

// --- Define sub-macros FIRST so the compiler knows they exist ---

#if HAS_PREHEAT
  #if PREHEAT_COUNT == 1
    #define _MAP_PREHEAT TB_ITEM(ICON_Preheat1, MSG_PREHEAT_1, doPreheat1),
  #elif PREHEAT_COUNT == 2
    #define _MAP_PREHEAT TB_ITEM(ICON_Preheat1, MSG_PREHEAT_1, doPreheat1), TB_ITEM(ICON_Preheat2, MSG_PREHEAT_2, doPreheat2),
  #elif PREHEAT_COUNT == 3
    #define _MAP_PREHEAT TB_ITEM(ICON_Preheat1, MSG_PREHEAT_1, doPreheat1), TB_ITEM(ICON_Preheat2, MSG_PREHEAT_2, doPreheat2), TB_ITEM(ICON_Preheat3, MSG_PREHEAT_3, doPreheat3),
  #else
    #define _MAP_PREHEAT TB_ITEM(ICON_Preheat1, MSG_PREHEAT_1, doPreheat1), TB_ITEM(ICON_Preheat2, MSG_PREHEAT_2, doPreheat2), TB_ITEM(ICON_Preheat3, MSG_PREHEAT_3, doPreheat3), TB_ITEM(ICON_Preheat4, MSG_PREHEAT_4, doPreheat4),
  #endif
#else
  #define _MAP_PREHEAT
#endif

#if HAS_LCD_BRIGHTNESS
  #define _MAP_BRIGHTNESS TB_ITEM(ICON_Brightness, MSG_BRIGHTNESS_OFF, turnOffBacklight),
#else
  #define _MAP_BRIGHTNESS
#endif

// --- Now define the array using those macros ---
// Note: All callback functions (autoHome, trammingwizard, etc.) are declared in dwin.h
// which is included via the include chain above.

const TBItem_t TBItemA[] = {
  TB_ITEM(0, MSG_OPTION_DISABLED, nullptr),
  TB_ITEM(ICON_Homing, MSG_AUTO_HOME, autoHome),
  #if HAS_BED_PROBE
    TB_ITEM(ICON_SetZOffset, MSG_PROBE_WIZARD, drawZOffsetWizMenu),
    TB_ITEM(ICON_Probe, MSG_AUTO_MESH, autoLevel),
    #if HAS_TRAMMING_WIZARD
      TB_ITEM(ICON_BedTramming, MSG_TRAMMING_WIZARD, trammingwizard),
    #endif
  #elif HAS_TRAMMING_WIZARD
    TB_ITEM(ICON_BedTramming, MSG_TRAMMING_WIZARD, trammingwizard),
  #elif !HAS_BED_PROBE
    TB_ITEM_STR(ICON_MoveZ0, "Home Z and disable", homeZAndDisable),
  #endif
  TB_ITEM(ICON_CloseMotor, MSG_DISABLE_STEPPERS, disableMotors),
  TB_ITEM(ICON_Cool, MSG_COOLDOWN, doCoolDown),
  _MAP_PREHEAT
  _MAP_BRIGHTNESS
  TB_ITEM(ICON_Reboot, MSG_RESET_PRINTER, rebootPrinter),
  TB_ITEM(ICON_WriteEEPROM, MSG_STORE_EEPROM, writeEEPROM),
  TB_ITEM(ICON_Park, MSG_FILAMENT_PARK_ENABLED, parkHead)
};
