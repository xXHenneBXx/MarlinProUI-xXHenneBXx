#PRO FIRMWARE

## Features
- **CVLM** - Creative CV laser module 
- **IS** - Input Shaping
- **LA** - Linear Advance  
- **MPC** - Model Predictive Temperature Control
- **NP** - Support for Neopixel  
- **Speaker** - Support for speaker tones
- **MM** - Firmware for Manual Mesh, use it if you don't have a probe.  
- **BLT** - BL/3D/CR Touch probe support.  
- **BQMPv2** - Biqu microprove v2
- **UBL** - Unified Bed Leveling

## Board nomenclature

- **422** - Creality board v4.2.2
- **427** - Creality board v4.2.7
- **301F1** - Creality board V2.4.S1.301 with STM32F103
- **301F4** - Creality board V2.4.S1.301 with STM32F401
- **SKRME3V3** - BIGTREETECH SKR Mini E3 V3 


## Supported Displays
- **CR10DSPLY** - Former monochrome LCD display 12864  
- **DACAI/DWIN/SYNWIT/TJC** - Color screen with knob encoder  


## Thermistor nomenclature

- **T1** - Creality stock temperature sensor
- **T5** - Marlin #5 (Volcano-style)
- **T13** - Marlin #13 (Sprite Pro, 100k Hisens 3950, up to 300C)
- **T20** - Marlin #20

### What was added (pultrusion-specific)
- **PETPreset**: PET/PETE optimized temperature and PID settings (240-290C range)
- **PullTune**: Pultrusion motion tuning for slow, steady filament pull
- **DualDrive**: Dual drive gear extruder settings (BMG-style 3:1 ratio)

## Running the Configurator

Execute the Python GUI:

```bash
python Configurator.pyw
```

> On Windows, `.pyw` files should be opened by pythonw.exe by default.

Select the printer, board, display, thermistor, and features, then press **Set Config** to preview the generation command. Enter a name (or press **Auto Name**), then press **Generate** to create the configuration files.

## Creating configurations programmatically

To create Ender3V2 Configuration files with a BLTouch and UBL support it is easy to write a little Python script to call the above function:

```Python
#!/usr/bin/python
import CreateConfigs
CreateConfigs.Generate('Ender3V2-422-BLTUBL', ['Ender3V2','422','BLT','UBL'])
```


## Custom features

Advanced users can create custom JSON configs in the `_features` folder:

```json
{
"Configuration.h" : [
  {
    "op": "CustomVal",
    "searchfor": "TEMP_SENSOR_0",
    "value": "13",
    "comment": "PET thermistor"
  }
],
"Configuration_adv.h" : [],
"Version.h" : []
}
```

Supported operations:
- **InsertAfter**: Insert text after matching a pattern
- **Custom**: Replace text after matching a mask (regex)
- **CustomVal**: Replace simple values (numbers, booleans)
- **Enable**: Enable a feature
- **Disable**: Disable a feature
- **Replace**: Replace a pattern with other text

For example to change the default tramming points you can write in the "Configuration_adv.h" section of the json the command:
```json
  {
    "op": "Custom",
    "searchfor": "TRAMMING_POINT_XY",
    "mask": "{.*}",
    "value":"{ { 29, 29 }, { 299, 29 }, { 299, 299 }, { 29, 299 } }"
  }
```

For disable Multiple probing you can write in the "Configuration.h" section of the json the command:
```json
  {
    "op": "Disable",
    "searchfor": "MULTIPLE_PROBING",
    "comment": "Custom disable"
  }
```
The comment line is optional. Masks are in regex format, use the provided json as examples.

## Compiling the firmware

Move the generated `Version.h`, `Configuration.h`, `Configuration_adv.h` to the Marlin folder, and `platformio.ini` to the project root. Compile using VSCode with PlatformIO and Auto Build Marlin.



## Credits

- Updated/Enhanced By xXHenneBXx
- Derived from **Professional Firmware** by MRiscoC
- Marlin is licensed under GPL

## Disclaimer

THIS FIRMWARE AND ALL OTHER FILES ARE PROVIDED FREE OF CHARGE WITH NO WARRANTY OR GUARANTEE. WE ARE NOT LIABLE FOR ANY DAMAGE TO YOUR EQUIPMENT, PERSON, OR PROPERTY DUE TO USE OF THIS FIRMWARE.
