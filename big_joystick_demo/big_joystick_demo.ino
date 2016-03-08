/* Arduino USB Joystick HID demo */

/* Author: Darran Hunt
 * Released into the public domain.
 */

#undef DEBUG

#define NUM_BUTTONS	40
#define NUM_AXES	8	       // 8 axes, X, Y, Z, etc

typedef struct joyReport_t {
    int16_t axis[NUM_AXES];
    uint8_t button[(NUM_BUTTONS+7)/8]; // 8 buttons per byte
} joyReport_t;

joyReport_t joyReport;


void setup(void);
void loop(void);
void setButton(joyReport_t *joy, uint8_t button);
void clearButton(joyReport_t *joy, uint8_t button);
void sendJoyReport(joyReport_t *report);


void setup() 
{
    Serial.begin(115200);
    delay(200);

    for (uint8_t ind=0; ind<8; ind++) {
	joyReport.axis[ind] = 0;
    }
    for (uint8_t ind=0; ind<sizeof(joyReport.button); ind++) {
        joyReport.button[ind] = 0;
    }
    
    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    pinMode(A2, INPUT);
    pinMode(A3, INPUT);
    pinMode(A4, INPUT);
    pinMode(A5, INPUT);
}

void loop() 
{
//    setButton(&joyReport, 1);
    
    joyReport.axis[0] = (analogRead(A0)-511)*64;
    joyReport.axis[1] = (analogRead(A1)-511)*64;
    joyReport.axis[2] = (analogRead(A2)-511)*64;
    joyReport.axis[3] = (analogRead(A3)-511)*64;
    joyReport.axis[4] = (analogRead(A4)-511)*64;
    joyReport.axis[5] = (analogRead(A5)-511)*64;

    sendJoyReport(&joyReport);

    delay(10);
}

// Send an HID report to the USB interface
void sendJoyReport(struct joyReport_t *report)
{
#ifndef DEBUG
    Serial.write((uint8_t *)report, sizeof(joyReport_t));
#else
    // dump human readable output for debugging
    for (uint8_t ind=0; ind<NUM_AXES; ind++) {
	Serial.print("axis[");
	Serial.print(ind);
	Serial.print("]= ");
	Serial.print(report->axis[ind]);
	Serial.print(" ");
    }
    Serial.println();
    for (uint8_t ind=0; ind<NUM_BUTTONS/8; ind++) {
	Serial.print("button[");
	Serial.print(ind);
	Serial.print("]= ");
	Serial.print(report->button[ind], HEX);
	Serial.print(" ");
    }
    Serial.println();
#endif
}

// turn a button on
void setButton(joyReport_t *joy, uint8_t button)
{
    uint8_t index = button/8;
    uint8_t bit = button - 8*index;

    joy->button[index] |= 1 << bit;
}

// turn a button off
void clearButton(joyReport_t *joy, uint8_t button)
{
    uint8_t index = button/8;
    uint8_t bit = button - 8*index;

    joy->button[index] &= ~(1 << bit);
}
