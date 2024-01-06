//для точки
#define thk 19 //пин точки
#define GAMES 4
//--для семисегменты--
#define oe 8  // пин данных
#define st_cp 7  // пин защелки
#define sh_cp 6  // пин тактов синхронизации

int doids[5] = {14,15,16,17,18};
int buts[5] = {13,12,11,10,9};

byte push[] = {10, 11, 12, 13}; //это то что на экран покажется
//byte digit[] = {1,2,4,8}; // 
//byte seq[] = {63,6,91,79,102,109,125,7,255,111};

byte razriad_vkl[] = {4, 3, 2, 5}; //пины разрядов
//{A, B, C, D, E, F, G,DP}
byte seg[15][8] = {
  {1, 1, 1, 1, 1, 1, 0, 0}, //Цифра 0
  {0, 1, 1, 0, 0, 0, 0, 0}, //Цифра 1
  {1, 1, 0, 1, 1, 0, 1, 0}, //Цифра 2
  {1, 1, 1, 1, 0, 0, 1, 0}, //Цифра 3
  {0, 1, 1, 0, 0, 1, 1, 0}, //Цифра 4
  {1, 0, 1, 1, 0, 1, 1, 0}, //Цифра 5
  {1, 0, 1, 1, 1, 1, 1, 0}, //Цифра 6
  {1, 1, 1, 0, 0, 0, 0, 0}, //Цифра 7
  {1, 1, 1, 1, 1, 1, 1, 0}, //Цифра 8
  {1, 1, 1, 1, 0, 1, 1, 0}, //Цифра 9
  {1, 1, 0, 0, 1, 1, 1, 0}, //P
  {0, 1, 1, 1, 1, 1, 0, 0}, //U
  {1, 0, 1, 1, 0, 1, 1, 0}, //S
  {0, 1, 1, 0, 1, 1, 1, 0}, //H
  {0, 0, 0, 0, 0, 0, 0, 0} // " "
};
int pause = 1; //пауза для динамики

//для помигать
unsigned long timer = 0;
bool sost = false;

bool game = false;


//void(* resetFunc) (void) = 0; // объявляем функцию reset

void setup()
{
  //Serial.begin(9600);
  pinMode(oe, OUTPUT);
  pinMode(st_cp, OUTPUT);
  pinMode(sh_cp, OUTPUT);
  randomSeed(analogRead(A5));
  pinMode(thk, OUTPUT);
  ////Serial.println("doid");
  for(int i=0; i < sizeof(doids)/sizeof(doids[0]); i++){
    pinMode(doids[i], OUTPUT);
    pinMode(buts[i],INPUT_PULLUP);
  }
  for (int i = 0; i < 4; i++)pinMode(razriad_vkl[i], OUTPUT);

}

unsigned long lastMillis =0;
unsigned long step = 10;
unsigned long count =0;
int lastLed = sizeof(doids)/sizeof(doids[0])-1;
int led = 0;

unsigned long waitStartTimer = 0;
unsigned long waitStart = 1000;

int countGames = GAMES;

