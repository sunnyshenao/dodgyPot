/*
  dodgyPot V3.0

  == Changelog ==
  + Version increment due to reimplementation in form of a finite state machine.
  + Changed variable names for readability.
  + Squashed the bug where volume range was from 0 to 98%. Works good now.
  - Ate some spaghetti
  + Made some more spaghetti

  == Mandatory disclamer ==
  This code is provided as is and I won't take responsibility for your hardware.
*/

#include <HID-Project.h>

int currentState;
int nextState;
int homing;
int currentPotValue;
int prevPotValue;
int currentMapValue;
int currentVolume;
bool muted;


void setup() {
  // Sets the pinmodes
  pinMode(A0, INPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
}

void loop() {
  switch (currentState) {
    // Mute check
    case 1:
      digitalWrite(2, LOW);
      digitalWrite(3, HIGH);
      // If the mute switch is in the muted position, set muted to true, send mute toggle and move to state 2
      if (digitalRead(A0) == HIGH) {
        Consumer.write(MEDIA_VOLUME_MUTE);
        muted = true;
        nextState = 2;
        // If mute switch is not in the muted position, leave muted as false and move to state 3
      } else if (digitalRead(A0) == LOW) {
        nextState = 3;
      }
      break;
    case 2:
      digitalWrite(2, LOW);
      digitalWrite(3, HIGH);
      // If the mute switch is still in the muted position, do nothing and loop back to state 2
      if (digitalRead(A0) == HIGH) {
        nextState = 2;
      }
      // If the mute switch is no longer in the muted position, set muted to false, send mute toggle and move to state 3
      if (digitalRead(A0) == LOW) {
        Consumer.write(MEDIA_VOLUME_MUTE);
        muted = false;
        nextState = 3;
      }
      break;
    case 3:
      digitalWrite(2, LOW);
      digitalWrite(3, HIGH);
      // If the mute switch is moved to the muted position, set muted to true and move to state 2
      if (digitalRead(A0) == HIGH) {
        Consumer.write(MEDIA_VOLUME_MUTE);
        muted = true;
        nextState = 2;
      } else {
        // If the mute switch is not in the muted position, perform an analog read of A0
        digitalWrite(3, LOW);
        digitalWrite(2, HIGH);
        delay(5);
        currentPotValue = analogRead(A0);
      }
      // If the difference between the read that was just performed and the previous stored potentiometer value differ by 10, move to state 4
      if (abs(currentPotValue - prevPotValue) > 10) {
        prevPotValue = currentPotValue;
        // Map the raw ADC count to a value between 0 and 52
        currentMapValue = map(currentPotValue, 0, 1023, 0, 52 );
        nextState = 4;
      } else {
        // If the value didn't change beyond background noise, return to state 1
        nextState = 1;
      }
      break;
    case 4:
      // If the mapped ADC value is not equal to the current volume, send volume commands and increment the volume count to match
      if (currentVolume != currentMapValue)
        // Increase and send volume up commands if volume count is below mapped value
        if (currentVolume < currentMapValue) {
          Consumer.write(MEDIA_VOLUME_UP);
          currentVolume++;
          nextState = 4;
          delay(5);
        } else {
          // Decrease and send volume down commands if volume count is above mapped value
          Consumer.write(MEDIA_VOLUME_DOWN);
          currentVolume--;
          nextState = 4;
          delay(5);
        }
      // If both volume counters agree, return to state 1
      if (currentVolume == currentMapValue) {
        nextState = 1;
      }
      digitalWrite(2, LOW);
      digitalWrite(3, HIGH);
      // If the mute switch is moved to the muted position, set muted to true and move to state 2
      if (digitalRead(A0) == HIGH) {
        Consumer.write(MEDIA_VOLUME_MUTE);
        muted = true;
        nextState = 2;
      }
      digitalWrite(2, HIGH);
      digitalWrite(3, LOW);
      break;
    default:
      // Sets critical values as a part of the default state. (Thanks Richard, sound advice)
      nextState = 1;
      muted = false;
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
  currentState = nextState;
}
