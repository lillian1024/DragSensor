#define YELLOW_ONE_LED      16
#define YELLOW_TWO_LED      10
#define RED_ONE_LED         2
#define RED_TWO_LED         9
#define WHITE_ONE_LED         -1
#define WHITE_TWO_LED         -1

#define SENSOR_START_ONE_OUT      14
#define SENSOR_START_ONE_IN       12

#define SENSOR_START_TWO_OUT      13
#define SENSOR_START_TWO_IN       15

#define USE_END_SENSOR 0

#define SENSOR_END_ONE_OUT      -1
#define SENSOR_END_ONE_IN       -1

#define SENSOR_END_TWO_OUT      -1
#define SENSOR_END_TWO_IN       -1

#define START_IN            0

float init_one_start_distance = 0;
float init_two_start_distance = 0;

float init_one_end_distance = 0;
float init_two_end_distance = 0;

float start_time = 0;

float one_time = 0;
float two_time = 0;

bool triggered = 0;
bool last_start = false;

float get_one_distance()
{
  digitalWrite(SENSOR_START_ONE_OUT, LOW);  
  delayMicroseconds(2);  
  digitalWrite(SENSOR_START_ONE_OUT, HIGH);  
  delayMicroseconds(10);  
  digitalWrite(SENSOR_START_ONE_OUT, LOW); 

  float duration = pulseIn(SENSOR_START_ONE_IN, HIGH);

  return (duration*.0343)/2;
}

float get_two_distance()
{
  digitalWrite(SENSOR_START_TWO_OUT, LOW);
  delayMicroseconds(2);
  digitalWrite(SENSOR_START_TWO_OUT, HIGH);
  delayMicroseconds(10);
  digitalWrite(SENSOR_START_TWO_OUT, LOW);

  float duration = pulseIn(SENSOR_START_TWO_IN, HIGH);

  return (duration*.0343)/2;
}

float get_one_end_distance()
{
  digitalWrite(SENSOR_END_ONE_OUT, LOW);
  delayMicroseconds(2);
  digitalWrite(SENSOR_END_ONE_OUT, HIGH);
  delayMicroseconds(10);
  digitalWrite(SENSOR_END_ONE_OUT, LOW);

  float duration = pulseIn(SENSOR_END_ONE_IN, HIGH);

  return (duration*.0343)/2;
}

float get_two_end_distance()
{
  digitalWrite(SENSOR_END_TWO_OUT, LOW);
  delayMicroseconds(2);
  digitalWrite(SENSOR_END_TWO_OUT, HIGH);
  delayMicroseconds(10);
  digitalWrite(SENSOR_END_TWO_OUT, LOW);

  float duration = pulseIn(SENSOR_END_TWO_IN, HIGH);

  return (duration*.0343)/2;
}

void reset_pos()
{
  init_one_start_distance = get_one_distance();
  init_two_start_distance = get_two_distance();

  init_one_end_distance = get_one_end_distance();
  init_two_end_distance = get_two_end_distance();
}

void setup() {
  pinMode(YELLOW_ONE_LED, OUTPUT);
  pinMode(YELLOW_TWO_LED, OUTPUT);
  pinMode(RED_ONE_LED, OUTPUT);
  pinMode(RED_TWO_LED, OUTPUT);

  pinMode(SENSOR_START_ONE_OUT, OUTPUT);
  pinMode(SENSOR_START_ONE_IN, INPUT);

  pinMode(SENSOR_START_TWO_OUT, OUTPUT);
  pinMode(SENSOR_START_TWO_IN, INPUT);

  pinMode(START_IN, INPUT);

  if (USE_END_SENSOR)
  {
    pinMode(WHITE_ONE_LED, OUTPUT);
    pinMode(WHITE_TWO_LED, OUTPUT);

    pinMode(SENSOR_END_ONE_OUT, OUTPUT);
    pinMode(SENSOR_END_ONE_IN, INPUT);

    pinMode(SENSOR_END_TWO_OUT, OUTPUT);
    pinMode(SENSOR_END_TWO_IN, INPUT);
  }

  reset_pos();
}

bool one_trigger()
{
  return get_one_distance() < (init_one_start_distance - 50);
}

bool two_trigger()
{
  return get_two_distance() < (init_two_start_distance - 50);
}

bool one_end()
{
  return get_two_distance() < (init_two_start_distance - 50);
}

bool two_end()
{
  return get_two_distance() < (init_two_start_distance - 50);
}

bool get_start()
{
  return digitalRead(START_IN) == HIGH;
}

void send_trigger()
{
  triggered = true;
}

void loop() 
{
  if (one_trigger() && !get_start())
  {
    digitalWrite(YELLOW_ONE_LED, HIGH);
    digitalWrite(RED_ONE_LED, LOW);
  }
  else
  {
    digitalWrite(YELLOW_ONE_LED, LOW);
  }

  if (two_trigger() && !get_start())
  {
    digitalWrite(YELLOW_TWO_LED, HIGH);
    digitalWrite(RED_TWO_LED, LOW);
  }
  else
  {
    digitalWrite(YELLOW_TWO_LED, LOW);
  }

  if (two_trigger() && one_trigger())
  {
    send_trigger();
  }
  if (triggered && !get_start())
  {
    if (!two_trigger())
    {
      digitalWrite(RED_TWO_LED, HIGH);
    }

    if (!one_trigger())
    {
      digitalWrite(RED_ONE_LED, HIGH);
    }
  }
  
  if (get_start())
  {
    if (!last_start)
    {
      start_time = millis();
      one_time = -1;
      two_time = -1;
    }
    last_start = true;
    triggered = false;
    digitalWrite(YELLOW_ONE_LED, LOW);
    digitalWrite(YELLOW_TWO_LED, LOW);
    if (USE_END_SENSOR)
    {
      if (one_end)
      {
        one_time = millis() - start_time;
        if (two_end < 0)
        {
          digitalWrite(WHITE_ONE_LED, HIGH);
        }
      }
      if (two_end)
      {
        two_time = millis() - start_time;
        if (one_end < 0)
        {
          digitalWrite(WHITE_TWO_LED, HIGH);
        }
      }
    }
  }else
  {
    last_start = false;
    if (USE_END_SENSOR)
    {
      digitalWrite(WHITE_ONE_LED, LOW);
      digitalWrite(WHITE_TWO_LED, LOW);
    }
  }
}
