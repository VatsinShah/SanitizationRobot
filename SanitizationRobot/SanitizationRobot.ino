//defining pin numbers to be used for the project
#define limitSwitch 2
#define resetButton 3
#define LIGHT 13
#define MOTOR_1 8
#define MOTOR_2 9

//defining booleans for keeping track of interrupts
volatile bool cal_end = false;
volatile bool reset_pressed = false;

//function to move down for "sec" seconds
void move_down(int sec){
  //turn the motor on
  digitalWrite(MOTOR_1, HIGH);
  digitalWrite(MOTOR_2, LOW);

  //keep moving for sec seconds
  delay(1000*sec);

  //stop moving
  motor_stop();
}

//function to move down for calibration
void move_down_cal(){
  //turn the motor on
  digitalWrite(MOTOR_1, HIGH);
  digitalWrite(MOTOR_2, LOW);
}

//function to stop the motor in its position with holding torque
void motor_stop(){
  //connecting both terminals of motor to same potential for holding torque
  digitalWrite(MOTOR_1, LOW);
  digitalWrite(MOTOR_2, LOW);
}

//function to move up for "sec" seconds
void move_up(int sec){
  //turn the motor on
  digitalWrite(MOTOR_1, LOW);
  digitalWrite(MOTOR_2, HIGH);

  //wait for sec seconds
  delay(1000*sec); //keep moving for sec seconds

  //stop moving
  motor_stop();
}

//function to calibrate the position of the lights
void Calibration(){
  //for debugging
  //Serial.println("inside calibration");

  //make sure light is off
  digitalWrite(LIGHT, HIGH);

  //make sure we assume calibration is required
  cal_end = false;

  //check the state of limit switch
  if(digitalRead(limitSwitch) == HIGH){
    //if limit switch is not pressed, move down
    move_down_cal();
  }

  else{
    //if limit switch is pressed make sure flag is set
    cal_end = true;
    //for debugging
    //Serial.println("LS is pressed");
  }

 /*
   * If the lights are'nt at the bottom most position, cal_end is false and can only be set true when an interrupt is triggered which is connected to limit switch 
 */
 
  //if motor is not already down, wait till it reaches down
  while(!cal_end);

  //once motor is definitely at the bottom, stop the motor
  motor_stop();

  //reset cal_end, included this just for robustness
  cal_end = false;

  //for debugging
  //Serial.println("end of calibration");
}

//ISR triggered when limit switch is pressed
void Calibration_end(){
  //for debugging
  //Serial.println("limit reached");

  //set value of cal_end
  cal_end = true;
}

//ISR triggered when start button is pressed
void Reset(){
  //for debugging
  //Serial.println("inside reset");

  //set value of reset_pressed
  reset_pressed = true;
}

void setup() {
  //for debugging
  //Serial.begin(115200);

  //prepare digital pins to control relays
  pinMode(LIGHT, OUTPUT);
  pinMode(MOTOR_1, OUTPUT);
  pinMode(MOTOR_2, OUTPUT);

  //set initial conditions
  digitalWrite(LIGHT, HIGH); //make sure light is off
  digitalWrite(MOTOR_1, LOW);
  digitalWrite(MOTOR_2, LOW);

  //set up the 2 interrupts
  pinMode(limitSwitch, INPUT_PULLUP);
  pinMode(resetButton, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(limitSwitch), Calibration_end, FALLING);
  attachInterrupt(digitalPinToInterrupt(resetButton), Reset, FALLING);
}

void loop() {
  //for debugging
  //Serial.println("waiting for input");
  
  //make sure this variable is false, because it WILL be set to true because of initialization
  reset_pressed = false;

  //infinite loop until reset is pressed
  while(!reset_pressed);

  //calibrate
  Calibration();

  //wait for 30 sec
  delay(30*1000);

  //for debugging
  //Serial.println("Switching light on...");
  
  //switch lights on
  digitalWrite(LIGHT,LOW);

  //for debugging
  //Serial.println("moving now");

  //initialize variable for movement time
  int sec = 14;

  //initialize variables for wait time
  unsigned long milli = 1000UL;
  unsigned long time_ = 3*60*milli;

  //initiate the movement
  delay(time_); //wait for 3 minutes
  move_up(sec);
  delay(time_); //wait for 3 minutes
  move_up(sec);
  delay(time_); //wait for 3 minutes
  
  //calibrate
  Calibration();
}
