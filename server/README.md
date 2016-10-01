# Relay Server

This is the python relay server used to connect the client to the Raspberry Pi, and the 
Pi to the LED-strip.

### Operation

The python server can continuously run and does not exit when it looses connection to the current
 websocket. In theory this script is made such that it requires no interaction after first setup.
 Thus it could be profitable to setup this script with supervisor, cron or some other
 process manager to make this run on startup. 
 Note: Because of the WS281x library, the script is unfortunately required to run as `sudo`.

### Python & Dependencies

You need to be running a python version >= 3.5.x, due to the use of `await` and async websockets.

If you need help installing Python 3.5 on the Raspberry Pi (since at the time of writing this
, I could only find the `3.4` version in the apt-get repo's) you can go to this [stackoverflow answer](http://stackoverflow.com/a/37079369).
Remember to install libssl before compiling python, or else you won't get `pip`.

You need the dependency `websockets` installed. 

You also need to compile and install [rpi_ws281x](https://github.com/jgarff/rpi_ws281x)
for the LED strip to work. If you use another type of controller, it shouldn't be too hard to
change the code to the new library.

### The LED Strip

If you need help wiring the the LED-strip, you could search google for the type of controller you're
using, you usually find a tutorial, document, article or images. This is something that might help you
get started using the [WS2812 strip](https://learn.sparkfun.com/tutorials/ws2812-breakout-hookup-guide).

### Future Work

A nice addition in the future would be to use the Arduino microcontroller as a relay and talk to it directly from
the client machine. This eliminates the use of networking, and it's arguably easier to setup.