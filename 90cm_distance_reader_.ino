
// MAIN LOGIC FOR USING THE DEPTH RANGER

// defines pins numbers
  const int trigPin = A5;
  const int echoPin = A4;
  const int buzzer = A3;

  const int ledPinRED = 5;
  const int ledPinGREEN = 6;
  const int ledPinBLUE = 9;
      
   int parallel;
   int numReps;
   int redledState = 0; 
   int lastredledState = 0;
   bool repIncrease = true;   
   int trainingMode;  

  unsigned long currentMillis;
  unsigned long TimeAboveParallel; 
  unsigned long period; // the value is a number of milliseconds 

  // defines variables
  long duration;
  int distance;
  int safetyDistance;

  //#define COMMON_ANODE

void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(buzzer, OUTPUT);
//  pinMode(ledPinRED, OUTPUT);
//  pinMode(ledPinGREEN, OUTPUT);
//  pinMode(ledPinBLUE, OUTPUT);
  Serial.begin(9600); // Starts the serial communication
  while (! Serial); 

  Serial.println("Thanks for using the DepthRanger!");
  Serial.println(" ");
  Serial.println("Lets calibrate your parallel. Place the DepthRanger on the barbell");
    
    if (parallel == NULL){
      
    }
    
  Serial.println("What mode will you be using today? Competition(1) or Training(0)?");

  TimeAboveParallel = millis(); //initial start time
}

void loop() {

  //If competition mode, count parallel but no indication during rep until after 
  //If training mode, beep and LED indicator for parallel
  //When calibrating, determine lowest distance travelled than trial and error 
  
   if (trainingMode == NULL){
      while (Serial.available() == 0){}
      trainingMode = Serial.parseInt();

    if (parallel == NULL){
      Serial.println("What is your parallel Height? ");
      while (Serial.available() == 0){}
      parallel = Serial.parseInt();

      if (period == NULL){
        Serial.println("How long will you be pausing?");
        while (Serial.available() == 0){}
        period = Serial.parseInt() * 1000;

        if (numReps == NULL){
          Serial.println("How many reps are you trying to hit?");
          while(Serial.available() == 0){}
          numReps = Serial.parseInt();
        }
      } 
    }
  }

  //Competition Mode
  if (trainingMode == 1){ 
  }

  //Training Mode
  if (trainingMode == 0){
  }
  
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
  else{
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
        Serial.println(" ");
        Serial.println("Nice job! Now how many reps?");
        while(Serial.available() == 0){}
        numReps = Serial.parseInt();
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

void setColor(int red, int green, int blue)
{
  #ifdef COMMON_ANODE
    red = 255 - red;
    green = 255 - green;
    blue = 255 - blue;
  #endif
  analogWrite(ledPinRED, red);
  analogWrite(ledPinGREEN, green);
  analogWrite(ledPinBLUE, blue);  
}
