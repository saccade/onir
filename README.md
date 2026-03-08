# onir

onir: manage dials, switches, and servos over I2C. (uses Arduino's Wire library)

# lab: dials & displays

* First: Write the *name of your lab partner* in your notebook.

In this lab, you and your partner have the goal of setting up a message to scroll across multiple screens, with multiple dials controlling it. You'll work up to this goal by running simpler scripts that will read a single local dial, drive a single local display, and run and combine these devices over a two-device network.

This is a practical get-it-done lab. Your goal is to get the setup working, so if you want, you may use AI to help you with this assignment. 

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

   Open the terminal (the black window with the green text where you can type commands to the computer). Run these three commands (commands must be precise --- *copy-paste* or type **exactly**):
    * ```mkdir Arduino/libraries```
    * ```cd Arduino/libraries```
    * ```git clone https://github.com/dr-kraemer-everett/onir```

3. *Update onir:*

   If you didn't *just* perform the git clone, enter the command ```git pull```. This will pull down any updates from me or your classmates into to your copy of onir.

4. *Run onir:*

   Restart the IDE. At the *very bottom* of ```File > Examples``` you should see the option ```onir_device_interface```.

#### onir not there?

If you don't see ```onir_device_interface``` at the bottom of ```Examples```, check your install setup -- you probably have onir in the wrong place. You can remove it with the command ```rm -r onir``` (be careful with the command ```rm -r``` though! it doesn't ask twice, and can do a lot of damage in the wrong situation!)

## set up hardware

You'll need a pair of boards with ko lab's basic IO setup.

### cable fabrication

If you somehow haven't yet fabricated any cables for one of these devices, it's time to do that! You'll find the supplies to make these cables at the front of the lab.

### cable harness

If everyone in your group has already made a cable, that's great! You're not brand new at this, so now put together a **four box harness** for your group. These should match the *wire color patterns* you see widely in use around the lab. You know I don't want to shut down your creative ideas, but on this, ask before innnovating.

*Materials:*

  * four 5-pin housings (Dupont connector header)
  * two 4-pin housings
  * two groups of 5 M/F wires for the dial. (match colors with existing lab hardware!)
  * one double-pair for I2C (again, match the color patterns you see around you.)

By now you know from experience that adding **housings** to cables lets you make a one time job of the fiddly work of getting dozens of single cable ends placed just right. A **cable harness** levels up this idea of single-shot cable placement. Instead of making a generic ribbon cable (like the display cables) or a function-splitter (like the dial cable and the I2C connector), the harness is a piece of *dedicated hardware* for one particular circuit — in this case, the lab's standard Uno input/output (or, *I/O*) circuit.

You've used this I/O setup repeatedly now:

   * **display** (4-digit, 7-segment): attached to pins 2 through 14 via two flat M/F ribbon cables
   * **dial**: attached to pins A1-A3 + 5V/GND via a split M/F ribbon cable
   * **I2C**: pairing cable bus connecting the GND/VIN pair and A4/A5 pair between *two Uno boards*

The key idea is to use the housings to organize the cables into the *same positions* they would be without the harness, but with easier setup and fewer opportunities to get the setup wrong. There are three types of 'box' in this harness:

  1. 2 of the standard 5-pin housing we've been attaching to the dials.
  2. 2 power boxes attached to the pins 5V/GND/GND/VIN:

|dial | board 1| board 2|
|----:|-------|---------|
| +   |  5V   |         |
| GND |  GND  |         |
|     |  GND  | GND     |
|     |  VIN  | VIN     |

3. 2 data boxes attached to pins A1-A5:

|dial | board 1 | board 2|
|----:|---------|--------|
|     |  A5     | A5     |
|     |  A4     | A4     |
|CLK  |  A3     |        |
|DT   |  A2     |        |
|SW   |  A1     |        |

<img src="resources/io-harness.png" width="50%">

When installing the harness, please be careful **not** to flip the power box. This would connect the 5V on one board to the VIN of the other (a potentially large short, since the Uno will gamely accept all kinds of terrible current sources), as well as hooking the VIN on the first board to the + signal pin on the dial -- again likely to be more than the poor device is made to handle.

Draw the four-box harness in your notebook, being sure to highlight the single crossed pair going into the 5V/GND junction, and get dr. kraemer's signature.

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

This sketch only uses one Uno board, one seven-segment display, and two 6-pin M/F ribbon cables. Make sure both the board and the display are in a letters-up condition. (For the display, this actually means orienting the decimal points at the bottom.) The bottom row of pins on the display connects to pins 13-8, and the top row to pins 7-2.

With the display attached, push the sketch ```display_local``` to the board. It should show the message "onir". Change the message to something else, then show dr. kraemer.

### ```dial_device``` and ```dial_client```:

This demo requires *two* Uno boards, plus the 4-pin M/M breadboard cable needed to make an I2C bridge between the boards.  Run ```dial_device``` on one board, and ```dial_client``` on the other. Attach the Serial Monitor tool to the ```dial_client``` board. Then turn the dial on the ```dial_device``` board -- if you see scrolling numbers, you have succeeded!

Try to make the display *show something different* when you turn the dial. Then show dr. kraemer your work.

### ```io_device``` and ```screen_client```:

This demo uses another 2-Uno circuit over an I2C cable, but with: (1) a second *dial* attached to the *client* board, and (2) a **display** attached to the **device** board. Push the sketch ```io_device``` to the board with the dial and display, and ```screen_client``` to the board with just a dial.

The ```io_device``` sketch starts up in a channel-selector mode which initially displays an enigmatic "0x08". This is not nonsense or an error, but rather the standard format for displaying an I2C channel. The channel can be anything from 0x08 (channel 8, the lowest available channel) to 0x77 (channel 119, the highest available).

Press the switch once to start the device on channel 8. If you have all the cables attached correctly, you should see your message scrolling on the display. Check that both dials are able to scroll the display. (At the software level this is very similar to being able to use two dials to control a pair of servomotors on an arm or a car.)

If your display scrolls, change the banner message from "ko labs -- onir --" to something that makes sense to you. Then show dr. kraemer your work.

### multiple displays

If you've gotten the two-device setup working, try for three! Get a breadboad and use it to bus together the VIN/GND power pairs and the A4/A5 data pairs from a trio of Uno boards. Set up two of the boards as ```io_device```s on channels ```0x08``` and ```0x09```, and the third as a ```screen_client```.

You won't be able to use the harness you made to do this -- you'll have to use three individual I2C connector cables to hook each Uno up to the breadboard. It's easy to get a pair of these wires flipped and end up with a short circuit instead of a display device. If you connect VIN to GND you'll probably let the "magic smoke" out of our equipment. So get another student to check your connections before you power up your circuit!

If everything is connected right, you should be able to (1) scroll your banner message on each device's display using that device's dial, and (2) scroll the message on *both* displays using the dial on the *client* board.

Multi-device I2C setups are notoriously finicky -- if you got this working, good job! Record your result in your notebook and get dr. kraemer to sign off on your work.

# license

The name 'onir' may sound dreamy, but it's also short for "oh, nothing I'd recommend." I hope this software will be ready to use outside my lab someday. But for now I wouldn't advise putting this near your hardware.

# developer updates

Mostly just some [notes](updates.md).

