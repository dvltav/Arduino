

/*
RFKeyfob Provides an example of how to read the XL-R02 RF module with signals from the four button key remote. 
Reads a digital input on pins 2-6, prints the result to the serial monitor. It is based on the PT 2272 chip. 
The application uses the analog pins since some shields use the digital pins. There is a relay controlled by pin A5. It is turned on by pressing button A on the remote.

*/

//Analog pins 0-4 has a output from the XL-R02A attached to them.

// the setup routine runs once when you press reset:

// Pin 5 is used to control a relay module. It is defined as an output. 
void setup() {

// initialize serial communication at 9600 bits per second: Serial.begin(9600);


pinMode(13, OUTPUT);
// make the pushbutton's pin an input: 
pinMode(A0, INPUT);

pinMode(A1, INPUT);

pinMode(A2, INPUT);

pinMode(A3, INPUT)

; pinMode(A4, INPUT);

pinMode(A5, OUTPUT);

// Make the inputs a pullup resistor

digitalWrite(A0,HIGH);

digitalWrite(A1,HIGH);

digitalWrite(A2,HIGH);

digitalWrite(A3,HIGH); digitalWrite(A4,HIGH);

digitalWrite(A5,LOW);

//pinMode(A5,OUTPUT);

// Wait for 1 second delay(1000); 
}

// the loop routine runs over and over again forever: 
void loop() {

// read the input pin:

int buttonD = digitalRead(A0);

int buttonC = digitalRead(A1);

int buttonB = digitalRead(A2);

int buttonA = digitalRead(A3);

int PinStateVT = digitalRead(A4);

// print out the state of the buttons:

Serial.print("A=");

Serial.print(buttonA); Serial.print("\tB=");

Serial.print(buttonB); Serial.print("\tC=");

Serial.print(buttonC); Serial.print("\tD=");

Serial.print(buttonD); Serial.print("\tVT=");

Serial.print(PinStateVT);

Serial.println();

if ( buttonD == 1 ) { 
  digitalWrite(13,HIGH);
  delay(1000); 
  digitalWrite(13,LOW);
} else{ 
  delay(100); 
}

// delay in between reads for stability

}
