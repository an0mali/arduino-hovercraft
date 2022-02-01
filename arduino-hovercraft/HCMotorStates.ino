
int thrustMinInput = 20;//min thrust input required to start thrust motors, this allows rudder testing/control without thrust motors
int thrustMinSpeed = 30;//min PWM value for thrust output

int liftFanSpd = 0;
int thrustFanSpd = 0;

bool isFirst = true;
bool thrustReversed = false;

int maxThrustCooldown = 2000; //cooldown time before reversing thrust 

void testReverseThrust(){
  vprint("testReverseThrust", "Testing thrust reverser. Spinning thrust fans up to 50.");
  setThrustFan(50);
  delay(1000);
  vprint("testReverseThrust", "Setting Reverse thrust to true.");
  toggleReverseThrust(true);
  vprint("testReverseThrust", "Initiating Reverse thrust.");
  setThrustFan(50);
  
}

void toggleReverseThrust(bool setRT) {
  //need to dump IR controller and see what button we can set to reverse thrust
  if (setRT != thrustReversed) {
    int thrustCooldown = 0;
    int setPinTo = LOW;
    if (setRT) {
      setPinTo = HIGH;
    };
    
    if (thrustFanSpd > 0) {
      int thrustCooldown = maxThrustCooldown * (thrustFanSpd / 254);
      setThrustFan(0);
      delay(10);
      digitalWrite(ReverserRelayPin, setPinTo);
      delay(10);
      delay(thrustCooldown);
      } else {
        digitalWrite(ReverserRelayPin, setPinTo);
      };
    thrustReversed = setRT;
};
}

void setThrustFan(int value) {
  int targetPin = ThrustMotorPin_L;
  if (thrustReversed) {
    targetPin = ReverserDrivePin;
  };
  if (thrustFanSpd !=  value) {
    if (not testOnly) {
    adjustLiftFan(value);
    analogWrite(targetPin, value);
    };
    thrustFanSpd = value;
  };
}

void adjustLiftFan(int thrustspd) {
  if (thrustspd == 0) {
    setLiftFan(180);
  }  else { 
    setLiftFan(254);
  };
}

void setLiftFan(int value) {
  if (liftFanSpd != value) {
    if (not testOnly) {
    analogWrite(LiftMotorPin, value);
    };
    liftFanSpd = value;
};
}

void setThrust(long raw_thrust) {
  if (isFirst) {
    isFirst = false;
    return;
  };
  int actual_thrust = 0;
  if (raw_thrust > thrustMinInput) {
   actual_thrust = raw_thrust * 2;
   if (actual_thrust > 255) {
    actual_thrust = 255;
   };
   
  };
  setThrustFan(actual_thrust);
}
