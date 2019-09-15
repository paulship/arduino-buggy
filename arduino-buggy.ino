/*
 * Here we set the pins that control the gate drives in the H bridge.
 */
#define right_pwm_pin 6
#define right_dir_pin 7
#define left_pwm_pin 5
#define left_dir_pin 4

/*
 * This defines the rate at which the power is ramped on and off the motor. The ramp time is in milliseconds,
 * and the step is out of a maximum of 255. In this example, we ramp 5/255% every 10ms. In other words, we
 * go from 0% to 100% in 0.51 seconds.
 */
#define RAMP_STEP 5
#define RAMP_TIME 10

/*
 * This is used to set which drive modes are available in the control code.
 */
enum drivemode_t {
  DRIVE_FWD,
  DRIVE_REV,
  TURN_LEFT,
  TURN_RIGHT
};

/*
 * This sets a drive instruction. It includes a drive mode, the power out of 255 to apply, and the length
 * of time to apply this for.
 */
typedef struct
{
  drivemode_t drivemode;
  uint8_t     pwm;
  uint16_t    duration;
} instruction_t;

/*
 * This is the array of instructions. The main loop code will go through each of these, driving the buggy,
 * before finally coming to a stop.
 */
static const instruction_t instructions[] = 
{
  { DRIVE_FWD,  100,  5000 },
  { TURN_LEFT,  100,  2000 },
  { DRIVE_FWD,  200,  5000 },
  { TURN_RIGHT, 100,  3000 },
  { DRIVE_REV,  200,  2000 }
};

/*
 * The setup function runs first. It simply sets the selected pins as outputs.
 */
void setup() {
  pinMode(right_dir_pin, OUTPUT);
  pinMode(right_pwm_pin, OUTPUT);
  pinMode(left_dir_pin, OUTPUT);
  pinMode(left_pwm_pin, OUTPUT);
}

/*
 * This is the main loop code. It runs through each of the instructions in the array above
 * in turn, before finally stopping the buggy.
 */
void loop()
{
  /*
   * First we work out the size of the array. We use a cunning trick here, to take the amount of memory used by the
   * entire instruction array, and then divide by the amount of memory used by a single instruction in the array, to
   * give us the number of instructions.
   */
  static const uint8_t n_instructions = sizeof(instructions) / sizeof(instructions[0]);

  /*
   * This is the start of the loop running through each instruction in the array.
   */
  for( uint8_t idx=0; idx<n_instructions; idx++ )
  {
    /*
     * For each instruction, we set the drive mode, for the requried length of time.
     */
    for( uint16_t t=0; t<instructions[idx].duration; t+=RAMP_TIME )
    {
      set_drivemode( instructions[idx].drivemode, instructions[idx].pwm );
      delay( RAMP_TIME );
    }
  }

  /*
   * Once we've gone through the array, we set drive mode to stop (i.e. drive forward but with 0 power).
   */
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

  return actual;
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
