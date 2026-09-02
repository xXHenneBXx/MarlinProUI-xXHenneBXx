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


/// TODO: Put macros into macros.h 
// Custom macros to handle language keys and literal strings seamlessly
#define TB_ITEM(I,L,V...) {I, GET_TEXT_F(L), V}
#define TB_ITEM_STR(I,S,V...) {I, F(S), V}

// Sub-macros definitions for toolbar

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
  #define _MAP_BRIGHTNESS TB_ITEM(ICON_Backlight, MSG_BRIGHTNESS_OFF, turnOffBacklight),
#else
  #define _MAP_BRIGHTNESS
#endif

const TBItem_t TBItemA[] = {
  { 0, nullptr, nullptr },  // Index 0 = empty slot sentinel
  TB_ITEM(ICON_Homing, MSG_AUTO_HOME, autoHome),                   // [1] Auto Home
  TB_ITEM(ICON_SetZOffset, MSG_PROBE_WIZARD, drawZOffsetWizMenu),  // [2] Z Probe Wizard
  #if ENABLED(LCD_BED_TRAMMING)
    TB_ITEM(ICON_Tram, MSG_TRAMMING_WIZARD, trammingWizard),       // [3] Tramming Wizard
  #endif
  TB_ITEM(ICON_CloseMotor, MSG_DISABLE_STEPPERS, disableMotors),   // [4]Disable Steppers
  #if HAS_PREHEAT
    TB_ITEM(ICON_PLAPreheat, MSG_PREHEAT_1, doPreheat1),           // [5]Preheat PLA
    TB_ITEM(ICON_ABSPreheat, MSG_PREHEAT_2, doPreheat2),           // [6]Preheat ABS
  #endif
  TB_ITEM(ICON_Cool, MSG_COOLDOWN, doCoolDown),                    // [7]Cooldown
  //_MAP_BRIGHTNESS                                                // [8]Backlight
  TB_ITEM(ICON_Reboot, MSG_RESET_PRINTER, rebootPrinter),          // [9]Reboot Printer
};
