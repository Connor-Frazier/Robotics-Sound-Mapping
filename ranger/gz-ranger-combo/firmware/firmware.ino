
// ref: makeblock examples/.../Me_Auriga_encoder_pid_speed

#include <MeAuriga.h>

MeEncoderOnBoard E1(SLOT1);
MeEncoderOnBoard E2(SLOT2);

MeUltrasonicSensor Sonar(PORT_8);

char command[64];
char cmd_idx = 0;

void
isr_e1()
{
    if(digitalRead(E1.getPortB()) == 0) {
        E1.pulsePosMinus();
    }
    else {
        E1.pulsePosPlus();;
    }
}

void
isr_e2()
{
    if(digitalRead(E2.getPortB()) == 0) {
        E2.pulsePosMinus();
    }
    else {
        E2.pulsePosPlus();;
    }
}

void
setup_pwm()
{
    // "Set PWM 8KHz", from the encoder PWM example
    TCCR1A = _BV(WGM10);
    TCCR1B = _BV(CS11) | _BV(WGM12);
    TCCR2A = _BV(WGM21) | _BV(WGM20);
    TCCR2B = _BV(CS21);
}

void
setup_enc(MeEncoderOnBoard* enc, void (*isr)())
{
    attachInterrupt(enc->getIntNum(), isr, RISING);
}

void
set_speed()
{
    int spL, spR;
    sscanf(command, "%d %d", &spL, &spR);

    E1.setMotorPwm(-spL);
    E2.setMotorPwm(+spR);
}

void
poll_serial()
{
    while (Serial.available()) {
        char cc = Serial.read();
        if (cc == '\n') {
            command[cmd_idx] = 0;
            set_speed();
            command[0] = 0;
            cmd_idx = 0;
            return;
        }
        else {
            command[cmd_idx++] = cc;
        }
    }
}

void
setup()
{
    setup_pwm();
    setup_enc(&E1, isr_e1);
    setup_enc(&E2, isr_e2);
    Serial.begin(9600);
}

void
loop()
{
    poll_serial();
    Serial.println(Sonar.distanceCm());

    E1.updateSpeed();
    E2.updateSpeed();
}
