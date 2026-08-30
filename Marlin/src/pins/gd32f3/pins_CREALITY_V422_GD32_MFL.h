/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2025 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 *
 * Creality V4.2.2 MFL Board Pin Definitions
 * Developer/Contributor: xXHenneBXx
 */

#pragma once
#include "env_validate.h"

#define ALLOW_GD32F3

#define BOARD_INFO_NAME      "Creality V4.2.2 MFL"
#define DEFAULT_MACHINE_NAME "Ender-3 MFL"

#define BOARD_NO_NATIVE_USB   // No native USB — host link is the CH340 on hardware USART0 (PA9/PA10)

// Release PB4 (also E0_STEP_PIN here) from its default JTAG NRST role.
// Datasheet Table 2-6: PB3 default function is JTDO, PB4 default is NJTRST.
// SWD (PA13=SWDIO, PA14=SWCLK) remains enabled for debugging/flashing.
#ifndef DISABLE_DEBUG
  #define DISABLE_DEBUG
#endif

#if !E_ERROR && (HAS_MULTI_HOTEND || E_STEPPERS > 1)
  #error "Creality v4 only supports 1 hotend / E stepper."
#endif

//
// EEPROM — onboard 24C16 (2K). PA11/PA12 have no hardware I2C alternate
// function on this chip (datasheet: PA11 alt = USART0_CTS/CAN0_RX/USBDM/
// TIMER0_CH3; PA12 alt = USART0_RTS/CAN0_TX/TIMER0_ETI/USBDP), so this runs
// as bit-banged I2C, matching standard Creality board practice.
//
#if NO_EEPROM_SELECTED
  #define IIC_BL24CXX_EEPROM
  //#define SDCARD_EEPROM_EMULATION
  #undef NO_EEPROM_SELECTED
#endif

#if ENABLED(IIC_BL24CXX_EEPROM)
  #ifndef IIC_EEPROM_SDA
    #define IIC_EEPROM_SDA                  PA11  // Bit-banged — no hardware I2C AF on this pin
  #endif
  #ifndef IIC_EEPROM_SCL
    #define IIC_EEPROM_SCL                  PA12  // Bit-banged — no hardware I2C AF on this pin
  #endif
  #define MARLIN_EEPROM_SIZE              0x800U  // 2K (24C16)
#elif ENABLED(SDCARD_EEPROM_EMULATION)
  #define MARLIN_EEPROM_SIZE              0x800U  // 2K
#endif

//
// Servos
//
#ifndef SERVO0_PIN
  #if DISABLED(USE_PIN_27_BOARD)
    #define SERVO0_PIN                      PB0   // BLTouch OUT — Default: PB0; Alt: ADC01_IN8, TIMER2_CH2, TIMER7_CH1_ON
  #else
    #define SERVO0_PIN                      PC6
  #endif
#endif

//
// Limit Switches — confirmed against Klipper's official 4.2.2/Neo config
// (endstop_pin: ^PA5 / ^PA6 for X/Y; Z uses BLTouch virtual endstop)
//
#ifndef X_STOP_PIN
  #define X_STOP_PIN                        PA5   // Default: PA5; Alt: SPI0_SCK, ADC01_IN5, DAC_OUT1 (used as GPIO)
#endif
#ifndef Y_STOP_PIN
  #define Y_STOP_PIN                        PA6   // Default: PA6; Alt: SPI0_MISO, ADC01_IN6, TIMER2_CH0, TIMER7_BRKIN (used as GPIO)
#endif
#ifndef Z_STOP_PIN
  #define Z_STOP_PIN                        PA7   // Default: PA7; Alt: SPI0_MOSI, ADC01_IN7, TIMER2_CH1, TIMER7_CH0_ON (used as GPIO)
#endif

#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                   PB1   // BLTouch IN — Default: PB1; Alt: ADC01_IN9, TIMER2_CH3, TIMER7_CH2_ON
#endif

#if ENABLED(PROBE_ENABLE_DISABLE) && !defined(PROBE_ENABLE_PIN)
  #define PROBE_ENABLE_PIN            SERVO0_PIN
#endif

//
// Filament Runout Sensor
//
#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN                    PA4   // "Pulled-high" — Default: PA4; Alt: SPI0_NSS, USART1_CK, ADC01_IN4, DAC_OUT0
#endif

//
// Steppers — every pin below matches Klipper's official printer-creality-
// ender3-v2-neo-2022.cfg exactly (step/dir/enable for X/Y/Z/E0), which
// explicitly states it targets this same GD32F303 4.2.2 board.
//
#ifndef X_STEP_PIN
  #define X_STEP_PIN                        PC2   // Default: PC2; Alt: ADC012_IN12 (used as GPIO)
