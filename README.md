# dodgyPot
Control windows audio with a potentiometer

There are many ways to do this and this is possibly the worst.
I am not a programmer, I am an electrician so with that out of the way, run this code at your own risk.
There are bugs which I probably won't fix because the department of good enough deemed it good enough.

Will only work on Arduino leonadro (or similar) or the Atmega32u4 (or similar) as they have HID support.
Will NOT work on a nano, uno, mega etc etc

This is designed to work with a prebuilt audio panel with less than ideal wiring but I didn't want
to ruin that so here we are. If you were building one for yourself, I would recommend using an analog
pin for the potentiometer wiper and using a digital pin for the mute switch reducing pin requirements by 1