void loop()
{
 
 ////Serial.print("game "); //Serial.println(game);
  if(game){
    ////////Serial.print("game "); //////Serial.println(led);
    if(millis() - lastMillis >= step){
      lastMillis=millis();
      count++;
       //Serial.println(count);
       dig_show(count);
      //showNum(count,3);
      for(int i=0; i < sizeof(buts)/sizeof(buts[0]);i++){
        if(!digitalRead(buts[i])){
         
          //Serial.print("i= "); //Serial.print(i); //Serial.print(" game "); //Serial.println(buts[i]);
          delay(5);
          if(i != led) continue; 
          if(countGames <= 0){
            game = false;
             for(int i=0; i < sizeof(doids)/sizeof(doids[0]);i++){
              digitalWrite(doids[i], LOW);
             }
          } 
          else countGames--;
          unsigned long pause = 1000;
          if(!game) pause = 5000;
          
          
          while(millis() - lastMillis < pause){
            dig_show(count);
            
          }
           Randomizer();
         
        } 
      }
    }
    
  }
  else{

   // //Serial.println("zdes");
    //delay(1000);
    //dig_show();
    showPUSH();
    for(int i=0; i < sizeof(buts)/sizeof(buts[0]); i++){
      //if(millis() - waitStartTimer > 500) game = false;
      if(!digitalRead(buts[i])){
        // //Serial.println(buts[i]); //Serial.println(" showPUSH");
        countGames = GAMES;
        game = true;
        count = 0;
        lastMillis = millis();
        delay(300);
        Randomizer();
        return;
      }

    }
    if(millis() - lastMillis> 500){
     ////Serial.print(led);  //////Serial.print("   "); //////Serial.print(doids[led]);
      
        lastMillis = millis();
        digitalWrite(doids[led], HIGH);
        digitalWrite(doids[lastLed], LOW);
        lastLed = led;
        led++;
        if(led> sizeof(doids)/sizeof(doids[0])-1) led = 0;
      
    }
  }
  
  
}


//ф-ция отрисовка семисегмента
void dig_show(int num) {
  int razriad[4];
  if(num>=10000) {
    game = false;
    return;
  }
  byte four, three, two,one;
  if(num >=1000){
    four = num/1000;
    razriad[0] = four;
    num = num%1000;
    three = num/100;
    razriad[1] = three;
    num = num%100;
    two = num/10;
    one = num%10;
    razriad[2] = two;
    razriad[3] = one;    
  }else if(num>99){
    razriad[0] = 14;
     three = num/100;
    razriad[1] = three;
    num = num%100;
    two = num/10;
    one = num%10;
    razriad[2] = two;
    razriad[3] = one;
  }else if(num >9){
    razriad[0] = 14;
    razriad[1] = 14;
    two = num/10;
    one = num%10;
    razriad[2] = two;
    razriad[3] = one;
  } else{
    razriad[0] = 14;
    razriad[1] = 14;
    razriad[2] = 14;
    razriad[3] = num;
  }

  //рисуем число
  for (int d = 0; d < 4; d++) {

    
    digitalWrite(st_cp, 0);
    //пишем число
    for (int i = 7; i >= 0; i--) {
      digitalWrite(sh_cp, 0);
      digitalWrite(oe, seg[razriad[3 - d]][i]);

      //дергаем такт sh
      digitalWrite(sh_cp, 1);
    }
    //когда протолкнули
    digitalWrite(st_cp, 1);
    //включаем разряд
 
    digitalWrite(razriad_vkl[d], 1);
    delay(pause);
    //выключаем разряд
    digitalWrite(razriad_vkl[d], 0);
    //delay(1);
  }

  if (millis() - timer > 1000){
    timer = millis();
    digitalWrite(thk, sost);
    sost = !sost;
  }
  
}



void showPUSH(){
//Serial.println("PUSH");
//рисуем число
digitalWrite(thk, LOW);
  for (int d = 0; d < 4; d++) {

    
    digitalWrite(st_cp, 0);
    //пишем число
    for (int i = 7; i >= 0; i--) {
      digitalWrite(sh_cp, 0);
      digitalWrite(oe, seg[push[3 - d]][i]);
      //дергаем такт sh
      digitalWrite(sh_cp, 1);
    }
    //когда протолкнули
    digitalWrite(st_cp, 1);
    //включаем разряд
 
    digitalWrite(razriad_vkl[d], 1);
    delay(pause);
    //выключаем разряд
    digitalWrite(razriad_vkl[d], 0);
    //delay(1);
  }
}

void Randomizer(){
 	led = random(0, sizeof(doids)/sizeof(doids[0]));
  	for(int i =0; i<sizeof(doids)/sizeof(doids[0]);i++){
 	 digitalWrite(doids[i], LOW);
	}
	digitalWrite(doids[led], HIGH); 
}



