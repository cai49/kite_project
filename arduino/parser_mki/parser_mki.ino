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

String msg = "";

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
    }
    else if (msg == "FORWARD")
    {
      digitalWrite(LED, HIGH);
      delay(100);
      digitalWrite(LED, LOW);
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
        }
        else 
        {
          debugln("Turning Left!");
        }
      }
    }

    Serial.println("Processed message: " + msg);
    delay(100);
  }
}
