// 9_TraceRail++-

const  int LED_PIN   =13;  //LED Pin
const int M1_D_PIN  = 4;  //M1 Direction Pin
const int M1_S_PIN  = 5;  //M1 Speed Pin
const int M2_S_PIN  = 6;  //M2 Speed Pin
const int M2_D_PIN  = 7;  //M2 Direction Pin
const int S_CR_PIN  =11;  //Sensor CenterR Pin
const int S_CL_PIN  =12;  //Sensor CenterL Pin
const int S_OR_PIN  = 9;  //Sensor OuterR Pin
const int S_OL_PIN = 10;  //Sensor OuterL Pin
const int PB_PIN    = 7;  // read S1-5 analog pin
const int SPEED_H = 255;
const int SPEED_M = 100;
const int SPEED_Z =   0;
const int FWD       = HIGH; //LOW  if it go reverse 
const int BWD       = LOW;  //HIGH if it goes reverse
const int MOTOR_L   = 1;    //Motor selection parameter Left
const int MOTOR_R   = 0;    //Motor selection parameter Right
const int BLACK   = LOW;    //Sensor output = Black
const int WHITE  = HIGH;    //Sensor output = White
int START =3;

void moveM (int motor, int dir, int speed) {
 static int oldM_dir[2] = {0, 0};
 static int oldM_speed[2] = {0, 0};
 int M_S_PIN[2];
 int M_D_PIN[2];
 M_S_PIN[0] = M1_S_PIN;
 M_S_PIN[1] = M2_S_PIN;
 M_D_PIN[0] = M1_D_PIN;
 M_D_PIN[1] = M2_D_PIN;
 if ((oldM_dir[motor] != dir) &&
 (oldM_speed[motor] != SPEED_Z) &&
 (speed != SPEED_Z)) { // dir change then pause
 analogWrite (M_S_PIN[motor], SPEED_Z);
 delay (100);
 }
 digitalWrite (M_D_PIN[motor], dir);
 analogWrite (M_S_PIN[motor], speed) ;
 oldM_dir[motor] = dir;
 oldM_speed[motor] = speed;
}

int getPB () {
 const int KEY_SENS_V[6] ={0,30,150,360,
535,760};
 const int KEY_KEEP = 100;
 int keyVolt;
 int i = 6;
 int oldI = 0;
 while (i != oldI) {
 oldI = i;
 delay (KEY_KEEP);
 keyVolt = analogRead(PB_PIN);
 for (i = 1; i < 6; i++) {
 if (keyVolt < KEY_SENS_V[i]) {
 break;
 }
 }
 }
 if (i == 6) {
 i = 0;
 }
 return i;
}

int wakeUp (int s) {
 int keyIn;
 while (true) { // loop
 ledBlinkN (1, 250, 250); // Waiting LED flushing)
 keyIn = getPB ( );
 if (keyIn == 0) { // not pressed
 continue;
 }
 if (keyIn <= s) { // Key is propper
 break;
 }
 } // end -while
 return keyIn;
} 

void ledBlinkN(int count, int onTime, int offTime) {
 int i;
 for (i = 0; i < count; i++) {
 digitalWrite (LED_PIN, HIGH);
 delay (onTime);
 digitalWrite (LED_PIN, LOW);
 delay (offTime);
 }
}
void setup() {
 pinMode (LED_PIN, OUTPUT);
 pinMode (M1_D_PIN, OUTPUT);
 pinMode (M2_D_PIN, OUTPUT);
 pinMode (S_CR_PIN, INPUT);
 pinMode (S_CL_PIN, INPUT);
 pinMode (S_OR_PIN, INPUT);
 pinMode (S_OL_PIN, INPUT);
 wakeUp (START);
}
void loop() {
 int readSCR;
 int readSCL;
 int readSOR;
 int readSOL;
 readSOR = digitalRead(S_OR_PIN);
 readSOL = digitalRead(S_OL_PIN);
 readSCR = digitalRead(S_CR_PIN);
 readSCL = digitalRead(S_CL_PIN);
 if(readSOR==WHITE || readSOL==WHITE) {
 if(readSCR==WHITE&&readSCL==WHITE) {
 moveM (MOTOR_L,FWD,SPEED_H);
 moveM (MOTOR_R,FWD,SPEED_H);
 }
 if(readSCR==BLACK&&readSCL==BLACK) {
 moveM (MOTOR_L,FWD,SPEED_H);
 moveM (MOTOR_R,FWD,SPEED_H);
 }
 if(readSCR==WHITE && readSCL== BLACK) {
 moveM (MOTOR_L,FWD,SPEED_Z);
 moveM (MOTOR_R,FWD,SPEED_H);
 }
 if (readSCR==BLACK && readSCL==WHITE) {
 moveM (MOTOR_R,FWD,SPEED_Z);
 moveM (MOTOR_L,FWD,SPEED_H);
 }
 }
 else {
 moveM (MOTOR_L,FWD,SPEED_Z);
 moveM (MOTOR_R,FWD,SPEED_Z);
 ledBlinkN (5, 500, 500);
 ledBlinkN (1, 2000, 1000);
 }
}
