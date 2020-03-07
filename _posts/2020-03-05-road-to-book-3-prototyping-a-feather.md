---
layout: default
title: "Road to the Book #3: The Countdown Clock and The Protoboard Feather"
image: /assets/images/posts/2020-03-05-card.jpg
description: In which we commit the countdown clock to a more permanent arrangement, and prototype a microcontroller board to drive it.
---

# Road to the Book #3: The Countdown Clock and The Protoboard Feather

Full disclosure: I am SO STOKED for this guide and the next one. So far, you've been putting together hardware that works with pre-made Feather boards. Which are rad, and powerful, and contain all kinds of electronic goodness to make it easy to build awesome things like [a log-shaped data logger](/2020/02/20/road-to-book-1-the-hiking-log/). This week, though, we get to peek into that electronic goodness by prototyping our own board, using basic electronic components, that does many of the same things; it will even plug in to the Ardiuino IDE! By the end, we'll run the last week's countdown clock using this protoboard creation, and you'll more or less be left with a clock that you built entirely from scratch, whose every component you understand. 

Before we do that, though, we need to move [the existing countdown clock build](/2020/02/27/road-to-book-2-the-countdown-clock/) off of the protoboard and on to something more permanent — partly because it's kind of fragile in its current state, but mostly because we need to free up the protoboard for this project! So this lesson comes in two parts: first up, through-hole soldering skills. You're going to move the entire countdown clock circuit onto a Perma-Proto board, soldering it permanently into place. 

Then, we'll learn about the resistors, **capacitors** and **integrated circuits** (**IC**s) that will drive our DIY microcontroller board. 

## Parts List

This comes in two parts. First, for soldering down the countdown clock:

* [Last week's build](http://oddlyspecific.org/2020/02/27/road-to-book-2-the-countdown-clock/), of course
* [Adafruit Perma-Proto Half-sized Breadboard](https://www.adafruit.com/product/1609)
* [0.1" Female header](https://www.adafruit.com/product/598) (optional but recommended)
* Red, black and yellow solid-core wire from last time...
* ...plus [green](https://www.adafruit.com/product/2988), [blue](https://www.adafruit.com/product/2989) and [orange](https://www.adafruit.com/product/2986) solid-core wire! (if you want it; you could just use yellow for everything, but I'm going to match the colors so it's easier to folllow)
* A Swiss Army Knife, X-Acto blade or other sharp implement (we're going to cut some traces on the protoboard).

Then, for the protoboard Feather, we're going to have to venture out to another supplier. Adafruit has tons of awesome boards and kits and supplies, but for individual electronic components, you need a supplier like DigiKey, which is like an everything store for components. 

* [SAM D11 Microcontroller](https://www.digikey.com/product-detail/en/microchip-technology/ATSAMD11D14A-SSUT/ATSAMD11D14A-SSUTCT-ND/5226483) (you'll need three by the end of these guides; consider grabbing a handful). 
* 2x [10µF capacitor](https://www.digikey.com/product-detail/en/tdk-corporation/FG28X5R1E106MRT00/445-181283-ND/7384816)
* 3x [1µF capacitor](https://www.digikey.com/product-detail/en/tdk-corporation/FG18X7R1E105KRT00/445-180204-ND/7326658)
* 1x [LM1117T-3.3 voltage regulator](https://www.digikey.com/product-detail/en/texas-instruments/LM1117T-3.3-NOPB/LM1117T-3.3-NOPB-ND/363593) (you only need one of these)
* 1x [1KΩ Resistor](https://www.digikey.com/product-detail/en/stackpole-electronics-inc/CF14JT1K00/CF14JT1K00CT-ND/1830350)
* 1x [10KΩ Resistor](https://www.digikey.com/product-detail/en/stackpole-electronics-inc/CF14JT10K0/CF14JT10K0CT-ND/1830374)
* 1x [100KΩ Resistor](https://www.digikey.com/product-detail/en/stackpole-electronics-inc/CF14JT100K/CF14JT100KCT-ND/1830399)
* [Diffused 3mm LED Pack](https://www.digikey.com/product-detail/en/4202/1528-2951-ND/10130501/) ([also available directly from Adafruit](https://www.adafruit.com/product/4202))
* and an [Adafruit SOIC-20 Breakout](https://www.digikey.com/product-detail/en/adafruit-industries-llc/1206/1528-1066-ND/5022795) ([also available directly from Adafruit](https://www.adafruit.com/product/1206))

Also note, some of the capacitors and resistors in this list have a price break at 10 or 25; these are very common parts, so you might want to order 10 or even 25 instead of the one or two I'm recommending you grab here. 

## Finishing Up the Countdown Clock

I love the Adafruit perma-proto we're using for this step. Since it's laid out and labeled just like our solderless breadboard, it's trivial to just recreate every wire in every hole and end up with a circuit that works just the same. Having said that, before we move the build over, it's useful to take a step back and understand just what we built. To do that, we're going to draw a schematic: 

![Alt text](/assets/images/posts/2020-03-05-clock-schematic.jpg)

Schematics like these are the bread and butter of electronics design. They won't be color coded like this, but they will have a few features that are illustrated here. 

First off: **symbols**. Each component in a circuit, whether a microcontroller or a humble resistor, gets its own symbolic representation in the schematic. Some symbols have a value (220Ω in the case of our small army of resistors). They'll also usually have a name, which is something we'll do later in this guide. 

More complex symbols like ICs should show all the available pins for the component, and they can be arranged sequentially like the pins of the chip — I've done this with the shift registers, for example — but they could just as easily be arranged logically, like the 7-segment displays, where the A-G ordering makes it easier to draw the connections. The important thing that your symbol needs to convey is the function of each pin that a wire can connect to, and a number representing the physical position of the pin (we'll do that in the next schematic too). 

Speaking of which: **nets**. These are the wires that connect pins to each other. Everything connected to a wire is a part of the same net. These wires can cross each other (you can see the clock, latch and data lines all crossing) but they don't actually connect unless there's a **junction**, represented by a dot at the intersection (like where the latch and clock lines turn 90 degrees to connect to each IC). You'll also often see **names** for nets (here in little tags). It would be unweildy to draw a line from the clock or latch pins all the way to the associated pin on the Feather, so we give it a name, and draw a net with the same name connected elsewhere in the schematic. These too are connected.

Similarly: **power** and **ground** rails. You remember that we set up our breadboard with a 5 volt and a 3 volt power rail, on the top and bottom, as well as ground rails on either side. Lots of things connect to power and ground, so to avoid having those wires everywhere, you'll often use symbols like arrows to represent a power source, and a symbol with a horizontal line or lines to symbolize ground. In a circuit with multiple different voltage levels like ours, you'll want to label them like we've done here; they won't be red and orange, but the 5V and 3V labels will make clear which power pin is connected to which device. 

Finally, while this isn't a universal feature of schematics, you will often see an X drawn over a pin that's been intentionally left **not connected** (**NC**). I did that for the Q0 output of the shift registers, and for the four pins on the breakout board that we didn't solder header to. Honestly though, I see a lot of schematics that omit this marking.

Anyway, there are tons more symbols you're likely to encounter (and we'll discuss them as they come up), but for the most part, this primer prepares you for the kinds of schematics you're likely to draw when designing your own circuits. And as we wire our countdown clock to our perma proto board, start to think about how the wires you are running are just connecting the various pins to various nets; it's going to make the next bit make even more sense. 

### Wire Up the Perma Proto

Before we start, take a look at the front and back side of your perma proto board. Note the numbered columns on the front side, and how the traces on the back connect the top and bottom columns, as well as the power and ground rails. 

![Alt text](/assets/images/posts/2020-03-05-clockbuild-010.jpg)

There's no step to take here; the only reason I mention it is to point out the copper traces, by way of reiterating that these electrical connections are how the perma-proto works, how it makes things connect. You probably already get this. But we're going to recall this moment next week, so I figured I'd call it out right now.

Anyway. Carefully withdraw the three shift registers from your solderless breadboard, and place them, with their notches aligned to the left, in the same places in the perma proto board: columns 1, 10 and 19 (but don't solder them yet): 

![Alt text](/assets/images/posts/2020-03-05-clockbuild-020.jpg)

You can start to see how if we just moved all the parts over as-is, matching every wire to every column, we could transfer the whole board over and it would work the same way. But there's one optimization I want us to make here, one that we couldn't make on our breadboard. It has to do with the army of 220Ω resistors. Take a look at the schematic, specifically the row of outputs at the bottom of the shift register. In fact, just focus on output Q1, the state of segment A: 

![Alt text](/assets/images/posts/2020-03-05-clock-nets-1.jpg)

If a net is everything connected to a single wire, you'll note that there are actually two nets here: 

1. the shift register's output Q1, connected to one side of the resistor, and
2. the display's input A, connected to the other side of the resistor.

![Alt text](/assets/images/posts/2020-03-05-clock-nets-2.jpg)

They're two separate nets, because they're on opposite sides of the resistor: the wire on the chip side is the signal from the shift register, and the wire on the display side is the current limited output to the segment. We solved this on our protoboard by plugging one end into the board, and the other end into the wire going to the display:

![Alt text](/assets/images/posts/2020-03-05-clockbuild-030.jpg)

But it's a fragile arrangement. It would be better if we could solder those resistors into the board — and it turns out, we can rework this perma-proto to do that! We're going to use a sharp knife to cut 21 of these columns in half, giving us a net for the outputs from the chip (which one end of the resistor will plug into) and a net for the input to the display (which the other side of the resistor will plug into. 

Taping the shift registers if necessary, turn over the board and look at the columns on the bottom: 

![Alt text](/assets/images/posts/2020-03-05-clockbuild-040.gif)

You're going to bisect columns 1-7 on the bottom side, as well as columms 9-15 and 19-25, leaving two pins at the bottom and three pins at the top. It may help to draw some kind of note with a Sharpie, to make clear which columns you're going to be cutting: 

![Alt text](/assets/images/posts/2020-03-05-clockbuild-050.jpg)

Remove the shift registers, and grab your Exacto blade (or some other kind of knife). CAREFULLY cut the traces, leaving two pins at the bottom and three pins at the top. Really get in there, though; don't be afraid to cut a groove into the fiberglass. You need to make sure that the two sides are electrically isolated, so no copper should be left in between. 

![Alt text](/assets/images/posts/2020-03-05-clockbuild-060.jpg)

![Alt text](/assets/images/posts/2020-03-05-clockbuild-070.jpg)

*Also note: if you accidentally cut a trace you didn't mean to (I certainly did while taking photos for this guide, don't worry, you can always patch it up later by connecting both sides with some solder or a wire.*

Now we have two nets for each of these columns: one to connect to the chip's output, and another to connect to the display's input. If you want, on the front side, you can use your Sharpie to draw a line where you cut the trace, either to double check your work or to serve as a reminder later on: 

![Alt text](/assets/images/posts/2020-03-05-clockbuild-080.jpg)

Now you're ready to start soldering! Start with the shift registers, being very careful to keep the notches aligned to the left, and the chips aligned with the correct columns: 

![Alt text](/assets/images/posts/2020-03-05-clockbuild-090.jpg)

![Alt text](/assets/images/posts/2020-03-05-clockbuild-100.jpg)

Next let's get that army of resistors in place. Pull them out of the cables...

![Alt text](/assets/images/posts/2020-03-05-clockbuild-120.jpg)

...and solder them in across the row you cut, making sure to leave one hole open at the bottom (that's where we'll connect the wires going to the displays). 

![Alt text](/assets/images/posts/2020-03-05-clockbuild-130.jpg)

![Alt text](/assets/images/posts/2020-03-05-clockbuild-140.jpg)

![Alt text](/assets/images/posts/2020-03-05-clockbuild-150.jpg)

Next up: the Extremely Accurate RTC. For this, we'll use that 0.1" female header, instead of soldering it directly to the board; it's a very nice RTC, and you might want to reuse it in a future project. 

Carefully break a length of four pins from the 36-pin header. It can help to score the plastic on both sides with a knife, and then break it with your hands.

![Alt text](/assets/images/posts/2020-03-05-clockbuild-160.jpg)

The header should split apart easily. A pin will likely fall out from the column you scored; discard it, but hang on to the rest of the header, we'll be using more of it next week. 

![Alt text](/assets/images/posts/2020-03-05-clockbuild-170.jpg)

Solder it into place. 

![Alt text](/assets/images/posts/2020-03-05-clockbuild-180.jpg)

![Alt text](/assets/images/posts/2020-03-05-clockbuild-190.jpg)

Next up: solder in the power and ground connections using red, orange and black wire. We're going to recreate the setup we had on the solderless breadboard: 5V on the top power rail, 3V on the bottom, and connect the two ground rails: 

![Alt text](/assets/images/posts/2020-03-05-clockbuild-200.jpg)

Solder wires to power the shift registers with 5 volts and the RTC with 3.3. To keep the layout tidy, I like to cut really short lengths of wire, just long enough to get the job done. 

![Alt text](/assets/images/posts/2020-03-05-clockbuild-210.jpg)

While you're at it, tie the OE and MR pins to GND and 5 volts, respectively, If you get confused, just consult the breadboard; you're making the same connections here as there. 

![Alt text](/assets/images/posts/2020-03-05-clockbuild-220.jpg)

Next, connect the shift registers' clock, latch and data lines. I used blue, green and yellow wires here, but the color doesn't matter, as long as the correct pins are connected to each other: 

![Alt text](/assets/images/posts/2020-03-05-clockbuild-230.jpg)

And while you have that wire out, cut some longer lengths of wire to connect the first shift register to the clock, latch and data lines on the Feather, as well as SCL and SDA. Let them trail off to the right; you'll eventually hook up your Feather somewhere in that general vicinity. 

![Alt text](/assets/images/posts/2020-03-05-clockbuild-240.jpg)

Now, unplug all the ribbon cables from the big red digits, and cut two sets of them in half. This will give you four sets of cables with female socket on one end, and stranded wire on the other. Set aside three and strip a short length of the ends; we'll use these to connect the displays to the board: 

![Alt text](/assets/images/posts/2020-03-05-clockbuild-250.jpg)

![Alt text](/assets/images/posts/2020-03-05-clockbuild-260.jpg)

Tin the ends of the wires using a little bit of solder...

![Alt text](/assets/images/posts/2020-03-05-clockbuild-270.jpg)

...and then solder them one by one them into the bottom row, in the order of the rainbow (red, orange, yellow, green, blue, purple, gray)

![Alt text](/assets/images/posts/2020-03-05-clockbuild-280.jpg)

By the way, if at any point in this you mess up and fill a hole with solder: 

![Alt text](/assets/images/posts/2020-03-05-clockbuild-290.jpg)

Don't fret. Use your solder sucker and soldering iron to suck it out, and try again. 

![Alt text](/assets/images/posts/2020-03-05-clockbuild-300.jpg)

![Alt text](/assets/images/posts/2020-03-05-clockbuild-310.jpg)

![Alt text](/assets/images/posts/2020-03-05-clockbuild-320.jpg)

Once you have everything soldered into place, plug in your big red digits and the DS3231 RTC. Hook the trailing wires up to power, data, clock, latch and I2C pins. 

![Alt text](/assets/images/posts/2020-03-05-clockbuild-330.jpg)

Now plug the Feather in to power! If all's gone well, you should see the same output: the number of days until 2021. 

![Alt text](/assets/images/posts/2020-03-05-clockbuild-340.jpg)

Set aside your perma-proto assembly. And unplug the remaining breadboard wires.

![Alt text](/assets/images/posts/2020-03-05-clockbuild-350.jpg)

You'll be using them for what comes next.

## The Protoboard Feather

The project for the rest of this lesson (and the next one) is a DIY microcontroller board that we'll soon turn into a DIY Feather. It's not going to be quite as full-featured as the Feather M0 Express you've been using — there won't be any Flash chip, enable pin or battery support, for example — but the tradeoff is that it's going to be very simple to understand. With just a dozen or so components, we're going to build a microcontroller board that plugs in to the USB port of your laptop, that you can program with the Arduino IDE. 

Earlier we talked a little bit about schematic diagrams, and drew one to represent the countdown clock that we built. It turns out that's often a pretty good first step in the making of things: draw a diagram of the thing you plan to build, then prototype it, then build it. So here's the schematic for the DIY Feather: 

![Alt text](/assets/images/posts/2020-03-05-schematic.png)

Looks daunting? Don't worry. We're going to walk through each part as we put it on the board. 

### The USB Port

![Alt text](/assets/images/posts/2020-03-05-usb.png)

This is just an adapter that runs the tiny pins of a Micro-USB port out to breadboard-friendly pins; there are no other parts. We're putting this piece down first, because this is where the power comes from: whether it's your laptop or a power brick, the USB port has a ground pin and a power pin that provides 5 volts of power. We can draw up to 500 mA from this power pin, and it's going to become our 5 volt power rail: 

![Alt text](/assets/images/posts/2020-03-05-build-020.jpg)

The ID pin, we're going to ignore. 

![Alt text](/assets/images/posts/2020-03-05-build-030.jpg)

The D+ and D- pins are USB data; this is the actual signal that goes from the microcontroller to your laptop. We'll hook these up in a moment. 

### The Voltage Regulator

![Alt text](/assets/images/posts/2020-03-05-regulator.png)

We haven't talked about this kind of part yet: a **voltage regulator** takes a power source at a higher voltage, and regulates it down to a lower voltage. In this case, our LM1117T-3.3 can take any voltage up to 6 volts, and regulate it down to 3.3 volts, which is what we need for our microcontroller and our 3.3 volt rail. The Feathers we've used so far have all had one of these, and if you ever wondered where that 3.3 volt power was coming from, this is the answer!

![Alt text](/assets/images/posts/2020-03-05-build-040.jpg)

The LM1117T has three pins: an IN pin for the 5 volts coming from the USB port, a GND pin to connect to ground, and an OUT pin that we'll connect to our 3.3 volt power rail. This regulator will provide up to 800 mA, which is way more than we're likely to need for our little microcontroller projects. 

![Alt text](/assets/images/posts/2020-03-05-build-050.jpg)

### Filtering Capacitors

![Alt text](/assets/images/posts/2020-03-05-regulator-caps.png)

There are a handful of really, really, *really* fundamental electronic components. One of them is the resistor; already we've used them to pull up signals and limit current. The **capacitor** is a similarly fundamental component, and similarly, you're going to find all kinds of different uses for them. 

A capacitor, fundamentally, stores an electrical charge. Its symbol, a pair of plates that aren't touching, is actually pretty accurate to the way it works: when a voltage is applied to it, a positive charge builds up on one side, and negative charge builds up on the other side. This charge builds to a maximum value, the capacitor's **capacitance**, which is measured in **farads** (F). A farad is a huge amount of capacitance, though, so you're likelier to encounter capacitors in the **picofarad** (**pF**), **nanofarad** (**nF**) or **microfarad** (**µF** or **uF**) range. 

The purpose of these two capacitors is to smooth out the voltage levels at either side of the power supply. Why do we need this? Well as it turns out, while your USB port tries to provide 5 volts, and the regulator tries to provide 3.3, that voltage can fluctuate a bit depending on what's going on in the circuit.

Say, for example, you turned on all seven segments of the countdown clock's three displays, all at once. Driving each segment at 7 mA, you'd have a sudden current draw of 147 mA on the 5 volt rail. This could cause the voltage on the 5 volt rail to drop momentarily. Why does that matter? Well, that 5 volt rail is also powering those three shift registers, which need a stable voltage level to do their job. Spikes and dips on the 5 volt rail will make your circuit unreliable. 

![Alt text](/assets/images/posts/2020-03-05-build-060.jpg)

So we have this 10µF capacitor, sitting between 5 volts and ground. As soon as you plug this gadget in, there will be a 5 volt potential between the two sides of the capacitor, so it will begin charging until it's full. Then, if you were to turn on those power-hungry LED's, instead of the voltage level dropping below 5 volts, the capacitor will begin to discharge, since it still has a 5 volt potential across its terminals. It's a buffer, of sorts, that keeps the voltage level stable. 

We can call it a **filtering** capacitor, because it filters out high frequency noise. We have these capacitors on both the 5 volt and the 3 volt rail, to smooth out both sides of the power supply. 

### The Bleeder Resistor

![Alt text](/assets/images/posts/2020-03-05-regulator-resistor.png)

The thing about a capacitor: it charges up, and then it stays charged. When you unplug your gadget from the USB port, it's still charged with 5 volts! And while that power might drain eventually through the other components on the board, it would be a lot safer to make sure that the capacitor discharges on its own. 

![Alt text](/assets/images/posts/2020-03-05-build-070.jpg)

This 100K resistor does the job. Sitting between 5V and ground, it provides a path for any charge stored in the capacitor to drain safely to ground when unplugged. 

### The SAM D11 Microcontroller

![Alt text](/assets/images/posts/2020-03-05-samd11.png)

This is the big part. The brains of the operation. A **microcontroller** or **MCU** is kind of like a little all-in-one computer, with its own processing unit, storage, RAM and peripherals. This little microcontroller gives you a lot of bang for a literal buck (it costs between 60¢ and $1.50): 17 pins of general purpose IO, hardware support for the I2C protocol that drives our Extremely Accurate RTC, as well as UART (which drove the Hiking Log's GPS) and SPI (which we started to talk about last week). It even has a native USB interface, which means we can more or less plug the USB pins from our computer straight into the MCU. 

But there's one thing we have to do first. You may have noticed that this chip doesn't have pins you can use to plug it into a breadboard. That's because this chip is **surface mount**; in fact, it's the first surface mount part we're going to deal with. To start prototyping with it, we are going to have to solder it on to a **breakout board**. 

#### A break for soldering the breakout board

I didn't have time to shoot photographs of this process myself, so I'm borrowing some images from [Adafruit's guide on this topic](https://learn.adafruit.com/smt-prototyping-using-breakout-pcbs?view=all) by Ladyada herself, CC-BY-SA 3.0 (the chip she's soldering has 14 pins, but the premise is the same). 

Here's how you do it: first, find the dot on the SAMD11 chip. This is pin 1. Now find the dot or pin 1 marker on the breakout board. Using your soldering iron, melt a little bit of solder onto that pad: 

![Alt text](/assets/images/posts/2020-03-05-build-072.jpg)

Now, with tweezers, set the chip down on the footprint. Holding on to the chip with your tweezers, melt that solder, and connect pin 1 of the chip to pin 1 on the breakout board. 

![Alt text](/assets/images/posts/2020-03-05-build-073.jpg)

Now, look at the breakout board from the top. Make sure all the pins are aligned with all the pads. If they're not? Melt the pin 1 solder again and reposition the chip with your tweezers, until it's perfect. Once you have it in place, look at the absolute other side of the chip, to pin 20. Use your soldering iron to connect pin 20 to the breakout board. This will lock the chip into position. 

![Alt text](/assets/images/posts/2020-03-05-build-076.jpg)

Finally, just go around the board and solder each pin to each pad! 

![Alt text](/assets/images/posts/2020-03-05-build-077.jpg)

When you're done, attach some headers, plug it in to the protoboard, and solder it down. 

![Alt text](/assets/images/posts/2020-03-05-build-079.jpg)

![Alt text](/assets/images/posts/2020-03-05-build-080.jpg)

#### And now back to the protoboard

Now it's time to wire up our microcontroller. Much like with the shift registers, you'll need to consult the datasheet to find out which pins do what:

![Alt text](/assets/images/posts/2020-03-05-build-085.png)

Some of the pins will be easy to understand; pins 17 and 16, for example, are power and ground, so you can connect those now (remember: power from the 3.3 volt rail; orange wire, if it helps).

![Alt text](/assets/images/posts/2020-03-05-build-090.jpg)

Similarly, we can see that RESET is on pin 11, so we can use a white wire to hook that signal up to a button (the other side of the button connects to ground): 

![Alt text](/assets/images/posts/2020-03-05-build-100.jpg)

But wait: that pin 11 had a weird name next to it, PA28. In fact, all of the pins have names that don't match up with their numbers. If you flip to page 13 of the datasheet, you'll find a table called "I/O Multiplexing and Considerations" which is likely to sow more confusion: 

![Alt text](/assets/images/posts/2020-03-05-build-106.png)

This device is much more complex than our shift registers were. It supports something called pin **multiplexing**, or **mux**ing, which means that you can assign different functions and peripherals to different pins. You can always use any of these pins as general purpose IO (for general purpose blinking!), but these columns A through H represent more specific things you can do with them. 

![Alt text](/assets/images/posts/2020-03-05-build-107.png)

We're going to talk about this more next week, but the important ones for us right now are the USB pins. On page 14, we see that in the COM column (column G) the USB pins are PA24 and PA25 (physical pins 14 and 15). So we're going to run a wire from PA24 (USB/DM or Data Minus) to the D- pin of the USB cable, and a wire from PA25 (USB/DP or Data Plus) to the D+ pin: 

![Alt text](/assets/images/posts/2020-03-05-build-110.jpg)

You might ask: why the complexity? Why not just mark these pins as USB and be done with it? The answer is that these microcontrollers are designed to be versatile. Let's say you were using this chip to control a motor, and didn't need USB. You could assign PA24 to a motor control function, and make PA25 a button to tap on, for example. You get to choose which pins you want to use for which purpose, which lets you bend the gadget to your specific needs. 

The point I'm trying to get at is, there's some inherent complexity, but there's a method to it, you'll get used to it, and after a while you'll be highlighting lines in spreadsheets figuring out which pins work best for which purposes. It actually gets kind of fun :)

## More Filtering / Decoupling Capacitors

![Alt text](/assets/images/posts/2020-03-05-samd11-caps.png)

There are two more capacitors drawn on here, near our microcontroller. These are smaller in value, 1µF, but serve a very similar purpose to the bigger capacitors near our power supply. 

The first one spans the pins connecting the microcontroller to power and ground. As the microcontroller does work, it may draw more or less current depending on what it's doing; this can manifest as little spikes and dips in the voltage level. Placing a capacitor near this power pin smooths out these spikes right there where they happen, so this noise doesn't make its way to the rest of the circuit. It's very similar to the filtering capacitors next to the power supply; in fact, I've seen the terms filtering and **decoupling** used interchangeably. 

![Alt text](/assets/images/posts/2020-03-05-build-120.jpg)

The fact is, as you add more IC's to your design, you'll generally want to put a capacitor like this near each one's power pin to act as a little energy reservoir. This is where the decoupling name comes from: without these capacitors in place, sudden current draw could cause a voltage drop that could affect other parts of the circuit; with this little capacitor in place, it can provide that extra bit of power quickly, decoupling the IC's power draw from the rest of the board. 

![Alt text](/assets/images/posts/2020-03-05-build-130.jpg)

The last filtering capacitor is betewen the AREF pin and ground. We're eventually going to configure that pin to act as a reference voltage for comparing other analog signals, so the capacitor here just keeps that reference voltage stable. It's not connected to anything right now, though.

## The LED

![Alt text](/assets/images/posts/2020-03-05-led.png)

Every board needs an LED for blinking. In our case, we've chosen to put the LED on PA31, which is physical pin 13. Stick the long leg of your LED in that column, and the short leg into an unused one; then run a 1KΩ current limiting resistor from the short leg to ground. 

![Alt text](/assets/images/posts/2020-03-05-build-130.jpg)

Color is your choice! I picked green.

## The SWD Interface

![Alt text](/assets/images/posts/2020-03-05-swd.png)

This last bit is more or less just a pair of wires for the **Serial Wire Debug** interface. The reason we need them: when we buy a microcontroller like this, it's blank, not running any kind of code, and certainly not the kind of code that would be able to talk to the Arduino IDE. We're going to use these wires to burn a **bootloader** onto the microcontroller, which will configure its USB pins to talk to our computer, and to run whatever code we put onto it with Arduino. 

![Alt text](/assets/images/posts/2020-03-05-build-140.jpg)

I'm using a yellow wire for SWCLK and a blue wire for SWDIO, plugged in to pins 12 and 13, respectively. The only additional component here is a 10KΩ pull-up resistor for SWCLK; this is just a requirement for the SWCLK line, which you can find in the datasheet.

![Alt text](/assets/images/posts/2020-03-05-build-150.jpg)

You'll also note that the SWDIO wire shares a pin with the LED. Two reasons for this: one, after we're done burning the bootloader, we can do whatever we want with this pin, including making it our blinky light. But also, two: this means that when you're burning the bootloader, you'll be able to see the LED light up whenever data is going over the debug interface. This will help you to see that it's working. 

Now all we have to do is burn a bootloader onto it, and we'll be able to program it with Arduino! 

## Burning the Bootloader

For this step, you'll need another Feather (again, you can reuse the M0 Express from the Hiking Log). We're going to use an Adafruit library called Adafruit_DAP to burn the bootloader onto our SAMD11. Install the following library from the Arduino Library Manager:

* Adafruit DAP Library

And then download this Arduino sketch: [Flash SAMD11 Bootloader](/assets/code/2020-03-05-Flash_SAMD11_Bootloader.zip)

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

## Running the Countdown Clock on the SAMD11

This is the last act of this week's guide, and I think it's pretty exciting. At this point, you've built a countdown clock from parts, and you've built a microcontroller from parts; now you're going to plug the one into the other. 

Before we start, though, you'll need to install one more bit of support code. The Arduino IDE doesn't have support for this chip out of the box, so we're going to have to add it. We've done this once before, to add Adafruit's boards, and [the same instructions](https://learn.adafruit.com/add-boards-arduino-v164/setup) still apply: open Arduino preferences, and add the following URL to the Additional Boards Manager URLs: 

```
https://www.mattairtech.com/software/arduino/package_MattairTech_index.json
```

and then in Tools -> Board -> Board Manager, install the "MattairTech SAM D/L/C Core". This adds support for a "Generic D11D14AS" board, which is what we are. 

Next up: locate the perma-proto countdown clock you built earlier, and plug it in to the prototype SAMD11 board we just built. 5V anf 3V power go to the power rails, ground to ground, and then the data connections should be as follows: 

* SDA: Pin 6 (this is also known as PA14)
* SCL: Pin 7 (this is also known as PA15)
* Shift register latch: Pin 8 (this is also known as PA16)
* Shift register data: Pin 9 (this is also known as PA22)
* Shift register clock: pin 10 (this is also known as PA23)

It should look something like this: 

![Alt text](/assets/images/posts/2020-03-05-connections.jpg)

Now, open the countdown clock sketch from last week. Change your pin definitions to match the new pinout. This generic board uses the SAMD11 port pin numbers, not the physical pin numbers, so it should look like this: 

```
const int PIN_DISPLAY_CLOCK = 23;
const int PIN_DISPLAY_LATCH = 16;
const int PIN_DISPLAY_DATA = 22;
```

From the Tools -> Board menu, select the "Generic D11D14AS" board. Plug your board in, and click Verify! And you'll get an error.

```
ld: Countdown.ino.elf section `.text' will not fit in region `FLASH'
ld: region `FLASH' overflowed by 1896 bytes
```

So here's the deal with our DIY microcontroller board: it's smaller than the boards you're probably used to. The Feather M0 you were using? Had 256 kilobytes of Flash memory. This board only has 16 kilobytes. That's a big difference! Not only that, but the bootloader we burned on to the board took up 4 kilobytes of that Flash, so we only have 12 kilobytes to work with. 

We're going to need to refactor our code to get it to fit on this chip. 

### Refactoring and Shrinking our code

A brief digression for a valid question you might have: why did I have us use this chip for this example, knowing how little room there was on it for code? A few reasons: one, the SAMD11 shares the same basic architecture as the SAMD51 that powers the Open Book, so the lessons you learn about things like pin muxing are directly applicable later on. Two: roomier chips like the SAMD21 all come in harder-to-solder packages, with really fine pins that require more soldering skill. The wide pitch of this chip is called **SOIC** (or **Small Outline Integrated Circuit**), and it's probably the easiest surface mount chip to solder when first starting out.

But mostly, I think it's awesome to work within constraints — and in this case, the constraints are the right kind of constraints: we're working with a well-engineered, solidly supported chip that has lots of features that will help us. And the exercise of slimming down this code will show where some of the fat came from, so you can be aware of this as you design your own projects in the future. 

So! Let's start slimming down. First things first: if you added any `Serial.println()` statements, delete them. That should shave off a few bytes.

Next, we can disable features we're not using. Go to Tools -> Serial Config, and select "NO_UART_ONE_WIRE_ONE_SPI". Remember, the UART was what we used to talk to the GPS in the Hiking Log last month; in larger Feathers like the M0 or M4, this peripheral is always enabled, but it does require some extra code. Disabling the UART means that we can omit that code. Click "Verify" again and see how we're doing; on my end: 

```
ld: Countdown.ino.elf section `.text' will not fit in region `FLASH'
ld: region `FLASH' overflowed by 480 bytes
```

Oof. So close. At this point, you may be looking at your code and wondering, what else can I cut out? I was there too. But then I looked at the Julian Date function: 

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

One thing I thought was that that's a lot of math, and maybe there was a way to simplify it. In the end, I figured out that by rewriting this only integer math, I could shrink the code size dramatically. [Wikipedia's entry on Julian days](https://en.wikipedia.org/wiki/Julian_day#Converting_Julian_calendar_date_to_Julian_Day_Number) has a formula that fits the bill: 

```
long JulianDate(int year, int month, int day) {
  return 367 * year - (7 * (year + 5001 + (month - 9) / 7)) / 4 + (275 * month) / 9 + day + 1729777;
}
```

Click "Verify" now and...

```
Sketch uses 11040 bytes (89%) of program storage space. Maximum is 12288 bytes.
```

The code fits! At this point, you can run the sketch, and you should see the date appear on your clock. 

![Alt text](/assets/images/posts/2020-03-05-final.jpg)

Congratulations! This clock is yours; you've built it up from base principles to functioning widget.

### Extra Credit

A couple of things you can try: When I built this last time, I added a pair of buttons to the clock board (there are two free columns on the perma-proto, 9 and 18). I used these to have the option of changing the countdown date or current time; there's not a ton of space left on the SAMD11, but I bet you could squeeze some code in to do that. 

Also, last week we talked about the Serial Peripheral Interface protocol, or SPI, and mentioned how the functions we wrote to flash wires on and off bear some striking similarities to that protocol. For this version, I had you wire the clock and data lines to the SAMD11's SCK and MOSI lines, which can function like the Clock and Data lines for our shift registers. Basically, you could get rid of those pin definitions, and run the clock using the SAMD11's native SPI support. This (counterintuitively) increases the code size a bit, since the SPI support code has to account for other modes and options beyond our simple bit-sending loop, but the code still fits, and it's worth experimenting with. [Here's an Arduino sketch with that code](/assets/code/2020-03-05-Flash_SAMD11_Bootloader.zip). 

![Alt text](/assets/images/posts/2020-03-05-nextweek-pcb.png)

Next week, we're going to take the prototype board and do something similar to what we did with the clock: move it to a more permanent home. But this time, we aren't going to use a perma-proto. This time, we're going to use our first custom **printed circuit board** (**PCB**).
