---
layout: default
title: "The Hiking Log"
description: First in a series of guides; teaching the skills to build the book.
---

# Object #1: The Hiking Log

This is the first in a series of educational build logs, designed to teach both the soldering skills necessary to build the Open Book, and the electronics knowledge that will give you the confidence to design your own projects in the future. These eight guides will leave you with five useful objects that you can mount on your bike or toss in a backpack. At each step, you’ll learn important electronics concepts, and move from breadboarding and through-hole soldering to surface mount technology and custom printed circuit board (PCB) design. The final project will be an open-source e-book reader that you’ll build yourself, from scratch. 

These project logs are based around a series of projects that I built in 2019 to teach myself these skills. At this time last year, I had never designed a PCB or soldered surface mount tech. If I’m being perfectly honest, I didn’t really even have an intuition for many basic electronics concepts. I learned by muddling my way through these projects, learning from build guides and open source schematics, from folks like Adafruit and Sparkfun, from answers on StackOverflow and from countless blog and forum posts. In the end, these projects provided what I feel was a good education, so I’m setting them down here (with some minor modifications for usability and progression). 

Anyway, enough with this introduction. The first project in this journey is the Hiking Log. It's designed to log weather and location data to a Flash chip, so that after your hike, you can see where you went and what the conditions were like.

![A wooden, log-shaped object sitting on a small red boulder in the desert. A white plastic sensor protrudes from the end](/assets/images/posts/2020-02-20-log-hero.jpg)

![A computer-rendered desert landscape, from perhaps 400 feet up; yellow data points dot a floodplain, and snake between two small hills. A popover above one data point lists a timestamp (November 2019) and data points; the location in Texas, a temperature of 75 degrees, relative humidity at 30 percent.](/assets/images/posts/2020-02-20-log-plot.jpg)

I’m going to get this out of the way quickly: I **love** this project. I’ve used it countless times on my camping trips. It inspired the name “Oddly Specific Objects”, in that it’s a data logger, that logs location and weather data, and is also shaped like a log. It’s also a union of two hobbies I want very much to support: making things, and getting into the outdoors.

The Hiking Log builds on the Adafruit Feather standard, which we’re going to be making extensive use of throughout these guides. If you’re at all familiar with the Arduino ecosystem, Feather is similar in that it’s a standard for microcontroller boards, that’s compatible with a wide variety of accessory boards (called “Feather Wings”). It’s become a de facto standard in the hobbyist community, owing, I sense, to a few features that make it super useful for modern designs: 

* 3.3V logic, which reduces the need for level shifting.
* Lithium Polymer battery power, so you can take your project on the go.
* Built-in battery charging as a core expectation, so you can plug any Feather into any USB port to recharge.

It’s also super small, making it ideal for portable projects.

## Before we Begin

I want to take a minute to note one important thing: this first guide is going to gloss over *A LOT*. The Feather M0 that we're going to use for this project has more compute power than the Apollo Guidance Computer that went to the moon. The GPS module on top uses RF magic that I don't even remotely comprehend. The power supply circuit — which seamlessly switches from battery to USB power — uses parts that we're not even going to think about until Lesson 5. The goal of this curriculum is to get to a point where you'll understand (almost) everything going on in this widget. But for today, we're mostly interested in understanding **power** and **logic**, the very basics of how gadgets turn on and how they talk to each other. 

Also, there are a handful of things that you’ll make use of throughout many of these projects that are worth mentioning at the outset. You'll need a soldering iron, of course; [this](https://www.adafruit.com/product/180) is basic and works well, or for an upgrade pick, I have [this one](https://www.adafruit.com/product/4244) and love it. You can cut and strip wires with something as basic as a Swiss Army Knife, but a wire stripper like [this one](https://www.adafruit.com/product/147) or [this one](https://www.adafruit.com/product/527) will make your life easier. 

