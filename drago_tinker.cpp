

#include "application.h"



int tinkerDigitalRead(String pin);
int tinkerDigitalWrite(String command);
int tinkerAnalogRead(String pin);
int tinkerAnalogWrite(String command);
void myHandler(const char *event, const char *data);
#define SKETCHNAME "drago-tinker.cpp"
#define LEFT_RELAY D3
#define RIGHT_RELAY D4
int leftState, rightState;





//PUT YOUR VARIABLES HERE

void setup()
{
	Particle.function("digitalread", tinkerDigitalRead);
	Particle.function("digitalwrite", tinkerDigitalWrite);
	Particle.function("analogread", tinkerAnalogRead);
	Particle.function("analogwrite", tinkerAnalogWrite);

	Particle.variable("sketch_name", SKETCHNAME);  //ktb 7.21.2015
	Particle.variable("LEFT_STATE", leftState);
	Particle.variable("RIGHT_STATE", rightState);

	Particle.subscribe("drago", myHandler, MY_DEVICES);


	//PUT YOUR SETUP CODE HERE
	pinMode(LEFT_RELAY, OUTPUT);
	pinMode(RIGHT_RELAY, OUTPUT);



}

void loop()
{
	//PUT YOUR LOOP CODE HERE
	leftState = digitalRead(LEFT_RELAY);
	rightState = digitalRead(RIGHT_RELAY);


}

int tinkerDigitalRead(String pin) {
	int pinNumber = pin.charAt(1) - '0';
	if (pinNumber< 0 || pinNumber >7) return -1;
	if(pin.startsWith("D")) {
		pinMode(pinNumber, INPUT_PULLDOWN);
		return digitalRead(pinNumber);}
	else if (pin.startsWith("A")){
		pinMode(pinNumber+10, INPUT_PULLDOWN);
		return digitalRead(pinNumber+10);}
	return -2;}

int tinkerDigitalWrite(String command){
	bool value = 0;
	int pinNumber = command.charAt(1) - '0';
	if (pinNumber< 0 || pinNumber >7) return -1;
	if(command.substring(3,7) == "HIGH") value = 1;
	else if(command.substring(3,6) == "LOW") value = 0;
	else return -2;
	if(command.startsWith("D")){
		pinMode(pinNumber, OUTPUT);
		digitalWrite(pinNumber, value);
		return 1;}
	else if(command.startsWith("A")){
		pinMode(pinNumber+10, OUTPUT);
		digitalWrite(pinNumber+10, value);
		return 1;}
	else return -3;}

int tinkerAnalogRead(String pin){
	int pinNumber = pin.charAt(1) - '0';
	if (pinNumber< 0 || pinNumber >7) return -1;
	if(pin.startsWith("D")){
		pinMode(pinNumber, INPUT);
		return analogRead(pinNumber);}
	else if (pin.startsWith("A")){
		pinMode(pinNumber+10, INPUT);
		return analogRead(pinNumber+10);}
	return -2;}

int tinkerAnalogWrite(String command){
	int pinNumber = command.charAt(1) - '0';
	if (pinNumber< 0 || pinNumber >7) return -1;
	String value = command.substring(3);
	if(command.startsWith("D")){
		pinMode(pinNumber, OUTPUT);
		analogWrite(pinNumber, value.toInt());
		return 1;}
	else if(command.startsWith("A")){
		pinMode(pinNumber+10, OUTPUT);
		analogWrite(pinNumber+10, value.toInt());
		return 1;}
	else return -2;}

void myHandler(const char *event, const char *data) {
		if (strcmp(data,"on")==0 || strcmp(data,"11")==0) {
			digitalWrite(LEFT_RELAY,HIGH);
			digitalWrite(RIGHT_RELAY,HIGH);
			Particle.publish("drago.state","11");
		} else if (strcmp(data,"off")==0 || strcmp(data,"00")==0) {
			digitalWrite(LEFT_RELAY,LOW);
			digitalWrite(RIGHT_RELAY,LOW);
			Particle.publish("drago.state","00");
		} else if (strcmp(data,"left")==0 || strcmp(data,"10")==0) {
			digitalWrite(LEFT_RELAY,HIGH);
			digitalWrite(RIGHT_RELAY,LOW);
			Particle.publish("drago.state","10");
		} else if (strcmp(data,"right")==0 || strcmp(data,"01")==0) {
			digitalWrite(LEFT_RELAY,LOW);
			digitalWrite(RIGHT_RELAY,HIGH);
			Particle.publish("drago.state","01");
		} else if (strcmp(data,"")==0) {
			String readstate = String(leftState);
			readstate.concat(String(rightState));
			Particle.publish("drago.state",readstate);
		}

	}
