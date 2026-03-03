# onir

onir: manage dials, switches, and servos over I2C (uses Arduino's Wire library)

# lab: dials & displays

* First: Write the *name of your lab partner* in your notebook.

In this lab, you and your parner have the goal of setting up a message to scroll across multiple screens, with multiple dials controlling it. You'll work up to this goal by running simpler scripts that will read a single local dial, drive a single local display, and run and combine these devices over a two-device network.

## install the library

This software (onir) is an *Arduino library*, so it has to be located in just the right place relative to your Arduino sketchbook location. By default on our Linux Mint workstations, the default sketchbook location is the directory ```~/Arduino/```, and the onir library will have to be at ```~/Arduino/libraries/onir```.

To get this set up right:

1. *Install Arduino IDE:*
   * Log on to your workstation account.
   * Make sure that you have the Arduino IDE installed.
   * Check that you can upload the ```File > Examples > Basics > Blink``` sketch.
   * If pushing from the IDE isn't working:
      1. make sure you're in the 'dialout' login group,
      2. try a different microcontroller, then
      3. check with dr. kraemer. (He may need to reboot your workstation, or assign you to a different one for this lab.)

2. *Install onir:*

   Open the terminal (the black window with the green text where you can type commands to the computer). Run these three commands (commands must be precise --- copy-paste or type exactly):
    * ```mkdir Arduino/libraries```
    * ```cd Arduino/libraries```
    * ```git clone https://github.com/dr-kraemer-everett/onir```

3. *Update onir:*

   Run the command ```git pull```. This will pull down any updates into to your copy of onir.

4. *Run onir:*

   Restart the IDE. At the *very bottom* of ```File > Examples``` you should see the option ```onir_device_interface```. If it's not present, check your install setup -- you probably have onir in the wrong place. You can remove it with the command ```rm -r onir``` (be careful with the command ```rm -r`` though! it doesn't ask twice, and can do a lot of damage in the wrong situation!.)

## set up hardware

You'll need a pair of boards with ko lab's basic IO setup. (Example board setups are on display.) The Uno IO setup has:
   * a display attached to pins 2 through 14,
   * a dial attached to pins A1-A3, and,
   * an I2C pairing cable attached to the VIN/GND pair and the A4/A5 pair.

### cable fabrication

If you haven't yet fabricated any cables for one of these devices, it's time to do that! You'll find the supplies to make these cables at the front of the lab.

## run sketches

Your goal is to run the sketches under ```onir_device_interface``` and show dr. kraemer your working demos. When you get a demo working, briefly record what you see in your lab notebook.

### ```dial_local.ino```:

This sketch only requires one Arduino Uno, one dial, and the 5-pin M/F breadboard cable needed to connect them. The pinout is:

|dial  | board
|----: |---
|CLK   | A3
|DT    | A2
|SW    | A1
| +    | 5V
|GND   | GND 

Open the *Serial Monitor* with ```Tools > Serial Monitor``` (or ```control-shift-M```). The monitor is a tool for talking to the board over the **modem**, and will show the numbers reported by the dial.

Push (upload) the dial_local sketch to the board, and turn the dial. If you see numbers scrolling up when you turn the dial to the right and down when you turn it to the left, you have succeeded! Note down this result in your lab notebook (for instance, "dial_local -- shows scrolling numbers.") Then show dr. kraemer and get him to sign off on your result.

### ```display_local.ino```:

This sketch only requires one Uno board, one seven-segment display, and two 6-pin M/F breadboard cables. Attach them using the demo, and push the sketch ```display_local``` to the board. It should show the message "onir". Change the message to something else, then show dr. kraemer.


### ```dial_device``` and ```dial_client```:

This demo requires *two* Uno boards, plus the 4-pin M/M breadboard cable needed to make an I2C bridge between the boards.  Run dial_device on one board, and dial_client on the other. Attach the serial monitor to the ```dial_client``` board, then turn the dial on the ```dial_device``` board. If you see scrolling numbers, you have succeeded!

Try to make the display do something different when the numbers scroll past. Then show dr. kraemer your work.



# license

The name 'onir' may sound dreamy, but it's also short for "oh, nothing I'd recommend." I hope this software will be ready to use outside my lab someday. But for now I wouldn't advise putting this near your hardware.

# developer updates

Mostly just some [notes](updates.md).
