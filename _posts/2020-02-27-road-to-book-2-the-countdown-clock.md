---
layout: default
title: "Road to the Book #2: The Countdown Clock, Part 1"
image: /assets/images/posts/2020-02-27-card.jpg
description: In which we learn about prototyping, current limiting, shift registers and I²C (and light up some big red numbers).
---

# Oddly Specific Object #2: The Countdown Clock, Part 1

Last week, we talked a bit about digital input and output. We kind of glossed over the mechanics of it, though. The yellow data wire somehow let us get temperature data from our sensor, but we didn't talk about the signals actually going over the line. We let a third-party library handle it. 

Not this time! This time around, the main interface of our project is going to be a series of big red digits, and we're going to drive them ourselves using one of the simplest — and most versatile — chips in our arsenal, one whose data sheet dates to 1982: a shift register. 

But first, let's get acquainted with the project: it's a countdown clock! 

![Alt text](/assets/images/posts/2020-02-27-clock-hero.jpg)

I made this clock last April. It was counting down to a date in July (long story). I'd seen plenty of four-digit segmented displays, but since a countdown to a particular day of the year is never going to go above 364 (or 365 in a leap year, I suppose), the extra digit sort of offended my aesthetic sensibilities. Plus these digits were nice and big, making a display you could see from the other side of house. 

## Understanding the Build

For this project, we're going to pair the trio of big red digits with three shift registers, one to drive each. For timekeeping, we're going to add in a DS3231 Extremely Accurate Real-Time Clock — and as a side note, I love that they included that in the name of the product. "[Extremely Accurate](https://datasheets.maximintegrated.com/en/ds/DS3231.pdf)." 

For the brains of this project, we're going to use a Feather. Which one? Here's the fun part: by lesson four, you're going to build your own Feather, from scratch, and use it to run this project! For the moment, though, you can reuse the M0 from the hiking log, or use any other SAMD Feather you have laying about (you'll see a Feather M4 Express in these build photos). 

### Parts List

