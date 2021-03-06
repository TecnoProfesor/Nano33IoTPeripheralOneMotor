#include <ArduinoBLE.h>

BLEService ledService("FFE0"); // Servicio

BLEByteCharacteristic switchCharacteristic("FFE1", BLERead | BLEWrite | BLENotify);

const int BLE_LED_PIN = LED_BUILTIN;

void setup() {
  Serial.begin(9600);
  // Leds
  pinMode(2,OUTPUT);
  pinMode(4,OUTPUT);  
  // L293D
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);  
  
  pinMode(BLE_LED_PIN,OUTPUT);
  digitalWrite( BLE_LED_PIN, LOW );

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("Failed");

    while (1);
  }
  //************************************************************************
  //************************************************************************
  
  BLE.setLocalName("IESVALLESOL1");
  
  //************************************************************************
  //************************************************************************
  
  BLE.setAdvertisedService(ledService);
  ledService.addCharacteristic(switchCharacteristic);
  BLE.addService(ledService);
  BLE.advertise();

  Serial.println("BLE Periférico");
}

void loop() {
  BLEDevice central = BLE.central();
  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());
    byte antvalue = (byte)0x00;
    while (central.connected()) {
      digitalWrite( BLE_LED_PIN, HIGH );
      
      byte perValue = (byte)0x00;
      switchCharacteristic.readValue(perValue);
      if (antvalue == perValue) {perValue = -1;}
      else {antvalue = perValue;}
      switch (perValue) {
        case 1: //Go backwards
          // Leds
          digitalWrite(2,HIGH);
          digitalWrite(4,LOW);
          
          digitalWrite(7,HIGH);
          digitalWrite(8,HIGH);
          digitalWrite(9,LOW);
          break;
        case 2: //Go forward
          // Leds
          digitalWrite(2,LOW);
          digitalWrite(4,HIGH);
                    
          digitalWrite(7,HIGH);          
          digitalWrite(8,LOW);
          digitalWrite(9,HIGH);
          break;
        case 3: //Stop robot
          // Leds
          digitalWrite(2,LOW);
          digitalWrite(4,LOW);
          
          // L293D
          // Stop both motors
          digitalWrite(7,LOW);
          digitalWrite(8,LOW);
          digitalWrite(9,LOW);
          break;
        case 4: // Left movement
          break;
        case 5: // Right movement
          break;
        case 6: // Async movement
          break;
      }     
      delay(500);
    }
    digitalWrite(2,LOW);
    digitalWrite(4,LOW);
   
    digitalWrite(7,LOW);
    digitalWrite(8,LOW);
    digitalWrite(9,LOW);
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
    digitalWrite( BLE_LED_PIN, LOW );
  }
}
