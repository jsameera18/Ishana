//#include <LiquidCrystal.h>

#include <virtuabotixRTC.h>

#include <Wire.h>
#include <DS3231.h>
#include <Stepper.h>
#include <SoftwareSerial.h>


virtuabotixRTC myRTC(6, 7, 8);


const int maxOps = 10;
int buttonValue;

//node mcu
SoftwareSerial espSerial(A0,A1);

//RTC
RTCDateTime now;
DS3231 rtc;

//stepper motor
const int stepsPerRevolution = 2048;
Stepper stepper = Stepper(stepsPerRevolution, 2, 4, 3, 5);

//electromagnet
int magnetSignal = 8;
int s0 = 9;
int s1 = 10;
int s2 = 11;
int s3 = 12;

//button & buzzer
int buttonPin = A2; 
bool buttonPressed = false;
int buzzerPin = A3;

//(stepsPerRevolution / 360) * 15
int stepsToShiftOneCell = 85;

//others 
struct DispenseTIme{
      byte module;
      byte amount;

      int year;
      byte month;
      byte day;
      byte hour;
      byte min;
};

const int temp = 10;
//get the string data from firebase
String dispenseTimings[temp] = {"d|00:10|1|2|", "a|00:30|3|2|"};
DispenseTIme dispenseTimeArray[temp];

void setup ()
{
  Serial.begin(9600);
  espSerial.begin(9600);
  myRTC.setDS1302Time(18, 10, 10, 7, 18, 9, 2020);

  
  //RTC--------------------------------
  if (!rtc.begin()) 
  {
    Serial.println("Couldn't find RTC Module");
    while (1);
  }

  if (rtc.lostPower()) 
  {
    Serial.println("RTC lost power, lets set the time!");
    rtc.adjust(DateTime(F(_DATE), F(TIME_)));
  }
  rtc.adjust(DateTime(F(_DATE), F(TIME_)));
  //-----------------------------------

  //Display
 // display.begin(SH1106_SWITCHAPVCC, 0x3C); 

  //magnets
  pinMode(magnetSignal, OUTPUT);
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);

  //button & buzzer
  pinMode(buttonPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
}


void loop () 
{
  now = ;

  //every midnight updates the prescription and stores it in th eeprom
  if(){
    fetchData();
    storeData();
  }

//checks every 30 mins for any pill to be dispensed
  if(){
    checkTimeToDispense();
  }

  

}

//data methods
void fetchData(){
    
}

void storeData(){
  
}

void sendLog(String logData){

}


//Dispensing methods
void checkTimeToDispense() {


  for(int i = 0;i<temp;i++){
    
      if(dispenseTimeArray[i].year == now.year() && dispenseTimeArray[i].month == now.month() && dispenseTimeArray[i].day == now.day()){

        if(dispenseTimeArray[i].hour == now.hour() && dispenseTimeArray[i].min == now.minute()){
          // check range
          dispensePill(dispenseTimeArray[i].module, dispenseTimeArray[i].amount);
        }
      }
  }

}

void dispensePill(byte module, byte amount) {
  

  int startTime = now.minute();

  while(now.minute() - startTime <= 5){
    waitForButtonConfirmation();

    if(buttonPressed){
      

      activateElectromagnet(module);
      for (int i = 0; i < amount; i++) {
        stepper.step(stepsToShiftOneCell);
        sendLog("timestamp:success");
        delay(1000);
      }
      deactivateElectromagnet(module);
      buttonPressed = false;
      

      
    }
  } 
}

void activateElectromagnet(byte module) {

  digitalWrite(s0, bitRead(module, 0));
  digitalWrite(s1, bitRead(module, 1));
  digitalWrite(s2, bitRead(module, 2));
  digitalWrite(s3, bitRead(module, 3));

  digitalWrite(magnetSignal, HIGH);
}

void deactivateElectromagne(byte module) {
  digitalWrite(s0, bitRead(module, 0));
  digitalWrite(s1, bitRead(module, 1));
  digitalWrite(s2, bitRead(module, 2));
  digitalWrite(s3, bitRead(module, 3));

  digitalWrite(magnetSignal, LOW);
}

//other methods
void waitForButtonConfirmation(){
  
  if(digitalRead(buttonPin) == 1){
    buttonPressed = true;
  }
}