/*
reibot.org
Demo program for TCS3200 from parallax and the parallax daughterboard
Call detectColor(out) where 'out' is pinC on the daughterboard. The detectColor will return a 0 if there is nothing color in front of sensor,
1 if red is in front, 2 if blue is in front, or 3 if blue is in front. You can comment out all the serial.print.
If you're tight on pins, remove the taosMode(int) method and all references to it. Remove pins from the TCS3200setup too.
If these wires are disconnected the TCS3200 will run on the highest frequency due to internal pullup resistors on S0 and S1
If you have multiple TCS3200 you may tie all the pins together except the outputs (pinC). Then just use detectColor(TCS3200's output) to
detect color on the selected TCS3200's output pin.
7/6/2011 works on arduino 0022
Taos pins connect to arduino pins 8-13. There is no order and should work on any digital i/o
*/
int S0 = 8;//pinB
int S1 = 9;//pinA
int S2 = 12;//pinE
int S3 = 11;//pinF
int out = 10;//pinC
int LED = 13;//pinD
void setup() {
TCS3200setup();
Serial.begin(115200);
Serial.print("\n\n\nready\n\n\n");
delay(100);
}
void loop() {
Serial.print(detectColor(out));
Serial.print("\n\n\n");
delay(1000);
}
int detectColor(int taosOutPin){
//isPresentTolerance will need to be something small if used in high light environment, large if used in dark environment.
//the color detection will work either way, but the larger isPresentTolerance is, the closer the object will need to be in front of sensor
double isPresentTolerance = 5;
double isPresent = colorRead(taosOutPin,0,0)/colorRead(taosOutPin,0,1);//number gets large when something is in front of sensor.
Serial.print("isPresent:");
Serial.println(isPresent,2);
Serial.print("isPresentTolerance currently set to:");
Serial.println(isPresentTolerance,2);
if(isPresent < isPresentTolerance){
Serial.println("nothing is in front of sensor");
return 0;
}
double red,blue,green;
double white = colorRead(taosOutPin,0,1);
red = white/colorRead(taosOutPin,1,1);
blue = white/colorRead(taosOutPin,2,1);
green = white/colorRead(taosOutPin,3,1);
Serial.print("red");
Serial.println(red);
Serial.print("blue");
Serial.println(blue);
Serial.print("green");
Serial.println(green);
if(red > blue && red > green){
Serial.println("red is in front");
return 1;
}
if(blue > green && blue > red){
Serial.println("blue is in front");
return 2;
}
if(green > blue && green > red){
Serial.println("green is in front");
return 3;
}
}
/*
This method will return the pulseIn reading of the selected color.
Since frequency is proportional to light intensity of the selected color filter,
the smaller pulseIn is, the more light there is of the selected color filter.
It will turn on the sensor at the start taosMode(1), and it will power off the sensor at the end taosMode(0)
color: 0=white, 1=red, 2=blue, 3=green
if LEDstate is 0, LED will be off. 1 and the LED will be on.
taosOutPin is the ouput of the TCS3200. If you have multiple TCS3200, all wires can be combined except the out pin
*/
double colorRead(int taosOutPin, int color, boolean LEDstate){
//make sure that the pin is set to input
pinMode(taosOutPin, INPUT);
//turn on sensor with highest frequency setting
taosMode(1);
//delay to let the sensor sit before taking a reading. Should be very small with this sensor
int sensorDelay = 1;
//set the pins to select the color
if(color == 0){//white
digitalWrite(S3, LOW); //S3
digitalWrite(S2, HIGH); //S2
// Serial.print(" w");
}else if(color == 1){//red
digitalWrite(S3, LOW); //S3
digitalWrite(S2, LOW); //S2
// Serial.print(" r");
}else if(color == 2){//blue
digitalWrite(S3, HIGH); //S3
digitalWrite(S2, LOW); //S2
// Serial.print(" b");
}else if(color == 3){//green
digitalWrite(S3, HIGH); //S3
digitalWrite(S2, HIGH); //S2
// Serial.print(" g");
}
double readPulse;
if(LEDstate == 0){
digitalWrite(LED, LOW);
}
if(LEDstate == 1){
digitalWrite(LED, HIGH);
}
delay(sensorDelay);
readPulse = pulseIn(taosOutPin, LOW, 80000);
//if the pulseIn times out, it returns 0 and that throws off numbers. just cap it at 80k if it happens
if(readPulse < .1){
readPulse = 80000;
}
//turn off color sensor and white LED to save power
taosMode(0);
return readPulse;
}
//setting mode to zero will put taos into low power mode. taosMode(0);
void taosMode(int mode){
if(mode == 0){
//power OFF
digitalWrite(LED, LOW);
digitalWrite(S0, LOW); //S0
digitalWrite(S1, LOW); //S1
// Serial.println("mOFFm");
}else if(mode == 1){
//this will put in 1:1
digitalWrite(S0, HIGH); //S0
digitalWrite(S1, HIGH); //S1
// Serial.println("m1:1m");
}else if(mode == 2){
//this will put in 1:5
digitalWrite(S0, HIGH); //S0
digitalWrite(S1, LOW); //S1
//Serial.println("m1:5m");
}else if(mode == 3){
//this will put in 1:50
digitalWrite(S0, LOW); //S0
digitalWrite(S1, HIGH); //S1
//Serial.println("m1:50m");
}
return;
}
void TCS3200setup(){
//initialize pins
pinMode(LED,OUTPUT); //LED pinD
//color mode selection
pinMode(S2,OUTPUT); //S2 pinE
pinMode(S3,OUTPUT); //s3 pinF
//color response pin (only actual input from taos)
//pinMode(out, INPUT); //out pinC
//communication freq output divider
pinMode(S0,OUTPUT); //S0 pinB
pinMode(S1,OUTPUT); //S1 pinA
return;
}
