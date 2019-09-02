#define right_pwm_pin 6
#define right_dir_pin 7
#define left_pwm_pin 5
#define left_dir_pin 4
#define led_pin 13

void setup() {
    pinMode(right_dir_pin, OUTPUT);
    pinMode(right_pwm_pin, OUTPUT);
    pinMode(left_dir_pin, OUTPUT);
    pinMode(left_pwm_pin, OUTPUT);
    pinMode(led_pin, OUTPUT);
}


// With the wiring I have at the moment, this makes the motors:
// For 1 second:
//   Left motor fwd at 50%
//   Right motor fwd at 25%
// For 2 seconds:
//   Left motor rev at 50%
//   Right motor rev at 75%
void loop() {
    analogWrite(right_pwm_pin, 64); // Send PWM signal to L298N Enable pin
    analogWrite(left_pwm_pin, 128); // Send PWM signal to L298N Enable pin

    digitalWrite(right_dir_pin, LOW);
    digitalWrite(left_dir_pin, LOW);
    digitalWrite(led_pin, LOW);
    delay(1000);
    digitalWrite(right_dir_pin, HIGH);
    digitalWrite(left_dir_pin, HIGH);
    digitalWrite(led_pin, HIGH);
    delay(2000);
}



