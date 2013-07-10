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

## EEPROM Erase

The Iambino settings and messages are stored in EEPROM. It some cases it may be necessary to perform a manual erasure. Holding the red button down while resetting the device will bring up the EEPROM menu. Use the black buttons to select an option and the red button to execute.

 * CANCEL - Do nothing, boot normal.
 * SETTINGS - Erase the settings but not the messages.
 * EVERYTHING - Erase everything.
 
## Basic Operation

Pressing a black button will play its message contents. Press a button or touch your key to cancel sending.

To program a message, hold a black button down until the LCD says "REC". Use your key to enter a message. Pressing a black button with stop recording and save the message. Use the red button to abort a recording.

The red button will enter and exit settings mode. The outer black buttons change the type of setting. The inner black buttons change the value of a setting. You can hold a button down to quickly move through settings or values.

## Settings

#### SPEED

Sending speed may be adjusted in this menu or with the potentiometer.

#### SPEED MIN and MAX

Adjusts the minimum and maximum speed range of the potentiometer.

#### MESSAGE BANK

There are two message banks in the Iambino. This allows you to switch between them so you can store eight messages of up to 111 characters each.

#### MODE

Choose from Iambic, Ultimatic, Bug, or Straight keying. Note that you can not program messages in straight key mode. The bug mode is useful for tuning your antenna when you need to transmit a long tone.

#### MEMORY

Memory type determines if new dots and dashes are looked at before the current element finishes sending. With Type B you send the letter N by pressing dash then dot as fast as you want. With Type A (no memory) you send an N by pressing dash then holding dot until it begins sending. You may also choose to only have memory on either dots or dashes.

#### SPACING

Spacing defines how the keyer will enforce the off time between elements, characters, and words. The character and word settings are useful tools to help you learn better spacing.

#### WEIGHTING

Weighting refers to the balance between the on and off time of a dot. Normally, a series of dots is exactly balanced 50% on and 50% off. If you want a heavier or lighter sound, adjust this to your preference.

#### PADDLE

Normally, a key is wired with the dots(left) on the tip of the plug and dashes(right) on the ring. You can reverse this in software so you don't have to rewire your key or want to share with someone who sends the other direction.

#### TX LAG

Some transmitters have a small delay between the time they are activated and when the transmission actually begins. Use this setting to lengthen the time the transmitter is activated for a elements. This is a constant time added to every element unlike weighting which will adjust based on the sending speed.

#### TONE

Frequency of the sidetone generator. The included speaker is resonant around 1300Hz. It will be significantly quieter at other frequencies. Larger speakers and headphones will have a flatter frequency response.

#### VOLUME

Loudness of the sidetone generator.

#### TX SIDETONE

Determines if the sidetone generator operates during transmissions. You can turn this off to use the sidetone in your transceiver. When off, the sidetone will only play while recording messages.

#### BACKLIGHT

Brightness of the LCD backlight
