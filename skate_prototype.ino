#include <Servo.h>

//Constants
#define MOTOR_PIN 9
#define NUM_GEAR 5
#define UP '1'
#define DOWN '0'
#define BRAKE '2'
#define STOP '3'

//Global variables
Servo esc1;
struct move {
        int cmd = 0; //movement command
        int gear = 0; //which speed category
        int rpm = 0; //actual speed
};
struct move control;

void move_() {
        static int speed_limit = 0;
        while (control.rpm <= speed_limit) {
                control.rpm += 1; //increases speed until max speed of gear is reached
                delay(20);
        }

        while (control.rpm > speed_limit) {
                control.rpm -= 1; //reduces speed until within range
        }

        for (int gear_state = 0; gear_state <= NUM_GEAR; gear_state += 1) {
                if (control.gear == gear_state) {
                        speed_limit = control.gear * (90 / NUM_GEAR); //speed_limit is out of 90
                }
        }

        if (control.cmd == DOWN && control.gear > 0) {
                control.gear -= 1;
        }
        else if (control.cmd == UP && control.gear < NUM_GEAR) {
                control.gear += 1;
        }
        else if (control.cmd == BRAKE && control.rpm > 0) {
                control.gear -= 1;
                control.rpm -= 5;
        }
        else if (control.cmd == STOP) {
                control.rpm = 0;
                control.gear = 0;
        }
        if (control.cmd != 0) {
                control.cmd  = 0;
        }
}

void sync() {
        esc1.write(90 - control.rpm);
}

void setup(){
        esc1.attach(MOTOR_PIN);
        Serial.begin(9600); // Default communication rate of the Bluetooth module
}

void loop() {
        if(Serial.available() > 0){ // Checks whether data is comming from the serial port
                control.cmd = Serial.read(); // Reads the data from the serial port
        }
        move_();
        sync();
        Serial.println(control.cmd);
        Serial.println(control.gear);
        Serial.println(control.rpm);
}
