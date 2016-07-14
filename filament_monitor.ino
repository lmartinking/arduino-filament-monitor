//
// Configuration
//

enum {
  SHUT_DOWN_THRESHOLD = 95,  // If the percentage difference in speed of the two encoders is more than this. SHUT DOWN!
  CHECK_INTERVAL = 4000,     // Check state every N milliseconds

  ENC_1_PIN = 3,  // Pin for first optical encoder  (attached to roller/feeder)
  ENC_2_PIN = 2,  // Pin for second optical encoder (attached to stepper motor)

  ENC_1_INTR = 0, // Interrupt number for ENC_1_PIN
  ENC_2_INTR = 1, // Interrupt number for ENC_2_PIN

  CTRL_PIN = 4,              // Pin for control/alarm
  CTRL_PIN_INACTIVE = HIGH,  // Initial state
  CTRL_PIN_ACTIVE = LOW,     // Active state (for triggering external shut down, eg: via solenoid)

  CTRL_PIN_TWIDDLE = 0,         // Set to 1 if you wish the control pin to be set to inactive before setting active
  CTRL_PIN_TWIDDLE_DELAY = 100, // Delay after being set inactive and then triggering active (N milliseconds)
};

//
// Global variables
//

static volatile int enc1_counter = 0;
static volatile int enc2_counter = 0;

//
// Program setup
//

void setup() {
  pinMode(ENC_1_PIN, INPUT_PULLUP);
  attachInterrupt(ENC_1_INTR, interrupt_enc1, CHANGE);

  pinMode(ENC_2_PIN, INPUT_PULLUP);
  attachInterrupt(ENC_2_INTR, interrupt_enc2, CHANGE);

  pinMode(CTRL_PIN, OUTPUT);
  digitalWrite(CTRL_PIN, CTRL_PIN_INACTIVE);

  Serial.begin(9600); // Uncomment for testing purposes, Arduino monitor able to monitor activations.
  Serial.println("SETUP");

  enc1_counter = 0;
  enc2_counter = 0;
}

//
// Main
//

void loop() 
{
  static int enc1_last = 0;
  static int enc2_last = 0;

  // Make local copies of the counters as interrupts could keep occuring during loop()
  int enc1 = enc1_counter;
  int enc2 = enc2_counter;

  int delta_1 = enc1 - enc1_last;
  int delta_2 = enc2 - enc2_last;

  Serial.println("LOOP");
  Serial.println(enc1);
  Serial.println(delta_1);

  Serial.println(enc2);
  Serial.println(delta_2);

  enc1_last = enc1;
  enc2_last = enc2;

  boolean shut_down = false;

  if (delta_2)
  {
    int diff = delta_2 - delta_1;
    int pct_diff =  (int)(((float)diff / (float)delta_2) * 100.0f);

    Serial.println("diff");
    Serial.println(diff);
    Serial.println("pct diff");
    Serial.println(pct_diff);
 
    if (pct_diff >= SHUT_DOWN_THRESHOLD)
    {
      shut_down = true; 
    }
  }

  if (shut_down)
  {
    Serial.println("SHUT DOWN!");

    if (CTRL_PIN_TWIDDLE)
    {
      digitalWrite(CTRL_PIN, CTRL_PIN_INACTIVE);
      delay(CTRL_PIN_TWIDDLE_DELAY);
    }

    digitalWrite(CTRL_PIN, CTRL_PIN_ACTIVE);

    enc1_counter = 0;
    enc2_counter = 0;
    enc1_last = 0;
    enc2_last = 0;

    shut_down = false;
  }

  Serial.println();

  delay(CHECK_INTERVAL); 
}

//
// Interrupt Handlers
//

void interrupt_enc1()
{
  enc1_counter++;
}

void interrupt_enc2()
{
  enc2_counter++;
}

