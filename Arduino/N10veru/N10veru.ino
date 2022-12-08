// N10veru

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

void setup() {
 pinMode (LED_PIN, OUTPUT);
 pinMode (M1_D_PIN, OUTPUT);
 pinMode (M2_D_PIN, OUTPUT);
 pinMode (S_CR_PIN, INPUT);
 pinMode (S_CL_PIN, INPUT);
 pinMode (S_OR_PIN, INPUT);
 pinMode (S_OL_PIN, INPUT);
 initialB = wakeUp (3);
 lineP = initialB;
 if (initialB == 3) {
 lineP = DIRECT;
 }
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
 if (readSOR ==WHITE || readSOL==WHITE) {
 if (readSOL == BLACK) {
 if (lineP == DIRECT) {
 modeTRACK = M_RH ;
 }
 if (lineP == LOOP) {
 modeTRACK = M_LH;
 }
 }
 if (readSOR == BLACK) {
 if (lineP == DIRECT) {
 modeTRACK = M_LH ;
 }
 if (lineP == LOOP) {
 modeTRACK = M_RH;
 }
 }
 // WHITE – WHITE -> GO FWD
 if(readSCR==WHITE&& readSCL==WHITE) {
 digitalWrite (LED_PIN, LOW);
 moveM (MOTOR_L,FWD,SPEED_H);
 moveM (MOTOR_R,FWD,SPEED_H);
 }
 // WHITE – BLACK -> TURN TO BLACK
 if (readSCR==WHITE && readSCL==BLACK) {
 digitalWrite (LED_PIN, LOW);
 moveM (MOTOR_L,FWD,SPEED_Z);
 moveM (MOTOR_R,FWD,SPEED_H);
 }
 // BLACK - WHITE -> TURN TO BLACK
 if (readSCR==BLACK && readSCL==WHITE) {
 digitalWrite (LED_PIN, LOW);
 moveM (MOTOR_R,FWD,SPEED_Z);
 moveM (MOTOR_L,FWD,SPEED_H);
 }
 // BLACK – BLACK -> TURN as modeTRACK
 if (readSCR==BLACK && readSCL==BLACK){
 digitalWrite (LED_PIN, HIGH);
 if (modeTRACK == M_RH) {
 moveM (MOTOR_R,FWD,SPEED_Z);
 moveM (MOTOR_L,FWD,SPEED_H);
 delay (10);
 }
 if (modeTRACK == M_LH) {
 moveM (MOTOR_L,FWD,SPEED_Z);
 moveM (MOTOR_R,FWD,SPEED_H);
 delay (10);
 }
 }
 }
 else { // Two OuterSensor BLACK
 moveM (MOTOR_L,FWD,SPEED_Z);
 moveM (MOTOR_R,FWD,SPEED_Z);
 ledBlinkN (5, 500, 500);
 if ((initialB == 3) && (lineP == DIRECT)) {
 lineP = LOOP;
 moveM (MOTOR_L,FWD,SPEED_H);
 moveM (MOTOR_R,FWD,SPEED_H);
 delay (2000);
 }
 }
}