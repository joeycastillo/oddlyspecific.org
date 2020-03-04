---
layout: default
title: "Road to the Book #2: The Countdown Clock, Part 1"
image: /assets/images/posts/2020-02-27-card.jpg
description: In which we commit the countdown clock to a more permanent arrangement, and prototype a microcontroller board to drive it.
---

# Road to the Book #3: The Countdown Clock and The Protoboard Feather

Full disclosure: I am SO STOKED for this guide and the next one. So far, you've been putting together hardware that works with pre-made Feather boards. Which are rad, and powerful, and contain all kinds of electronic goodness to make it easy to build awesome things like a log-shaped data logger. This week, though, we get to peek into that electronic goodness by prototyping our own board, using basic electronic components, that does many of the same things; it will even plug in to the Ardiuino IDE! By the end, we'll run the countdown clock  last week on this protoboard creation, and you'll more or less be left with a clock that you built entirely from scratch, whose every component you understand. 

Before we do that, though, we need to move the existing countdown clock off of the protoboard and on to something more permanent — partly because it's kind of fragile in its current state, but mostly because we need to free up the protoboard for this project! So this lesson comes in two parts: first up, through-hole soldering skills. You're going to move the entire countdown clock circuit onto a Perma-Proto board, soldering it permanently into place. 

Then, we'll learn about the resistors, **capacitors** and **integrated circuits** (**IC**s) that will drive our DIY microcontroller board. 

## Parts List

[WIP]

## Finishing Up the Countdown Clock

[WIP]

## The Protoboard Feather

The project for the rest of this lesson (and the next one) is a DIY microcontroller board that we'll soon turn into a DIY Feather. It's not going to be quite as full-featured as the Feather M0 Express you've been using — there won't be any Flash chip, enable pin or battery support — but the tradeoff is that it's going to be very simple to understand. With just a dozen or so components, we're going to build a microcontroller board that plugs in to the USB port of your laptop, that you can program with the Arduino IDE. 

Earlier we talked a little bit about schematic diagrams, and drew one to represent the countdown clock that we built. It turns out that's often a pretty good first step in the making of things: draw a diagram of the thing you plan to build, then prototype it, then build it. So here's the schematic for the DIY Feather: 

![Alt text](/assets/images/posts/2020-03-05-schematic.png)

Looks daunting? Don't worry. We're going to walk through each part as we put it on the board. 

## The USB Port

![Alt text](/assets/images/posts/2020-03-05-usb.png)

This is just an adapter that runs the tiny pins of a Micro-USB port out to breadboard-friendly pins; there are no other parts. We're putting this piece down first, because this is where the power comes from: whether it's your laptop or a power brick, the USB port has a ground pin and a power pin that provides 5 volts of power. We can draw up to 500 mA from this power pin, and it's going to become our 5 volt power rail: 

![Alt text](/assets/images/posts/2020-03-05-build-020.jpg)

The ID pin, we're going to ignore. 

![Alt text](/assets/images/posts/2020-03-05-build-030.jpg)

The D+ and D- pins are USB data; this is the actual signal that goes from the microcontroller to your laptop. We'll hook these up in a moment. 

## The Voltage Regulator

![Alt text](/assets/images/posts/2020-03-05-regulator.png)

We haven't talked about this kind of part yet: a **voltage regulator** takes a power source at a higher voltage, and regulates it down to a lower voltage. In this case, our LM1117T-3.3 can take any voltage up to 6 volts, and regulate it down to 3.3 volts, which is what we need for our microcontroller and our 3.3 volt rail. The Feathers we've used so far have all had one of these, and if you ever wondered where that 3.3 volt power was coming from, this is the answer!

![Alt text](/assets/images/posts/2020-03-05-build-040.jpg)

The LM1117T has three pins: an IN pin for the 5 volts coming from the USB port, a GND pin to connect to ground, and an OUT pin that we'll connect to our 3.3 volt power rail. This regulator will provide up to 800 mA, which is way more than we're likely to need for our little microcontroller projects. 