#endif
#ifndef X_DIR_PIN
  #define X_DIR_PIN                         PB9   // Default: PB9; Alt: TIMER3_CH3, SDIO_D5 (8-bit SDIO only — unused, board runs 4-bit); Remap: I2C0_SDA, CAN0_TX
#endif
#define X_ENABLE_PIN                        PC3   // Shared — Default: PC3; Alt: ADC012_IN13 (used as GPIO)

#ifndef Y_STEP_PIN
  #define Y_STEP_PIN                        PB8   // Default: PB8; Alt: TIMER3_CH2, SDIO_D4 (8-bit SDIO only — unused); Remap: I2C0_SCL, CAN0_RX
#endif
#ifndef Y_DIR_PIN
  #define Y_DIR_PIN                         PB7   // Default: PB7; Alt: I2C0_SDA, TIMER3_CH1; Remap: USART0_RX, SPI0_IO3
#endif
#define Y_ENABLE_PIN                X_ENABLE_PIN

#ifndef Z_STEP_PIN
  #define Z_STEP_PIN                        PB6   // Default: PB6; Alt: I2C0_SCL, TIMER3_CH0; Remap: USART0_TX, SPI0_IO2
#endif
#ifndef Z_DIR_PIN
  #define Z_DIR_PIN                         PB5   // Default: PB5; Alt: I2C0_SMBA, SPI2_MOSI, I2S2_SD; Remap: TIMER2_CH1, SPI0_MOSI
#endif
#define Z_ENABLE_PIN                X_ENABLE_PIN

// PB4/PB3 default to JTAG (NJTRST / JTDO) — only usable as GPIO because
// DISABLE_DEBUG is set above. SWD (PA13/PA14) remains enabled.
#ifndef E0_STEP_PIN
  #define E0_STEP_PIN                       PB4   // Default: NJTRST; Alt: SPI2_MISO; Remap: TIMER2_CH0, PB4, SPI0_MISO
#endif
#ifndef E0_DIR_PIN
  #define E0_DIR_PIN                        PB3   // Default: JTDO; Alt: SPI2_SCK, I2S2_CK; Remap: PB3, TRACESWO, TIMER1_CH1, SPI0_SCK
#endif
#define E0_ENABLE_PIN               X_ENABLE_PIN

//
// Temperature Sensors — confirmed against Klipper config (sensor_pin PC5/PC4)
//
#define TEMP_0_PIN                          PC5   // TH1 — Default: PC5; Alt: ADC0/ADC1_IN15
#define TEMP_BED_PIN                        PC4   // TB1 — Default: PC4; Alt: ADC0/ADC1_IN14

//
// Heaters / Fans — confirmed against Klipper config (heater_pin PA1/PA2, fan pin PA0)
//
#ifndef HEATER_0_PIN
  #define HEATER_0_PIN                      PA1   // HEATER1 — Default: PA1; Alt: USART1_RTS, ADC012_IN1, TIMER1_CH1, TIMER4_CH1
#endif
#ifndef HEATER_BED_PIN
  #define HEATER_BED_PIN                    PA2   // HOT BED — Default: PA2; Alt: USART1_TX, ADC012_IN2, TIMER1_CH2, TIMER4_CH2, TIMER8_CH0, SPI0_IO2
#endif
#ifndef FAN0_PIN
  #define FAN0_PIN                          PA0   // FAN — Default: PA0; Alt: WKUP, USART1_CTS, ADC012_IN0, TIMER1_CH0/ETI, TIMER4_CH0, TIMER7_ETI
#endif
#define FAN_SOFT_PWM_REQUIRED                      // GD32_MFL HAL: fans run via software PWM

//
// SD Card — SD_DETECT_PIN corrected to PC7 (matches real upstream file;
// PA10 as originally drafted collides with USART0_RX, the host serial pin)
//
#define SD_DETECT_PIN                       PC7   // Default: PC7; Alt: I2S2_MCK, TIMER7_CH1, SDIO_D7 (8-bit SDIO only — unused, used as GPIO input)
#define SDCARD_CONNECTION ONBOARD
#define ONBOARD_SDIO
#define NO_SD_HOST_DRIVE                           // SD card only seen by the printer

