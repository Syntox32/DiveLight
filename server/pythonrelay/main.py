import asyncio, os, websockets, time
import threading

try:
    LED_ENABLED = os.uname()[4][:3] == 'arm' # Only use LEDs if we're on the raspberry pi
except:
    LED_ENABLED = False

# LED strip configuration:
LED_COUNT      = 300      # Number of LED pixels.
LED_PIN        = 18      # GPIO pin connected to the pixels (must support PWM!).
LED_FREQ_HZ    = 800000  # LED signal frequency in hertz (usually 800khz)
LED_DMA        = 5       # DMA channel to use for generating signal (try 5)
LED_BRIGHTNESS = 255     # Set to 0 for darkest and 255 for brightest
LED_INVERT     = False   # True to invert the signal (when using NPN transistor level shift)

#lock = threading.Lock()
LED_DATA = []

print("initalizing led-strip...")
if LED_ENABLED:
    print("LED's enabled.")
    from neopixel import *
    print("initalizing led-strip...")
    # Create NeoPixel object with appropriate configuration.
    strip = Adafruit_NeoPixel(LED_COUNT, LED_PIN, LED_FREQ_HZ, LED_DMA, LED_INVERT, LED_BRIGHTNESS)
    # Intialize the library (must be called once before other functions).
    strip.begin()
else:
    print("LED's DISABLED.")

def ledLoop():
    stop = True
    while True:
        #with lock:
        for i in range(len(LED_DATA)):
            c = LED_DATA[i]
            strip.setPixelColorRGB(i, int(c[0]), int(c[1]), int(c[2]))
        strip.show()
        time.sleep(1000.0/1000.0/60)

async def hello(websocket, path):
    print("path: " + path)
    stop = False
    global LED_DATA;
    while not stop:
        name = await websocket.recv()
        #print("{0} - {1}".format(count, name))
        if "quit" in name:
            stop = True
        #with lock:
        LED_DATA = []
        name = name.split(":")
        if name[0] in "begin":
            count = int(name[1])
            #print(count)
            for i in range(count):
                c = name[2 + i].split(",")
                #with lock:
                LED_DATA.append(c)
                #print(len(LED_DATA))

if __name__ == "__main__":
    t = threading.Thread(target=ledLoop)
    t.daemon = True
    t.start()

    print("starting server...")
    start_server = websockets.serve(hello, '172.16.42.9', 6969)

    asyncio.get_event_loop().run_until_complete(start_server)
    asyncio.get_event_loop().run_forever()