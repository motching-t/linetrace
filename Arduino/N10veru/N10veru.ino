い// N10veru
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