Finally: this bit is a bigger investment, but if you want to make enclosures for your projects, I think a 3D printer is an indispensable tool for a maker. Personally? I recommend the [Monoprice Maker Select](https://www.monoprice.com/product?p_id=13860) as a first printer; print quality is adequate, and I've found it reliable, but mostly it’s monumentally cheap! You can occasionally find one [open-box for $150](https://www.monoprice.com/product?p_id=31186), which is what I paid for mine; but even if you can’t find an open-box one, I think it's still a great value at $220. 

## Understanding the Build

The Hiking Log is going to make use of two boards from Adafruit: the Feather M0 Express, and the Ultimate GPS FeatherWing, which will be mounted on top of each other. We’ll pair these two boards with a DHT22 temperature and humidity sensor; the Ultimate GPS will provide time and location data, and the DHT22 will provide weather data. Power will come from a big 2,200 mAh battery, which will give the log multiple days of power for overnight camping trips. Finally, a slide switch on the bottom will let us turn the log on and off.

#### The Parts List:
* [Red solid-core wire](https://www.adafruit.com/product/288)
* [Yellow solid-core wire](https://www.adafruit.com/product/289)
* [Black solid-core wire](https://www.adafruit.com/product/290)
* [Leaded](https://www.adafruit.com/product/1886) or [Lead-Free Solder](https://www.adafruit.com/product/1930) (your choice)
* [Feather M0 Express](https://www.adafruit.com/product/3403)
* [Short headers kit](https://www.adafruit.com/product/2940)
* [Ultimate GPS FeatherWing](https://www.adafruit.com/product/3133)
* [DHT22 temperature and humidity sensor](https://www.adafruit.com/product/385)
* [2200 mAh battery](https://www.adafruit.com/product/1781)
* [SPDT Slide Switch](https://www.adafruit.com/product/805)
* [Half-size solderless breadboard](https://www.adafruit.com/product/64)
* For 3D Printing: [wood-fill PLA](https://www.monoprice.com/product?p_id=12507)

The overall build will look something like this, if you imagine that the two boards mounted on top of each other:

![A visual diagram of two boards side by side, connecting a sensor to power, data and ground pins, as well a toggle switch connecting a pin marked "enable" to GND](/assets/images/posts/2020-02-20-fritzing.png)

Let's walk through the major parts of the design, before we start soldering it together. 

### Power (and voltage)

First, let’s understand the rough nature of how the build is powered. We’re going to mount the GPS FeatherWing on top of the Feather M0 Express. The Feather provides a 3.3 volt **power rail** on the 3V3 pin. Some of that power is used to operate the microcontroller and the Flash chip on board, but there's plenty leftover, so the wing is going to use that power to run the GPS. We're also going to power the DHT22 from that 3.3 volt power source, using a red wire to the 3V3 pin. 

For power to flow, it has to flow somewhere. That's what the GND pin is for. The way I like to imagine voltage is like a water tower: there's some amount of water in the tower, and it's raised up in the air. It towers 3.3 volts above **ground** (which sits at 0 volts). The height of the water tower represents the difference in potential, or the **voltage**, between the two. In some very fundamental way we can imagine our entire circuit as a skinny pipe that connects the higher potential to the lower potential: 

![A pencil sketch of a water tower at a "height" of 3.3 volts](/assets/images/posts/2020-02-20-watertower-01.png)

Now, there's nothing that says that 3.3 is the highest voltage we can have; in fact, there are two higher potentials in our circuit. The battery usually provides about 3.7 volts, and a USB port provides 5. And really, the circuit is more than a skinny pipe; along the way there are components all over the place that take the flow of that water (i.e. the **current**), and do things with it, like store it in little reservoirs along the way, twist valves that affect which way it flows, or use it to turn gears that do useful things.

One of those useful things is checking the temperature. 

### The Temperature Sensor (and digital logic)

The DHT22 is a great little sensor, with one very awesome feature: it only needs one wire to communicate! This means that we only need to run three wires to it: power, ground and data. The specifics of what goes over the data line are a bit in the weeds at this point — we're going to use an Arduino library that speaks its language — but there is one very important concept that we need to talk about to understand how it works, and that's **digital logic**. Most people know that computers think in terms of 1's and 0's. What's less common knowledge is how those 1's and 0's are represented in the physical world. If you pinch a wire between your fingers, you might wonder: what about it makes the signal inside of it a 1 or a 0? 

One of the fun things about building stuff like this is that manufacturers — at least the maker-friendly ones — tend to publish exhaustive documentation of the products they make, so whatever you're wondering about: you can find out! In the case of the Feather M0's microcontroller, the SAMD21, you can open up [its data sheet](http://ww1.microchip.com/downloads/en/DeviceDoc/SAM_D21_DA1_Family_Data%20Sheet_DS40001882E.pdf) and look for a table called "Electrical Characteristics". In that table, you'll find something like this: 

![A section of data sheet with information about logic levels.](/assets/images/posts/2020-02-20-datasheet.png)

The important bits: 

* Input low-level voltage: Max. 0.3 × VDD
* Input high-level voltage: Min. 0.55 × VDD

Since, as we covered above, we're powering this gadget with 3.3 volts, we can do the math: a 0 or **logic low** is any voltage level below 0.99 volts, and a 1 or **logic high** is any voltage level above 1.815 volts. 

With digital logic, it's very important to make sure that a digital signal is either above the logic high level, or below the logic low level. If the microcontroller is driving it, that's easy: it can set the logic level to 3.3 volts or zero, and any gadget can watch the pipe to see if the voltage on that line is high or low. Imagine a tiny person in our SAMD21, with two buttons in front of them controlling valves at the top and bottom of the water tower. When they press one button, current flows from the top of the tower into whatever gadget is connected to the pipe; when they press the other button, current flows from that gadget down to the ground:

![The same sketch of a water tower. Next to it, a tall box labeled "Push/Pull Output" has two pipes coming from it, one at the maximum height of 3.3 volts, the other down at the ground; both lead to a box labeled "other gadget". A person in the foreground sits at a table with two buttons: "HIGH" and "LOW".](/assets/images/posts/2020-02-20-watertower-02.png)

This works, and it's called a **push-pull** output. The output can either **source** current, i.e. sit out there atop the water tower at a potential of ~3.3 volts, or it can **sink** current, sitting there at the bottom of the water tower at a potential of ~0 volts. The amount of current in either of these cases is small; the point isn't to have a lot of power flowing over the data line. Rather, the voltage (i.e. the difference in potential) lets the microcontroller communicate a 1 or a 0 to whatever device is at the other end. 

This is all well and good for our microcontroller, but as it turns out, it's not how our sensor works. The tiny person manning our DHT22 only has one button, the one that connects the signal line to ground. More or less: it can only set the output line to GND, indicating a 0: 

![Water tower again. On the ground next to it, a short box labeled "Open Drain Output" has one pipe coming from it, down at the ground, leading to a box labeled "other gadget". A person in the foreground sits at a table with just one button: "LOW".](/assets/images/posts/2020-02-20-watertower-03.png)

This is called an **open drain** output. Basically, the signal line is either logic low, or it's not. The thing is, when it's not low, it's also not high. It's "floating", which means it's not connected to any voltage in particular. This is bad news — and it's also where we have to abandon our water tower analogy for a minute, because electricity isn't actually like water. The fact is, there are all kinds of electrical fields floating in the air around you, and you circuit can pick up on these like an antenna. If a pin is floating, there's no telling what voltage level it's at: above the logic high level, below the low — in between?! You just don't know. In reading this floating signal, you could easily get a 0, instead of the 1 that the sensor is trying to communicate by not driving the line low. 

To fix this, we need to connect the data line to 3.3 volts whenever it's not driven low. So we add a **pull-up resistor**. It's just a regular resistor, but we call it that because of the way we're using it. It connects the data line to the 3.3 volt rail, so that whenever the sensor isn't driving it low, it's automatically high. And because the water tower analogy is getting a bit tortured, I'm also going to draw it as a traditional schematic.

![One last water tower. "Open Drain Output" still sits on the ground with its pipe to "other gadget", but now a second pipe connects "other gadget" to the water tower. To the right of the scene, a schematic diagram shows two boxes, one labeled "OUT" and the other labeled "IN", connected to each other by a line. The line is also connected to 3V3 via a resistor.](/assets/images/posts/2020-02-20-watertower-04.png)

You might ask: why do we need a resistor? Why not just connect the data line directly to 3.3 volts? 

That would actually work to pull the line high, but it would fail the moment the sensor tried to drive the line low. When that happened, it would connect the 3.3 volt line directly to ground, causing a short circuit. Short circuits are very bad. Remember, we need a skinny pipe that lets a small amount of current flow. A short circuit is like a big hole in the water tower: it lets way too much current flow, which can damage the circuit. The pull-up resistor keeps the input high when the drain is closed, while limiting the amount of current that can flow when the drain is open. 

![The schematic from before, animating high and low states with a red line.](/assets/images/posts/2020-02-20-schematic-animated.gif)

Before moving on: we aren't going to use them in this circuit, but it's good to note that you can also use a resistor as a **pull-down** to 0 volts; this is useful when you have outputs that can only source current, but not sink it. We'll make use of these eventually. 

### The On/Off Switch

Next, let's talk about that on/off switch. Feathers have a really useful feature called an "Enable" pin. This pin isn't connected to the microcontroller; instead, it's connected to an input on the voltage regulator, which provides that 3V3 power rail we were talking about. As long as the Enable pin is at a logic high level, the power supply is going to supply those 3.3 volts. When it does go low, the regulator will will stop supplying those 3.3 volts, which will turn off our gadget. 

To keep the regulator on, the Feather connects this pin (via — again — a pull-up resistor!) to the highest voltage coming in to the board: either the USB plug, which is 5 volts, or the battery voltage, ~3.7 volts. We're going to add a switch that connects the Enable pin to GND, or logic low, instead. It's very similar to what was happening with the temperature sensor above: the Enable line is pulled high, until someone pulls it low; in this case, though, the tiny chip person with the button is a regular sized person (you!) with a literal switch that you can use to tell the regulator what to do. 

### The GPS Wing

The cool thing about the GPS wing — aside from the fact that it, y'know, computes your location on earth by listening to radio signals FROM SPACE — is that to a certain extent, it's an entire computer of its own, mounted on our microcontroller board like a backpack. 

A quick primer on GPS, because I honestly didn't understand this at first. GPS isn't like your cell phone service. There's no two-way communication with the GPS constellation; the satellites don't see you there, and they certainly don't "know" anything about where you are. All each satellite broadcasts is its own position in space, along with a very precise timestamp indicating when it sent that data. A GPS module (like the one on on the wing) listens for these signals, and when it has a few of them from a few satellites, it can use that time and distance data to triangulate your exact position on the globe. 

At that point, all it has to do is communicate it to us. The communication standard that the FeatherWing uses is just lines of ASCII text that it sends, one line at a time, through the TX pin on the Wing (aka the RX pin on the Feather). 

We can also talk back to it via the TX pin on the Feather (aka the RX pin on the Wing) when we want to give it commands. 

### The Enclosure / 3D Printing

OK, so here's the deal: this project is ideal in a 3D printed enclosure. If you don't want to invest in a 3D printer, you can probably find a project enclosure that will work, but this guide is going to assume that you want to 3D print the log, because it's awesome! If you don't want to print the log, you should still be able to follow along; just do whatever makes sense to fit the parts in the enclosure that you're using.

I printed with wood-fill PLA, which is plastic with wood fibers, so that it would feel like an actual log. The top half has a cutout for the GPS antenna; the bottom half has a cutout for the slide switch. Inside, the log has cutouts sized for the Feather, Wing, battery and sensor:  

Design files are available [on Tinkercad](https://www.tinkercad.com/things/fs752lxl3uC-hiking-log-enclosure) (TODO: Update Files!!!)

3D printing involves depositing material layer by layer, from bottom to top, to create a solid object. Two important things to note here: first, a 3D model, generally speaking, only represents the "skin" of an object. What's inside? Is it hollow, or solid? How solid? Solid like stone or solid like foam? Or is it filled with air pockets, like the honeycombs of a beehive? You'll have to decide what kind of **infill** to use. 

Second: your 3D printer is (most likely) operating in Earth gravity, while your 3D model is not. Where your 3D model might have overhangs (like Pride Rock in the Lion King), or bits that connect to the model on top but not the bottom (like the Anglerfish in Finding Nemo), your printer can mostly only print things that connect to the layer below. If you want to print something like this, you're going to face challenges unless you add some **supports** underneath.

We'll be able to address both of these things when we **slice** an object for printing. Personally, I use [Cura](https://ultimaker.com/software/ultimaker-cura), which is excellent free and open source slicing software. I printed these parts with **Gyroid** infill at 15% (the orange squiggly bits below); it's is a type of infill that's pretty strong against being crushed from any direction. In addition, I asked it for supports at angles over 80 degrees (the cyan zigzags), which more or less only creates supports for the big flat areas where the filament would otherwise start to sag. 

![A screenshot of the Cura software, showing the log as it would appear in mid-print. Zig-zagging cyan lines appear in the cavity where the battery would fit; squiggly orange lines are beginning to fill what will become the inside of the log.](/assets/images/posts/2020-02-20-cura.png)

I'm not going to dwell too much on the 3D printing part, partly because there's no "right" answer, and partly because you'll learn way more by just trying some stuff. Mostly though I'm moving on because we're 3,000 words in, and we haven't even started building. 

## Can we build it already?

Yes we can. 

To get the Feather mounted under the Wing, we’re going to install pin headers on the GPS, and short socket headers on the Feather. First, insert 12 and 16 pin headers against the right side of your breadboard, in rows b and h, long pins down:

![Build photo; matches what was just described.](/assets/images/posts/2020-02-20-build-010.jpg)

Next, insert the GPS FeatherWing atop those pins, and solder them into place:

![Build photo](/assets/images/posts/2020-02-20-build-020.jpg)

Remove the wing from the breadboard, and then grab your red, black and yellow wires. Solder them to 3V, GND and A1, respectively. You don’t have to use A1, or even an analog pin — the DHT22 communicates with a digital signal — but this is how I wired mine. Leave the wires pretty long, at least as long as the log enclosure; we’ll trim them to length in a moment.

![Build photo](/assets/images/posts/2020-02-20-build-030.jpg)

Now you can get your Feather M0 out, along with the slim socket headers. Use a little bit of tape to secure the headers to the side of the board:

![Build photo](/assets/images/posts/2020-02-20-build-040.jpg)

Then turn the board upside down, and solder the socket headers in. Make sure all the pins have good electrical contact, but don’t overdo it with the solder; if it gets sucked into the socket header, you’ll have trouble mounting it to the GPS board:

![Build photo](/assets/images/posts/2020-02-20-build-050.jpg)

*(Your board probably won’t look as torn up as mine; this one's been through a lot)*

Finally, for the moment, grab your switch, which will become our On/Off switch. Insert it in the UNDERSIDE of the Feather, into the three holes at the bottom right of the 5x5 grid:

![Build photo](/assets/images/posts/2020-02-20-build-060.jpg)

![Build photo](/assets/images/posts/2020-02-20-build-070.jpg)

Taping the switch in place, turn the board right side up, and solder ONLY the top pin, which should be in row three of the 5x5 grid:

![Build photo](/assets/images/posts/2020-02-20-build-080.jpg)

Next, take the bottom pin. Bend it 90 degrees toward the bottom row marked GND…

![Build photo](/assets/images/posts/2020-02-20-build-090.jpg)

…and solder it to both its own hole and the GND row: 

![Build photo](/assets/images/posts/2020-02-20-build-100.jpg)

Finally, bend the middle wire 90 degrees to the right:

![Build photo](/assets/images/posts/2020-02-20-build-110.jpg)

 Flip the board over, and grab your yellow wire. Cut a short length (about the distance from the EN pin to the hole with the middle pin of the switch), strip both ends, and kink one into a small hook shape: 

![Build photo](/assets/images/posts/2020-02-20-build-120.jpg)

You’re going to use this wire to connect the center pin of the switch to the EN pin, roughly like so: 

![Build photo](/assets/images/posts/2020-02-20-build-130.jpg)

Solder the hooked end of the wire to the EN pin: 

![Build photo](/assets/images/posts/2020-02-20-build-140.jpg)

Then flip the board over and, making sure that both the switch pin and the wire are meeting at the same hole…

![Build photo](/assets/images/posts/2020-02-20-build-150.jpg)

…solder them together, along with both holes: 

![Build photo](/assets/images/posts/2020-02-20-build-160.jpg)

At this point, you should be able to mount the Wing atop the Feather (running the battery cable in between), and already get a sense for how the project is going to fit together! 

![Build photo](/assets/images/posts/2020-02-20-build-170.jpg)

Don’t plug the battery in just yet, though. First, let’s set up the DHT22 sensor. Secure the wires in that small channel at the bottom; if you’re looking at it with the USB port facing you, they should be in black / yellow / red order, like the flag of Belgium: 

![Build photo](/assets/images/posts/2020-02-20-build-180.jpg)

Next, add some tape under the wires, and seat your sensor where it’s going to go. Note or mark where the sensor leads end; you’re going to strip your wires at this point to connect them to the sensor: 

![Build photo](/assets/images/posts/2020-02-20-build-190.jpg)

Cut and strip those wires! You may have to remove the tape, or even the whole assembly, from the log. That’s fine, you can re-tape them; we mostly put everything in the log to be sure of the dimensions. 

![Build photo](/assets/images/posts/2020-02-20-build-200.jpg)

Now, with your soldering iron, connect the red wire to the bottom pin of the sensor, the yellow wire to the one next to it, and the black wire to the top pin. Leave the remaining pin disconnected. If you want to, you can even bend it the opposite direction to keep it out of the way, or clip it off once you’ve tested the wiring. 

![Build photo](/assets/images/posts/2020-02-20-build-210.jpg)

Finally, the last step in setting up the sensor: we’re going to need to attach that pull-up resistor we were talking about earlier. We’ll make compact work of it: it needs to connect the A1 pin to the 3V pin, so bend it around those pins… 

![Build photo](/assets/images/posts/2020-02-20-build-220.jpg)

…solder it into place…

![Build photo](/assets/images/posts/2020-02-20-build-230.jpg)

…and trim away the excess.

![Build photo](/assets/images/posts/2020-02-20-build-240.jpg)

Now we’re in business! Plug in the battery, and seat everything in the bottom half of the log: 

![Build photo](/assets/images/posts/2020-02-20-build-250.jpg)

Then, close it up! You can keep it all together with a rubber band: 

![Build photo](/assets/images/posts/2020-02-20-build-260.jpg)

## Programming Your Log (Arduino Basics)

You absolutely do not need an illustration for this step, but I think it's fun. Plug the log you've built in to your computer: 

[image]

Open up the [Arduino IDE](https://www.arduino.cc/en/Main/Software). If you haven't done so already, follow [these instructions](https://learn.adafruit.com/add-boards-arduino-v164/setup) to install Adafruit's SAMD board support. You'll also need to install the following libraries: 

* DHT Sensor library
* Adafruit GPS Library
* Adafruit SPIFlash
* Adafruit TinyUSB Library

The DHT sensor library, perhaps unsurprisingly, talks to the DHT22 sensor. The GPS library does the same for the GPS wing. Those last two libraries, though. They interface with a couple of peripherals that you might not remember soldering. These two peripherals came with the board.

The first peripheral is a little chip on the Feather, next to the protoboard area. It's a two megabyte Flash chip; every Adafruit board with "Express" in the name comes with one. We're going to use it to store the data we collect on our hikes, and the SPIFlash library is going to be our interface to it. This guide isn't going to talk about it much more than that, but SPI is a major topic by the end (and one we'll start to explore at the end of the next guide).

You might not even think of the second peripheral as a device in its own right, but it's one of the most powerful: I'm talking about the USB port. Since the SAMD21 at the heart of this gadget has native USB functionality, we can easily use the USB port to do all kinds of USB things, from emulating a keyboard to showing up as a thumb drive. That's what TinyUSB is going to do for us: turn the log into a hard drive so we can get our data out. 

### The Arduino Sketch

We're going to start with a blank sketch; just click File -> New. 

First, let's bring in our libraries. At the top, add imports for the two libraries we need, (and one more for a function we're going to end up using): 

```
#include <Adafruit_SPIFlash.h>
#include <Adafruit_GPS.h>
#include <DHT.h>
#include <avr/dtostrf.h>
```

While you're there, you can also add a couple of globals for the GPS, filesystem and sensor: 

```
Adafruit_GPS GPS(&Serial1);
DHT dht(A1, DHT22);
Adafruit_FlashTransport_SPI flashTransport(SS1, &SPI1);
Adafruit_SPIFlash flash(&flashTransport);
FatFileSystem fatfs;
```

From here, we're going to add all of our setup code in the setup() function, and our looping code in loop() function. If you haven't done Arduino stuff before: the code in setup() runs once when the device turns on or resets, and the code in loop() runs repeatedly. 

### The Setup

The DHT sensor and the Flash chip don't require much in the way of setup; there are no special parameters to set or options to configure. We just need to tell them to get started: 

```
dht.begin();
flash.begin();
fatfs.begin(&flash);
```

The GPS, on the other hand, is a complex beast. Luckily, the Adafruit GPS library has some presets that make configuring it reasonably straightforward. 

```
GPS.begin(9600);
// Turn on RMC (recommended data) and GGA (fix data), including altitude
GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
// Set the update rate to 1 Hz
GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
// Turn off messages about antenna state
GPS.sendCommand(PGCMD_NOANTENNA);
```

Each of the macros in those sendCommand calls expand to a string of characters that gets sent out on the Feather's TX pin (to the GPS Wing's RX pin). If you want to see what those commands look like, you're in luck! If you're in the Western hemisphere, I recommend you add these two commands that aren't in the Adafruit library; they seem to improve accuracy. Add them, experiment with them, see if they make a difference for you:

```
// Enable satellite-based augmentation (SBAS)
GPS.sendCommand("$PMTK313,1*2E\r\n");
// Set differential GPS data source to WAAS
GPS.sendCommand("$PMTK301,2*2E\r\n");
```

At this point, the Flash filesystem, the temperature sensor and the GPS are all set up!

### The Loop

Now let's turn our attention to the loop function. First, we're going to need to update the GPS. Data comes in on the Feather's RX line (from the Wing's TX line), and the only way the GPS module will be able to process it is if we tell it to read. 


```
/* GPS PARSING */
GPS.read();
```

This reads whatever characters came in on the RX line. Remember: the GPS is just sending us sentences of data, one line at a time. Which means that if that input included a newline, we might have a new sentence: 

```
if (GPS.newNMEAreceived()) {
  // try to parse the sentence, but if we can't, no worries, just try again on the next loop.
  if (!GPS.parse(GPS.lastNMEA())) return;
}
```

This GPS.parse method does A LOT. The sentence that came in could have included data about our location and heading, the time of day, the number of satellites in view, all kinds of good stuff. The parse method takes all of that data, and updates the instance variables in the GPS class so that we can just query `GPS.satellites` and get the latest value for how many satellites are in view. 

Next, we're going to log the data! We can't log all the time, though. To keep it simple, we're going to log a data point every minute. The GPS module gives us a super-accurate, satellite-synchronized real-time clock, so all we have to do is: 

1. Set a variable `shouldLog` to true.
2. Log a data point when the seconds on the clock read :00
3. Set `shouldLog` to false, so we don't keep logging all second long, then
4. When the clock reads :01, set `shouldLog` to true again.

Ergo: 

```
static bool shouldLog = true;
int seconds = GPS.seconds;
// Log data every minute, provided we have a valid timestamp
if (shouldLog && seconds == 0 && GPS.year != 0) {
  String datapoint;
  // TODO: add data to datapoint
  // TODO: write datapoint to file
  shouldLog = false;
}
else if (seconds == 1) {
  shouldLog = true;
}
```

The data we log is going to be in CSV format, short for **comma separated values**. It's basically a spreadsheet, where each value is in a column, with the comma as the divider.

First, let's create the timestamp with a sprintf call, and add it to the datapoint: 

```
char timestamp[20];
sprintf(timestamp, "20%02d-%02d-%02dT%d:%02d:%02d", GPS.year, GPS.month, GPS.day, GPS.hour, GPS.minute, seconds);
datapoint += timestamp;
datapoint += ',';
```

This will be our first column. Next let's add data points for our GPS fix:

```
datapoint += (int)GPS.fix;
datapoint += ',';
```

Pretty simple: 1 if we have a fix, 0 if we don't. Now we can add data points for our location. We don't want to log this if our location is invalid, though, so let's wrap it in a conditional: 

```
if (GPS.fix) {
  char outstr[15];
  dtostrf(GPS.latitudeDegrees, 7, 7, outstr);
  datapoint += outstr;
  datapoint += ',';
  dtostrf(GPS.longitudeDegrees, 7, 7, outstr);
  datapoint += outstr;
  datapoint += ',';
  datapoint += GPS.HDOP;
  datapoint += ',';
  datapoint += GPS.speed * 1.852001;
  datapoint += ',';
  datapoint += GPS.angle;
  datapoint += ',';
  datapoint += GPS.altitude;
  datapoint += ',';
}
else
{
  datapoint += ",,,,,,";
}
```

The dtostrf call is like the sprintf above, but for formatting double precision numbers like our GPS coordinates. After that, we log some more data that the GPS has for us: HDOP (horizontal dilution of position) tells us how accurate our location is. Speed, the GPS gives to us in knots, like for ships. We multiply it by 1.852001 to get this number in kilometers per hour. Angle is your heading in degrees; the GPS doesn't have a compass, but if you're moving, the GPS can tell you what direction you're going (true north is 0). And then altitude is meters above sea level.

If we don't have a fix, we just output six commas, to keep the columns aligned. This will show up as empty cells in our spreadsheet.

One last GPS thing that I find fun: the GPS module also reports the number of satellites it's using for the calculation. I find this data point so abundantly fun, so let's add it: 

```
datapoint += (int)GPS.satellites;
datapoint += ',';
```

Now it's time to log our sensor data. It's actually pretty simple, thanks to the awesome library that's talking to the sensor for us: 

```
float t = dht.readTemperature();
float h = dht.readHumidity();

if (isnan(t) || isnan(h)) {
  datapoint += ",,";
}
else {
  datapoint += t;
  datapoint += ',';
  datapoint += h;
  datapoint += ',';
}
```

Basically we log the temperature (in celsius) and relative humidity (in percent) if the values are valid, or blank values if not.

Finally, we didn't talk much about analog inputs (we will revisit this in lesson 5), but the Feather M0 Express includes a battery voltage monitor on pin 9. It returns a 10-bit integer (0-1023) that represents half of the battery voltage, which is useful to show us how much of a charge the log is working with. Let's log that too: 

```
float measuredvbat = analogRead(9);
measuredvbat /= 1023;
measuredvbat *= 3.3;
measuredvbat *= 2;
datapoint += (measuredvbat);
```

First we convert the number from an integer from 0-1023 to a float from 0-1. Then we multiply by 3.3 volts, the reference voltage, since 1.0 represents the maximum of 3.3 volts. Finally, we multiply by 2, since pin 9 represents half of the battery voltage. 

If you're wondering how this battery monitor works, don't worry, we're going to build one ourselves soon.

With that, we can log the data to the Flash chip! The TODO line for this should be just below the battery measurement; replace it with this: 

```
File dataFile = fatfs.open("log.csv", FILE_WRITE);
if (dataFile)
{
  dataFile.println(datapoint);
  dataFile.close();
}
```

You could call it here, but all you'll get when you open this in your spreadsheet program is a table of numbers. A header describing the columns would be nice, so add this before the println(datapoint) call: 

```
if (dataFile.size() == 0) {
  const char header[] = "Timestamp,GPS Fix,Latitude,Longitude,HDOP,Speed,Angle,Altitude,Satellites In View,Temperature (C),Relative Humidity (%),Battery Voltage";
  dataFile.println(header);
}
```

Finally, the Feather comes with an LED, so you might as well make use of the blinkenlight to tell the user that something happened. Add this right after dataFile.close():

```
digitalWrite(LED_BUILTIN, HIGH);
delay(10);
digitalWrite(LED_BUILTIN, LOW);
```

At this point, you're done! Make sure that the "Feather M0 Express" board is selected in the Tools -> Board menu, and that your board is selected in the Tools -> Port menu. Run the sketch, and let the log sit near a window for a bit to make sure it gets a fix. Once it does, every minute, on the minute, you should see the little red LED blip on for just a fraction of a second. That's your sketch, logging data to the log.

### Getting files off the Log

To get files off, we're going to need a different sketch. Go to File -> Examples -> Adafruit TinyUSB -> Mass Storage -> msc_external_flash. Make sure that the "Feather M0 Express" board is selected in the Tools -> Board menu, and that your board is selected in the Tools -> Port menu. Then, run the sketch! You should see a drive called "CIRCUITPY" appear connected to your computer. 

This is the contents of the two-megabyte Flash chip on the Feather. You should be able to see log.csv there, and copy it to your hard drive. Note that the log won't log things as long as this sketch is running. You'll have to run your previous sketch to get that functionality back.

## Extra Credit: Make It Your Own

I first built the Hiking Log what feels like ages upon ages ago. Over the years I've wanted to add various features, from an on-off switch (which made it into this guide!) to a screen (which did not). Here's the thing, though: we are using a grand total of five pins on this board, which still leaves a ton of room for expansion. There's five more analog inputs available to you, almost the entire top row of digital IO, and while we haven't talked about SPI or I2C, those are both buses that you could use to add more sensors or drive a display! Which is what I was doing this week: 

[image]

In this case, I wired the display to the SPI bus (which we'll start to talk about in "extra credit" next week), plus some of the IO pins up top (taking care to leave pin 9, the battery monitor, free). I also spent some time painting the log with acrylic paint (burnt umber) to give the exterior a more log-like appearance (taking care to mask the ends with painter's tape, to make sure the wood inside shone through). 

[image]

Maybe you want to print the log in high-visibility orange, or add a UV light sensor; perhaps you want to add a clock display and a speaker, to wake you up for the sunrise. That last idea literally just occurred to me as I was writing this closing; with the location data on the log, you can calculate the sunrise completely offline. And there's a true analog output on pin A0 that could totally drive a speaker.

ANYWAY. The point of this, as we close out the first in this eight part series, is that you don't have to follow the instructions, or accept the widget that I or anyone else puts before you. Take this design, build on it; come up with your own ideas, build on them. 

Next week we're going to light up some more blinkenlights, and get even closer to bare metal by driving clock and data lines ourselves, just flashing wires high and low. We'll also learn about Ohm's Law and current limiting, the aforementioned I2C protocol, and we'll make heavy use of one really useful gadget in particular: the shift register.