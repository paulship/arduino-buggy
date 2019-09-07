#define right_pwm_pin 6
#define right_dir_pin 7
#define left_pwm_pin 5
#define left_dir_pin 4
#define led_pin 13

#define RAMP_STEP 5

enum drivemode_t {
  DRIVE_FWD,
  DRIVE_REV,
  TURN_LEFT,
  TURN_RIGHT
};

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


void set_drivemode( const drivemode_t desired_mode, const uint8_t desired_pwm )
{
  static drivemode_t actual_mode = DRIVE_FWD;
  static uint8_t actual_pwm = 0;

  if( desired_mode != actual_mode )
  {
    if( actual_pwm == 0 )
    {
      actual_mode = desired_mode;
    }
    else
    {
      actual_pwm = ramp_to_pwm( actual_pwm, 0 );
    }
    
  }
  else
  {
    actual_pwm = ramp_to_pwm( actual_pwm, desired_pwm );
  }
}


uint8_t ramp_to_pwm( uint8_t actual, uint8_t target )
{
  if( target < actual )
  {
    if( ( actual - target ) < RAMP_STEP )
    {
      actual = target;
    }
    else
    {
      actual -= RAMP_STEP;
    }
  }
  else
  {
    if( ( target - actual ) < RAMP_STEP )
    {
      actual = target;
    }
    else
    {
      actual += RAMP_STEP;
    }
  }
}
