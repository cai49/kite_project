// Debug 
#define debug_flag 0

#if debug_flag
   #define debug_begin(x)   Serial.begin(x)
   #define debug(x)         Serial.print(x)
   #define debugln(x)       Serial.println(x)
#else
   #define debug_begin(x)
   #define debug(x)
   #define debugln(x)
#endif

// Motor Driver
const int m1_pulse_pin = 8;
const int m1_direction_pin = 9;
const int m1_enable_pin = 10;

const int m2_pulse_pin = 11;
const int m2_direction_pin = 12;
const int m2_enable_pin = 13;

const int DV_MUX = 500;

// Serial Directives
const String wait_directive = "WAIT";
const String rotate_directive = "ROTATE";
const String set_speed_directive = "SPEED";

String msg = "";

// Direction Constants
const int RIGHT = 0;
const int LEFT = 1;

const int REVERSE = 0;
const int FORWARD = 1;

int direction = FORWARD;

// Speed Control
const int FAST = 200;
const int NOM = 125;
const int SLOW = 75;
const int STOP = 0;
int drive_speed = NOM;

// Simple State Machine
const int IDLE = 0;
const int DRIVE = 1;
int state = IDLE;

void setup() {
  Serial.begin(115200);
  while(!Serial) {}

  pinMode(m1_direction_pin, OUTPUT);
  pinMode(m1_enable_pin, OUTPUT);
  pinMode(m1_pulse_pin, OUTPUT);

  pinMode(m2_direction_pin, OUTPUT);
  pinMode(m2_enable_pin, OUTPUT);
  pinMode(m2_pulse_pin, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    debugln(Serial.readString());
    msg = Serial.readStringUntil('\n');

    int end_string = msg.length();

    if (msg == "LOCATE") 
    {
      state = IDLE;
    }
    else if (msg == "END") 
    {
      state = IDLE;
    }
    else if (msg == "FORWARD")
    {
      state = DRIVE;
      direction = FORWARD;
    }
    else 
    {
      if(wait_directive == msg.substring(0, wait_directive.length()))
      {
        String station = msg.substring(wait_directive.length()+1, end_string);
        int wait_time =  station.toInt();

        debugln("Parametro: " + station);
        debugln("Numero: " + String(wait_time));

        if(wait_time!=0)
        {
          delay(wait_time);
        }
        else
        {
          debugln("Received wait directive: " + station);
        }
      }
      else if (rotate_directive == msg.substring(0, rotate_directive.length()))
      {
        String direction = msg.substring(rotate_directive.length()+1, end_string);

        if (direction == "RIGHT") 
        {
          debugln("Turning Right!");
          rotate(RIGHT);
        }
        else
        {
          debugln("Turning Left!");
          rotate(LEFT);
        }
      }
      else if (set_speed_directive == msg.substring(0, set_speed_directive.length()))
      {
        String new_speed = msg.substring(set_speed_directive.length()+1, end_string);

        if (new_speed == "FAST")
        {
          drive_speed = FAST;
        }
        else if (new_speed == "NOM")
        {
          drive_speed = NOM;
        }
        else if (new_speed == "SLOW")
        {
          drive_speed = SLOW;
        }
      }
    }

    Serial.println("Processed message: " + msg);
    delay(100);
  }

  switch (state) 
  {
    case DRIVE:
      drive_state();
      break;
    case IDLE:
      digitalWrite(m1_enable_pin, LOW);
      digitalWrite(m1_pulse_pin, LOW);

      digitalWrite(m2_enable_pin, LOW);
      digitalWrite(m2_pulse_pin, LOW);
      break;
  }
}

void drive_state()
{
  // digitalWrite(LED, HIGH);
  // delay(100);
  // digitalWrite(LED, LOW);

  for (int i = 0; i < drive_speed; i++)
  {
    digitalWrite(m1_direction_pin, direction);
    digitalWrite(m1_enable_pin, HIGH);
    digitalWrite(m1_pulse_pin, HIGH);

    digitalWrite(m2_direction_pin, direction);
    digitalWrite(m2_enable_pin, HIGH);
    digitalWrite(m2_pulse_pin, HIGH);
    delayMicroseconds(DV_MUX);

    digitalWrite(m1_pulse_pin, LOW);
    digitalWrite(m2_pulse_pin, LOW);
    delayMicroseconds(DV_MUX);
  }
}

void rotate(int dir)
{
    if (dir == RIGHT)
    {
      
    }
    else if (dir == LEFT)
    {
      
    }
}
