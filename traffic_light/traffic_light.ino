 int RED1 =13,YELLOW1 =12,GREEN1 =11,PEDRED1 =7, PEDGREEN1 =6,RED2 =10,YELLOW2 =9,GREEN2 =8,PEDRED2 =5,PEDGREEN2 =4,i=1;
 unsigned long previousMillis = 0;
 unsigned long currentMillis = millis();
 //unsigned long interval = 1000;
 void setup() {
   Serial.begin(9600);
   pinMode(RED1 , OUTPUT);
   pinMode(YELLOW1 , OUTPUT);
   pinMode(GREEN1 , OUTPUT);
   pinMode(RED2 , OUTPUT);
   pinMode(YELLOW2 , OUTPUT);
   pinMode(GREEN2 , OUTPUT);
   digitalWrite(RED1,1);
   digitalWrite(RED2,1);
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
     Serial.print(currentMillis);Serial.print("\t");Serial.print(interval);Serial.print("\t");Serial.print(previousMillis);Serial.print("\t");Serial.print(currentMillis - previousMillis);Serial.print("\n");
     currentMillis = millis();
     if(currentMillis - previousMillis >= interval) {
       previousMillis = currentMillis;break;
     }
     else{
       Serial.print("\nasdfdsa\n");
     }
   }
 }
 void red(int count){
   if(count==1){
     digitalWrite(GREEN1,0);digitalWrite(YELLOW1,1);delay1(1000);
     digitalWrite(YELLOW1,0);digitalWrite(RED1,1);delay1(3000);
     digitalWrite(RED2,0);digitalWrite(GREEN2,1);delay1(5000);
   }
   if(count==2){
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
 
