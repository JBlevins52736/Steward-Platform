/****
Required Arduino hardware packages:
https://adafruit.github.io/arduino-board-index/package_adafruit_index.json
https://espressif.github.io/arduino-esp32/package_esp32_index.json

Required Libraries:
Arduino ESP32Servo.h

****/

// includes
#include <ESP32Servo.h>

// macros
#define NUM_SERVOS 6
#define MIN_SERVO_POS 10
#define MID_SERVO_POS 90
#define MAX_SERVO_POS 170
#define TIMESTAMP_PERIOD 500
#define NUM_PLATFORM_STATES 24

// structs
typedef struct 
{
  uint8_t pin;
  int8_t trim;
  float offset;
  uint8_t last;
  uint8_t next;
} SERVO_DATA;

// globals
bool calibrate = false;
uint32_t timestamp = 0;
int8_t platform_offset = 0;
uint8_t platform_state = 0;

/*****************/
/**** TODO #1 ****/
/*****************/

// create/initialize 6x array of servo objects
Servo servos[NUM_SERVOS];

// create/initialize 6x array of SERVO_DATA (pin=?, trim=0, last=90, next=90)
SERVO_DATA sdat[NUM_SERVOS] = {
  {pin: 14, trim: 7, offset:-23.01, last: MID_SERVO_POS, next: MID_SERVO_POS},
  {pin: 32, trim: -7, offset:23.01, last: MID_SERVO_POS, next: MID_SERVO_POS},
  {pin: 15, trim: 6, offset:-23.01, last: MID_SERVO_POS, next: MID_SERVO_POS},
  {pin: 33, trim: 0, offset:23.01, last: MID_SERVO_POS, next: MID_SERVO_POS},
  {pin: 27, trim: 10, offset:-23.01, last: MID_SERVO_POS, next: MID_SERVO_POS},
  {pin: 12, trim: 3, offset:23.01, last: MID_SERVO_POS, next: MID_SERVO_POS}
};

// create/initialize 24x6 array of DOF platform positions from Solidworks simulation (heave, surge, sway, yaw, pitch, roll)
float platform_positions[NUM_PLATFORM_STATES][NUM_SERVOS] = {
  
   // Initial state (Home position for all servos)
  // {60, 60, 60, 60, 60, 60},
  // {90, 90, 90, 90, 90, 90},
  // {120, 120, 120, 120, 120, 120}

  // Heave positions 
  {25.35, 154.65, 25.35, 154.65, 25.35, 154.65}, // State 1
  {90, 90, 90, 90, 90, 90}, 
  {135.82, 44.18, 135.82, 44.18, 135.82, 44.18}, // State 2
  {90, 90, 90, 90, 90, 90},
  // Surge positions 
   {73.49, 132.98, 125.68, 83.79, 96.21, 22.11}, // State 3
   {90, 90, 90, 90, 90, 90}, 
   {137.74, 42.26, 22.17, 62.49, 117.51, 125.68}, // State 4
   {90, 90, 90, 90, 90, 90}, 

  // Sway positions
  {64.21, 45.35, 85.14, 121.15, 150.44, 108.64},  // State 5
  {90, 90, 90, 90, 90, 90}, 
  {135.65, 115.79, 108.64, 29.56, 58.85, 85.14},    // State 6
  {90, 90, 90, 90, 90, 90}, 

  // Yaw positions 
  {115.79, 121.14, 115.79, 121.14, 115.79, 121.14},   // State 7
  {90, 90, 90, 90, 90, 90}, 
  {27.73, 54.98, 27.73, 54.98, 27.73, 54.98}, // State 8
  {90, 90, 90, 90, 90, 90}, 
  

  // Pitch positions
  {58.68, 121.32, 70.94, 24.43, 152.04, 109.06}, // State 9
  {90, 90, 90, 90, 90, 90}, 
  {118.09, 61.91, 104.18, 136.99, 43.12, 75.82},  // State 10
  {90, 90, 90, 90, 90, 90}, 

  // Roll positions 
  {124.79, 128.5, 45.19, 95.94, 97.36, 42.57},    // State 11
  {90, 90, 90, 90, 90, 90}, 
  {51.38, 55.11, 137.61, 82.62, 84.05, 134.96},   // State 12
  {90, 90, 90, 90, 90, 90}  
};
// process KVP (KEY=VAL)
String procKVP(String skvp)
{
  String sret;
	bool rv = true;

	if (skvp.length())
	{
    skvp.toUpperCase();

		int16_t z = skvp.indexOf("=");
    if (z == -1) z = skvp.length();
		String skey = skvp.substring(0, z);
		String sval = skvp.substring(z + 1);
		uint32_t ival = sval.toInt();
		float fval = sval.toFloat();

		//****************************//
		//***** sval length == 0 *****//
		//****************************//
		if (sval.length() == 0)
		{
      // set calibration mode
			if (skey == "C") sval = String(calibrate);

      // print trim all servos
      else if (skey == "T")
      {
        for (uint8_t i = 0; i < NUM_SERVOS; i++)
        {
          sval += " ";
          sval += String(sdat[i].trim);
        }
      }

      // print trim specific servo
      else if (skey[0] == 'T')
      {
        int8_t i = skey[1] - 48;
        if ((i  < 0) || (i >= NUM_SERVOS)) rv = false;
        else sval = String(sdat[i].trim);
      }

      // print offset all servos
      else if (skey == "O") sval  = String(platform_offset);

      // else error
			else rv = false;
		}

		//*********************************//
		//***** else sval length != 0 *****//
		//*********************************//

    // set calibration state
    else if (skey == "C") calibrate = ival;

    // set trim all servos
    else if (skey == "T") {for (uint8_t i = 0; i < NUM_SERVOS; i++) sdat[i].trim = ival;}

    // set trim specific servo
    else if (skey[0] == 'T')
    {
      int8_t i = skey[1] - 48;
      if ((i  < 0) || (i >= NUM_SERVOS)) rv = false;
      else sdat[i].trim = ival;
    }

    // set offset all servos
    else if (skey == "O") platform_offset = ival;

    // else error
		else rv = false;

    // if command OK
		if (rv == true)
		{
			if (sval.length()) sret = skey + "=" + sval + " OK";
			else sret = skey + " OK";
		}

    // else command ER
    else sret = skvp + " ER";

    Serial.println(sret);
	}

	return(sret);
}

