#include <RGBLed.h>         // RGB LED lib for status LED
#include <HCSR04.h>         // Ultrasonic sensor library

/// Debug 
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

// Ultrasonic Module 
const uint8_t trigger_pin = 3;
const uint8_t echo_pin = 4;

double distance;                              // Represents the current measured distance by the sensor
double clearance_distance = 10.0;             // Set minimum working distance to be 10cm

// Motor Driver
const uint8_t l_dir_pin = 5;
const uint8_t l_en_pin = 6;
const uint8_t l_pul_pin = 7;

const uint8_t r_dir_pin = 8;
const uint8_t r_en_pin = 9;
const uint8_t r_pul_pin = 10;

const int MUX = 500;                          // Time between stepper pulses (in us)

/// Sensor Board
// Status RGB LED
const uint8_t R_pin = 12;
const uint8_t G_pin = 11;
const uint8_t B_pin = 13;

RGBLed status_led(R_pin, G_pin, B_pin, RGBLed::COMMON_ANODE);

// CNY70 Sensors
const uint8_t S0_pin = A0;    // Right Sensor
const uint8_t S1_pin = A1;    // Middle Sensor
const uint8_t S2_pin = A2;    // Left Sensor

bool S0 = false;
bool S1 = false;
bool S2 = false;

// Less than this value will indicate dark color
const int S0_tres = 40;
const int S1_tres = 300;
const int S2_tres = 300;

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
int drive_speed = FAST;
int prev_drive_speed = drive_speed;

// Simple State Machine
const int IDLE = 0;
const int DRIVE = 1;
const int STUCK = 2;
const int ERROR = 3;
const int RR = 4;
const int RL = 5;
int state = IDLE;
int prev_state = state;

bool waiting = true;
int waiting_time = 100;

void setup() {
  Serial.begin(115200);
  while(!Serial) {}
  Serial.flush();

  HCSR04.begin(trigger_pin, echo_pin);

  pinMode(l_dir_pin, OUTPUT);
  pinMode(l_en_pin , OUTPUT);
  pinMode(l_pul_pin, OUTPUT);

  pinMode(r_dir_pin, OUTPUT);
  pinMode(r_en_pin , OUTPUT);
  pinMode(r_pul_pin, OUTPUT);

  pinMode(S0_pin, INPUT);
  pinMode(S1_pin, INPUT);
  pinMode(S2_pin, INPUT);

  measure_clearance();
}

void loop() {
  if (waiting)
  {
    waiting_time -= 1;

    if (waiting_time <= 0)
    {
      waiting = false;
    }
    
    debugln(waiting_time);
  }

  if (Serial.available() > 0 && !waiting) {
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
      drive_speed = SLOW;
      state = DRIVE;
      // direction = FORWARD;
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
          waiting_time = wait_time;
          waiting = true;
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
          prev_state = state;
          state = RR;
        }
        else
        {
          debugln("Turning Left!");
          prev_state = state;
          state = RL;
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
      // else
      // {
      //   state = ERROR;
      // }
    }

    // Since RPi5 request a reply, send the processed message.
    Serial.println("Processed message: " + msg);
      
    // Delay allows the serial buffer to be adequately filled
    delay(100);
  }

  // Update distance before any state desicion
  measure_clearance();

  if (distance < clearance_distance) 
  {
    prev_state = state;
    state = STUCK;
  }

  switch (state) 
  {
    case DRIVE:
      // Drive to whatever target speed
      drive_state();

      status_led.setColor(RGBLed::GREEN);
      break;
    
    case IDLE:
      // Turn OFF the Motors
      status_led.setColor(RGBLed::BLUE);

      prev_drive_speed = drive_speed;
      drive_speed = STOP;
      break;
    
    case STUCK:
      // Stop any movement and set the status LED to Orange
      // Wait for 10 seconds before checking if unstuck
      status_led.setColor(255, 165, 0);

      prev_drive_speed = drive_speed;
      drive_speed = STOP;

      while (distance < clearance_distance)
      {
        measure_clearance();              // Update measured distance              
      }

      drive_speed = prev_drive_speed;
      state = prev_state;

      break;

    case ERROR:
      // Turn OFF the Motors and set status LED to blinking red
      status_led.flash(RGBLed::RED, 250);
      break;
    
    case RR:
        rotate(RIGHT);
        state = prev_state;
      break;

    case RL:
        rotate(RIGHT);
        state = prev_state;
      break;
  }
}

