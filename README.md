This is just a clone of the Google Code project I did years ago that adds some much-needed features to a Spektrum DX6i RC transmitter.
Old version: https://code.google.com/archive/p/dx6i-enhancements/

Blog post for this project (includes images): http://hackcorrelation.blogspot.de/2014/03/spektrum-dx6i-backlight-and-timer-hack.html

I might port this to the lowest common denominator Arduino variant but for now this project exists only for historical reasons.


**Project goals**:
* add automatic timer start and pause to the DX6i remote
* minimal parts count: preferably only the PIC10F222 and nothing else
* support both zero throttle at bottom and zero throttle at middle (collective pitch)
* provide an audio warning when the remote is being left unused and turned on
* provide active backlight control - with timeout

**Update 5th Sep 2012**: source code published, looking for professional review

Nice to have features that would probably require a different chip:
* proportional timer control depending on throttle position. This would beep annoyingly if done with the current setup
* fix lame Spektrum bug with "throttle hold" not working for airplane setup. It is a bug since DX8 does it correctly
* calibrate throttle range once and save it in EEPROM

**Update 6th May 2012**: second firmware version is available, meeting all the required goals. Development is stopped unless bugs are found.

Video with the first version: https://vimeo.com/39839925

Video with the second version: https://vimeo.com/41668214

Inactivity timer sound: https://vimeo.com/41668215

Check the wiki for descriptions. Binary downloads are available as well as hardware instructions.
