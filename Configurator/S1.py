#!/usr/bin/env python3

# ------------------------------------------------------------------------------
# Ender3S1 Configurations generator script
# Author: xXHenneBXx
# URL: https://github.com/xXHenneBXx/Pultrusion
# ------------------------------------------------------------------------------
import CreateConfigs

CreateConfigs.Generate('Ender3S1-F1-MM', ['Ender3S1','301F1','MM'])
CreateConfigs.Generate('Ender3S1-F4-MM', ['Ender3S1','301F4','MM'])
CreateConfigs.Generate('Ender3S1-F1', ['Ender3S1','301F1','BLT'])
CreateConfigs.Generate('Ender3S1-F1-UBL', ['Ender3S1','301F1','BLT','UBL'])

# FreeFlash.json is a custom made JSON file where you must write
# the features you want to disable to make room for mayor features
CreateConfigs.Generate('Ender3S1-F4', ['Ender3S1','301F4','BLT','FreeFlash'])
CreateConfigs.Generate('Ender3S1-F4-UBL', ['Ender3S1','301F4','BLT','UBL','FreeFlash'])
