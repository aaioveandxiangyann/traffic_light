#include <SPI.h> //nfc declaration...
#include <MFRC522.h> 
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);//...until here
 
int RED1 =8,YELLOW1 =7,GREEN1 =6,PEDRED1 =5, PEDGREEN1 =4,RED2 =18,YELLOW2 =17,GREEN2 =16,PEDRED2 =15,PEDGREEN2 =14,i=1;
unsigned long previousMillis = 0;
unsigned long currentMillis = millis();
unsigned long temp = 0; //to store transition remaining time
//unsigned long interval = 1000;
void setup() {
  Serial.begin(9600);
  SPI.begin();			// Init SPI bus
  mfrc522.PCD_Init();	// Init MFRC522 card
  pinMode(RED1 , OUTPUT);
  pinMode(YELLOW1 , OUTPUT);
  pinMode(GREEN1 , OUTPUT);
  pinMode(PEDRED1 , OUTPUT);
  pinMode(PEDGREEN1 , OUTPUT);
  pinMode(RED2 , OUTPUT);
  pinMode(YELLOW2 , OUTPUT);
  pinMode(GREEN2 , OUTPUT);
  pinMode(PEDRED2 , OUTPUT);
  pinMode(PEDGREEN2 , OUTPUT);
  digitalWrite(RED1,1);
  digitalWrite(RED2,1);
  digitalWrite(PEDRED1,1);
  digitalWrite(PEDRED2,1);
  digitalWrite(YELLOW1,0);
  digitalWrite(YELLOW2,0);
  digitalWrite(GREEN1,0);
  digitalWrite(GREEN2,0);
  delay(1000);
  digitalWrite(RED1,0);
  digitalWrite(GREEN1,1);delay(5000);
}
void delay1(unsigned long interval){
  for(;;){
    //Serial.print(currentMillis);Serial.print("\t");
    //Serial.print(interval);Serial.print("\t");
    //Serial.print(previousMillis);Serial.print("\t");
    //Serial.print(currentMillis - previousMillis);Serial.println();
    //Serial.print("RYB(PR,PB)1:");Serial.print(digitalRead(RED1));Serial.print(digitalRead(YELLOW1));Serial.print(digitalRead(GREEN1));Serial.print(digitalRead(PEDRED1));Serial.print(digitalRead(PEDGREEN1));Serial.print("\t");
    //Serial.print("RYB(PR,PB)2:");Serial.print(digitalRead(RED2));Serial.print(digitalRead(YELLOW2));Serial.print(digitalRead(GREEN2));Serial.print(digitalRead(PEDRED2));Serial.print(digitalRead(PEDGREEN2));
    //Serial.println();
    //if(digitalRead(GREEN1)==1)Serial.print("Green1 is green!");//判斷式
    
    currentMillis = millis();
    if(currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      break;
    }
    else{
      // Look for new cards
      if ( ! mfrc522.PICC_IsNewCardPresent()) {
        //return;
      }
      // Select one of the cards
      if ( ! mfrc522.PICC_ReadCardSerial()) {
        //return;
      }
      // Dump UID
      //Serial.print("Card UID:");
      for (byte i = 0; i < 4; i++) {
        //Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
        //Serial.print(mfrc522.uid.uidByte[i], HEX);
        if(mfrc522.uid.uidByte[0]!=0){
          //digitalWrite(GREEN1,0);digitalWrite(GREEN2,0);digitalWrite(YELLOW1,0);digitalWrite(YELLOW2,0);//todo: chg direct red to time-based turn red
          if(digitalRead(GREEN1)==1){                                                                                                                   //綠燈流程
            temp = currentMillis - previousMillis;
            temp = 5000 - temp;
            delay(temp);
             digitalWrite(GREEN1,0);digitalWrite(YELLOW1,1);delay(1000);digitalWrite(YELLOW1,0);digitalWrite(RED1,0);delay(3000);
             digitalWrite(PEDRED1,0);digitalWrite(PEDRED2,0);
             digitalWrite(PEDGREEN1,1);digitalWrite(PEDGREEN2,1);delay(3000);
             for(int j=0;j<3;j++){
            digitalWrite(PEDGREEN1,0);digitalWrite(PEDGREEN2,0);delay(500);
            digitalWrite(PEDGREEN1,1);digitalWrite(PEDGREEN2,1);delay(500);
            }
             digitalWrite(GREEN2,1);delay(5000);  digitalWrite(GREEN2,0); digitalWrite(YELLOW2,1);delay(3000); digitalWrite(YELLOW2,0);
             digitalWrite(RED2,1);delay(1000); 
             digitalWrite(GREEN1,1);
             temp = 5000 - temp; 
             delay(temp);
            }else if(digitalRead(YELLOW1)==1){
            temp = currentMillis - previousMillis;
            temp = 3000 - temp;
            digitalWrite(RED1,1);delay(3000);
             digitalWrite(PEDGREEN1,1);digitalWrite(PEDGREEN2,1);delay(3000);
             for(int j=0;j<3;j++){
            digitalWrite(PEDGREEN1,0);digitalWrite(PEDGREEN2,0);delay(500);
            digitalWrite(PEDGREEN1,1);digitalWrite(PEDGREEN2,1);delay(500);
            }
            digitalWrite(GREEN2,1);delay(5000);digitalWrite(GREEN2,0);digitalWrite(YELLOW2,1);delay(3000);digitalWrite(YELLOW2,0);
            digitalWrite(RED2,1);delay(1000); digitalWrite(GREEN1,1);delay(5000);digitalWrite(GREEN1,0);  
            digitalWrite(YELLOW1,1);
             temp = 3000- temp; 
             delay(temp);
            }else if(digitalRead(RED1)==1){
               temp = 3000- temp;
               digitalWrite(PEDGREEN1,1);digitalWrite(PEDGREEN2,1);delay(3000);
               for(int j=0;j<3;j++){
                  digitalWrite(PEDGREEN1,0);digitalWrite(PEDGREEN2,0);delay(500);
                  digitalWrite(PEDGREEN1,1);digitalWrite(PEDGREEN2,1);delay(500);
             }
                 digitalWrite(GREEN2,1);delay(5000);digitalWrite(GREEN2,0);digitalWrite(YELLOW2,1);delay(3000);digitalWrite(YELLOW2,0);digitalWrite(RED2,1);delay(1000);
                 digitalWrite(GREEN1,1);delay(5000);digitalWrite(GREEN1,0);digitalWrite(YELLOW1,1);delay(5000);digitalWrite(YELLOW1,0);
                 temp = 3000- temp; 
                 delay(temp);
                 digitalWrite(RED1,1);
              }
          digitalWrite(YELLOW1,1);digitalWrite(YELLOW2,1);delay(1000);
          digitalWrite(YELLOW1,0);digitalWrite(YELLOW2,0);
          digitalWrite(RED1,1);digitalWrite(RED2,1);delay(1000);
          digitalWrite(PEDRED1,0);digitalWrite(PEDRED2,0);
          digitalWrite(PEDGREEN1,1);digitalWrite(PEDGREEN2,1);delay(3000);
          for(int j=0;j<3;j++){
            digitalWrite(PEDGREEN1,0);digitalWrite(PEDGREEN2,0);delay(500);
            digitalWrite(PEDGREEN1,1);digitalWrite(PEDGREEN2,1);delay(500);
          }
          digitalWrite(PEDGREEN1,0);digitalWrite(PEDGREEN2,0);
          digitalWrite(PEDRED1,1);digitalWrite(PEDRED2,1);delay(1000);
        }
        mfrc522.uid.uidByte[i]=0;
      } 
      //Serial.print("\t");
      mfrc522.PICC_HaltA();
    }
  }
}
void red(int count){
  if(count==1){
    delay(50);
    digitalWrite(GREEN1,0);digitalWrite(YELLOW1,1);delay1(1000);
    digitalWrite(YELLOW1,0);digitalWrite(RED1,1);delay1(3000);
    digitalWrite(RED2,0);digitalWrite(GREEN2,1);delay1(5000);
  }
  if(count==2){
    delay(50);
    digitalWrite(GREEN2,0);digitalWrite(YELLOW2,1);delay1(1000);
    digitalWrite(YELLOW2,0);digitalWrite(RED2,1);delay1(3000);
    digitalWrite(RED1,0);digitalWrite(GREEN1,1);delay1(5000);
  }
}
void loop() {
  if(i==1){
    red(i);
    i++;
  }
  if(i==2){
    red(i);
    i--;
  }
}

