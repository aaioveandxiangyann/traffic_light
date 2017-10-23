#include <SPI.h> //nfc declaration...
#include <MFRC522.h>
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN); //...until here

int RED1 = 8, YELLOW1 = 7, GREEN1 = 6, PEDRED1 = 5, PEDGREEN1 = 4, RED2 = 18, YELLOW2 = 17, GREEN2 = 16, PEDRED2 = 15, PEDGREEN2 = 14, j = 0 , i = 0 , status=0;
int sequence=0;
int queue[4]={0,0,0,0}, prev_queue[4], allled[10]={RED1,YELLOW1,GREEN1,PEDRED1,PEDGREEN1,RED2,YELLOW2,GREEN2,PEDRED2,PEDGREEN2};
unsigned long previousMillis = 0;
unsigned long currentMillis = millis();
unsigned long temp = 0; //to store transition remaining time
unsigned long interval = 1000;

void setup(){
    Serial.begin(9600);
    SPI.begin(); // Init SPI bus
    mfrc522.PCD_Init(); // Init MFRC522 card
    for(i=0;i<10;i++)pinMode(allled[i],OUTPUT);
    for(j=1;j>=0;j--){
      for(i=0;i<10;i++)digitalWrite(allled[i],j);
      delay(500);
    }
}

void loop(){
  
  //change sequence block
  for(i=0;i<4;i++)prev_queue[i]=queue[i];
  switch(sequence){
    case 10:
      queue[0]=15;queue[1]=18;queue[2]=8;queue[3]=5;break;
    case 20:
      queue[0]=14;queue[1]=18;queue[2]=6;queue[3]=5;break;
    case 30:
      queue[0]=14;queue[1]=18;queue[2]=7;queue[3]=5;break;
    case 40:
      queue[0]=15;queue[1]=18;queue[2]=8;queue[3]=5;break;
    case 50:
      queue[0]=15;queue[1]=16;queue[2]=8;queue[3]=4;break;
    case 60:
      queue[0]=15;queue[1]=17;queue[2]=8;queue[3]=4;break;
  }
  (sequence==60)?sequence=10:sequence+=10;
  //change sequence block end
  
  //check for interrupts
  //TODO
  //check for interrupts end

  //display changes
  for(i=0;i<4;i++)digitalWrite(prev_queue[i], 0);
  for(i=0;i<4;i++)digitalWrite(queue[i], 1);
  delay(1000);
        Serial.print("RYG(PedR,PedG)1:");
        for(i=0;i<5;i++)Serial.print(digitalRead(allled[i]));
        Serial.print("\t");
        Serial.print("RYG(PedR,PedG)2:");
        for(i=5;i<10;i++)Serial.print(digitalRead(allled[i]));
        Serial.print(" Sequence");
        Serial.print(sequence);
        Serial.println();
}

