String cmd, CMDcur;
int programNumber;
uint8_t counter = 0;
uint8_t PWM_White = 2;
uint8_t PWM_UV = 250;
uint8_t UV_LED = 5;
uint8_t WHITE_LED = 6;
uint8_t pinsNum = 3;
uint8_t stepsNum = 3;

void setup()
{
  pinMode(2, INPUT); /// Our camera strobe in HIGH - Acquiring, LOW - not acquiring
  pinMode(UV_LED, OUTPUT);// UV LED
  pinMode(WHITE_LED, OUTPUT);// White LED
  //  for(int i = 0; i < WL_NUM; i++)
  //    pinMode(5 + i ,OUTPUT); // LED control OUTPUTS (could be later moved to PWM or AM intensity control)
  //  pinMode(WL_NUM + 5, OUTPUT);  //Loop control; happens only once per cycle of all LEDs; for debug
  pinMode(13, OUTPUT); ///LAMP !
  analogWrite(WHITE_LED, PWM_White);
  Serial.begin(115200);
  Serial.setTimeout(100);
  attachInterrupt(0, Strob_Input_Handler, RISING);
  while (!Serial) {
    ;
  }
}


void Strob_Input_Handler() {
  if (counter == 1) {
    analogWrite(UV_LED, PWM_UV);
  }
  else {
    analogWrite(UV_LED, 0);
  }
  counter++;  // + синхр.
  if (counter == 2) {
    counter = 0;
  }
}
void waiting_4_command() {
  int PWM_VAL, PWM_VALH, PWM_VALL;
  cmd = "";
  if (Serial.available()) {
    cmd = Serial.readString();
    cmd.trim();
  }


  if (cmd.substring(0, 2) == "UV") {
    PWM_VALH = cmd[2] - '0';
    PWM_VALL = cmd[3] - '0';
    if (cmd[3] != '\0') {
      PWM_VAL = (PWM_VALH * 10) + (PWM_VALL * 1);
    }
    else {
      PWM_VAL = (PWM_VALH * 1) + (PWM_VALL * 0);
    }
    PWM_UV = PWM_VAL;
  }

  if (cmd.substring(0, 2) == "WH") {
    PWM_VALH = cmd[2] - '0';
    PWM_VALL = cmd[3] - '0';
    if (cmd[3] != '\0') {
      PWM_VAL = (PWM_VALH * 10) + (PWM_VALL * 1);
    }
    else {
      PWM_VAL = (PWM_VALH * 1) + (PWM_VALL * 0);
    }
    PWM_White = PWM_VAL;
    analogWrite(WHITE_LED, PWM_White);
  }
}

void loop()
{
  waiting_4_command();
}
