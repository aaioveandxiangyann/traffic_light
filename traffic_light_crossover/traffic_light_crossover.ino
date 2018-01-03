#include "Wire.h"
#include <RTClib.h>
#include <SPI.h> //nfc declaration...
#include <MFRC522.h>
#define SS_PIN 10
#define RST_PIN 9
#define BUZZER_PIN 2
MFRC522 mfrc522(SS_PIN, RST_PIN); //...until here

RTC_DS1307 RTC;

int RED1 = 8, YELLOW1 = 7, GREEN1 = 6, PEDRED1 = 5, PEDGREEN1 = 4, RED2 = 3, YELLOW2 = 17, GREEN2 = 16, PEDRED2 = 15, PEDGREEN2 = 14, j = 0 , i = 0 , interrupt = 0;
int sequence=0;
int queue[4]={0,0,0,0}, prev_queue[4], allled[10]={RED1,YELLOW1,GREEN1,PEDRED1,PEDGREEN1,RED2,YELLOW2,GREEN2,PEDRED2,PEDGREEN2};
unsigned long previousMillis = 0,previousMillis1 = 0, buzzertime=0;
unsigned long currentMillis = millis();
unsigned long temp = 0; //to store transition remaining time
bool temp2=0; // 判斷新卡是否讀取連續兩次
unsigned long interval = 1000,cooldown=60000;
void setup(){
    Serial.begin(9600);
    SPI.begin(); // Init SPI bus
    Wire.begin();
    RTC.begin();
    if (! RTC.isrunning()) {
      Serial.println("RTC is NOT running!");
      // following line sets the RTC to the date & time this sketch was compiled
      RTC.adjust(DateTime(__DATE__, __TIME__));
    }
    pinMode(BUZZER_PIN,OUTPUT);
    for(i=0;i<10;i++)pinMode(allled[i],OUTPUT);
    for(j=1;j>=0;j--){
      for(i=0;i<10;i++)digitalWrite(allled[i],j);
      delay(500);
    }
}

//UID check part 1 courtesy Stackexchange user Pwdr
unsigned long getID(){
  if ( ! mfrc522.PICC_ReadCardSerial()) { //Since a PICC placed get Serial and continue
    return -1;
  }
  /*unsigned long hex_num;
  hex_num =  mfrc522.uid.uidByte[0] << 24; //shift by 24 bits
  hex_num += mfrc522.uid.uidByte[1] << 16;
  hex_num += mfrc522.uid.uidByte[2] <<  8;
  hex_num += mfrc522.uid.uidByte[3];
  //mfrc522.PICC_HaltA(); // Stop reading
  return hex_num;*/
  return 0;
}
//UID check part 1 end

void interrupt_seq(){
  
  switch(sequence){
    case 70:interval+=9000;interrupt=0;temp2=1;break;
    //case 30:case 60:interval+=5000;interrupt=0;temp2=1;Serial.println(interval);break;
    default:return;
  }
}

void passed(){
  /*INSERT BEEPBEEP*/
  Serial.println();
  Serial.print("Passed");
  Serial.println();
  if(temp2==0)digitalWrite(BUZZER_PIN,HIGH);
  buzzertime=millis();
  interrupt = 1;
}

