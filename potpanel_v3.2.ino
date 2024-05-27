/*
  dodgyPot V3.2

  == Changelog ==
  + Added a music switch state machine to toggle music with the left switch.
  + Global enable variable just works... somehow...
  - Removed some gross.
  + Added some gross.
  * Discovered bug in testing where the volume knob will lose sync with the state machine.

  == Mandatory disclamer ==
  This code is provided as is and I won't take responsibility for your hardware.
*/

#include <HID-Project.h>

int currentVolumeState;
int currentMediaState;
int nextVolumeState;
int nextMediaState;
int homing;
int currentPotValue;
int prevPotValue;
int currentMapValue;
int currentVolume;
bool enable;


void setup() {
  // Sets the pinmodes
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(3, OUTPUT);
}

void loop() {
  //========================================= Volume control =================================================
  // This state machine runs all of the volume and muting functions on the right side knobs
  switch (currentVolumeState) {
    // Mute check
    case 1:
      digitalWrite(4, LOW);
      digitalWrite(5, HIGH);
      // If the mute switch is in the muted position, set muted to true, send mute toggle and move to state 2
      if (digitalRead(A1) == HIGH) {
        Consumer.write(MEDIA_VOLUME_MUTE);
        enable = true;
        nextVolumeState = 2;
        // If mute switch is not in the muted position, leave muted as false and move to state 3
      } else if (digitalRead(A1) == LOW) {
        nextVolumeState = 3;
      }
      break;
    case 2:
      digitalWrite(4, LOW);
      digitalWrite(5, HIGH);
      // If the mute switch is still in the muted position, do nothing and loop back to state 2
      if (digitalRead(A1) == HIGH) {
        nextVolumeState = 2;
      }
      // If the mute switch is no longer in the muted position, set muted to false, send mute toggle and move to state 3
      if (digitalRead(A1) == LOW) {
        Consumer.write(MEDIA_VOLUME_MUTE);
        enable = false;
        nextVolumeState = 3;
      }
      break;
    case 3:
      digitalWrite(4, LOW);
      digitalWrite(5, HIGH);
      // If the mute switch is moved to the muted position, set muted to true and move to state 2
      if (digitalRead(A1) == HIGH) {
        Consumer.write(MEDIA_VOLUME_MUTE);
        enable = true;
        nextVolumeState = 2;
      } else {
        // If the mute switch is not in the muted position, perform an analog read of A1
        digitalWrite(5, LOW);
        digitalWrite(4, HIGH);
        delay(5);
        currentPotValue = analogRead(A1);
      }
      // If the difference between the read that was just performed and the previous stored potentiometer value differ by 10, move to state 4
      if (abs(currentPotValue - prevPotValue) > 10) {
        prevPotValue = currentPotValue;
        // Map the raw ADC count to a value between 0 and 52
        currentMapValue = map(currentPotValue, 0, 1023, 0, 52 );
        nextVolumeState = 4;
      } else {
        // If the value didn't change beyond background noise, return to state 1
        nextVolumeState = 1;
      }
      break;
    case 4:
      // If the mapped ADC value is not equal to the current volume, send volume commands and increment the volume count to match
      if (currentVolume != currentMapValue)
        // Increase and send volume up commands if volume count is below mapped value
        if (currentVolume < currentMapValue) {
          Consumer.write(MEDIA_VOLUME_UP);
          currentVolume++;
          nextVolumeState = 4;
          delay(5);
        } else {
          // Decrease and send volume down commands if volume count is above mapped value
          Consumer.write(MEDIA_VOLUME_DOWN);
          currentVolume--;
          nextVolumeState = 4;
          delay(5);
        }
      // If both volume counters agree, return to state 1
      if (currentVolume == currentMapValue) {
        nextVolumeState = 1;
      }
      digitalWrite(4, LOW);
      digitalWrite(5, HIGH);
      // If the mute switch is moved to the muted position, set muted to true and move to state 2
      if (digitalRead(A1) == HIGH) {
        Consumer.write(MEDIA_VOLUME_MUTE);
        enable = true;
        nextVolumeState = 2;
      }
      digitalWrite(4, HIGH);
      digitalWrite(5, LOW);
      break;
    default:
      // Sets critical values as a part of the default state. (Thanks Richard, sound advice)
      nextVolumeState = 1;
      enable = false;
      prevPotValue = 0;
      currentVolume = 0;
      homing = 0;
      // Homes the windows volume slider to a known position (0) by "running it into an end stop"
      while (homing < 120) {
        Consumer.write(MEDIA_VOLUME_DOWN);
        homing++;
        delay(5);
      }
      break;
  }
  currentVolumeState = nextVolumeState;
  //========================================= Media control ==================================================
  // This state machine controls the music on the left loudspeaker switch.
  switch (currentMediaState) {
    // Media play pause check
    case 1:
      digitalWrite(2, LOW);
      digitalWrite(3, HIGH);
      // If the mute switch is in the muted position, set muted to true, send mute toggle and move to state 2
      if (digitalRead(A0) == HIGH) {
        Consumer.write(0xCD);
        enable = true;
        nextMediaState = 2;
        // If mute switch is not in the muted position, leave muted as false and move to state 3
      } else if (digitalRead(A0) == LOW) {
        nextMediaState = 3;
      }
      break;
    case 2:
      digitalWrite(2, LOW);
      digitalWrite(3, HIGH);
      // If the mute switch is still in the muted position, do nothing and loop back to state 2
      if (digitalRead(A0) == HIGH) {
        nextMediaState = 2;
      }
      // If the mute switch is no longer in the muted position, set muted to false, send mute toggle and move to state 3
      if (digitalRead(A0) == LOW) {
        Consumer.write(0xCD);
        enable = false;
        nextMediaState = 1;
      }
      break;
    default:
      nextMediaState = 1;
      break;
  }
  currentMediaState = nextMediaState;
}