#define BOARD_SDIO_D0                        PC8   // Default: PC8; Alt: TIMER7_CH2, SDIO_D0; Remap: TIMER2_CH2
#define BOARD_SDIO_D1                        PC9   // Default: PC9; Alt: TIMER7_CH3, SDIO_D1; Remap: TIMER2_CH3
#define BOARD_SDIO_D2                        PC10  // Default: PC10; Alt: UART3_TX, SDIO_D2; Remap: USART2_TX, SPI2_SCK, I2S2_CK
#define BOARD_SDIO_D3                        PC11  // Default: PC11; Alt: UART3_RX, SDIO_D3; Remap: USART2_RX, SPI2_MISO
#define BOARD_SDIO_CLK                       PC12  // Default: PC12; Alt: UART4_TX, SDIO_CK; Remap: USART2_CK, SPI2_MOSI, I2S2_SD
#define BOARD_SDIO_CMD                       PD2   // Default: PD2; Alt: TIMER2_ETI, SDIO_CMD, UART4_RX

/**
 * Debug port
 *       -----
 *       | 1 | VCC
 *       | 2 | PA13   (SWDIO)
 *       | 3 | PA14   (SWCLK)
 *       | 4 | GND
 *       -----
 */
#define DEBUG_02_PIN                        PA13
#define DEBUG_03_PIN                        PA14

//
// Hardware UART pins — for documentation and sanity checks only; changing
// these does not move the peripheral, since GD32F303's USART/UART pin pairs
// are fixed unless AFIO-remapped. GD32 MFL numbers UARTs from 0; the classic
// STM32 HAL numbers them from 1 — both listed since either may be relevant
// depending on which HAL this build actually compiles against.
//
//#ifdef ARDUINO_ARCH_MFL                             // GD32 MFL HAL numbering (UART0-4)
  //#define UART0_TX_PIN                      PA9    // Host serial (CH340) — confirmed via Klipper's official config for this board
  //#define UART0_RX_PIN                      PA10   // Host serial (CH340) — confirmed; do NOT also use PA10 for SD_DETECT
  //#define UART1_TX_PIN                      PA2    // Shares a pin with HEATER_BED_PIN — not usable simultaneously
  //#define UART1_RX_PIN                      PA3    // Not connected on this board
  //#define UART2_TX_PIN                      PB10   // TJC display link — broken out at EXP3 pin 3 (see EXP3 block below)
  //#define UART2_RX_PIN                      PB11   // TJC display link — broken out at EXP3 pin 4
  //#define UART3_TX_PIN                      PC10   // Shares a pin with SDIO_D2 — not usable simultaneously while SDIO is active
  //#define UART3_RX_PIN                      PC11   // Shares a pin with SDIO_D3 — not usable simultaneously while SDIO is active
  //#define UART4_TX_PIN                      PC12   // Shares a pin with SDIO_CK — not usable simultaneously while SDIO is active
  //#define UART4_RX_PIN                      PD2    // Shares a pin with SDIO_CMD — not usable simultaneously while SDIO is active
//#else                                               // Classic STM32 HAL numbering (UART1-5)
  //#define UART1_TX_PIN                      PA9    // Host serial (CH340)
  //#define UART1_RX_PIN                      PA10   // Host serial (CH340)
  //#define UART2_TX_PIN                      PA2    // Shares a pin with HEATER_BED_PIN
  //#define UART2_RX_PIN                      PA3    // Not connected on this board
  //#define UART3_TX_PIN                      PB10   // TJC display link — EXP3 pin 3
  //#define UART3_RX_PIN                      PB11   // TJC display link — EXP3 pin 4
  //#define UART4_TX_PIN                      PC10   // Shares a pin with SDIO_D2
  //#define UART4_RX_PIN                      PC11   // Shares a pin with SDIO_D3
  //#define UART5_TX_PIN                      PC12   // Shares a pin with SDIO_CK
  //#define UART5_RX_PIN                      PD2    // Shares a pin with SDIO_CMD
//#endif