// read KVP
String readKVP(void)
{
	bool rv = false;
	static uint8_t x = 0;
	static String sstr = "";

	if (x == 0) sstr = "";

	while (Serial.available())
	{
		char c = (char)Serial.read();
		if ((c == '\r') || (c == '\n'))
		{
			x = 0;
			rv = true;
			break;
		}

		x++;
		sstr += c;
	}

	return(rv ? sstr : String(""));
}

void setup()
{
  
  /*****************/
  /**** TODO #2 ****/
  /*****************/
  
  // initialize serial object
  Serial.begin(115200);
  
  // wait for terminal to open
  while(!Serial || (millis() <= 5000));
 

  // Attach each servo to its corresponding pin and set to MID_SERVO_POS (90 degrees)
  for (int i = 0; i < NUM_SERVOS; i++) {
    servos[i].attach(sdat[i].pin);      
    servos[i].write(MID_SERVO_POS+sdat[i].trim  + sdat[i].offset);     

  Serial.println("setup() complete");
}

void loop()
{
  // capture timenow in milliseconds
    uint32_t timenow = millis();
  // process commands
  procKVP(readKVP());
  
  /*****************/
  /**** TODO #3 ****/
  /*****************/

  // if calibrating servo trim
  if (calibrate) 
  {
	   for (int i = 0; i < NUM_SERVOS; i++) {
      // Set servo position to 90 degrees plus its trim value, constrained to the defined limits
      float pos = constrain(MID_SERVO_POS + sdat[i].trim - sdat[i].offset , MIN_SERVO_POS, MAX_SERVO_POS);
      servos[i].write(pos);
    }
    return;
  }

  /*****************/
  /**** TODO #5 ****/
  /*****************/

  // calculate ratio of timenow to timestamp relative 500ms period
  // for all servos
    // calculate lerped position between last and next using ratio
    // write servo to lerped position, use contrain() to limit range.
  float ratio = float(timenow - timestamp) / TIMESTAMP_PERIOD;


  // Print the interpolation ratio for debugging
  Serial.print("Interpolation ratio: ");
  Serial.println(ratio);

  for (int i = 0; i < NUM_SERVOS; i++) {
    float pos = sdat[i].last + ratio * (sdat[i].next - sdat[i].last);
    pos = constrain(pos, MIN_SERVO_POS, MAX_SERVO_POS);
    servos[i].write(pos);

  // Debugging information for interpolated position
    Serial.print("Servo ");
    Serial.print(i);
    Serial.print(" position: ");
    Serial.println(pos - sdat[i].trim);
  }
  /*****************/
  /**** TODO #4 ****/
  /*****************/

  // create timestamp that triggers every 500ms
	// reset timestamp to timenow
	// set next platform state (0..23..etc)
	// for all servos
	  // save servo next position to servo last position
	  // set servo next position to platform position for state, plus servo trim value
// Calculate the ratio for interpolation over the 500 ms period
  
  if (timenow - timestamp >= TIMESTAMP_PERIOD) {
   
    timestamp = timenow;

    
    platform_state = (platform_state + 1) % NUM_PLATFORM_STATES;

    for (int i = 0; i < NUM_SERVOS; i++) {
      
      sdat[i].last = sdat[i].next;

      sdat[i].next = platform_positions[platform_state][i] + sdat[i].trim - sdat[i].offset ;
    }
  }


  
}
