#include <RTClib.h>
#include <SPI.h> //nfc declaration...
#include <MFRC522.h>
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN); //...until here


int RED1 = 8, YELLOW1 = 7, GREEN1 = 6, PEDRED1 = 5, PEDGREEN1 = 4, RED2 = 18, YELLOW2 = 17, GREEN2 = 16, PEDRED2 = 15, PEDGREEN2 = 14, j = 0 , i = 0 , interrupt = 0;
int sequence=0;
int queue[4]={0,0,0,0}, prev_queue[4], allled[10]={RED1,YELLOW1,GREEN1,PEDRED1,PEDGREEN1,RED2,YELLOW2,GREEN2,PEDRED2,PEDGREEN2};
unsigned long previousMillis = 0,previousMillis1 = 0;
unsigned long currentMillis = millis();
unsigned long temp = 0; //to store transition remaining time
bool temp2=0; // 判斷新卡是否讀取連續兩次
unsigned long interval = 1000,cooldown=60000;
unsigned long interva3,interva4,interva5;
void setup(){
    Serial.begin(9600);
    SPI.begin(); // Init SPI bus
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
  unsigned long hex_num;
  hex_num =  mfrc522.uid.uidByte[0] << 24; //shift by 24 bits
  hex_num += mfrc522.uid.uidByte[1] << 16;
  hex_num += mfrc522.uid.uidByte[2] <<  8;
  hex_num += mfrc522.uid.uidByte[3];
  //mfrc522.PICC_HaltA(); // Stop reading
  return hex_num;
}
//UID check part 1 end

void interrupt_seq(){
  temp2=1;
  switch(sequence){
    case 20:case 50:interval+=9000;interrupt=0; break;
    //case 10:case 30:case 40:case 60:interval+=9000;interrupt=0;break;
    default:return;
  }
}

void loop(){
  currentMillis = millis();
  if(currentMillis-previousMillis >= interval){
    previousMillis=currentMillis;
      
    //change sequence block
    for(i=0;i<4;i++)prev_queue[i]=queue[i];
    switch(sequence){
      case 10:
        queue[0]=15;queue[1]=18;queue[2]=8;queue[3]=5;interval=1000;break;
      case 20:
        queue[0]=14;queue[1]=18;queue[2]=6;queue[3]=5;interval=9000;break;
      case 30:
        queue[0]=14;queue[1]=18;queue[2]=7;queue[3]=5;interval=3000;break;
      case 40:
        queue[0]=15;queue[1]=18;queue[2]=8;queue[3]=5;interval=1000;break;
      case 50:
        queue[0]=15;queue[1]=16;queue[2]=8;queue[3]=4;interval=9000;break;
      case 60:
        queue[0]=15;queue[1]=17;queue[2]=8;queue[3]=4;interval=3000;break;
        
      default:break;//THIS SHOULD NOT BE HAPPENING
    }
    (sequence==60)?sequence=10:sequence+=10;
    //change sequence block end
    
    //check for interrupts
    if(interrupt)interrupt_seq();
    //check for interrupts end
  
    //display changes
    for(i=0;i<4;i++)digitalWrite(prev_queue[i], 0);
    for(i=0;i<4;i++)digitalWrite(queue[i], 1);
    Serial.print("RYG(PedR,PedG)1:");for(i=0;i<5;i++)Serial.print(digitalRead(allled[i]));Serial.print("\t");
    Serial.print("RYG(PedR,PedG)2:");for(i=5;i<10;i++)Serial.print(digitalRead(allled[i]));
    Serial.print(" NextSeq: ");Serial.print(sequence);
    Serial.println();
  }else{
    //UID check part 2
    Serial.println(interval);
    if((temp2==1) && (millis()-previousMillis1 >= cooldown)){
      Serial.println("cooldown"); // Init MFRC522 card
      temp2=0;
    }else{
      mfrc522.PCD_Init();
      //Serial.print("e:");
      //Serial.print(currentMillis);
      //Serial.println();
    }
    if(mfrc522.PICC_IsNewCardPresent() && (temp2==0)) {  
       unsigned long uid = getID();Serial.println("in");
      //if(((uid != -1) && (temp2 == 0)) || (((uid != -1) && (temp2 == 1)) && (currentMillis - previousMillis >= 10000))){  //判斷刷卡時間10秒或判斷是不是新卡
       if(uid != -1){
        Serial.print("Card detected, UID: "); Serial.println(uid);
        previousMillis1=currentMillis;
        Serial.println(millis()-previousMillis1);
        interrupt = 1;
       //temp2 = !temp2;
      }
    }
    //UID check part 2 end
    delay(10);
  }
}

