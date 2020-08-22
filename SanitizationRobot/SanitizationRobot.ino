#define limitSwitch 2
#define resetButton 3
#define LIGHT 13
#define MOTOR_1 8
#define MOTOR_2 9

volatile bool cal_end = false;
volatile bool reset_pressed = false;
void move_down(int sec){
  digitalWrite(MOTOR_1, HIGH);
  digitalWrite(MOTOR_2, LOW);
  delay(1000*sec); //keep moving for sec seconds
  motor_stop();
}

void move_down_cal(){
  digitalWrite(MOTOR_1, HIGH);
  digitalWrite(MOTOR_2, LOW);
}

void motor_stop(){
  digitalWrite(MOTOR_1, LOW);
  digitalWrite(MOTOR_2, LOW);
}

void move_up(int sec){
  digitalWrite(MOTOR_1, LOW);
  digitalWrite(MOTOR_2, HIGH);
  cal_end = false;
  delay(1000*sec); //keep moving for sec seconds
  motor_stop();
}

void Calibration(){
  Serial.println("inside calibration");
  
  digitalWrite(LIGHT, HIGH);
  cal_end = false;
  if(digitalRead(limitSwitch) == HIGH){
    move_down_cal();
  }
  else{
    cal_end = true;
    Serial.println("LS is pressed");
  }
  while(!cal_end);
  motor_stop();
  cal_end = false;
  Serial.println("end of calibration");
}

void Calibration_end(){
  Serial.println("limit reached");
  
  cal_end = true;
}

void Reset(){
  Serial.println("inside reset");
  reset_pressed = true;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  pinMode(LIGHT, OUTPUT);
  pinMode(MOTOR_1, OUTPUT);
  pinMode(MOTOR_2, OUTPUT);
  
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
  // put your main code here, to run repeatedly:
  Serial.println("waiting for input");
  reset_pressed = false;
  while(!reset_pressed);
  Calibration();
  delay(30*1000); //wait for 30 sec
  Serial.println("Switching light on...");
  digitalWrite(LIGHT,LOW);
  Serial.println("moving now");
  
  int sec = 14;
  int sec_2 = 14;
  unsigned long milli = 1000UL;
  unsigned long time_ = 3*60*milli;
  delay(time_); //wait for 3 minutes
  move_up(sec);
  delay(time_); //wait for 3 minutes
  move_up(sec);
  delay(time_); //wait for 3 minutes
  Calibration();
  reset_pressed = false;
}
