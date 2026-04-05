#include <Arduino.h>
#include "HCSR04.h"
#include "SSI3430-01A.h"

HCSR04 US1(25,26);
HCSR04 US2(27,14);
HCSR04 US3(36,39);
SSI3430_01A valve1 (13);
SSI3430_01A valve2 (2);

void setup() {
  Serial.begin(115200);
}

void loop() {
  long Dtank = US1.getDistance(100);
  long Dazolla_trad = US2.getDistance(50);
  long Dazolla_int = US3.getDistance(50);
  Serial.print(Dtank);
  Serial.print(Dazolla_trad);
  Serial.println(Dazolla_int);
  valve1.control(Dazolla_trad, "Traditional Azolla");
  valve2.control(Dazolla_int, "Innovative Azolla");
  delay(1000);
}

