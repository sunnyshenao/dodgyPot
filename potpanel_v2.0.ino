#include <HID-Project.h>

int prevPotValue = 0;
int potValue;
int mapValue;
int prevMapValue;
int currentVolume = 0;
bool muteLock = false;

void setup() {
  int i = 0;
  pinMode(A0, INPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  Consumer.begin();
  //A0 is the combined mute volume sense pin, pin 2 is potentiometer drive and pin 3 is mute drive
  delay(1000);
  while (i < 120) {
    Consumer.write(MEDIA_VOLUME_DOWN);
    i++;
    delay(5);
    //"Homes" the volume control slider in windows to a known position (zero)
  }
}

void loop() {
  digitalWrite(2, LOW);
  digitalWrite(3, HIGH);
  if (digitalRead(A0) == HIGH) {
    if(muteLock == false){
      Consumer.write(MEDIA_VOLUME_MUTE);
      muteLock = true;
      //Pulls D2 low and asserts D3 high to check if the mute position is selected
      //if it is, send mute command and set mute lock to true to prevent spamming
    }
  } else {
    muteLock = false;
    digitalWrite(3, LOW);
    digitalWrite(2, HIGH);
    //Pulls D3 low and asserts D2 high to drive the potentiometer and obtain an analog
    //reading on A0 of the volume level only if the mute switch is not in the mute position
    //150K pull down resistor will work to as low as 10K (tested) although intuition screams
    //that anything below 1M should affect volume adjustment (I don't have 1M on hand)
    potValue = analogRead(A0);
    if (abs(potValue - prevPotValue) > 10) {
      prevPotValue = potValue;  
      mapValue = map(potValue, 0, 1023, 0, 50 ) + 1;
      if (mapValue > 50) {
        mapValue = 50;
        //Maps the value read from potentiometer A0 to 50 and adds one for full scale
        //will only update if the A0 raw value has changed by at least 10
      }
    }
    if (currentVolume < mapValue) {
      Consumer.write(MEDIA_VOLUME_UP);
      currentVolume++;
      delay(5);
      //If the current volume is above the mapped volumes value, send one volume up command,
      //increment the current volume and delay for stability
    }
    if (currentVolume > mapValue) {
      Consumer.write(MEDIA_VOLUME_DOWN);
      currentVolume--;
      delay(5);
      //If the current volume is below the mapped volumes value, send one volume down command,
      //decrement current volume and delay for stability
    }
    if (currentVolume == mapValue) {
      //No operation
    }
  }
  digitalWrite(3, LOW);
  digitalWrite(2, HIGH);
}
