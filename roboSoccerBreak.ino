// ---- BTS7960 (IBT-2) LEFT module : one motor ----
#define LPWM_F 9    // RPWM : forward
#define LPWM_B 5    // LPWM : backward
#define LEN    4    // R_EN + L_EN tied together

// ---- BTS7960 (IBT-2) RIGHT module : one motor ----
#define RPWM_F 10   // RPWM : forward
#define RPWM_B 11   // LPWM : backward
#define REN    12   // R_EN + L_EN tied together

void setup() {
  pinMode(LPWM_F, OUTPUT);  pinMode(LPWM_B, OUTPUT);  pinMode(LEN, OUTPUT);
  pinMode(RPWM_F, OUTPUT);  pinMode(RPWM_B, OUTPUT);  pinMode(REN, OUTPUT);
  digitalWrite(LEN, HIGH);   // enable left driver
  digitalWrite(REN, HIGH);   // enable right driver

  Serial.begin(9600);
}

void loop() {

  int x = pulseIn(3, HIGH, 25000);
  delay(5);
  int y = pulseIn(2, HIGH, 25000);
  delay(5);

  x = pulseToPWM_FB(x);
  y = pulseToPWM_FB(y);

  int left = x + y;
  int right = x - y;
  Set_Speed(left, right);
}

int pulseToPWM_FB(int P) {
    if(P<900)
      return 0;

    P = map(P, 900, 2100, -400, 400);
    P = constrain(P, -255, 255);

    if ( abs(P) <= 50 )
      P = 0;

    return P;
}

int pulseToPWM_LR(int P) {
    if(P<900)
      return 0;

    P = map(P, 900, 2100, -200, 200);
    P = constrain(P, -130, 130);

    if ( abs(P) <= 10 )
      P = 0;

    return P;
}


inline void Set_Speed(int L, int R){
  L = constrain(L, -255, 255);
  R = constrain(R, -255, 255);

  if(L>0)
    { analogWrite(LPWM_F, L);  analogWrite(LPWM_B, 0); }
  else if(L<0)
    { analogWrite(LPWM_F, 0);  analogWrite(LPWM_B, -L); }
  else{
    digitalWrite(LPWM_F, HIGH); digitalWrite(LPWM_B, HIGH);   // active brake
    // change both to LOW to coast instead
  }

  if(R>0)
    { analogWrite(RPWM_F, R);  analogWrite(RPWM_B, 0); }
  else if(R<0)
    { analogWrite(RPWM_F, 0);  analogWrite(RPWM_B, -R); }
  else{
    digitalWrite(RPWM_F, HIGH); digitalWrite(RPWM_B, HIGH);   // active brake
  }

  Serial.print(L); Serial.print(' '); Serial.print(R); Serial.println(' ');
}