* The solderless breadboard from last time. 
* [A Feather M0 or M4 board](https://www.adafruit.com/category/777) (You can reuse the one from the Hiking Log, if you want)
* [3-Pack of 74HC595 Shift Registers](https://www.adafruit.com/product/450)
* [25-pack of 220Ω resistors](https://www.adafruit.com/product/2780)
* 3x [1.8" Single Digit Segment Display](https://tinkersphere.com/segmented-number-leds/854-large-18-single-digit-segment-display-super-bright-red.html)
* [40-pack Female-Female breadboarding cables](https://www.adafruit.com/product/266) (these will connect the segmented displays)
* [Breadboarding wire bundle](https://www.adafruit.com/product/153) (we'll use these to wire up the breadboard)
* [DS3231 Extremely Accurate RTC Breakout](https://www.adafruit.com/product/3013)
* [CR1220 Coin Cell](https://www.adafruit.com/product/380) (if out of stock, probably available at your local drugstore)
* Optional [Extra-long break-away male header](https://www.adafruit.com/product/400) (it makes it easy to turn the female-female cables into female/male cables, which we have to do once here).
* Possibly [a level shifter](https://www.adafruit.com/product/757); see the note at the end, couldn't hurt to grab one to have on hand.

### The DS3231 Extremely Accurate Real Time Clock (and I²C)

Remember that sensor from the log? It used a one-wire communication protocol. Which was rad, but also a bit limiting. For one thing, the protocol is kind of proprietary, meaning we had to import a whole library just to talk to that one gadget. It would be cool if we had a protocol that worked with many gadgets, so we only had to import that code once. For another thing, it was the only gadget we were able to have connected to that pin. Say we wanted to add some more sensors, like an accelerometer for step tracking, a magnetometer to serve as a compass, or a UV light sensor. With this one-wire scheme, we'd need another wire for each of them. It would be great to connect multiple devices to a single input or output. 

As it turns out, there are several standards for this kind of communication. One of the most ubiquitous is the **Inter-Integrated Circuit Bus**, or **I²C**. I²C is at once simple to explain, and kind of complex in practice. This guide is going to gloss over some of the more complex things, like optional modes that you're unlikely to encounter in your projects, and keep it simple: the bus consists of two wires, **SCL** or the **Clock** line, and SDA or the **Data** line. Connected to these two wires, there is an **I²C Master** device on the bus — that's our Feather! — and one or more **I²C slave** devices, which are often sensors, but can also be other things like IO expanders, DACs or even displays! 

This is all by way of saying, our extremely accurate real-time clock is just such a device, and we're going to connect it to the SCL and SDA lines on the Feather.

The clock and data lines are pretty much what they sound like: the master device drives the clock line — it's basically a continuous stream of high and low pulses — and either the master or the slave devices drive the Data line to communicate. Both sides of the bus use the open drain output style that we talked about last time — which is to say, they can only drive the lines low — so both SCL and SDA both need a pull-up resistor. We don't need to worry about that in this project, though, because the DS3231 breakout has those on board. 

In order to connect multiple devices to the I²C bus, each one needs to have its own **address**. Some I²C slave devices have an address permanently assigned at the factory; others have pins you can tie high or low to change the adddress. In the case of our DS3231, the address is fixed, so whenever we want to talk to it, we send its address (0x68) over the data line, and that tells the DS3231 to start listening. 

We'll talk about this more when we get to programming the clock. 

### The Shift Registers (and serial output)

Up until now, we've been dealing with pretty complex modules that we've been able to interface with using libraries. Take the temperature sensor in the log, for example. It used a very specific one-wire communication protocol, but we didn't have to code any of that. We just imported a library, and wrote `sensor.getTemperature()`. 

That all changes with this project. To drive the three giant digits, we're going to use a trio of 74HC595 **shift registers**, and while I suppose you could find some kind of library to drive them, they're kind of basic, fundamental parts, and you're going to learn a lot by writing code to control them from scratch. 

So here's the deal with shift registers: they're amazing. They're all over the place in electronics design, because they're just so useful. Say you wanted to get the state of 8 buttons. You could use 8 pins on your microcontroller — or you could use a shift register to turn those read those 8 button states, and translate them into pulses on a wire. This is an example of a **parallel in, serial out** shift register; there are eight lines in parallel coming in, and one line transmitting the data serially coming out.

Say you need to drive three 7-segment digits, but you don't have 21 pins on your feather? OH WAIT, it's precisely our predicament! What a fortunate turn of events. We'll use a **serial in, parallel out** shift register to turn one serial output signal into many more parellel outputs. 

We're going to run three wires from our Feather to one of the shift registers. We'll call these wires "Clock", "Data" and "Latch." The clock is much like the clock on the I²C bus: it just rises (high) and falls (low) regularly. 

The data line represents a 1 or a 0 that we're sending to the chip; we set it high or low, and whenever the clock line rises, it will take whatever value is on the data line, and store it (in jargon terms: we **shift in** that bit). How does the chip store that bit? It has a **shift register** with room for eight bits, so it takes the bits it has, it "shifts" them to the left, and stores the new bit in the rightmost position. 

Finally, when the latch line rises, the chip will take whatever values it has in its shift register, move them to its **storage register**, and set the 8 output pins high or low. If we know which bits correspond to which segments on our display, we can create a set of 10 bytes, patterns that light up the right segments to display the digits 0 through 9, and send them over to the registers! 

There's one last piece of the puzzle that lets us drive three displays with one serial interface. You'll notice that with 8 bits of storage, and bits shifting to the left, you'll start to lose things as you shift in more than 8 bits. Luckily, the 74HC595 includes another pin, a **serial data output** pin. Whenever we shift in a bit, the register **shifts out** the bit that overflowed. If we connect that serial output to the input of a second shift register, we can clock 16 bits in, and control 16 outputs. Connect the serial output of that second register to the input of a third, and we can control 24 outputs! 

Speaking of outputs: it's probably time to talk about the user interface for our clock: the big red numbers. 

### The Big Red Numbers (and Ohm's Law)

The way to think about these big chunky numbers is pretty simple: it's basically a bunch of LED's. Last time around, we blinked an LED when the hiking log logged a data point. It was pretty simple: we set the output line high, which caused the microcontroller to source current at 3.3 volts, and that current flowed through the LED to make it blink. And while sure, like we said, the point of digital IO isn't to have large amounts of current flowing, an LED doesn't need that much power to light up. 

Remember last time, when we were talking about Ohm's Law? It's back! If you'll recall, we connected a 100 KΩ resistor between power and ground, and in so doing, got a very small amount of current flowing. Before we do this next thing, one important note. The same way we connected that 100K resistor between power andf ground? We could also have connected two 50K resistors in a row (also described as **in series**): 

![Alt text](/assets/images/posts/2020-02-27-understand-resistors-in-series.png)

Note that the voltage at the top is still 5V relative to the ground, and the voltage at the bottom is still 0V relative to the ground. But if you measured the voltage between that halfway point and the ground? You'd get 2.5V. 

![Alt text](/assets/images/posts/2020-02-27-understand-voltage-drop.png)

There is a voltage across the various **nodes** of a circuit. This may not seem terribly interesting when it's two identical resistors in series, but what we're about to do is connect a resistor and an LED in series. 

Let's take a typical LED like that little red blinkenlight on your Feather. Let's say we wanted to power it with the 3.3 volts coming from our regulator. First, we'd need to look at its [datasheet](https://media.digikey.com/pdf/Data%20Sheets/Lite-On%20PDFs/LTST-C170CKT.pdf) (this isn't necessarily the exact model of LED, but it works to explain the concept). There's a couple of lines somewhere in there that describe two important numbers: the **forward voltage**, and the **forward current**. 

![Alt text](/assets/images/posts/2020-02-27-datasheet-forward-voltage.png)

![Alt text](/assets/images/posts/2020-02-27-datasheet-forward-current-max.png)

The forward voltage is the voltage level we have to reach in order for the LED to turn on; for this LED, the forward voltage is 1.8V. The forward current is usually listed in terms of the maximum amount of current you can put through the LED. In this case, the maximum continuous current is 40mA. In practice, you can supply a value much lower than this; a little current goes a long way. In fact, elsewhere in the data sheet, you can find a graph of the LED's output at various forward currents. 

![Alt text](/assets/images/posts/2020-02-27-datasheet-forward-current.png)

So let's say we want to light this LED with 1.5mA of current (typical for an indicator light). Our schematic will look something like this: 

![Alt text](/assets/images/posts/2020-02-27-understand-current-limiting-led.png)

To figure out the value of the resistor, we take the total voltage of the circuit (3.3 volts) and subtract the forward current of the LED (1.8V). This leaves 1.5 volts. Now plug it into that Ohm's Law equation from last time: 

**R = 1.5V / 0.0015A =1000Ω**

1000 ohms, or 1KΩ, usually ends up being a safe bet for a resistor value for driving LED's. And indeed, our Feather M0 has a resistor of roughly this magnitude (2.2KΩ) connected in series with the pin 13 LED, which is why we were able to blink without worrying about any of this!

For these big segmented numbers from Tinkersphere, the web page describes the expected operating current: between 1 and 20 mA. It also describes a value that it claims is the forward voltage (1.8 V). Unfortunately this number seems to be completely off. My theory is that there are two LED's in series inside each segment, because the forward voltage seems to me to be much closer to 3.6 volts. Anyway, we're going to guess at that value, and try to drive these segments at something on the order of 7 mA.

## The Build

Honestly, this build, we gotta just do it; it's going to be a little crazy, lots of wires all over the place, but you'll get a sense for it quick. First, let's set the clock in the breadboard. Take the pin header that came with it, and break it off so that you only have four pins. Place them at the far right of the breadboard, long pins down...

![Alt text](/assets/images/posts/2020-02-27-build-020.jpg)

...and solder them to the breakout's Vin, GND, SCL and SDA pins: 

![Alt text](/assets/images/posts/2020-02-27-build-030.jpg)

A breadboard like this is great, because it makes it easy to connect things. All the top columns are connected, and all the bottom columns are connected, so instead of having to hook wires around things (like you did with the Enable pin last time), you can just plug a thing in, and it's connected to the other things in that column. So, right now, use blue and yellow wires to connect SCL and SDA on your Feather to the SCL and SDA columns we just created. 

![Alt text](/assets/images/posts/2020-02-27-build-040.jpg)

And while we're at it, let's hook up power and ground. Connect a black wire from the GND pin to the bottom blue row, then another black wire from that row to the top blue row. These two rows are our ground rails. Then connect an orange wire from the 3V pin to the bottom red row, and a red wire from the USB pin to the top red row. The top red row is now our 5 volt rail, and the bottom red row is our 3.3 volt rail. 

![Alt text](/assets/images/posts/2020-02-27-build-050.jpg)

Now we can connect 3.3 volt devices to power at any pin on that bottom red rail, and connect 5 volt devices to power at any pin on that top red rail, and to ground at either of the blue rails. In fact, let's power the DS3231 with 3.3 volts; connect its power and ground columns to the 3V3 and ground rails (To keep things from getting confused, let's use orange wires for 3.3 volt power, and red wire for 5 volt power). 

Next, set the three shift registers in the breadboard. There should be some kind of indicator, either a dot or a notch, on one side; align all those notches to the left side of the board, in columns 1, 10 and 19: 

![Alt text](/assets/images/posts/2020-02-27-build-060.jpg)

To know which pins do which things, we'll [check the datasheet](https://assets.nexperia.com/documents/data-sheet/74HC_HCT595.pdf) for a section called "Pin Description." This will give us a diagram of each pin, and a description of what it does: 

![Alt text](/assets/images/posts/2020-02-27-build-070.png)

Most of the pins on the three chips will be configured the same way; only the data line will be different. For all of these, we want to make the following connections: 

* Q1 - Q7 (pins 1-7): these are outputs to each of the big red numbers.
* GND (pin 8): connects to the ground rail.
* MR (pin 10): Master Reset. We'll tie this to the 5v rail. We could control it from the Feather, pulling it low to reset the register, but we don't need to.
* SHCP (pin 11): We'll connect this to the pin we're calling "Clock" on the Feather; it's the shift register's clock pin (SHCP).
* STCP (pin 12): We'll connect this to the pin we're calling "Latch" on the Feather. We'll connect this to the pin we're calling "Latch" on the Feather; it's the storage register's clock pin (STCP).
* OE (pin 13): Output Enable. We'll tie this to GND. We could control it from the Feather, pulling it high to turn off or dim the outputs, but it's simpler to keep them on all the time.
* Q0 (pin 15): We'll leave this disconnected, it's the eighth output, but we only have 7 segments to drive. (You could connect it to the decimal point, but you'd need a different resistor value.)
* VCC (pin 16): connected to 5v power.

Assuming the clock and latch lines are on pins 10 and 11 (I'm using yellow and green, respectively), you could wire it like this.

![Alt text](/assets/images/posts/2020-02-27-build-080.png)

The two differences are going to have to do with the DS and Q7S pins (pins 14 and 9). On the rightmost shift register, DS is going to be connected to the line on the Feather we're calling "Data"; it's the serial data input. Then its Q7S, the output of the bit that overflowed, will be connected to the middle shift register's DS pin, serving as the input to that shift register. Same thing there: the middle shift register's Q7S will be attached to the leftmost's DS. And the leftmost register's Q7S will be left disconnected. Assuming the data line (blue) on pin 12, it should in theory look something like this: 

![Alt text](/assets/images/posts/2020-02-27-build-090.png)

Of course, in practice, it's totally fine if it looks something like this; just make sure all the columns are connected correctly.

![Alt text](/assets/images/posts/2020-02-27-build-100.jpg)

Before we go on, I want to point something out that wasn't clear to me when I first started making circuits. All wires that are connected to each other are at the exact same point in the circuit; they are a part of a single **node**. Like, there's this temptation to imagine current flowing from one place to another like a series of pipes, but in practice it's not useful to think that way. For example, this is how I wired the clock line the first time I did this project: 

![Alt text](/assets/images/posts/2020-02-27-build-110.png)

It satisfied me aesthetically that the clock line went to the first chip first, then to the second chip, then to the third chip. I imagined little clock pulses going from place to place along the wire. In practice though, for most signals we're going to work with, *this is not the case*. My first attempt at wiring the clock line is functionally equivalent to doing this: 

![Alt text](/assets/images/posts/2020-02-27-build-120.png)

Now, you could lay out the breadboard this way if you wanted to; unless you're dealing with really high speed signals like USB, it doesn't really matter which wires go where, as long as they connect the right pins to the right nodes. The point I want to drive home is that no matter how you wire it, all three clock pins on all three chips — and all the pins in all three of those columns — are connected, directly, to pin 10, just as if we'd soldered them all together at one point. 

But I digress. 

Next, let's hook up the big red numbers. Tinkersphere doesn't provide much of a datasheet for these displays, but having worked with them before, here's the pinout that I've discovered (plus the outputs from the '595 from its datasheet): 

![Alt text](/assets/images/posts/2020-02-27-build-130.jpg)

We're going to hook the segments up so that A-G correspond to signals Q1-Q7. But we can't hook them up directly. Remember: we need to limit the amount of current going to them. After a quick mathy interlude, we'll attach one end of our female-female jumper wires to the display pins, and the other to the current limiting resistors): 

![Alt text](/assets/images/posts/2020-02-27-build-140.png)

Assume for the moment that the forward voltage of these LED segments is 3.6 volts. We're powering them with 5 volts, from the USB pin, so we need to solve for 1.4 volts. We want to drive these LED's at about 7 milliamps. Quick Ohm's Law calculation: 

**R = 1.4 volts / 7 mA = 200Ω**

200 ohms isn't the most common number, so let's round up to the nearest common resistor value (220Ω). Each of these segments is going to get a 220 ohm resistor in series with the segment: 

![Alt text](/assets/images/posts/2020-02-27-build-150.jpg)

Okay wow, that's a bit unweildy. Trim the leads a bit to make them a bit more manageable (but don't trim too much, we're going to reuse these next week when we make this design a bit more permanent). 

![Alt text](/assets/images/posts/2020-02-27-build-160.jpg)

Once you have your army of resistors in file, go ahead and start plugging in the ribbon cable. You can tear the ribbon cable apart however you want, but I chose this color arrangement so that outputs Q1-Q7 and segments A-G both matched the colors of the rainbow in order: red, orange, yellow, green blue, purple (and then gray was the last color after that). 

![Alt text](/assets/images/posts/2020-02-27-build-170.jpg)

Black goes to one of the two ground pins at the bottom; to plug it in to the breadboard, use one of the extra long header pins, or just a one of the spare leads you trimmed off of one of the resistors. 

![Alt text](/assets/images/posts/2020-02-27-build-180.jpg)

![Alt text](/assets/images/posts/2020-02-27-build-190.jpg)

All told, your project should look something like this: 

![Alt text](/assets/images/posts/2020-02-27-build-200.jpg)

At last! We have our project wired up. Let's do one last thing before we start coding. Grab the coin cell, and insert it into the DS3231 breakout. 

![Alt text](/assets/images/posts/2020-02-27-build-210.jpg)

Now it's got backup power, so when we set the clock, it should stick. 

## Coding the Clock

This one comes in four parts: 

1. First, we're going to use an external library, RTCLib, to get the Extremely Accurate Real-Time Clock ticking. 
2. Next, we're going to figure out a way to store our deadline across resets. This will require writing some raw I²C commands to the wire.
3. Then we'll devise a method for calculating the number of days until the deadline.
4. Finally, we're going to make the big red digits display a big red number.

### The DS3231 Extremely Accurate RTC (and I²C)

Create a new, blank sketch, and install the following library:

* RTCLib by Adafruit

Import it and create a global for the RTC and the deadline we'll be counting down to: 

```
#include <RTClib.h>
RTC_DS3231 rtc;
long deadline = 0;
```

We're going to use this object for setting and checking the date and time. This is going to make things simpler for us, but don't worry, we're going to peek under the hood in a moment. 

Add the following code to the `setup` method: 

```
rtc.begin();
if (rtc.lostPower()) {
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // TODO: set deadline
}
```

Believe it or not, that's it! The `rtc.adjust` line sets the time to the exact time that the sketch was compiled, and we can call `rtc.now()` to get the current date and time. The RTC will continue ticking as log as the coin cell has power, even if USB power is disconnected.

One catch, though: that's the only thing that's saved when USB power is disconnected. Eventually, we want to be able to set the countdown date on the device, which means we're also going to have to store that somewhere. And as it turns out, the DS3231 has a handful of other registers that can serve as space to back up our date. 

### Storing the Deadline (and more I²C!)

The goal: we want to stash a year, a month and a day somewhere in the DS3231's storage registers. There's clearly registers for the current year, month and day, but what other registers exist on this chip, where might we stash this data? Again, we get to engage in the pasttime of [consulting the datasheet](https://datasheets.maximintegrated.com/en/ds/DS3231.pdf)! 

![Alt text](/assets/images/posts/2020-02-27-programming-ds3231-datasheet.png)

This table lists all the registers on the DS3231. Registers 0x00 through 0x06 seem to contain the current date and time, which the RTCLib must be setting for us when we adjust the time. But registers 0x07 through 0x0D have to do with alarms. What's that? 

The DS3231, as it turns out, supports an alarm function; if enabled, it can output a signal on the INT/SQW pin to set off an alarm when a target time is reached. We're not using this function, but we could use those registers to stash three numbers for our countdown deadline, and they would be saved along with the current date and time! 

So here's our plan for the register map: 

* Register 0x08: normally used to store Alarm 1 minutes, we'll store the year for our deadline here (00-60 signifying 2000-2060)
* Register 0x09: normally used to store Alarm 1 hours, we'll use it to store the month for our deadline (1-12)
* Register 0x0A: normally used to store Alarm 1 day, we'll use it to store the day of the month for the deadline (1-31)

This is not how the makers of this chip intended for these registers to be used. But it works! Now we just need to learn how to write values to these registers over the I²C bus. 

As it turns out, our SAMD21 microcontroller has hardware specifically designed for I²C, and a support library for all the low-level stuff we would need to understand to drive the I²C bus. So we're going to leverage that by importing the Wire library: 

```#include <Wire.h>```

You'll also remember from above that I²C devices have an address. You can have a dozen I²C devices on the same bus; you select the one you want to talk to by sending its address over the wire, and then you read or write data. So we need two functions: a function to write to a register, and a function to read from one: 

```
void write_I2C_register(uint8_t addr, uint8_t reg, uint8_t val) {
  Wire.beginTransmission(addr);
  Wire.write((byte)reg);
  Wire.write((byte)val);
  Wire.endTransmission();
}

uint8_t read_I2C_register(uint8_t addr, uint8_t reg) {
  Wire.beginTransmission(addr);
  Wire.write((byte)reg);
  Wire.endTransmission();
  Wire.requestFrom(addr, (byte)1);
  return Wire.read();
}
```

These two functions, incidentally, are [more or less borrowed directly from RTCLib](https://github.com/adafruit/RTClib/blob/master/RTClib.cpp). It's not quite the bare metal of flashing clock and data wires on and off, but you can see the basics of an I²C transaction: in the first case, we begin the transmission by sending the address of the device we want to talk to; then we write the address of the register in that device, and the value that we want to store there. Similarly, to read from a register: we send the address of our device and the address of the register, then request a byte from the same address. 

There's one last thing we need to do to actually store values in these registers. The DS3231 uses an encoding format called Binary Coded Decimal (BCD). It encodes a two-digit number in one byte by breaking it in half. This is a little in the weeds but bear with me: a byte is 8 bits long, and can represent any number between 0 and 255 (2^8 = 256). If you split that byte in half, into two four-bit **nibbles**, you can represent two numbers between 0 and 15 (2^4 = 16). It's easier to see it visualized: 

![Alt text](/assets/images/posts/2020-02-27-programming-bcd.png)

You can see where single digit numbers fit in four bits, so we just use the upper bits for the tens place, and the lower bits in the ones place. So to stash numbers in the registers, we need to write a function to pack them in BCD format, and a function to unpack them: 

```
uint8_t packValue(uint8_t value) {
  uint8_t ones = value % 10;
  uint8_t tens = (value / 10) % 10;
  return (tens << 4) | ones;
}

uint8_t unpackValue(uint8_t packed) {
  uint8_t ones = packed & 0xF;
  uint8_t tens = (packed >> 4) & 0xF;
  return tens * 10 + ones;
}
```

With these four functions implemented, we can fix the TODO item in our setup code; for now, we'll count down days until the new year:

```
if (rtc.lostPower()) {
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  write_I2C_register(DS3231_ADDRESS, 0x08, packValue(21)); // 2021
  write_I2C_register(DS3231_ADDRESS, 0x09, packValue(1));
  write_I2C_register(DS3231_ADDRESS, 0x0A, packValue(1));
}
```

Now, whenever we want to get a value out of the DS3231, we can use `read_I2C_register` and `unpackValue` to do it 

But before we do that, we need to add our methodology for comparing dates. 

### A Brief Interlude for Dates

Calendars are complicated. There are 365 days in a year, except every four years when there are 366, except every 100 years when you don't add an extra day (except every 400 years when you do)... yeah. It's a headache. 

To calculate the days until our deadline, we're going to use a concept called the [Julian Day](https://en.wikipedia.org/wiki/Julian_day). This is a single number that counts the number of actual, full calendar days since January 1, 4713 BC in the Julian calendar (which was November 24, 4714 BC in our modern Gregorian calendar). Point is, we need to add a method to calculate a Julian date from for a given year, month and day: 

```
long JulianDate(int year, int month, int day) {
  long centuries = year / 100;
  long leaps = centuries / 4;
  long leapDays = 2 - centuries + leaps;  // note: is negative!
  long yearDays = 365.25 * (year + 4716); // days until 1 jan this year
  long monthDays = 30.6001* (month + 1);  // days until 1st month
  return leapDays + day + monthDays + yearDays -1524.5;
}
```

 Now, we can set our deadline at the very end of the setup function...
 
 ```
deadline = JulianDate(2000 + unpackValue(read_I2C_register(DS3231_ADDRESS, 0x08)), 
unpackValue(read_I2C_register(DS3231_ADDRESS, 0x09)), 
unpackValue(read_I2C_register(DS3231_ADDRESS, 0x0A)));
```

...and start to populate the loop() function: 

```
DateTime now = rtc.now();
long julianNow = JulianDate(now.year(), now.month(), now.day());
long remaining = deadline - julianNow - 1;
```

If you want, you can run a `Serial.println(remaining)` in here; run the sketch, and you'll see the number of days until the new year, printed repeatedly to the Serial console. 

### The Big Red Numbers

FINALLY! The moment you've been waiting for. To recap: we are using seven of the shift register's eight outputs to drive each display. We're going to use three pins to transmit 24 bits of data to the first shift register, three bytes representing the state of the displays, and it will in turn shuffle the bits it can't fit to the second and third shift registers. 

In each byte, the lower seven bits will represent whether a given segment is on (1) or off (0). This diagram shows the segments of the display, how they correspond to the bits in the byte, and how we're going to write a series of bytes that represent each of the digits, from 0 to 9: 

![Alt text](/assets/images/posts/2020-02-27-digits.jpg)

So let's add that code right up top, with our globals: 

```
byte Digits[] = {
  B1111110, // 0
  B0110000, // 1
  B1101101, // 2
  B1111001, // 3
  B0110011, // 4
  B1011011, // 5
  B1011111, // 6
  B1110000, // 7
  B1111111, // 8
  B1111011, // 9
};
```

Now when we want to output a digit (like a 5) to the display, we can just use `Digits[5]` and have a bit pattern that lights up the correct segments. But how do we actually output something to the display? For that, we're going to go as close to bare metal as we've ventured so far. First, let's create some #defines for our pins, right there after the Digits: 

```
const int PIN_DISPLAY_CLOCK = 10;
const int PIN_DISPLAY_LATCH = 11;
const int PIN_DISPLAY_DATA = 12;
```

And we'll have to set them up as outputs, at the end of the setup function: 

```
pinMode(PIN_DISPLAY_DATA, OUTPUT);
pinMode(PIN_DISPLAY_LATCH, OUTPUT);
pinMode(PIN_DISPLAY_CLOCK, OUTPUT);
```

Here's the sequence of events that need to happen for us to write three bytes to the shift registers. First, we bring the latch line low. It will stay low while we repeat two steps, repeatedly, for each of the bits we want to send out:

1. We set the data line to the value of the bit we want to send. 
2. We pulse the clock line high, and then low. This clock pulse shifts a 1 or a 0 into the shift register, depending on the state of the data line.
3. We return to step 1 until all 24 bits are "shifted out".

The crucial thing to understand here: after step 2, when we've pulsed the clock pin, we can do whatever we want to the data line; the bit we just clocked in is set. If that bit was a 1, and we want to send another 1, we can leave the data line high and pulse the clock again. If we want to send a 0, we can bring the data line low, and then pulse the clock. Whenever the clock pin rises, a bit gets shifted into the shift register. It looks something like this (for a single byte):

![Alt text](/assets/images/posts/2020-02-27-clock-and-data.jpg)

Finally, once we've set all 24 bits, we set the latch line high. This sets all the outputs to match the state of the shift registers, turning our segments on or off. It looks more or less like this (again only showing one byte): 

![Alt text](/assets/images/posts/2020-02-27-and-latch.jpg)

So let's write functions to implement this! 

```
void byteOut(byte b) {
  for (int i = 0; i < 8; i++)  {
    digitalWrite(PIN_DISPLAY_DATA, (b & (1 << i)) ? HIGH : LOW);
    digitalWrite(PIN_DISPLAY_CLOCK, HIGH);
    digitalWrite(PIN_DISPLAY_CLOCK, LOW);            
  }
}

void display(byte b1, byte b2, byte b3) {
  digitalWrite(PIN_DISPLAY_LATCH, LOW);
  byteOut(b1);
  byteOut(b2);
  byteOut(b3);
  digitalWrite(PIN_DISPLAY_LATCH, HIGH);
}
```

It basically loops through the byte, and at each bit sets the data line to match the state of each bit before pulsing the clock. Now we can put the last piece of the puzzle into place: back in loop, after calculating the remaining days, we can add code to calculate each digit, and display it: 

```
unsigned char ones = remaining % 10;
unsigned char tens = (remaining / 10) % 10;
unsigned char hundreds = (remaining / 100) % 10;

if (remaining < 0) {
  // if date is passed, display " - "
  display(0, 0b00000001, 0);
} else {
  display(Digits[hundreds], Digits[tens], Digits[ones]);
}
```

Save and run the sketch. You should see your big red numbers light up! 

![Alt text](/assets/images/posts/2020-02-27-all-done.jpg)

## Extra Credit

This build is actually going to be with us for a couple of weeks; next week, we're going to take this protoboard arrangement and solder it into a perma-proto, partly to make the build less of a ratsnest, but also to free up your solderless breadboard so you can prototype the DIY Feather that's going to run the countdown clock eventually. This is all by way of saying, leave this build be for now; there are things we can do to improve it, but they'll happen next week. 

One bit of extra thing to think about, though: toward the end, when we were flashing a clock and a data pin on and off? That may have seemed like some oddly specific protocol that's specific to this particular model of shift register, but in fact, you implemented a very barebones version of one of the most ubiquitous standards in electronics: the Serial Peripheral Interface. This is the interface that you used to write data to the Flash chip on the Hiking Log, and if you own a digital camera with an SD card, it's one of the interfaces for reading and writing data to that card.

In fact, those SCK, MOSI and MISO pins on your Feather? They're the clock and data lines for an SPI bus; MOSI is master-out, slave-in — the equivalent of what we were doing with our data line — and MISO (master-in, slave-out) is a second data line for peripherals to talk back. They both use these signals very similarly to the way we did just now, clocking in bits of data when the clock line changes. 

To be clear: there are different modes of SPI, bit order to worry about, and the Feather's SAMD21 has hardware acceleration and all kinds of optimizations for high-speed communication over the SPI bus. Still, fundamentally, by learning to send individual bits and bytes to these shift registers, you learned something about how the bus that drives a Flash chip, an SD card and (eventually) the Open Book's e-ink screen works. Which I think is pretty rad. 

## THAT FINAL PROMISED NOTE: The Level Shifter

**We did something a little bit unusual here with logic levels: powering the shift registers with 5 volts, but using 3.3 volt logic for their inputs.** The 74HC595 shift registers I got from Adafruit were from NXP, and had no trouble at all with this arrangement. But I also tried some TI-branded 595's from Tinkersphere that would not respond to the 3V logic. If you find yourself at this point in the guide and , [this inexpensive level shifter will solve the problem](https://www.adafruit.com/product/757). I'll add a note about wiring it shortly; fact is, I didn't run into this issue until I'd already written the whole lesson, and it's easier to explain all this if we assume our shift registers' inputs work with our microcontroller's outputs. Also, this trick only works because we're only outputting from a 3.3V chip to the 5V shift registers; *NEVER connect the output from a 5V peripheral to a 3.3v input without proper level shifting*.

