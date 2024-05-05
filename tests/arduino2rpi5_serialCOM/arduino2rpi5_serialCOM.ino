#define LED 13 

void setup() 
{
  Serial.begin(115200);
  while(!Serial) {}
}

void loop() 
{
  Serial.println("Hello, World!");
  delay(500);
}
