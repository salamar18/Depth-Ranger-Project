/*********************************************************************
 This is an example for our nRF51822 based Bluefruit LE modules

 Pick one up today in the adafruit shop!

 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!

 MIT license, check LICENSE for more information
 All text above, and the splash screen below must be included in
 any redistribution
*********************************************************************/

#include <Arduino.h>
#include <SPI.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"


#include "BluefruitConfig.h"

#if SOFTWARE_SERIAL_AVAILABLE
  #include <SoftwareSerial.h>
#endif

/*=========================================================================
    APPLICATION SETTINGS

    FACTORYRESET_ENABLE       Perform a factory reset when running this sketch
   
                              Enabling this will put your Bluefruit LE module
                              in a 'known good' state and clear any config
                              data set in previous sketches or projects, so
                              running this at least once is a good idea.
   
                              When deploying your project, however, you will
                              want to disable factory reset by setting this
                              value to 0.  If you are making changes to your
                              Bluefruit LE device via AT commands, and those
                              changes aren't persisting across resets, this
                              is the reason why.  Factory reset will erase
                              the non-volatile memory where config data is
                              stored, setting it back to factory default
                              values.
       
                              Some sketches that require you to bond to a
                              central device (HID mouse, keyboard, etc.)
                              won't work at all with this feature enabled
                              since the factory reset will clear all of the
                              bonding data stored on the chip, meaning the
                              central device won't be able to reconnect.
    MINIMUM_FIRMWARE_VERSION  Minimum firmware version to have some new features
    MODE_LED_BEHAVIOUR        LED activity, valid options are
                              "DISABLE" or "MODE" or "BLEUART" or
                              "HWUART"  or "SPI"  or "MANUAL"
    -----------------------------------------------------------------------*/
    #define FACTORYRESET_ENABLE         1
    #define MINIMUM_FIRMWARE_VERSION    "0.6.6"
    #define MODE_LED_BEHAVIOUR          "MODE"
/*=========================================================================*/

// Create the bluefruit object, either software serial...uncomment these lines
/*
SoftwareSerial bluefruitSS = SoftwareSerial(BLUEFRUIT_SWUART_TXD_PIN, BLUEFRUIT_SWUART_RXD_PIN);

Adafruit_BluefruitLE_UART ble(bluefruitSS, BLUEFRUIT_UART_MODE_PIN,
                      BLUEFRUIT_UART_CTS_PIN, BLUEFRUIT_UART_RTS_PIN);
*/

/* ...or hardware serial, which does not need the RTS/CTS pins. Uncomment this line */
// Adafruit_BluefruitLE_UART ble(Serial1, BLUEFRUIT_UART_MODE_PIN);

/* ...hardware SPI, using SCK/MOSI/MISO hardware SPI pins and then user selected CS/IRQ/RST */
Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

/* ...software SPI, using SCK/MOSI/MISO user-defined SPI pins and then user selected CS/IRQ/RST */
//Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_SCK, BLUEFRUIT_SPI_MISO,
//                             BLUEFRUIT_SPI_MOSI, BLUEFRUIT_SPI_CS,
//                             BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);


// A small helper
void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}




//VARIABLES

  // defines pins numbers
  const int trigPin = A5;
  const int echoPin = A4;
  const int buzzer = A3;
  const int ledPinRED = 5;
  const int ledPinGREEN = 6;
  const int ledPinBLUE = 9;
      
   char setParallel[BUFSIZE +1];
   char setReps[BUFSIZE +1];
   char trainingMode[BUFSIZE +1];
   char setRepsAgain[BUFSIZE +1];
   int redledState = 0; 
   int lastredledState = 0;
   bool repIncrease = true; 

  unsigned long currentMillis;
  unsigned long TimeAboveParallel; 
  char setPeriod[BUFSIZE +1]; // the value is a number of seconds for pause

  long duration;
  int distance;
  int safetyDistance;

  int parallel; 
  int numReps;
  unsigned long period; 

  
  



