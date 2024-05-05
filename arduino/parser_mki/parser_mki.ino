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
  }
}