![Alt text](/assets/images/posts/2020-03-05-build-050.jpg)

## Filtering Capacitors

![Alt text](/assets/images/posts/2020-03-05-regulator-caps.png)

There are a handful of really, really, *really* fundamental electronic components. One of them is the resistor; already we've used them to pull up signals and limit current. The **capacitor** is a similarly fundamental component, and similarly, you're going to find all kinds of different uses for them. 

A capacitor, fundamentally, stores an electrical charge. Its symbol, a pair of plates that aren't touching, is actually pretty accurate to the way it works: when a voltage is applied to it, a positive charge builds up on one side, and negative charge builds up on the other side. This charge builds to a maximum value, the capacitor's **capacitance**, which is measured in **farads** (F). A farad is a huge amount of capacitance, though, so you're likelier to encounter capacitors in the **picofarad** (**pF**), **nanofarad** (**nF**) or **microfarad** (**µF** or **uF**) range. 

The purpose of these two capacitors is to smooth out the voltage levels at either side of the power supply. Why do we need this? Well as it turns out, while your USB port tries to provide 5 volts, and the regulator tries to provide 3.3, that voltage can fluctuate a bit depending on what's going on in the circuit.

Say, for example, you turned on all seven segments of the countdown clock's three displays, all at once. Driving each segment at 7 mA, you'd have a sudden current draw of 147 mA on the 5 volt rail. This could cause the voltage on the 5 volt rail to drop momentarily. Why does that matter? Well, that 5 volt rail is also powering those three shift registers, which need a stable voltage level to do their job. Spikes and dips on the 5 volt rail will make your circuit unreliable. 

![Alt text](/assets/images/posts/2020-03-05-build-060.jpg)

So we have this 10µF capacitor, sitting between 5 volts and ground. As soon as you plug this gadget in, there will be a 5 volt potential between the two sides of the capacitor, so it will begin charging until it's full. Then, if you were to turn on those power-hungry LED's, instead of the voltage level dropping below 5 volts, the capacitor will begin to discharge, since it still has a 5 volt potential across its terminals. It's a buffer, of sorts, that keeps the voltage level stable. 

We can call it a **filtering** capacitor, because it filters out high frequency noise. We have these capacitors on both the 5 volt and the 3 volt rail, to smooth out both sides of the power supply. 

## The Bleeder Resistor

![Alt text](/assets/images/posts/2020-03-05-regulator-resistor.png)

The thing about a capacitor: it charges up, and then it stays charged. When you unplug your gadget from the USB port, it's still charged with 5 volts! And while that power might drain eventually through the other components on the board, it would be a lot safer to make sure that the capacitor discharges on its own. 

![Alt text](/assets/images/posts/2020-03-05-build-070.jpg)

This 100K resistor does the job. Sitting between 5V and ground, it provides a path for any charge stored in the capacitor to drain safely to ground when unplugged. 

## The SAM D11 Microcontroller

![Alt text](/assets/images/posts/2020-03-05-samd11.png)

