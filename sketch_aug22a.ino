    #define pwma_pin 6
    #define dira_pin 7
    #define pwmb_pin 5
    #define dirb_pin 4
    #define led_pin 13

    void setup() {
      pinMode(dira_pin, OUTPUT);
      pinMode(pwma_pin, OUTPUT);
      pinMode(dirb_pin, OUTPUT);
      pinMode(pwmb_pin, OUTPUT);
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
      analogWrite(pwma_pin, 64); // Send PWM signal to L298N Enable pin
      analogWrite(pwmb_pin, 128); // Send PWM signal to L298N Enable pin

      digitalWrite(dira_pin, LOW);
      digitalWrite(dirb_pin, LOW);
      digitalWrite(led_pin, LOW);
      delay(1000);
      digitalWrite(dira_pin, HIGH);
      digitalWrite(dirb_pin, HIGH);
      digitalWrite(led_pin, HIGH);
      delay(2000);

    }
    
