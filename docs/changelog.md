## v1.8
- Compatibility Update for OFW 0.103.1-rc and above

## v1.7
- Added code refactors from xMasterX and WillyJL
- Minor update of Readme

## v1.6
- Fix crashes on exit on fw 0.100.3

## v1.5
- Support for ufbt compilation (assets fixed)

## v1.4
- Fixed some restrictions that prevented build under 0.99.1

## v1.3
- Fixed some memory leaks
- Changed Subghz Protocol Registry to Fipper API conform variant
- Version number in Start Screen
- Code formatting

## v1.2
- Fixed Memory bug in Last Station UI
- Added auto-correction when entries in First/Last station/pager are out of range

## v1.1
- Created a new UI Input View as FW does not supply one for numbers
- New UI to Set First Station
- New UI to Set Last Station
- New UI to Set First Page
- New UI to Set Last Pager
- Removed Vibro/Sound settings as not used

Known issues
- After setting last station, the settings view is weird. Some kindo of memory bug. But data remains correct. 
- Extensive use can cause crashes, must be some memory leak left


## v1.0
- Added support for TD174

## v0.9
- Added support for TD165

## v0.8
- Added Repeats feature 
- Repeats configuration in settings
- Usage of repeats in T119
- Usage of repeats in TD157

## v0.7

- Added support for TD157
- Some log & comment cleaning
- Moved common code to new location
- Removed testing assets
- Fixed bad menu label (from "scene1" to "send data")

## v0.6

- Display when data is being generated and when it is being sent
- Fixed issue where callbacks were sent infitiv when leaving the transmission page
- Fixed blinking remaining when leaving transmission page prematurely
- Set default last station Number lower to prevent crashes on first try


## v0.5

Compiled .sub data is read out and sent via SubGhz. Currently only support for T119. 

## v0.1

Can now generate a temporary .sub file for Retekess T119 Pager triggers. Must still be run via SubGhz App