This is the big part. The brains of the operation. A **microcontroller** or **MCU** is kind of like a little all-in-one computer, with its own processing unit, storage, RAM and peripherals. This little microcontroller gives you a lot of bang for a literal buck (it costs between 60¢ and $1.50): 17 pins of general purpose IO, hardware support for the I2C protocol that drives our Extremely Accurate RTC, as well as UART (which drove the Hiking Log's GPS) and SPI (which we started to talk about last week). It even has a native USB interface, which means we can more or less plug the USB pins from our computer straight into the MCU. 

But there's one thing we have to do first. You may have noticed that this chip doesn't have pins you can use to plug it into a breadboard. That's because this chip is **surface mount**; in fact, it's the first surface mount part we're going to deal with. To start prototyping with it, we are going to have to solder it on to a **breakout board**. 

[image]

Here's how you do it: first, find the dot on the SAMD11 chip. This is pin 1. Now find the dot or pin 1 marker on the breakout board. Using your soldering iron, melt a little bit of solder onto that pad: 

[image]

Now, with tweezers, set the chip down on the footprint. Holding on to the chip with your tweezers, melt that solder, and connect pin 1 of the chip to pin 1 on the breakout board. 

[image]

Now, look at the breakout board from the top. Make sure all the pins are aligned with all the pads. If they're not? Melt the pin 1 solder again and reposition the chip with your tweezers, until it's perfect. Once you have it in place, look at the absolute other side of the chip, to pin 20. Use your soldering iron to connect pin 20 to the breakout board. This will lock the chip into position. 

[image]

Finally, just go around the board and solder each pin to each pad! When you're done, attach some headers, and plug it in to the protoboard. 

![Alt text](/assets/images/posts/2020-03-05-build-080.jpg)

Much like with the shift registers, you'll need to consult the datasheet to find out which pins do what. 

![Alt text](/assets/images/posts/2020-03-05-build-085.png)

Some of the pins will be easy to understand; pins 17 and 16, for example, are power and ground, so you can connect those now (remember: power from the 3.3 volt rail; orange wire, if it helps).

![Alt text](/assets/images/posts/2020-03-05-build-090.jpg)

Similarly, we can see that RESET is on pin 11, so we can use a white wire to hook that signal up to a button (the other side of the button connects to ground): 

![Alt text](/assets/images/posts/2020-03-05-build-100.jpg)

But wait: that pin 11 had a weird name next to it, PA28. In fact, all of the pins have names that don't match up with their numbers. If you flip to page 13 of the datasheet, you'll find a table called "I/O Multiplexing and Considerations" which is likely to sow more confusion: 

![Alt text](/assets/images/posts/2020-03-05-build-106.png)

This device is much more complicated than our shift registers. It supports something called pin **multiplexing**, or **mux**ing, which means that you can assign different functions and peripherals to different pins. You can always use any of these pins as general purpose IO (for general purpose blinking!), but these columns A through H represent more specific things you can do with them. 

![Alt text](/assets/images/posts/2020-03-05-build-107.png)

We're going to talk about this more next week, but the important ones for us right now are the USB pins. On page 14, we see that in the COM column (column G) the USB pins are PA24 and PA25 (physical pins 14 and 15). So we're going to run a wire from PA24 (USB/DM or Data Minus) to the D- pin of the USB cable, and a wire from PA25 (USB/DP or Data Plus) to the D+ pin: 

![Alt text](/assets/images/posts/2020-03-05-build-110.jpg)

You might ask: why the complexity? Why not just mark these pins as USB and be done with it? The answer is that these microcontrollers are designed to be versatile. Let's say you were using this chip to control a motor, and didn't need USB. You could assign PA24 to a motor control function, and make PA25 a button to tap on, for example. The point I'm trying to get at is, there's some inherent complexity, but there's a method to it, you'll get used to it, and after a while you'll be highlighting lines in spreadsheets figuring out which pins work best for which purposes. 

It actually gets kind of fun :)

## More Filtering / Decoupling Capacitors

![Alt text](/assets/images/posts/2020-03-05-samd11-caps.png)

There are two more capacitors drawn on here, near our microcontroller. These are smaller in value, 1µF, but serve a very similar purpose to the bigger capacitors near our power supply. 

The first one spans the pins connecting the microcontroller to power and ground. As the microcontroller does work, it may draw more or less current depending on what it's doing; this can manifest as little spikes and dips in the voltage level. Placing a capacitor near this power pin smooths out these spikes right there where they happen, so this noise doesn't make its way to the rest of the circuit. It's very similar to the filtering capacitors next to the power supply; in fact, I've seen the terms filtering and **decoupling** used interchangeably. 

![Alt text](/assets/images/posts/2020-03-05-build-120.jpg)

The fact is, as you add more IC's to your design, you'll generally want to put a capacitor like this near each one's power pin to act as a little energy reservoir. This is where the decoupling name comes from: without these capacitors in place, sudden current draw could cause a voltage drop that could affect other parts of the circuit; with this little capacitor in place, it can provide that extra bit of power quickly, decoupling the IC's power draw from the rest of the board. 

