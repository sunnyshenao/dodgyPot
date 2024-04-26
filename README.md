# dodgyPot
v3.0

Control Windows audio with a potentiometer

Rewrote the original Italian delicacy into a relatively nicely structured finite state machine. This is not the best way to control Windows audio.

<h1>An explanation of why this is a terrible idea</h1>
Now, some very smart people a long time ago decided to add volume control as part of the functionality of a keyboard. Since it would be quite annoying to have 100 keyboard buttons dedicated to every volume percentage possible, the boffins decided to have the keyboard send volume up and down commands RELATIVE to the current volume. The associated scancode for any keyboard button can be found <a-href="https://gist.github.com/MightyPork/6da26e382a7ad91b5496ee55fdc73db2">here</a>. With that knowledge, it becomes immediately obvious why having a potentiometer, which relies on absolute positioning to control the Windows volume slider, which relies on relative positioning is a terrible idea. Usually in this sort of application a rotary encoder would be the perfect candidate.

<h1>Why?</h1>
You tell me.

<h1>Theory of operation</h1>
When the program starts for the first time (Power restored, plugged in etc), the currentVolume variable is not indexed with Windows volume. The only way we can index this with relative commands is to "run it into an end stop". This is achieved by sending 120 volume down commands. No matter where the slider is it is at zero even if Windows misses a few of them (unlikely on a PC from the past decade).

After the program has "homed" the audio slider, normal state machine operation takes over. The only thing of note is that state 4 is not a real state, yet the amalgamation of 3 states into 1 state which can be done in high level languages. State 4 handles both the upwards and downwards incrementing of the volume slider whilst checking if it coincides with the count in memory and the analog read value.

<h1>I'm gonna build one</h1>
Don't. Don't build this one in particular, build one of the hundreds of rotary encoder variants of this project. This is designed to help in an extremely niche situation where you somehow have the exact same audio control panel off some battleship with a 15 position sub-D connector on it AND you would like to hook it up to your PC.

</hr>
v2.0

Control Windows audio with a potentiometer

There are many ways to do this and this is possibly the worst.
I am not a programmer, I am an electrician so with that out of the way, run this code at your own risk.
There are bugs which I probably won't fix because the department of good enough deemed it good enough.

Will only work on Arduino Leonado (or similar) or the Atmega32u4 (or similar) as they have HID support.
Will NOT work on a nano, uno, mega etc etc

This is designed to work with a prebuilt audio panel with less than ideal wiring but I didn't want
to ruin that so here we are. If you were building one for yourself, I would recommend using an analog
pin for the potentiometer wiper and using a digital pin for the mute switch reducing pin requirements by 1