/**************************************************************************/
/*!
    @brief  Sets up the HW an the BLE module (this function is called
            automatically on startup)
*/
/**************************************************************************/
void setup(void)
{
//  while (!Serial);  // required for Flora & Micro
//  delay(500);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(buzzer, OUTPUT);

  Serial.begin(115200);

  /* Initialise the module */
  if ( !ble.begin(VERBOSE_MODE) )
  {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }
  Serial.println( F("OK!") );

  if ( FACTORYRESET_ENABLE )
  {
    /* Perform a factory reset to make sure everything is in a known state */
    Serial.println(F("Performing a factory reset: "));
    if ( ! ble.factoryReset() ){
      error(F("Couldn't factory reset"));
    }
  }

  /* Disable command echo from Bluefruit */
  ble.echo(false);

  ble.verbose(false);  // debug info is a little annoying after this point!

  /* Wait for connection */
  while (! ble.isConnected()) {
      delay(500);
  }

  // LED Activity command is only supported from 0.6.6
  if ( ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION) )
  {
    // Change Mode LED Activity
    Serial.println(F("******************************"));
    Serial.println(F("Change LED activity to " MODE_LED_BEHAVIOUR));
    ble.sendCommandCheckOK("AT+HWModeLED=" MODE_LED_BEHAVIOUR);
    Serial.println(F("******************************"));
  }

  delay(5000);
  ble.print("AT+BLEUARTTX=");
  ble.println("Thanks for using the DepthRanger!\n\n");
  ble.print("AT+BLEUARTTX=");
  ble.println("      ");
  ble.print("AT+BLEUARTTX=");
  ble.println("Lets calibrate your parallel. Place the DepthRanger on the barbell");
  ble.print("AT+BLEUARTTX=");
  ble.println("          ");
  ble.print("AT+BLEUARTTX=");
  ble.println("What mode will you be using today? Competition(1) or Training(0)?");

  //sets trainingMode
  while (strcmp(trainingMode, "") == 0){

    ble.println("AT+BLEUARTRX");
    ble.readline();
    if (strcmp(ble.buffer, "OK") == 0) {
      // no data
      continue;
    }
    // Some data was found, its in the buffer
    strcpy(trainingMode, ble.buffer);
    ble.waitForOK();
  }
  

  ble.print("AT+BLEUARTTX=");
  ble.println("What is your parallel height?");

  //sets parallel height
  while (strcmp(setParallel, "") == 0){

    ble.println("AT+BLEUARTRX");
    ble.readline();
    if (strcmp(ble.buffer, "OK") == 0){
      //no data
      continue;
    }
    // Some data was found, its in the buffer
    strcpy(setParallel, ble.buffer);
    ble.waitForOK();
  }

  ble.print("AT+BLEUARTTX=");
  ble.println("How long will you be pausing?");
  
  //sets pause length 
  while (strcmp(setPeriod, "") == 0){

    ble.println("AT+BLEUARTRX");
    ble.readline();
    if (strcmp(ble.buffer, "OK") == 0){
      //no data
      continue;
    }
    // Some data was found, its in the buffer
    strcpy(setPeriod, ble.buffer);
    ble.waitForOK();
  }

  ble.print("AT+BLEUARTTX=");
  ble.println("How many reps are you trying to hit?");

  //sets rep count 
  while (strcmp(setReps, "") == 0){

    ble.println("AT+BLEUARTRX");
    ble.readline();
    if (strcmp(ble.buffer, "OK") == 0){
      //no data
      continue;
    }
    // Some data was found, its in the buffer
    strcpy(setReps, ble.buffer);
    ble.waitForOK();
  }
  parallel = atoi(setParallel);
  numReps = atoi(setReps);
  period = atoi(setPeriod) * 1000;
}

/**************************************************************************/
/*!
    @brief  Constantly poll for new command or response data
*/
/**************************************************************************/
void loop(void)
{
  currentMillis = millis(); //get the current "time", actually the number of milliseconds since the program started) 


  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  // Calculating the distance
  distance= duration*0.034/2;

  // Lets user know squat has reached parallel
  safetyDistance = distance;

  if (safetyDistance <= parallel){  
    analogWrite(buzzer, 255);
    setColor(255, 0, 0); 
  }
  else {
    analogWrite(buzzer, 0);
    setColor(0,0,0); 
  }

 if (safetyDistance >= parallel){
    TimeAboveParallel = currentMillis;
  }

  // Lets user know when period seconds passed at parallel
  if (currentMillis - TimeAboveParallel >= period){   

    //sets LED to WHITE
    setColor(255, 255, 255); //WHITE LED ON
    analogWrite(buzzer, 0);
  }
    
  // Prints the distance and reps on the Serial Monitor
  if (safetyDistance <= parallel){    
      
    if (repIncrease == true){     
      numReps --;
      repIncrease = false;
    
      if (numReps == 0){
        int i = 0;
        
        while (i < 3){
          //indicates end of set by beeping and flashing LED 3 times 
          setColor(255, 0, 0); //RED ON
          analogWrite(buzzer, 255);
          delay(100);
          setColor(0, 0, 0); //RED OFF
          analogWrite(buzzer, 0);
          delay(100);
          i++;
        }
        ble.print("AT+BLEUARTTX=");
        ble.println("                             ");
        ble.print("AT+BLEUARTTX=");
        ble.println("Nice job! Now how many reps?");

         setRepsAgain[0] = '\0';
         //sets rep count 
         while (strcmp(setRepsAgain, "") == 0){

          ble.println("AT+BLEUARTRX");
          ble.readline();
          if (strcmp(ble.buffer, "OK") == 0){
            //no data
             continue;
          }
          
          // Some data was found, its in the buffer
          strcpy(setRepsAgain, ble.buffer);
          ble.waitForOK();
          //numReps = atoi(setRepsAgain);
          
        }
        numReps = atoi(setRepsAgain);
      }
    }
  } 
  else if (safetyDistance > parallel + 2){
    repIncrease = true; 
  }
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" Reps #");
  Serial.println(numReps);
}

//Function to set LED color
void setColor(int red, int green, int blue)
{
  analogWrite(ledPinRED, red);
  analogWrite(ledPinGREEN, green);
  analogWrite(ledPinBLUE, blue);  
}
