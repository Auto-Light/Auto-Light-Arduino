/*
  433 MHz RF REMOTE REPLAY sketch
     Written by ScottC 24 Jul 2014
     Arduino IDE version 1.0.5
     Website: http://arduinobasics.blogspot.com
     Receiver: XY-MK-5V      Transmitter: FS1000A/XY-FST
     Description: Use Arduino to receive and transmit RF Remote signal          
 ------------------------------------------------------------- */
#include <Arduino.h>
#define rfTransmitPin 4  //RF Transmitter pin = digital pin 4
 
 const int codeSize = 35;      //The size of the code to transmit
 int codeToTransmit[codeSize]; //The array used to hold the RF code
 int light_up[]=    {9,5,5,6,5,5,5,5,5,5,5,6,6,6,6,6,5,5,5,5,6,6,5,5,5,5,5,5,5,5,6,6,5,8,3}; //The RF code that will turn the light ON
 //int light_down[]=   {5,2,1,5,1,5,2,2,2,2,2,1,4,5,2,2,2,2,2,2,2,2,2,1,6};
 //The RF code that will turn the light OFF
 int timeDelay=10;      // The variable used to calibrate the RF signal lengths.
 

  /*-----------------------------------------------------------------
    transmitCode(): Used to transmit the signal to the RF receiver on
                    the fan/light. There are 6 different HIGH-LOW signal combinations.
                   
                    SH = short high   or  LH = long high  
                                     PLUS
         SL = short low    or    LL = long low    or    VLL = very long low
                   
  -------------------------------------------------------------------*/
   void transmitCode(){
   //initialise the variables
    int highLength = 0;
    int lowLength = 0;
   
    //The signal is transmitted 6 times in succession - this may vary with your remote.      
    for(int j = 0; j<7; j++){
      for(int i = 0; i<codeSize; i++){
        Serial.println(codeToTransmit[i]);
        switch(codeToTransmit[i]){
                case 3: // SL + VLH
                  highLength=0;
                  lowLength=178;
                break;
                case 5: // LL + SH
                  highLength=5;
                  lowLength=4;
                break;
                case 6: // LL + LH
                  highLength=5;
                  lowLength=10;
                break;
                case 8: // LL + VVLH
                  highLength=5;
                  lowLength=255;
                break;
                case 9: // VLL + SH
                  highLength=9;
                  lowLength=4;
                break;
        }
           
         /* Transmit a HIGH signal - the duration of transmission will be determined
            by the highLength and timeDelay variables */
         digitalWrite(rfTransmitPin, HIGH);    
         delayMicroseconds(highLength*timeDelay);
         
         /* Transmit a LOW signal - the duration of transmission will be determined
            by the lowLength and timeDelay variables */
         digitalWrite(rfTransmitPin,LOW);    
         delayMicroseconds(lowLength*timeDelay);  
      }
    }
 }
 void setup(){
   Serial.begin(9600);        // Turn the Serial Protocol ON
   pinMode(rfTransmitPin, OUTPUT);   //Transmit pin is an output          
 }
 
  void loop(){
    transmitCode();  // transmit the code to RF receiver on the Fan/Light
   
    timeDelay+=10;    //Increment the timeDelay by 10 microseconds with every transmission
    Serial.println(timeDelay);
    delay(1000);      //Each transmission will be about 2 seconds apart.
  }