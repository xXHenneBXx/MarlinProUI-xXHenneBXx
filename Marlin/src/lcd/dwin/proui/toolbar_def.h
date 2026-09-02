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
    #define _MAP_PREHEAT TB_ITEM(ICON_PLAPreheat, MSG_PREHEAT_1, doPreheat1),
  #elif PREHEAT_COUNT >= 2
    #define _MAP_PREHEAT TB_ITEM(ICON_PLAPreheat, MSG_PREHEAT_1, doPreheat1), TB_ITEM(ICON_ABSPreheat, MSG_PREHEAT_2, doPreheat2),
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
// Note: All callback functions (autoHome, etc.) are declared in dwin.h
// which is included via the include chain above.

const TBItem_t TBItemA[] = {
  { 0, nullptr, nullptr },  // Index 0 = empty slot sentinel
  TB_ITEM(ICON_Homing, MSG_AUTO_HOME, autoHome),                   // [1] Auto Home
  TB_ITEM(ICON_SetZOffset, MSG_PROBE_WIZARD, drawZOffsetWizMenu),  // [2] Z Probe Wizard
  #if ENABLED(LCD_BED_TRAMMING)
    TB_ITEM(ICON_Tram, MSG_TRAMMING_WIZARD, trammingWizard),       // [3] Tramming Wizard
  #endif
  TB_ITEM(ICON_CloseMotor, MSG_DISABLE_STEPPERS, disableMotors),   // [4]Disable Steppers
  #if HAS_PREHEAT
    TB_ITEM(ICON_PLAPreheat, MSG_PREHEAT_1, doPreheat1), 
    TB_ITEM(ICON_ABSPreheat, MSG_PREHEAT_2, doPreheat2),          // [5]Preheat PLA
  #endif
  TB_ITEM(ICON_Cool, MSG_COOLDOWN, doCoolDown),                    // [6]Cooldown
  //_MAP_BRIGHTNESS
  TB_ITEM(ICON_Reboot, MSG_RESET_PRINTER, rebootPrinter),          // Reboot Printer
};
