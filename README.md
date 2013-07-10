# Iambino - Iambic Keyer for Arduino

Coming soon from http://AE9RB.com/

## Requirements

Iambino will only run on Arduino hardware containing an ATmega328 or ATmega32u. The Uno, Leonardo, and later models of the Duemilanove are compatible. This is the most common type of Arduino so you should not have trouble finding one.

Iambino requires Arduino 1.0 or higher. If you have older software then you will need to upgrade.

## Software Install

WARNING: You must not install the shield until the Iambino sketch is loaded on your Arduino. This is valid advice whenever you change any shield on your Arduino.

Begin by following the official Getting Started for Arduino. Once you have the LED blinking from this guide you are ready to install Iambino.

 http://arduino.cc/en/Guide/HomePage
 
Obtain the Iambino sketch by downloading the ZIP file from GitHub. Simply use the "Download ZIP" button to get the latest version.

 https://github.com/AE9RB/iambino

Unzip the contents on your hard drive and rename the folder to "iambino". The ZIP file will probably unzip to a folder called "iambino-master" which will cause problems so you must rename it. Once you have the correct folder name, open "iambino.ino" in Arduino using the "Open..." option in the "File" menu. 

Next, use the "Upload" option in the file menu to program the hardware. If you don't get any errors then you can proceed to installing the shield with the power disconnected. If it boots to the LCD saying "IAMBINO" then you are done.

## EEPROM Reset

The Iambino settings and messages are stored in EEPROM. It some cases it may be necessary to manually force a reset. Holding the red button down while resetting the device will bring up a reset menu. Use the black buttons to select an option and the red button to execute.

 * CANCEL - Do nothing, boot normal.
 * SETTINGS - Erase the settings but not the messages.
 * EVERYTHING - Erase everything.
 
## Basic Operation

...more documentation coming soon...