void loop(){
  DateTime timenow = RTC.now();
  currentMillis = millis();
  if(currentMillis-previousMillis >= interval){
    previousMillis=currentMillis;
      
    //change sequence block
    for(i=0;i<4;i++)prev_queue[i]=queue[i];
    switch(sequence){
      case 10:
        queue[0]=PEDRED2;queue[1]=RED2;queue[2]=RED1;queue[3]=PEDRED1;interval=2000;break;
      case 20:
        queue[0]=PEDGREEN2;queue[1]=RED2;queue[2]=GREEN1;queue[3]=PEDRED1;interval=9000;break;
      case 30:
        queue[0]=PEDRED2;queue[1]=RED2;queue[2]=GREEN1;queue[3]=PEDRED1;interval=1000;break;
      case 40:
        queue[0]=PEDRED2;queue[1]=RED2;queue[2]=YELLOW1;queue[3]=PEDRED1;interval=1500;break;
      case 50:
        queue[0]=PEDRED2;queue[1]=RED2;queue[2]=RED1;queue[3]=PEDRED1;interval=2000;break;
      case 60:
        queue[0]=PEDRED2;queue[1]=GREEN2;queue[2]=RED1;queue[3]=PEDGREEN1;interval=9000;break;
      case 70:
        queue[0]=PEDRED2;queue[1]=GREEN2;queue[2]=RED1;queue[3]=PEDRED1;interval=1000;break;
      case 80:
        queue[0]=PEDRED2;queue[1]=YELLOW2;queue[2]=RED1;queue[3]=PEDRED1;interval=1500;break;
        
      default:break;//THIS SHOULD NOT BE HAPPENING
    }
    (sequence==80)?sequence=10:sequence+=10;
    //change sequence block end
    
    //check for interrupts
    if(interrupt)interrupt_seq();
    //check for interrupts end

    if(currentMillis-buzzertime>700)digitalWrite(BUZZER_PIN,LOW);
    
    //display changes
    for(i=0;i<4;i++)digitalWrite(prev_queue[i], 0);
    for(i=0;i<4;i++)digitalWrite(queue[i], 1);
    Serial.print("RYG(PedR,PedG)1:");for(i=0;i<5;i++)Serial.print(digitalRead(allled[i]));Serial.print("\t");
    Serial.print("RYG(PedR,PedG)2:");for(i=5;i<10;i++)Serial.print(digitalRead(allled[i]));
    Serial.print(" NextSeq: ");Serial.print(sequence);Serial.print("\t");Serial.print(interval);
    Serial.println();
  }else{
    if(interrupt)interrupt_seq();
    if(currentMillis-buzzertime>700)digitalWrite(BUZZER_PIN,LOW);
    //UID check part 2
    if((temp2==1) && (millis()-previousMillis1 >= cooldown)){
      Serial.println("cooldown end"); // Init MFRC522 card
      temp2=0;
    }else{
      mfrc522.PCD_Init();
    }
    if(mfrc522.PICC_IsNewCardPresent() && (temp2==0)) {  
       unsigned long areyouthere = getID();
       if(areyouthere != -1){
        previousMillis1=currentMillis;
        //*****************************************************************************************//
        // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
        MFRC522::MIFARE_Key key;
        for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;
      
        //some variables we need
        byte block;
        byte len;
        MFRC522::StatusCode status;
      
        //mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid)); //dump some details about the card
      
        //mfrc522.PICC_DumpToSerial(&(mfrc522.uid));      //see all blocks in hex
      
        //-------------------------------------------
      
        //Serial.print(F("資料: "));
      
        byte buffer1[18];
      
        block = 4;
        len = 18;
      
        //------------------------------------------- GET DATA
        status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 4, &key, &(mfrc522.uid)); //line 834 of MFRC522.cpp file
        if (status != MFRC522::STATUS_OK) {
          Serial.print(F("Authentication failed: "));
          Serial.println(mfrc522.GetStatusCodeName(status));
          return;
        }
      
        status = mfrc522.MIFARE_Read(block, buffer1, &len);
        if (status != MFRC522::STATUS_OK) {
          Serial.print(F("Reading failed: "));
          Serial.println(mfrc522.GetStatusCodeName(status));
          return;
        }
      
        //PRINT DATA
        for (uint8_t i = 0; i < 8; i++)
        {
          if (buffer1[i] != 32)
          {
            buffer1[i]-=60;
            if(buffer1[i]<48 || buffer1[i]>57)return;
            //Serial.write(buffer1[i]);
          }
        }
        //Serial.print(" ");
        long buffer3[8],buffer4[3];
        for(uint8_t i = 0; i < 8; i++){
          buffer3[i]=buffer1[i];
          switch(buffer3[i]){
            case '0':buffer3[i]=0;break;
            case '1':buffer3[i]=1;break;
            case '2':buffer3[i]=2;break;
            case '3':buffer3[i]=3;break;
            case '4':buffer3[i]=4;break;
            case '5':buffer3[i]=5;break;
            case '6':buffer3[i]=6;break;
            case '7':buffer3[i]=7;break;
            case '8':buffer3[i]=8;break;
            case '9':buffer3[i]=9;break;
          }
          if(i<4){
            buffer4[0]*=10;
            buffer4[0]+=buffer3[i];
          }
          else if(i<6){
            buffer4[1]*=10;
            buffer4[1]+=buffer3[i];
          }
          else{
            buffer4[2]*=10;
            buffer4[2]+=buffer3[i];
          }
        }
        /*Serial.println();
        Serial.print("Parsed YYYYMMDD ");
        Serial.print(buffer4[0]);Serial.print(buffer4[1]);Serial.print(buffer4[2]);Serial.println();*/
        
        /*//---------------------------------------- GET ID
      
        Serial.print(F("ID: "));
        byte buffer2[18];
        block = 1;
      
        status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 1, &key, &(mfrc522.uid)); //line 834
        if (status != MFRC522::STATUS_OK) {
          Serial.print(F("Authentication failed: "));
          Serial.println(mfrc522.GetStatusCodeName(status));
          return;
        }
      
        status = mfrc522.MIFARE_Read(block, buffer2, &len);
        if (status != MFRC522::STATUS_OK) {
          Serial.print(F("Reading failed: "));
          Serial.println(mfrc522.GetStatusCodeName(status));
          return;
        }
      
        //PRINT ID
        for (uint8_t i = 0; i < 16; i++) {
          Serial.write(buffer2[i] );
        }*/
      
        /*Serial.print("\tYYYY: ");
        Serial.print(timenow.year());
        Serial.print("\tMM: ");
        Serial.print(timenow.month());
        Serial.print("\tDD: ");
        Serial.print(timenow.day());
        Serial.print("\n");*/
        //year matching
        //is 2017 < 2018
        if(timenow.year()<buffer4[0]){
          passed();
        }
        else if(timenow.year()==buffer4[0]){
          if(timenow.month()<buffer4[1])passed();
          else if(timenow.month()==buffer4[1]){
            if(timenow.day()<=buffer4[2])passed();
          }
        }
        //----------------------------------------
        //Serial.println("BOOP");
        delay(100); //change value if you want to read cards faster
      
        mfrc522.PICC_HaltA();
        mfrc522.PCD_StopCrypto1();
        //*****************************************************************************************//
      }
    }
    //UID check part 2 end
    delay(10);
  }
}

