/**
 * toolBar for PRO UI
 * Author: Miguel A. Risco-Castillo (MRISCOC)
 * version: 3.2.0
 * Date: 2023/09/12
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://gnu.org>.
 *
 */

#pragma once

#include "../../../inc/MarlinConfigPre.h"

#include "dwin.h"

#define TBHEIGHT 28
#define TBYOFFSET 20
#define TBYPOS (STATUS_Y - (TBHEIGHT + 5))
#define B_YPOS (TBYPOS + 5)
#define B_XPOS 24

// Maximum number of toolbar slots visible on screen
constexpr uint8_t TBMaxOpt = 5;

typedef struct {
  uint8_t icon = 0;
  FSTR_P caption = nullptr;
  void (*onClick)() = nullptr;
} TBItem_t;

extern const TBItem_t TBItemA[];

// Returns the total number of available toolbar item definitions
uint8_t TBGetCount();

// Draw the toolbar icon row at the bottom of the screen (overlay on main menu)
void drawToolBar();

// Execute the toolbar action for a given slot index (0..TBMaxOpt-1)
bool toolbarClick(uint8_t slot);

// Toolbar setup menu (implemented in dwin.cpp)
void drawTBSetupMenu();