void drive_state()
{
  // Ask CNY70 sensors for their state and drive accordingly.
  update_sensors();

  // Drive fully forward
  if(S1 && (!S0 && !S2))
  {
      for (int i = 0; i < drive_speed; i++)
      {
        digitalWrite(l_dir_pin, HIGH);
        digitalWrite(r_dir_pin, LOW);
        digitalWrite(l_en_pin, HIGH);
        digitalWrite(r_en_pin, HIGH);
        digitalWrite(l_pul_pin, HIGH);
        digitalWrite(r_pul_pin, HIGH);
        delayMicroseconds(500);
        
        digitalWrite(l_pul_pin, LOW);
        digitalWrite(r_pul_pin, LOW);
        delayMicroseconds(500);

        delay(5);
      }
  }
  // Follow the line right
  else if (!S0 && (S1 || S2))
  {

    for (int i = 0; i < drive_speed/5; i++)
    {
      // digitalWrite(l_dir_pin, LOW);
      digitalWrite(r_dir_pin, LOW);
      // digitalWrite(l_en_pin, HIGH);
      digitalWrite(r_en_pin, HIGH);
      // digitalWrite(l_pul_pin, HIGH);
      digitalWrite(r_pul_pin, HIGH);
      delayMicroseconds(MUX);
      
      // digitalWrite(l_pul_pin, LOW);
      digitalWrite(r_pul_pin, LOW);
      delayMicroseconds(MUX);

      delay(5);
    }
    
  }
  // Follow the line left
  else if (!S2 && (S1 || S0))
  {
    for (int i = 0; i < drive_speed/5; i++)
    {
      digitalWrite(l_dir_pin, HIGH);
      // digitalWrite(r_dir_pin, HIGH);
      digitalWrite(l_en_pin, HIGH);
      // digitalWrite(r_en_pin, HIGH);
      digitalWrite(l_pul_pin, HIGH);
      // digitalWrite(r_pul_pin, HIGH);
      delayMicroseconds(MUX);
      
      digitalWrite(l_pul_pin, LOW);
      // digitalWrite(r_pul_pin, LOW);
      delayMicroseconds(MUX);

      delay(5);
    }
  }
  else 
  {
    for (int i = 0; i < drive_speed; i++)
      {
        digitalWrite(l_dir_pin, HIGH);
        digitalWrite(r_dir_pin, LOW);
        digitalWrite(l_en_pin, HIGH);
        digitalWrite(r_en_pin, HIGH);
        digitalWrite(l_pul_pin, HIGH);
        digitalWrite(r_pul_pin, HIGH);
        delayMicroseconds(500);
        
        digitalWrite(l_pul_pin, LOW);
        digitalWrite(r_pul_pin, LOW);
        delayMicroseconds(500);

        delay(5);
      }
  }
}

void rotate(int dir)
{
    if (dir == RIGHT)
    {
      for (int i = 0; i < drive_speed * 2; i++)
      {
        digitalWrite(l_dir_pin, HIGH);
        digitalWrite(r_dir_pin, HIGH);
        digitalWrite(l_en_pin, HIGH);
        digitalWrite(r_en_pin, HIGH);
        digitalWrite(l_pul_pin, HIGH);
        digitalWrite(r_pul_pin, HIGH);
        delayMicroseconds(MUX);
        
        digitalWrite(l_pul_pin, LOW);
        digitalWrite(r_pul_pin, LOW);
        delayMicroseconds(MUX);

        delay(5);
      }
    }
    else if (dir == LEFT)
    {
      for (int i = 0; i < drive_speed * 2; i++)
      {
        digitalWrite(l_dir_pin, LOW);
        digitalWrite(r_dir_pin, LOW);
        digitalWrite(l_en_pin, HIGH);
        digitalWrite(r_en_pin, HIGH);
        digitalWrite(l_pul_pin, HIGH);
        digitalWrite(r_pul_pin, HIGH);
        delayMicroseconds(MUX);
        
        digitalWrite(l_pul_pin, LOW);
        digitalWrite(r_pul_pin, LOW);
        delayMicroseconds(MUX);

        delay(5);
      }
    }
}

void measure_clearance()
{
  double* ds = HCSR04.measureDistanceCm();
  distance = ds[0];

  // debug("1: ");
  // debug(distance);
  // debugln(" cm");
  // debugln("---");
}

void update_sensors()
{
  int curr_s0 = analogRead(S0_pin);
  int curr_s1 = analogRead(S1_pin);
  int curr_s2 = analogRead(S2_pin);

  S0 = curr_s0 < S0_tres;
  S1 = curr_s1 < S1_tres;
  S2 = curr_s2 < S2_tres;

  // debugln("R: " + String(S0));
  // debugln("M: " + String(S1));
  // debugln("L: " + String(S2));

  // debug("R: ");
  // debugln(curr_s0);

  // debug("M: ");
  // debugln(curr_s1);

  // debug("L: ");
  // debugln(curr_s2);
}
