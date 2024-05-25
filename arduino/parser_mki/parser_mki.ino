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

#define LED 13

const String wait_directive = "WAIT";
const String rotate_directive = "ROTATE";
const String set_speed_directive = "SPEED";

String msg = "";

const int RIGHT = 0;
const int LEFT = 1;

const int FAST = 100;
const int NOM = 75;
const int SLOW = 25;
const int STOP = 0;
int drive_speed = NOM;

const int IDLE = 0;
const int DRIVE = 1;
int state = IDLE;

void setup() {
  Serial.begin(115200);
  while(!Serial) {}

  pinMode(LED, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    debugln(Serial.readString());
    msg = Serial.readStringUntil('\n');

    int end_string = msg.length();

    if (msg == "LOCATE") 
    {
      digitalWrite(LED, HIGH);
    }
    else if (msg == "END") 
    {
      digitalWrite(LED, LOW);
      state = IDLE;
    }
    else if (msg == "FORWARD")
    {
      state = DRIVE;
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
}

void drive_state()
{
  digitalWrite(LED, HIGH);
  delay(100);
  digitalWrite(LED, LOW);
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
