const int ACPin = A2;
#define ACTectionRange 20;
#define VREF 5
#define bitValue 1024

float readACCurrentValue(){
  float ACCurrentValue = 0;
  float peakVoltage = 0;
  float voltageVirtualValue = 0;
  for (int i = 0; i < 5; i++){
    peakVoltage += analogRead(ACPin);
    delay(1);
  }
  peakVoltage = peakVoltage / 5;
  voltageVirtualValue = peakVoltage * 0.707;

  voltageVirtualValue = (voltageVirtualValue / bitValue * VREF) / 2;

  ACCurrentValue = voltageVirtualValue * ACTectionRange;

  return ACCurrentValue;
}

void setup() {
  Serial.begin(115200);
  pinMode(13, OUTPUT);
}

void loop() {
  float ACCurrentValue = readACCurrentValue();
  Serial.print(ACCurrentValue);
  Serial.println(" A");
  digitalWrite(13, HIGH);
  delay(500);
  digitalWrite(13, LOW);
  delay(500);
}
