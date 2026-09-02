/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */


/**
 * DWIN general defines and data structs for PRO UI
 * Based on the original work of: Miguel Risco-Castillo (MRISCOC)
 * Developer: xXHenneBXx
 * Version: 3.13.x
 * Date: 2026/07/25
 */


#pragma once
#include "../../../inc/MarlinConfigPre.h"
#include <stddef.h>
#include "../../../core/types.h"
//#include "proui.h"

//#define NEED_HEX_PRINT 1
//#define DEBUG_DWIN 1

#define TJC_DISPLAY
//#if ENABLED(TJC_DISPLAY)
//  #define TJC_DISPLAY  // Enable for TJC display
//  #elif ENABLED(DACAI_DISPLAY)
//    #define DACAI_DISPLAY       // Enable for DACAI display
//#endif	
//#define TITLE_CENTERED      // Center Menu Title Text

/**
 * Architectures
 */
#if defined(__STM32F1__) || defined(STM32F1) || defined(ARDUINO_ARCH_HC32) || defined(ARDUINO_ARCH_MFL) || MB(CREALITY_V24S1_301, CREALITY_V24S1_301F4, CREALITY_V4, CREALITY_V422_GD32_MFL, CREALITY_V422)
  #define DASH_REDRAW 1
#endif


#ifndef TBYOFFSET
  #define TBYOFFSET 20
#endif


#define HAS_FEEDRATE_EDIT 1   // Feedrate percentage
#define HAS_FLOW_EDIT 1       // Flow percentage

#if DISABLED(PROBE_MANUALLY) && ANY(AUTO_BED_LEVELING_BILINEAR, AUTO_BED_LEVELING_LINEAR, AUTO_BED_LEVELING_3POINT, AUTO_BED_LEVELING_UBL)
  #define ASSISTED_TRAMMING 1
#endif

#if !HAS_BED_PROBE && ENABLED(BABYSTEPPING)
  #define JUST_BABYSTEP 1
#endif

#if ANY(BABYSTEPPING, HAS_BED_PROBE, HAS_WORKSPACE_OFFSET)
  #define HAS_ZOFFSET_ITEM 1
#endif

#define RGB(R,G,B) ((R << 11) | (G << 5) | (B)) // R,B: 0..31; G: 0..63
#define GetRColor(color) ((color >> 11) & 0x1F)
#define GetGColor(color) ((color >>  5) & 0x3F)
#define GetBColor(color) ((color >>  0) & 0x1F)

// RGB565 colors: https://rgbcolorpicker.com/565
#define COLOR_WHITE         0xFFFF
#define COLOR_YELLOW        RGB(0x1F, 0x3F, 0x00)
#define COLOR_RED           RGB(0x1F, 0x00, 0x00)
#define COLOR_ERROR_RED     0xB000  // Error!
#define COLOR_BG_RED        0xF00F  // Red background color
#define COLOR_BG_WINDOW     0x31E8  // Popup background color
#define COLOR_BG_BLUE       0x1125  // Dark blue background color
#define COLOR_BG_BLACK      0x0841  // Black background color
#define COLOR_ICONBLUE      0x45FA  // Lighter blue that matches icons/accents
#define COLOR_POPUP_TEXT    0xD6BA  // Popup font background color
#define COLOR_LINE          0x3A6A  // Split line color//#define COLOR_RECTANGLE     0xEE2F  // Blue square cursor color
#define COLOR_PERCENT       0xFE29  // Percentage color
#define COLOR_BARFILL       0x10E4  // Fill color of progress bar
#define COLOR_SELECT        0x33BB  // Selected color