/*void delay1(unsigned long interval)
{
    for (;;) {
        //Serial.print(currentMillis);Serial.print("\t");
        //Serial.print(interval);Serial.print("\t");
        //Serial.print(previousMillis);Serial.print("\t");
        //Serial.print(currentMillis - previousMillis);Serial.println();
        Serial.print("RYG(PR,PG)1:");
        Serial.print(digitalRead(RED1));
        Serial.print(digitalRead(YELLOW1));
        Serial.print(digitalRead(GREEN1));
        Serial.print(digitalRead(PEDRED1));
        Serial.print(digitalRead(PEDGREEN1));
        Serial.print("\t");
        Serial.print("RYG(PR,PG)2:");
        Serial.print(digitalRead(RED2));
        Serial.print(digitalRead(YELLOW2));
        Serial.print(digitalRead(GREEN2));
        Serial.print(digitalRead(PEDRED2));
        Serial.print(digitalRead(PEDGREEN2));
        Serial.println();
        //if(digitalRead(GREEN1)==1)Serial.print("Green1 is green!");//判斷式

        currentMillis = millis();
        if (currentMillis - previousMillis >= interval) {
            previousMillis = currentMillis;
            break;
        }
        else {
            // Look for new cards
            if (!mfrc522.PICC_IsNewCardPresent()) {
                //return;
            }
            // Select one of the cards
            if (!mfrc522.PICC_ReadCardSerial()) {
                //return;
            }
            // Dump UID
            //Serial.print("Card UID:");
            for (byte i = 0; i < 4; i++) {
                //Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
                //Serial.print(mfrc522.uid.uidByte[i], HEX);
                if (mfrc522.uid.uidByte[0] != 0) {
                    //digitalWrite(GREEN1,0);digitalWrite(GREEN2,0);digitalWrite(YELLOW1,0);digitalWrite(YELLOW2,0);//todo: chg direct red to time-based turn red
                    if (digitalRead(GREEN1) == 1) { //綠燈流程
                        temp = currentMillis - previousMillis;
                        temp = 5000 - temp;
                        delay(temp);
                        digitalWrite(GREEN1, 0);
                        digitalWrite(YELLOW1, 1);
                        delay(1000);
                        digitalWrite(YELLOW1, 0);
                        digitalWrite(RED1, 1);
                        delay(3000);
                        digitalWrite(PEDRED1, 0);
                        digitalWrite(PEDRED2, 0);
                        digitalWrite(PEDGREEN1, 1);
                        digitalWrite(PEDGREEN2, 1);
                        delay(3000);
                        for (int j = 0; j < 3; j++) {
                            digitalWrite(PEDGREEN1, 0);
                            digitalWrite(PEDGREEN2, 0);
                            delay(500);
                            digitalWrite(PEDGREEN1, 1);
                            digitalWrite(PEDGREEN2, 1);
                            delay(500);
                        }
                        digitalWrite(PEDGREEN1, 0);
                        digitalWrite(PEDGREEN2, 0);
                        digitalWrite(PEDRED1, 1);
                        digitalWrite(PEDRED2, 1);
                        delay(1000);
                        digitalWrite(RED2, 0);
                        digitalWrite(GREEN2, 1);
                        delay(5000);
                        digitalWrite(GREEN2, 0);
                        digitalWrite(YELLOW2, 1);
                        delay(1000);
                        digitalWrite(YELLOW2, 0);
                        digitalWrite(RED2, 1);
                        delay(3000);
                        digitalWrite(RED1, 0);
                        digitalWrite(GREEN1, 1);
                        temp = 5000 - temp;
                        delay(temp);
                    }
                    else if (digitalRead(YELLOW1) == 1) {
                        temp = currentMillis - previousMillis;
                        temp = 3000 - temp;
                        delay(temp);
                        digitalWrite(YELLOW1, 0);
                        digitalWrite(RED1, 1);
                        delay(3000);
                        digitalWrite(PEDRED1, 0);
                        digitalWrite(PEDRED2, 0);
                        digitalWrite(PEDGREEN1, 1);
                        digitalWrite(PEDGREEN2, 1);
                        delay(3000);
                        for (int j = 0; j < 3; j++) {
                            digitalWrite(PEDGREEN1, 0);
                            digitalWrite(PEDGREEN2, 0);
                            delay(500);
                            digitalWrite(PEDGREEN1, 1);
                            digitalWrite(PEDGREEN2, 1);
                            delay(500);
                        }
                        digitalWrite(PEDGREEN1, 0);
                        digitalWrite(PEDGREEN2, 0);
                        digitalWrite(PEDRED1, 1);
                        digitalWrite(PEDRED2, 1);
                        delay(1000);
                        digitalWrite(RED2, 0);
                        digitalWrite(GREEN2, 1);
                        delay(5000);
                        digitalWrite(GREEN2, 0);
                        digitalWrite(YELLOW2, 1);
                        delay(1000);
                        digitalWrite(YELLOW2, 0);
                        digitalWrite(RED2, 1);
                        delay(3000);
                        digitalWrite(RED1, 0);
                        digitalWrite(GREEN1, 1);
                        delay(5000);
                        digitalWrite(GREEN1, 0);
                        digitalWrite(YELLOW1, 1);
                        temp = 3000 - temp;
                        delay(temp);
                    }


                    else if (digitalRead(GREEN2) == 1) { //綠燈流程
                        temp = currentMillis - previousMillis;
                        temp = 5000 - temp;
                        Serial.print("debug1");
                        delay(temp);
                        digitalWrite(GREEN2, 0);
                        digitalWrite(YELLOW2, 1);Serial.print("debug2");
                        delay(1000);
                        digitalWrite(YELLOW2, 0);
                        digitalWrite(RED2, 1);Serial.print("debug3");
                        delay(3000);
                        digitalWrite(PEDRED2, 0);
                        digitalWrite(PEDRED1, 0);
                        digitalWrite(PEDGREEN2, 1);
                        digitalWrite(PEDGREEN1, 1);Serial.print("debug4");
                        delay(3000);
                        for (int j = 0; j < 3; j++) {
                            digitalWrite(PEDGREEN2, 0);
                            digitalWrite(PEDGREEN1, 0);
                            delay(500);Serial.print("debug5");
                            digitalWrite(PEDGREEN2, 1);
                            digitalWrite(PEDGREEN1, 1);
                            delay(500);
                        }Serial.print("debug6");
                        digitalWrite(PEDGREEN2, 0);
                        digitalWrite(PEDGREEN1, 0);
                        digitalWrite(PEDRED2, 1);
                        digitalWrite(PEDRED1, 1);
                        delay(1000);
                        digitalWrite(RED1, 0);
                        digitalWrite(GREEN1, 1);
                        delay(5000);
                        digitalWrite(GREEN1, 0);
                        digitalWrite(YELLOW1, 1);
                        delay(1000);
                        digitalWrite(YELLOW1, 0);
                        digitalWrite(RED1, 1);
                        delay(3000);
                        digitalWrite(RED2, 0);
                        digitalWrite(GREEN2, 1);
                        temp = 5000 - temp;
                        delay(temp);
                    }
                    else if (digitalRead(YELLOW2) == 1) {
                        temp = currentMillis - previousMillis;
                        temp = 3000 - temp;
                        delay(temp);
                        digitalWrite(YELLOW2, 0);
                        digitalWrite(RED2, 1);
                        delay(3000);
                        digitalWrite(PEDRED2, 0);
                        digitalWrite(PEDRED1, 0);
                        digitalWrite(PEDGREEN2, 1);
                        digitalWrite(PEDGREEN1, 1);
                        delay(3000);
                        for (int j = 0; j < 3; j++) {
                            digitalWrite(PEDGREEN2, 0);
                            digitalWrite(PEDGREEN1, 0);
                            delay(500);
                            digitalWrite(PEDGREEN2, 1);
                            digitalWrite(PEDGREEN1, 1);
                            delay(500);
                        }
                        digitalWrite(PEDGREEN2, 0);
                        digitalWrite(PEDGREEN1, 0);
                        digitalWrite(PEDRED2, 1);
                        digitalWrite(PEDRED1, 1);
                        delay(1000);
                        digitalWrite(RED1, 0);
                        digitalWrite(GREEN1, 1);
                        delay(5000);
                        digitalWrite(GREEN1, 0);
                        digitalWrite(YELLOW1, 1);
                        delay(1000);
                        digitalWrite(YELLOW1, 0);
                        digitalWrite(RED1, 1);
                        delay(3000);
                        digitalWrite(RED2, 0);
                        digitalWrite(GREEN2, 1);
                        delay(5000);
                        digitalWrite(GREEN2, 0);
                        digitalWrite(YELLOW2, 1);
                        temp = 3000 - temp;
                        delay(temp);
                    }
                    else if (digitalRead(RED2) == 1 && status==2) {
                        temp = 3000 - temp;
                        delay(temp);
                        digitalWrite(PEDRED2, 0);
                        digitalWrite(PEDRED1, 0);
                        digitalWrite(PEDGREEN2, 1);
                        digitalWrite(PEDGREEN1, 1);
                        delay(3000);
                        for (int j = 0; j < 3; j++) {
                            digitalWrite(PEDGREEN2, 0);
                            digitalWrite(PEDGREEN1, 0);
                            delay(500);
                            digitalWrite(PEDGREEN2, 1);
                            digitalWrite(PEDGREEN1, 1);
                            delay(500);
                        }
                        digitalWrite(PEDGREEN2, 0);
                        digitalWrite(PEDGREEN1, 0);
                        digitalWrite(PEDRED2, 1);
                        digitalWrite(PEDRED1, 1);
                        delay(1000);
                        digitalWrite(RED1, 0);
                        digitalWrite(GREEN1, 1);
                        delay(5000);
                        digitalWrite(GREEN1, 0);
                        digitalWrite(YELLOW1, 1);
                        delay(3000);
                        digitalWrite(YELLOW1, 0);
                        digitalWrite(RED1, 1);
                        delay(1000);
                        digitalWrite(RED2, 0);
                        digitalWrite(GREEN2, 1);
                        delay(5000);
                        digitalWrite(GREEN2, 0);
                        digitalWrite(YELLOW2, 1);
                        delay(5000);
                        digitalWrite(YELLOW2, 0);
                        temp = 3000 - temp;
                        delay(temp);
                        digitalWrite(RED2, 1);
                    }
          else if (digitalRead(RED1) == 1 && status==1) {
                        temp = 3000 - temp;
                        delay(temp);
                        digitalWrite(PEDRED1, 0);
                        digitalWrite(PEDRED2, 0);
                        digitalWrite(PEDGREEN1, 1);
                        digitalWrite(PEDGREEN2, 1);
                        delay(3000);
                        for (int j = 0; j < 3; j++) {
                            digitalWrite(PEDGREEN1, 0);
                            digitalWrite(PEDGREEN2, 0);
                            delay(500);
                            digitalWrite(PEDGREEN1, 1);
                            digitalWrite(PEDGREEN2, 1);
                            delay(500);
                        }
                        digitalWrite(PEDGREEN1, 0);
                        digitalWrite(PEDGREEN2, 0);
                        digitalWrite(PEDRED1, 1);
                        digitalWrite(PEDRED2, 1);
                        delay(1000);
                        digitalWrite(RED2, 0);
                        digitalWrite(GREEN2, 1);
                        delay(5000);
                        digitalWrite(GREEN2, 0);
                        digitalWrite(YELLOW2, 1);
                        delay(3000);
                        digitalWrite(YELLOW2, 0);
                        digitalWrite(RED2, 1);
                        delay(1000);
                        digitalWrite(RED1, 0);
                        digitalWrite(GREEN1, 1);
                        delay(5000);
                        digitalWrite(GREEN1, 0);
                        digitalWrite(YELLOW1, 1);
                        delay(5000);
                        digitalWrite(YELLOW1, 0);
                        temp = 3000 - temp;
                        delay(temp);
                        digitalWrite(RED1, 1);
                    }
                    mfrc522.uid.uidByte[i] = 0;
                }
                //Serial.print("\t");
                mfrc522.PICC_HaltA();
            }
        }
    }
}
void red(int count)
{
    if (count == 1) {
        delay(50);
        digitalWrite(GREEN1, 0);
        digitalWrite(YELLOW1, 1);
        delay1(1000);
        digitalWrite(YELLOW1, 0);
        digitalWrite(RED1, 1);
    status=1;
        delay1(3000);
        digitalWrite(RED2, 0);
        digitalWrite(GREEN2, 1);
        delay1(5000);
    }
    if (count == 2) {
        delay(50);
        digitalWrite(GREEN2, 0);
        digitalWrite(YELLOW2, 1);
        delay1(1000);
        digitalWrite(YELLOW2, 0);
        digitalWrite(RED2, 1);
    status=2;
        delay1(3000);
        digitalWrite(RED1, 0);
        digitalWrite(GREEN1, 1);
        delay1(5000);
    }
}
void loop()
{
    if (i == 1) {
        red(i);
        i++;
    }
    if (i == 2) {
        red(i);
        i--;
    }
}*/