/**
 * ============================================================================
 * TJC / DWIN Display — EXP3 connector
 * ============================================================================
 * CORRECTED: pins 3/4 are PB10/PB11 (real hardware UART2, GD32 MFL numbering)
 * — NOT PC0/PC1 as in the original draft. PC0/PC1 have no UART alternate
 * function at all on this chip and don't appear anywhere in the real
 * upstream pins_CREALITY_V4.h; the display link is a genuine hardware
 * peripheral, not bit-banged serial. Confirmed two ways:
 *   1) Upstream Marlin's reviewed pins_CREALITY_V4.h uses PB10/PB11 for
 *      EXP3_03_PIN/EXP3_04_PIN under RET6_12864_LCD / HAS_DWIN_E3V2.
 *   2) Klipper's official config for this exact board documents "serial
 *      (on USART3 PB11/PB10) ... broken out on the 10 pin IDC cable used
 *      for the LCD module as follows: 3: Tx, 4: Rx" — matching pin-for-pin.
 *
 *        ------
 *  PC6  | 1  2 | PB2
 *  PB10 | 3  4 | PB11
 *  PB14   5  6 | PB13
 *  PB12 | 7  8 | PB15
 *  GND  | 9 10 | +5V
 *        ------
 *         EXP3
 *
 *   PC6  - Default: PC6; Alt: I2S1_MCK, TIMER7_CH0, SDIO_D6 (8-bit SDIO only, unused); Remap: TIMER2_CH0
 *   PB2  - Default: PB2, BOOT1 strap pin — sampled only at reset/boot; safe to reuse as GPIO
 *          afterward as long as its reset-time level matches your board's intended boot mode
 *   PB10 - TJC/display serial TX (hardware UART2) — Default: PB10; Alt: I2C1_SCL, USART2_TX; Remap: TIMER1_CH2
 *   PB11 - TJC/display serial RX (hardware UART2) — Default: PB11; Alt: I2C1_SDA, USART2_RX; Remap: TIMER1_CH3
 *   PB14 - Default: PB14; Alt: SPI1_MISO, USART2_RTS, TIMER0_CH1_ON, TIMER11_CH0
 *   PB13 - Default: PB13; Alt: SPI1_SCK, USART2_CTS, TIMER0_CH0_ON, I2S1_CK
 *   PB12 - Default: PB12; Alt: SPI1_NSS, I2C1_SMBA, USART2_CK, TIMER0_BRKIN, I2S1_WS
 *   PB15 - Default: PB15; Alt: SPI1_MOSI, TIMER0_CH2_ON, I2S1_SD, TIMER11_CH1
 */
#if ANY(RET6_12864_LCD, HAS_DWIN_E3V2, IS_DWIN_MARLINUI, TJC_DISPLAY)

  #ifndef EXP3_01_PIN
    #define EXP3_01_PIN                     PC6
  #endif
  #ifndef EXP3_02_PIN
    #define EXP3_02_PIN                     PB2
  #endif
  #ifndef EXP3_03_PIN
    #define EXP3_03_PIN                     PB10  // TJC TX — CORRECTED from PC0
  #endif
  #ifndef EXP3_04_PIN
    #define EXP3_04_PIN                     PB11  // TJC RX — CORRECTED from PC1
  #endif
  #ifndef EXP3_05_PIN
    #define EXP3_05_PIN                     PB14  // Encoder
  #endif
  #ifndef EXP3_06_PIN
    #define EXP3_06_PIN                     PB13  // Beeper — confirmed via Klipper's [output_pin beeper] pin: PB13
  #endif
  #ifndef EXP3_07_PIN
    #define EXP3_07_PIN                     PB12  // Encoder Button 2
  #endif
  #ifndef EXP3_08_PIN
    #define EXP3_08_PIN                     PB15  // Encoder Button 1
  #endif

#elif ANY(CR10_STOCKDISPLAY, FYSETC_MINI_12864_2_1)
  #error "Define RET6_12864_LCD, HAS_DWIN_E3V2, IS_DWIN_MARLINUI, or TJC_DISPLAY to select pins for the LCD with this board."
#endif

#if ANY(HAS_DWIN_E3V2, IS_DWIN_MARLINUI, TJC_DISPLAY)

  #ifndef BEEPER_PIN
    #define BEEPER_PIN               EXP3_06_PIN
  #endif

  #define BTN_ENC                    EXP3_05_PIN  // ENC
  #define BTN_EN1                    EXP3_08_PIN  // Encoder A
  #define BTN_EN2                    EXP3_07_PIN  // Encoder B

  // TJC serial link — real hardware UART2 (PB10 TX / PB11 RX), not bit-banged.
  // Which Marlin "serial port index" this maps to depends on how the active
  // HAL's Serial objects are declared (GD32_MFL vs classic STM32) — confirm
  // against that HAL's variant/serial setup rather than assuming a fixed
  // index; upstream's own pins_CREALITY_V4.h does not hardcode one here.
  //#ifndef TJC_TX_PIN
    //#define TJC_TX_PIN                EXP3_03_PIN  // PB10
  //#endif
  //#ifndef TJC_RX_PIN
    //#define TJC_RX_PIN                EXP3_04_PIN  // PB11
  //#endif

#elif ENABLED(CR10_STOCKDISPLAY)

  #if DISABLED(USE_PIN_27_BOARD)
    #define BEEPER_PIN               EXP3_01_PIN
  #endif

  #define BTN_ENC                    EXP3_02_PIN
  #define BTN_EN1                    EXP3_03_PIN
  #define BTN_EN2                    EXP3_05_PIN

  #define LCD_PINS_RS                EXP3_07_PIN
  #define LCD_PINS_EN                EXP3_08_PIN
  #define LCD_PINS_D4                EXP3_06_PIN

#endif // Creality V4.2.2 MFL