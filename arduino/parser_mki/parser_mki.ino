#define LED 13

String msg = "";

void setup() {
  Serial.begin(115200);
  while(!Serial) {}

  pinMode(LED, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    msg = Serial.readStringUntil('\n');
    Serial.println("Received message: " + msg);

    if (msg == "LOCATE") {
      digitalWrite(LED, HIGH);
    }
    else if (msg == "END") {
      digitalWrite(LED, LOW);
    }
    else 
    {
      String directiva = msg.substring(0,3);
      if(directiva=="WAIT"){
        String parametro_letra = msg.substring(5,7);
        int parametro_num =  parametro_letra.toInt();

        if(parametro_num!=0){
          delay(parametro_num);
        }
        else{
          Serial.println("Received wait directive: " + parametro_letra);
        }
      }
    }
  }
}
