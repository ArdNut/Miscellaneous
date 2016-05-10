
// Define the digital I/O pins to use
#define LOOP1  2
#define LOOP2  3
#define ARM     6
#define RESET   7
#define ALERT 12
#define LED     13

#define SLEEPTM 250

#define SLOWFLASH  10
#define FASTFLASH    2

bool arm_state;  // T = armed, F = unarmed
bool alarmed;    // T = in alarm state, F = all quiet
bool led_state;   // control on-board LED

bool loop1state;  // status of sense loop 1
bool loop2state;  // status of sense loop 2
bool in_alarm;     // T = alarm active, F = quiet

int led_cnt;        // flash cycle counter
int flash_rate;    // count divisor

// We will use Arduino's built-in pinMode() function to set the input or output
// behavior of the discrete digital I/O pins
void setup()
{
     // Initialize pins 2, 3, 6, and 7 as digital inputs
     pinMode(LOOP1, INPUT);
     pinMode(LOOP2, INPUT);
     pinMode(ARM, INPUT);
     pinMode(RESET, INPUT);

     // Initialize pins 12 and 13 as a digital outputs
     pinMode(ALERT, OUTPUT);
     pinMode(LED, OUTPUT);
     
     arm_state = false;
     alarmed   = false;
     led_state = false;
     
     loop1state = true;
     loop2state = true;
     in_alarm = false;
     
     led_cnt = 0;
     flash_rate = SLOWFLASH;
     digitalWrite(LED, LOW);
}


void loop()
{
    // Get arm switch value
  arm_state = digitalRead(ARM);
  if (!digitalRead(RESET)) {
    in_alarm = false;    
  }
  
   // If not armed, just loop with a short pause  
  if (!arm_state) {
      flash_rate =   SLOWFLASH;
      in_alarm = false;
  }
  else {
      flash_rate =   FASTFLASH;
  }
  
  // Check the sense loops
  loop1state = digitalRead(LOOP1);
  loop2state = digitalRead(LOOP2);
  if (arm_state) {
    if ((loop1state) || (loop2state)) {
      in_alarm = true;
    }
  }

  if (in_alarm) {
    digitalWrite(ALERT, HIGH);
  }
  else {
      digitalWrite(ALERT, LOW);
  }
  
  led_cnt++;    
  if (!(led_cnt % flash_rate)) {
      led_cnt = 0;
      led_state = !led_state;
      digitalWrite(LED, led_state);
  }
    
  delay(SLEEPTM);  
}
