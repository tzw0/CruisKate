/*
 * This program operates the esc by bluetooth and gives a simulation by led
 */

//LIBRARYS
#include <SoftwareSerial.h>
#include <Servo.h>

//CONSTANTS
#define VEL_MAX 100
#define RED_LED 2
#define GREEN_LED 3
#define BLUE_LED 4
#define RX 10
#define TX 11
#define ESCPIN 6 //esc's pin
#define PRINT_VELOCITY {\
        Serial.print("velocity = ");\
        Serial.println(skate.velocity);}


//GLOBAL VARIABLES
Servo esc;             // create servo object to control a servo motor
SoftwareSerial BTserial(RX,TX); // RX, TX
int reciv; //command recieved from master
struct skate_board {
        int state[2] = {0,0}; //array[0]-green button state | array[1]-black button state
        int velocity = 0; //actual velocity of brushless motor, whr max velocity is vel_scale
        int special = 0; //a mode to be toggled to change settings
        int accel_time = 50; //the time taken to increase speed by 1 in milliseconds
        int brake_time = 20; //the time taken to decrease speed by 1 in milliseconds
};
struct skate_board skate;//a variable, skate of skate_board data type is declared

//INITIALISATION & SETUP
void setup() {
        BTserial.begin(38400); //bluetooth
        Serial.begin(9600); //serial monitor
        Serial.println("Slave:");
        PRINT_VELOCITY;
        pinMode(RX, INPUT); //rx
        pinMode(TX, OUTPUT); //tx
        pinMode(RED_LED, OUTPUT);
        pinMode(GREEN_LED, OUTPUT);
        pinMode(BLUE_LED, OUTPUT);
        esc.attach(ESCPIN);           // attaches the servo on servopin
        delay(1);                                                                                 //shannon, what is this for?
        esc.writeMicroseconds(400);   // primes the ESC so it is ready to go
}

//MAIN & LOOP
void loop() {
        while (BTserial.available()) {
                reciv = BTserial.read();
                //      Serial.println(reciv);      //prints char sent from master device
                check_command();
        }
        speed_control();
}

void check_command() {
        if (reciv == 'O') { //green button off
                skate.state[0] = 0;
                digitalWrite(RED_LED, LOW);
        }
        else if (reciv == 'L') { //green button on
                skate.state[0] = 1;
                digitalWrite(RED_LED, HIGH);
        }
        else if (reciv == '0') { //red button off
                skate.state[1] = 0;
                digitalWrite(GREEN_LED, LOW);
        }
        else if (reciv == '1') { //red button on
                skate.state[1] = 1;
                digitalWrite(GREEN_LED, HIGH);
        }
        reciv = 0;
}

void speed_control() {
        //4 cases: constant speed- {0,0}, accelerate- {1,0}, brake- {0,1}, special- {1,1}
        if (skate.state[0] == 0 && skate.state[1] == 0) {
                //constant speed
        }
        else if (skate.state[0] == 1 && skate.state[1] == 0 && skate.velocity < VEL_MAX) {
                skate.velocity += 1;//accelerate
                PRINT_VELOCITY
                        delay(skate.accel_time);
        }
        else if (skate.state[0] == 0 && skate.state[1] == 1 && skate.velocity > 0) {
                skate.velocity -= 1;//brake
                PRINT_VELOCITY
                        delay(skate.brake_time);
        }
        else if (skate.state[0] == 1 && skate.state[1] == 1 && skate.special == 0) {
                digitalWrite(BLUE_LED, HIGH);
                delay(500);
                skate.special = 1;
                //special ON
        }
        else if (skate.state[0] == 1 && skate.state[1] == 1 && skate.special == 1) {
                digitalWrite(BLUE_LED, LOW);
                delay(500);
                skate.special = 0;
                //special OFF
        }
        if (skate.velocity < 0) {
                skate.velocity = 0;
        }

        speed_sync();
}

void speed_sync() { //shank
        int val = map(skate.velocity, 0, VEL_MAX, 0, 2000); //scale it to use it with the servo (value between 0 and 2000)
        esc.writeMicroseconds(val); // sets the servo speed according to the scaled value
        Serial.print("ESC at speed: ");
        Serial.println(val);
}
