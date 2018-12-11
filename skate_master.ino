//MASTER

//01LO
#include <SoftwareSerial.h>
SoftwareSerial BTserial(5,6); // RX, TX

int sent, state[2] = {0,0};

void setup() {
  BTserial.begin(38400);
  Serial.begin(9600);
  Serial.println("Master:");
  pinMode(5, INPUT); //rx
  pinMode(6, OUTPUT); //tx
  pinMode(4, INPUT);
  pinMode(11, INPUT);
}

void loop() {
  while (Serial.available()) { //prints char sent from serial monitor
    sent = Serial.read();
    Serial.println(sent);
    BTserial.write(sent);
    sent = 0;
  }
  if (digitalRead(11) && state[0] == 0) { //command 0, button 4 ON, GREEN BUTTON
    Serial.println("GREEN-HIGH");
    BTserial.write('1');
    state[0] = 1;
  }
  else if (!digitalRead(11) && state[0] == 1) {//command 1, button 4 OFF
    Serial.println("GREEN-LOW");
    BTserial.write('0');
    state[0] = 0;
  }
  if (digitalRead(4) && state[1] == 0) {//command 2, button 11 ON, BLACK BUTTON
    Serial.println("BLACK-HIGH");
    BTserial.write('L'); //letters
    state[1] = 1;
  }
  else if (!digitalRead(4) && state[1] == 1) {//command 3, button 11 OFF
    Serial.println("BLACK-LOW");
    BTserial.write('O'); //letters
    state[1] = 0;
  }
}