![Alt text](/assets/images/posts/2020-03-05-build-130.jpg)

The last filtering capacitor is betewen the AREF pin and ground. We're eventually going to configure that pin to act as a reference voltage for comparing other analog signals, so the capacitor here just keeps that reference voltage stable. It's not connected to anything right now, so you don't have to worry about it.

## The LED

![Alt text](/assets/images/posts/2020-03-05-led.png)

Every board need an LED for blinking. In our case, we've chosen to put the LED on PA31, which is physical pin 13. Stick the long leg of your LED in that column, and the short leg into an unused one; then run a current limiting resistor from the short leg to ground. 

![Alt text](/assets/images/posts/2020-03-05-build-130.jpg)

Color is your choice! I picked green.

## The SWD Interface

![Alt text](/assets/images/posts/2020-03-05-swd.png)

This last bit is more or less just a pair of wires for the **Serial Wire Debug** interface. The reason we need them: when we buy a microcontroller like this, it's blank, not running any kind of code, and certainly not the kind of code that would be able to talk to the Arduino IDE. We're going to use these wires to burn a **bootloader** onto the microcontroller, which will configure its USB pins to talk to our computer, and to run whatever code we put onto it with Arduino. 

![Alt text](/assets/images/posts/2020-03-05-build-140.jpg)

II'm using a yellow wire for SWCLK and a blue wire for SWDIO, plugged in to pins 12 and 13, respectively. The only additional component here is a pull-up resistor for SWCLK; this is just a requirement for the SWCLK line, which you can find in the datasheet.

![Alt text](/assets/images/posts/2020-03-05-build-150.jpg)

You'll also note that the SWDIO wire shares a pin with the LED. Two reasons for this: one, after we're done burning the bootloader, we can do whatever we want with this pin, including making it our blinky light. But also, two: this means that when you're burning the bootloader, you'll be able to see the LED light up whenever data is going over the debug interface. This will help you to see that it's working. 

Now all we have to do is burn a bootloader onto it, and we'll be able to program it with Arduino! 

## Burning the Bootloader

For this step, you'll need another Feather (again, you can reuse the M0 Express from the Hiking Log). We're going to use an Adafruit library called Adafruit_DAP to burn the bootloader onto our SAMD11. Install the following library from the Arduino Library Manager:

* Adafruit DAP Library

And then download this Arduino sketch: 

[link]

We're going to connect the Reset, SWDIO and SWCLK pins from the protoboard to pins 10, 11 and 12 on the Feather that's doing the programming. 

![Alt text](/assets/images/posts/2020-03-05-build-160.jpg)

While we're at it, just to keep things simple, plug the USB pin from the programming feather in to the protoboard's 5V rail, and GND to the protoboard's ground. This will let us power both boards from one USB port: 

![Alt text](/assets/images/posts/2020-03-05-build-170.jpg)

Run the sketch! And then open the Arduino serial monitor. You should see something like this: 

![Alt text](/assets/images/posts/2020-03-05-build-175.png)

Also the programming feather's red LED should be blinking, to signal that it's done.

![Alt text](/assets/images/posts/2020-03-05-build-180.jpg)

Press the reset button on the protoboard, and you should see its LED start pulsing on and off. That's the bootloader code running! 

![Alt text](/assets/images/posts/2020-03-05-build-200.jpg)

Unplug the programming feather, along with the SWD and power wires. Finally, you can plug the device in to your computer, and you should see the green light pulse. 

![Alt text](/assets/images/posts/2020-03-05-build-210.jpg)

Congratulations, you've just bootstrapped a basic microcontroller board! 

## Running the Countdwown Clock on the SAMD11

This is the last act of this week's guide, and I think it's pretty exciting. At this point, you've built a countdown clock from parts, and you've built a microcontroller from parts; now you're going to plug the one into the other. 

[WIP]