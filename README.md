# dodgyPot
== Changelog v3.2==

Changelogs are now located in the header of the code :)

Control Windows audio with a potentiometer

THIS WILL ONLY WORK WITH AN ARDUINO LEONADRO , PRO MICRO OR SIMILAR AS THEY HAVE HID SUPPORT.

Rewrote the original Italian delicacy into a relatively nicely structured finite state machine. This is not the best way to control Windows audio.

<h2>An explanation of why this is a terrible idea</h2>
Now, some very smart people a long time ago decided to add volume control as part of the functionality of a keyboard. Since it would be quite annoying to have 100 keyboard buttons dedicated to every volume percentage possible, the boffins decided to have the keyboard send volume up and down commands RELATIVE to the current volume. The associated scancode for any keyboard button can be found [here](https://gist.github.com/MightyPork/6da26e382a7ad91b5496ee55fdc73db2). With that knowledge, it becomes immediately obvious why having a potentiometer, which relies on absolute positioning to control the Windows volume slider, which relies on relative positioning is a terrible idea. Usually in this sort of application a rotary encoder would be the perfect candidate.

<h2>Why did you do this?</h2>
You tell me.

<h2>Theory of operation</h2>
When the program starts for the first time (Power restored, plugged in etc), the currentVolume variable is not indexed with Windows volume. The only way we can index this with relative commands is to "run it into an end stop". This is achieved by sending 120 volume down commands. No matter where the slider was, it is at zero now even if Windows misses a few of them (unlikely on a PC from the past decade).

After the program has "homed" the audio slider, normal state machine operation takes over. The only thing of note is that state 4 is not a real state, but it is the amalgamation of 3 states into 1 state which can be done in high level languages. State 4 handles both the upwards and downwards incrementing of the volume slider whilst checking if it coincides with the count in memory and the analog read value.

Due to the nature of how this panel was wired, we can not check the volume and the mute status at the same time. The digital pins are cycled in a manner which allows the analog measurement of the potentiometer OR the digital measurement of the mute switch but not both at the same time.

<h2>I'm gonna build one</h2>
Don't. Don't build this one in particular, build one of the hundreds of rotary encoder variants of this project. This is designed to help in an extremely niche situation where you somehow have the exact same audio control panel off some battleship with a 15 position sub-D connector on it AND you would like to hook it up to your PC.
