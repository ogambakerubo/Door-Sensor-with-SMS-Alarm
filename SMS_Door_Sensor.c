#include <SoftwareSerial.h>

#define NUMBER_OF_STRING 1
#define MAX_STRING_SIZE 15

// set the GSM module to receive and transmit data on pins 9 and 10 respectively
SoftwareSerial SIM900A(9, 10);
// digital pin 2 has a reedSwitch attached to it. Give it a name:
int reedSwitch = 2;
// SMS recepients
char recepients[NUMBER_OF_STRING][MAX_STRING_SIZE] = {"xxxxxxxx"}; // Enter recepient(s) numbers

String message;
// reedswitch status
int switchState = 0;
int lastState = 0;

void setup() {
  // initialise serial communication at 9600 bits per second:
  Serial.begin(9600);
  // initialise SIM900A communication at 115200 bits per second:
  SIM900A.begin(115200);
  // make the reedswitch's pin an input:
  pinMode(reedSwitch, INPUT_PULLUP);
}

void loop() {
  // read the input pin:
  switchState = digitalRead(reedSwitch);
  // print out the state of the switch:
  if (switchState != lastState) {
    if (switchState == HIGH) {
      message =  "Door was opened";
      // send SMS indicating the door has been opened
      sendMssg();
    } else {
      message = "Door was closed";
      // send SMS indicating the door has been closed
      sendMssg();
    }
  }
  lastState = switchState;
}

void sendMssg() {
  for (int i = 0; i < NUMBER_OF_STRING; i++ ) {
    SIM900A.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
    delay(1000);  // delay of 1000 milliseconds
    SIM900A.println();
    SIM900A.print("AT+CMGS=");
    SIM900A.print("\"+2547"); // country code
    SIM900A.print(recepients[i]);
    SIM900A.print("\"");
    SIM900A.println();
    delay(500);
    SIM900A.println(message);// The SMS text you want to send
    delay(500);
    SIM900A.println((char)26);// ASCII code of CTRL+Z
    delay(500);
  }
}
