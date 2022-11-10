//Github V1.06
//car system ~ mmmmmmmmartin ~ 13/11/2020
//----------------------------------------
//knob
//left button blinker
//right button blinker
//alarm mode (hazard)
//highlights
//measure temperature
//connect to VSC
//----------------------------------------
const int KNOB = 14;
const int LDR = 16;
const int TEMP = 15;
const int LEFTKEY = 9;
const int RIGHTKEY = 8;
const int BOUNCE_DELAY = 50;
const int LIGHT_DELAY = 5000; //5 seconds
const int BLINK_DELAY = 500;  //half a second
const int HAZARD_DELAY = 500; //half a second
const int TEMP_DELAY = 5000;  //5 seconds
int lastkey1state = HIGH;
int lastkey2state = HIGH;
int key1state = HIGH;
int key2state = HIGH;
int lasttime = 0;
const int LEFTBLINK = 7;
const int RIGHTBLINK = 6;
const int REDLIGHT = 4;
const int GREENLIGHT = 5;
bool blink_state = false;
unsigned long blink_time = 0;
int last_input;
bool lightsOff = false;
bool hazard = false;
bool hazardblink = false;
bool hazardonce = false;
unsigned long hazardtime = 0;
int threshold = 150;
unsigned long light_start = 0;
unsigned long temp_start = 0;
bool hlonce = false;

void setup()
{
  pinMode(LEFTKEY, INPUT_PULLUP);
  pinMode(RIGHTKEY, INPUT_PULLUP);
  pinMode(LEFTBLINK, OUTPUT);
  pinMode(RIGHTBLINK, OUTPUT);
  pinMode(REDLIGHT, OUTPUT);
  pinMode(GREENLIGHT, OUTPUT);
  pinMode(KNOB, INPUT);
  pinMode(LDR, INPUT);
  Serial.begin(9600);
}

void loop()
{
  int knob_value = map(analogRead(KNOB), 0, 1023, 1, 6);
  int input = read_key();
  //Serial.println(knob_value);
  turnsignals(input, knob_value);
  hazardalarm();
  highlights();
  temp();

}

//method temperature
void temp()
{
  int read_temp = map(analogRead(TEMP), 0, 1023, 100, -55); //the data that returns from the mapping is as close as possible to the actual ones
  if (millis() - temp_start > TEMP_DELAY)
  {
    char placeholder[10];
    sprintf(placeholder, "temp %d", read_temp); //this concatenates string with the returned value from the mapping result
    Serial.println(placeholder);
    temp_start = millis();
  }
}

//method that checks if light is under certain threshold for 5 seconds or more which turns on the highlights
void highlights()
{
  int read_ldr = analogRead(LDR);
  if (read_ldr < threshold)
  {
    if (millis() - light_start > LIGHT_DELAY)
    {
      digitalWrite(GREENLIGHT, HIGH);
      light_start = millis();
      Serial.println("HIGHLIGHTS ON");
      hlonce = true;
    }

  }
  else
  {
    light_start = millis();
    digitalWrite(GREENLIGHT, LOW);
    if (hlonce)
    {
      Serial.println("HIGHLIGHTS OFF");
      hlonce = false;
    }
  }
}

//method for hazard mode
void hazardalarm()
{
  if (Serial.available())
  {
    if (Serial.readStringUntil('\n') == "alarm")
    {
      hazard = !hazard; //state switching
      if (hazard)
      {
        hazardonce = true;
        Serial.println("ALARM ON");
      }
    }
  }
  if (hazard)
  {
    alarmblink();
  }
  else
  {
    if (hazardonce)
    {
      digitalWrite(LEFTBLINK, LOW);
      digitalWrite(RIGHTBLINK, LOW);
      digitalWrite(REDLIGHT, LOW);
      hazardonce = false;
      Serial.println("ALARM OFF");
    }

  }
}

//method which puts out the hazard state as a real world indicator (makes lights go blink-blink basiclly)
void alarmblink()
{
  if (millis() - hazardtime > HAZARD_DELAY)
  {
    digitalWrite(LEFTBLINK, hazardblink);
    digitalWrite(RIGHTBLINK, hazardblink);
    digitalWrite(REDLIGHT, hazardblink);
    hazardblink = !hazardblink; //from lecture example
    hazardtime = millis();
  }
}

//method which holds everything that works with turn signals
void turnsignals(int input, int knob_value)
{

  if (knob_value != 3)
  {
    lightsOff = true;
  }
  if (lightsOff)
  {
    if (knob_value > 2 && last_input == 1)
    {
      last_input = 0; lightsOff = false;
    }
    if (knob_value < 4 && last_input == 2)
    {
      last_input = 0; lightsOff = false;
    }

  }


  if (input == last_input && !hazard)
  {
    digitalWrite(RIGHTBLINK, LOW);
    digitalWrite(LEFTBLINK, LOW);
    last_input = 0;
  }
  else if (input != 0 )
  {
    if (!hazard) {
      if (input == 1)
      {
        digitalWrite(RIGHTBLINK, LOW);
        if (last_input != 1 )
        {
          Serial.println("LEFT");
        }
        blink(LEFTBLINK);
      }
      else if (input == 2)
      {
        digitalWrite(LEFTBLINK, LOW);
        if (last_input != 2 )
        {
          Serial.println("RIGHT");
        }
        blink(RIGHTBLINK);
      }
    }
    last_input = input;
  }
  else
  { if (!hazard) {
      if (last_input == 1 )
      {
        blink(LEFTBLINK);
      }
      else if (last_input == 2)
      {
        blink(RIGHTBLINK);
      }
    }
  }
}

//method that accepts given LED and makes it blink
void blink(int blinker) //parameter
{
  if (millis() - blink_time > BLINK_DELAY)
  {
    digitalWrite(blinker, blink_state);
    blink_state = !blink_state; //from lecture example
    blink_time = millis();
  }
}

//from example Key Display Debounce - Richshield.zip
int read_key()
{
  int key = 0;
  int key1 = digitalRead(LEFTKEY);
  int key2 = digitalRead(RIGHTKEY);

  if (key1 != lastkey1state)
  {
    lasttime = millis();
  }
  if (key2 != lastkey2state)
  {
    lasttime = millis();
  }

  if (millis() - lasttime > BOUNCE_DELAY) {

    if (key1state != key1) {
      key1state = key1;
      if (lastkey1state == LOW)
        key = 1;
    }
    if (key2state != key2) {
      key2state = key2;
      if (lastkey2state == LOW)
        key = 2;
    }
  }
  lastkey1state = key1;
  lastkey2state = key2;
  return key;
}