#define defColorBackground  COLOR_BG_BLACK       // Default Background color
#define defColorCursor      RGB( 0, 50, 63);    // Frame Selection color
#define defColorTitleBg     RGB( 0, 40, 53);     // Title Background color
#define defColorTitleTxt    COLOR_WHITE          // Title text color
#define defColorText        COLOR_WHITE          // Text
#define defColorSelected    RGB( 0,173,204);     // Bright cyan selected highlight
#define defColorSplitLine   RGB( 28, 38, 45);    // Divider
#define defColorHighlight   COLOR_WHITE          // Highlight Color
#define defColorStatusBg    RGB( 0, 40, 53);      // Status background color 
#define defColorStatusTxt   RGB(180,240,255); 
#define defColorPopupBg     RGB( 16, 22, 28);
#define defColorPopupTxt    COLOR_POPUP_TEXT
#define defColorAlertBg     RGB( 60,  8, 12);     // red alert background
#define defColorAlertTxt    COLOR_YELLOW
#define defColorPercentTxt  COLOR_PERCENT
#define defColorBarfill     COLOR_BARFILL
#define defColorIndicator   COLOR_WHITE
#define defColorCoordinate  COLOR_WHITE
#define defColorButton      RGB(0, 23, 16)


#if ALL(LED_CONTROL_MENU, HAS_COLOR_LEDS)
  #define defColorLeds      LEDColorWhite()
#endif
#if CASELIGHT_USES_BRIGHTNESS
  #define defCaseLightBrightness 255
#endif

#ifndef Z_AFTER_HOMING
  #define Z_AFTER_HOMING 0
#endif

#define DEF_PIDCYCLES 5

/**
 * ProUI internal feature flags
 */
#ifdef ProUI
  #if HAS_BED_PROBE
    #define PROUI_ITEM_ZFR      // Add a menu item to change Z_PROBE_FEEDRATE_SLOW - probe speed
  #endif
  #if ALL(SDCARD_SORT_ALPHA, SDSORT_GCODE)
    #define PROUI_MEDIASORT     // Enable option to sort G-code files
  #endif
  #if ENABLED(POWER_LOSS_RECOVERY)
    #define PROUI_ITEM_PLR      // Tune > Power-loss Recovery
  #endif
  #if HAS_JUNCTION_DEVIATION
    #define PROUI_ITEM_JD       // Tune > Junction Deviation
  #endif
  #if HAS_LIN_ADVANCE_K
    #define PROUI_ITEM_ADVK 1   // Tune > Linear Advance
  #endif
  #if DISABLED(DISABLE_TUNING_GRAPH)
    #define PROUI_TUNING_GRAPH 1
  #endif
  #if PROUI_TUNING_GRAPH
    #define PROUI_ITEM_PLOT     // Plot temp graph viewer
  #endif
  #define HAS_GCODE_PREVIEW 1   // Preview G-code model thumbnail
  #define HAS_CUSTOM_COLORS 1   // Change display colors
  #define HAS_ESDIAG 1          // View End-stop/Runout switch continuity
  #define HAS_LOCKSCREEN 1      // Simple lockscreen
  #define HAS_SD_EXTENDER 1     // Enable to support SD card extender cables
  #define USE_GRID_MESHVIEWER 1 // Enable for two mesh graph types

  #if HAS_PROUI_MESH_EDIT
    #define Z_OFFSET_MIN  -3.0  // (mm)
    #define Z_OFFSET_MAX   3.0  // (mm)
	#endif
  #if ENABLED(HYBRID_THRESHOLD)
    #define HYBRID_THRESHOLD_MENU // Enable Hybrid Threshold menu
  #endif
  #if HAS_STEALTHCHOP
    #define STEALTHCHOP_MENU      // Enable StealthChop menu (352 bytes)
  #endif
  #if HAS_MESH
    #define Z_OFFSET_MIN -3.0 // (mm)
    #define Z_OFFSET_MAX  3.0 // (mm)
  #endif
  #define HAS_FEEDRATE_EDIT 1 // Feedrate percentage
  #define HAS_FLOW_EDIT 1     // Flow percentage
  #define HAS_TOOLBAR 1       // Enable Toolbar
#endif