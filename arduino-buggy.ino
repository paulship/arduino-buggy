#define right_pwm_pin 6
#define right_dir_pin 7
#define left_pwm_pin 5
#define left_dir_pin 4
#define led_pin 13

#define RAMP_STEP 5
#define RAMP_TIME 10

enum drivemode_t {
  DRIVE_FWD,
  DRIVE_REV,
  TURN_LEFT,
  TURN_RIGHT
};

typedef struct
{
  drivemode_t drivemode;
  uint8_t     pwm;
  uint16_t    duration;
} instruction_t;

static const instruction_t instructions[] = 
{
  { DRIVE_FWD,  100,  5000 },
  { TURN_LEFT,  100,  2000 },
  { DRIVE_FWD,  200,  5000 },
  { TURN_RIGHT, 100,  3000 },
  { DRIVE_REV,  200,  2000 }
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
void loop()
{
  static const uint8_t n_instructions = sizeof(instructions) / sizeof(instructions[0]);

  for( uint8_t idx=0; idx<n_instructions; idx++ )
  {
    for( uint16_t t=0; t<instructions[idx].duration; t+=RAMP_TIME )
    {
      set_drivemode( instructions[idx].drivemode, instructions[idx].pwm );
      delay( RAMP_TIME );
    }
  }

  while( true )
  {
    set_drivemode( DRIVE_FWD, 0 );
    delay( RAMP_TIME );
  }
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

  set_output( actual_mode, actual_pwm );
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

void set_output( const drivemode_t drivemode, const uint8_t pwm )
{
  switch( drivemode )
  {
    case DRIVE_FWD:
    {
      analogWrite(left_pwm_pin, pwm);
      digitalWrite(left_dir_pin, LOW);
      analogWrite(right_pwm_pin, pwm);
      digitalWrite(right_dir_pin, LOW);
      break;
    }
    case DRIVE_REV:
    {
      analogWrite(left_pwm_pin, 255-pwm);
      digitalWrite(left_dir_pin, HIGH);
      analogWrite(right_pwm_pin, 255-pwm);
      digitalWrite(right_dir_pin, HIGH);
      break;
    }
    case TURN_LEFT:
    {
      analogWrite(left_pwm_pin, 255-pwm);
      digitalWrite(left_dir_pin, HIGH);
      analogWrite(right_pwm_pin, pwm);
      digitalWrite(right_dir_pin, LOW);
      break;
    }
    case TURN_RIGHT:
    {
      analogWrite(left_pwm_pin, pwm);
      digitalWrite(left_dir_pin, LOW);
      analogWrite(right_pwm_pin, 255-pwm);
      digitalWrite(right_dir_pin, HIGH);
      break;
    }
  }
}
