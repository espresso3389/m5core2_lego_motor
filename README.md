# M5Stack Core2 with DC Motor Module

An attempt to use [GBMQ-GM12BY20](https://www.amazon.com/GBMQ-GM12BY20-Micro-Reduction-Magnetic-Encoder/dp/B07J5MRHKH) on [M5Stack Core2](https://m5stack.com/collections/m5-core/products/m5stack-core2-esp32-iot-development-kit) with [DC Motor Module](https://m5stack.com/products/lego-module).

![](extras/overview.jpg)

## GBMQ-GM12BY20 connector PIN re-rearrangement

The connector pin should be rearranged to match to the following picture:

![](extras/pin-code-colors.jpg)

## Changes to the original code

From the original [LEGO_PLUS.ino](https://github.com/m5stack/M5Stack/blob/master/examples/Modules/LEGO_PLUS/LEGO_PLUS.ino), I modified the following things:

- Replace `M5Stack.h` with `M5Core2.h`
- Copy `writeBytes`/`readBytes` methods from `utility/CommUtil.h` and replace every `Wire` with `Wire1`.
    - Core2 uses `Wire` on GPIO32,33 combination whereas Core uses `Wire` on GPIO21,22
    - On Core2, `Touch.begin` initializes `Wire1` on GPIO21,22
- Remove `M5.Power.begin()` from `setup` function

# Appendix

## GBMQ-GM12BY20 original connector PIN layout

The PIN assignment for [GBMQ-GM12BY20 DC 6V 2W Micro Speed Reduction Motor Mini Gear Box Motor with Magnetic Coded Disc Hall Encoder (30RPM)](https://www.amazon.com/GBMQ-GM12BY20-Micro-Reduction-Magnetic-Encoder/dp/B07J5MRHKH), according to the documentation on Amazon page:

Code  | Description
------|---------------
Black |Motor -
Red   |Motor +
Brown |Encoder +
Green |Encoder -
Blue  |Encoder A
White |Encoder B

And according to several sources, M5Stack Core2's PIN arrangement is:

PIN | Cable | RJ12 | Description
----|----|---|-------------
M+  | Black | 2 | motor +
G   | Green | 4 | encoder Vcc (3.3V - 5V)
A   | Yellow | 5 | encoder A
B   | Blue | 6 | encoder B
V   | Red | 3 | encoder GND
1   | White | 1 | motor -

### References

- [Re: Cheaper alternative to brickpi?](https://www.raspberrypi.org/forums/viewtopic.php?t=239548#p1463935)
- [Using LEGO MINDSTORMS Motors with BeagleBone Blue](https://lechnology.com/2017/03/using-lego-mindstorms-motors-with-beaglebone-blue/)

