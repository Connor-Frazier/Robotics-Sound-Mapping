
#include <MeAuriga.h>

MeRGBLed led(0, 12);

int enable = 0;
int curr = 0;
char temp[16];

void setup() {
    led.setpin(44);
    Serial.begin(9600);
}

void loop() {
    if (Serial.available() > 0) {
        memset(temp, 0, 16);
        Serial.readBytesUntil('\n', temp, 16);
        Serial.print("got: ");
        Serial.println(temp);
    }

    if (strcmp("on", temp) == 0) {
        enable = 1;
    }
    else {
        enable = 0;
    }

    for (int ii = 0; ii < 12; ++ii) {
        if (ii == curr && enable > 0) {
            led.setColorAt(ii, 0, 100, 0);
        }
        else {
            led.setColorAt(ii, 0, 0, 0);
        }
    }
    led.show();

    curr = (curr + 1) % 12;
    delay(100);
}
