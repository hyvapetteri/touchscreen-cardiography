# touchscreen-cardiography
![DOI https://doi.org/10.5281/zenodo.827194](https://zenodo.org/badge/95604106.svg)

Measuring the heartbeat using a capacitive touch screen. FT5206-controlled touchscreen + arduino UNO

## touchscreen = capacitive sensor array
A normal smartphone touchscreen has a grid of capacitance sensors (in the order of 100-200 sensors).
A touchscreen controller is the chip that reads the sensor
values and forwards the information to the phone's processor. Most touchscreen controllers only
report higher-level attributes, such as the number of touches, touch coordinates, and touch gestures. So in most cases
the raw capacitance values cannot be accessed programmatically, which is a shame, since all kinds of cool stuff can
be done with a large sensor array.

One such example is capacitive imaging, which has been demonstrated with the same touchsreen controller line
by [optisimon](https://github.com/optisimon)

http://optisimon.com/raspberrypi/touch/ft5406/2016/07/13/raspberry-pi-7-inch-touchscreen-hacking/

The FT5x06 line has a "Factory Mode", which allows reading of raw capacitance values. The datasheet 
(http://caxapa.ru/thumbs/457511/FTS_AN_CTPM_Standard_eng_ver1.1.pdf) says that a full scan of the touchscreen sensors
takes about 10 ms, which
would mean that sampling could be done at 100 Hz at best. In practice I was able to get to about 50 Hz using only
part of the sensors, since reading out the values via I2C also takes quite some time.

## Implementation
Parts used:
- touchscreen (with display for future projects, also possible to get individually)
    - http://www.ebay.com/itm/4-3-inch-TFT-LCD-Module-Display-I2C-Serial-w-Capacitive-Touch-Panel-Tutorial-/291854295216
- 0.5 mm FPC connector adapter
    - https://www.adafruit.com/product/1492
- Adruino UNO R3

The touchscreen was powered from arduino's 3.3V pin. The SCL and SDA pins were connected with 4.7 kOhm pull-up resistors to 3.3V,
and the internal pull-up resistors were disabled programmatically (I hope, see arduino code).
