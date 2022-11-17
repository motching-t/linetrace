//  linetrace
const	int	LED_PIN  = 13; 	//LED Pin
const	int	M1_D_PIN  = 4;	//M1 Direction Pin
const	int	M1_S_PIN  = 5;	//M1 Speed Pin
const	int	M2_S_PIN  = 6; 	//M2 Speed Pin
const	int	M2_D_PIN  = 7; 	//M2 Direction Pin
const	int	S_CR_PIN  =11;	//Sensor CenterR Pin
const	int	S_CL_PIN  =12;	//Sensor CenterL Pin
const	int	S_OR_PIN  = 9; 	//Sensor OuterR Pin
const	int	S_OL_PIN = 10; 	//Sensor OuterL Pin
const	int	SPEED_H = 255;
const	int	SPEED_M = 100;
const	int	SPEED_Z   = 0;
const int FWD       = HIGH; //LOW  if it go reverse 
const int BWD       = LOW;  //HIGH if it goes reverse
const int MOTOR_L   = 1;    //Motor selection parameter Left
const int MOTOR_R   = 0;    //Motor selection parameter Right
const	int	BLACK   = LOW;
const	int	WHITE  = HIGH;

const int PB_PIN    = 7;  // read S1-5 analog pin



void  moveM (int motor, int dir, int speed) {
  static  int oldM_dir[2]   = {0, 0}; // static memory of previous call 
  static  int oldM_speed[2] = {0, 0}; // static memory of previous call
  int         M_S_PIN[2];             // speed PIN
  int         M_D_PIN[2];             // directionPIN
  M_S_PIN[0] =  M1_S_PIN;
  M_S_PIN[1] =  M2_S_PIN;
  M_D_PIN[0] =  M1_D_PIN;
  M_D_PIN[1] =  M2_D_PIN;


  if ((oldM_dir[motor] != dir) && (oldM_speed[motor] != SPEED_Z) 
                               && (speed != SPEED_Z)) { // dir change then pause
    analogWrite (M_S_PIN[motor], SPEED_Z);  // set PWM zero
    delay (100);
  }
  
  digitalWrite (M_D_PIN[motor], dir);       // set direction
  analogWrite  (M_S_PIN[motor], speed) ;    // stop for short time
  oldM_dir[motor]   = dir;                  // memorize previous direction
  oldM_speed[motor] = speed;                // memorize previous speed
}

int  getPB ()  {             // Sense Key in pin and get number
  const int KEY_SENS_V[6]   ={0, 30, 150, 360, 535, 760}; // threshold voltage
  const int KEY_KEEP  = 100;  //Key button must be kept for 100ms 
  int       keyVolt;
  int       i     = 6;        //initial value not to exit loop at first time 
  int       oldI  = 0;        //initial value not to exit loop at first time

  while (i != oldI) {         //loop until i equals to previous i (oldI) 
    oldI = i;                 // memorize key num candidate in oldI
    delay (KEY_KEEP);         // supress chattering
    keyVolt = analogRead(PB_PIN);     // read Volt from the button
    for (i = 1; i < 6; i++)   {       // 1- 5 out of 0 -5
      if (keyVolt < KEY_SENS_V[i]) {      //key number is identified  
        break;  
      }
    }                         // exit after key num is identified as i  
  }                           // exit when i=oldI  (key_num is kept for delay)
  if (i == 6) {               // PB is not pushed
    i = 0;
  }
  return i;                    
}

void  ledBlinkN (int count, int onTime, int offTime)  { // Blink "count" times
  int i;
  for (i = 0; i < count; i++)   {
    digitalWrite (LED_PIN, HIGH);
    delay (onTime);  
    digitalWrite (LED_PIN, LOW);
    delay (offTime);
  }
}

int wakeUp (int s) {     // wait for certain key
  int         keyIn;
  while (true) {                  // loop
    ledBlinkN (1, 250, 250);      // Waiting signal(LED flushing) 
    keyIn = getPB ( );
    if ((0 < keyIn ) && (keyIn <= s )) {    // Key is propper 
      break;              //   then break and return
    }
  }                       // continue until proper key is pressed
  return keyIn;
}

void setup() {
  pinMode (LED_PIN, OUTPUT);
  pinMode (M1_D_PIN, OUTPUT);
  pinMode (M2_D_PIN, OUTPUT);
  pinMode (S_CR_PIN, INPUT);
  pinMode (S_CL_PIN, INPUT);
//  pinMode (S_OR_PIN, INPUT);
//  pinMode (S_OL_PIN, INPUT);
  wakeUp (1);     // wakeUp at S1
}

void  loop() {
  int readSCR;
  int readSCL;
//  int readSOR;        // no use
//  int readSOL;        // no use

  while (true) {
    readSCR = digitalRead(S_CR_PIN);
    readSCL = digitalRead(S_CL_PIN);
    if (readSCR == WHITE && readSCL == BLACK) {
      moveM (MOTOR_L,FWD,SPEED_Z);      // L FWD Stop
      moveM (MOTOR_R,FWD,SPEED_H);      // R FWD FullSpeed
    }
    if (readSCR == BLACK && readSCL == WHITE) {
      moveM (MOTOR_R,FWD,SPEED_Z);      // R FWD Stop
      moveM (MOTOR_L,FWD,SPEED_H);      // L FWD FullSpeed
    }
  }
}
