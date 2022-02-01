//MelMotorSuite
#include "IRremote.h"
int IRRPin = 2;
IRrecv IR_Rec(IRRPin);
decode_results IR_Results;
#include "Servo.h"

bool verbose_debug = true;
bool enable_lift = false;

int ThrustMotorPin_L = 5;
//int ThrustMotorPin_R = 11;
int ReverserRelayPin = 9;
int ReverserDrivePin = 4;
int LiftMotorPin = 6;
int RudderServoPin = 10;

Servo RudderServo;
int RudderCenter = 135;
int RudderAng = 90;
int lowRudderAng = 60;
int RudderCurrent = 0;
int RudderMinInput = 1;
int prevRudderCmd[2];

bool liftEnabled;
bool testOnly = false;

int dirInputVer;
void setup() {

  if (verbose_debug) {
    Serial.begin(9600);
    delay(1000);
    Serial.println("Initializing...");
  }
  setupHCIR();
  RudderServo.attach(RudderServoPin);
  testRudder();
  vprint("setup", "Complete. Ready ");
  //
  //debugMotors();
  //testReverseThrust();
}

void startLift() {
  vprint("startLift", "Starting lift motor...");
  if (not liftEnabled) {
    analogWrite(LiftMotorPin, 150);
    delay(1000);
    analogWrite(LiftMotorPin, 250);
    delay(1000);
    liftEnabled = true;
  };
  vprint("startLift", "Lift motor ready!");
}

void stopLift() {
  if (liftEnabled) {
    analogWrite(LiftMotorPin, 0);
    liftEnabled = false;
  };
}
  
void dirInput(int flip_dir, int inpval) {
  //if (inpval != dirInputVer) {
  //  dirInputVer = inpval;
  //  return;
  //};
  int rudderDir = 1;
  bool lowInput;

  int addAng = RudderAng;
  if (inpval < 25) {
    rudderDir = 0;
  } else if (flip_dir == 0) {
    rudderDir = -1;
  };

  int rudderDegrees = RudderCenter + (addAng * rudderDir);
  setRudder(rudderDegrees);
}

void testRudder() {
  vprint("testRudder", "Testing Rudder...");
  setRudder(RudderCenter + RudderAng);
  delay(300);
  setRudder(RudderCenter - RudderAng);
  delay(300);
  setRudder(RudderCenter);
  delay(300);
}
void setRudder(int value) {
  if (RudderCurrent != value) {
    vprint("setRudder", "Setting rudder to " + String(value));
    RudderServo.write(value);
    RudderCurrent = value;
    delay(50);
  };
}

void loop() {
  checkIR();
}
