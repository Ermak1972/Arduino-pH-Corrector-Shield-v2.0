
/*
USE_GITHUB_USERNAME=cyberplanru
*/
// CiberPlant Inc
// http://www.cyberplant.ru
//Пример кода для Arduino pH Corrector Shield v2.0
//Программа позволяет управлять уровнем pH в автоматическом режиме
//Устройство считывает показания с pH зонда и подает сигналы на двва перильстатических насоса
//для подачи в раствор корректирующей жидкости

#include <LiquidCrystal.h> //Библиотека LCD
#include <EEPROMex.h> // расширенная библиотека EEPROM 
#include <SoftwareSerial.h>  //we have to include the SoftwareSerial library, or else we can't use it.  
#define rx 5                     //pin 5 для работы с pH Circuit
#define tx 3                      //pin 3 для работы с pH Circuit
#define pumpPhup 7  // насос pH Up 
#define pumpPhdown 6 // насос pH Down
SoftwareSerial myserial(rx, tx); 

LiquidCrystal lcd(8, 9, 10, 11, 12, 13);  // подключение LCD дисплея
int m=0; //переменная для экранов меню

// адраса хранения параметров в энергонезависимой памяти Arduino:
int addressPhlow = 0;
int addressPhhigh = addressPhlow+sizeof(double);
int addressDose = addressPhhigh+sizeof(double);
int addressPhdelay = addressDose+sizeof(double);
int addressL = addressPhdelay+sizeof(int);
int addressR = addressL+sizeof(int);

//  Считывание параметров из энергонезависимой памяти:
double pHlow = EEPROM.readDouble(addressPhlow);
double pHhigh = EEPROM.readDouble(addressPhhigh);
double dose = EEPROM.readDouble(addressDose);
int pHdelay = EEPROM.readInt(addressPhdelay);
int l = EEPROM.readInt(addressL);
int c = 0;
int r = EEPROM.readInt(addressR);

// массивы для меню
char* ledString[2] = {"On", "Off"};
char* control[3] = {"pH 4", "pH 7", "pH 10"};
char* mode[2] = {"auto", "manual"};

int flag=0; // переменная для стабилизации кнопки

long previousMillis = 0; 
long interval = pHdelay * 60000;  //задержка корректировки в минутах

byte setPin = A0; //кнопка сохранения в энергонезависимую память
byte menuPin = A1; //кнопка переключения пунктов меню
byte upPin = A3; //увеличение значения отображаемого параметра
byte downPin = A2;//уменьшение значения отображаемого параметра

char ph_data[20];                  //we make a 20 byte character array to hold incoming data from the pH. 
char computerdata[20];             //we make a 20 byte character array to hold incoming data from a pc/mac/other. 
byte received_from_computer=0;     //we need to know how many characters have been received.                                 
byte received_from_sensor=0;       //we need to know how many characters have been received.
byte arduino_only=0;               //if you would like to operate the pH Circuit with the Arduino only and not use the serial monitor to send it commands set this to 1. The data will still come out on the serial monitor, so you can see it working.  
byte startup=0;                    //used to make sure the Arduino takes over control of the pH Circuit properly.
float ph=0;                        //used to hold a floating point number that is the pH. 
byte string_received=0;  
void phSetLEDs(byte enabled);

void setup() {

  //Установка пинов кнопок как входов
  pinMode(menuPin, INPUT);
  pinMode(setPin, INPUT);
  pinMode(upPin, INPUT);
  pinMode(downPin, INPUT);
  //Установка пинов для насосов как выходов
  pinMode (pumpPhup, OUTPUT);
  pinMode (pumpPhdown, OUTPUT);
  
     Serial.begin(38400);         //enable the hardware serial port
     myserial.begin(38400); 
     
  //Установка количества столбцов и строк дисплея
  lcd.begin(8, 2);
  // Вывод приветствия
  lcd.print("ver 2.0");
  // задержка
  delay (500);
  lcd.clear();
}


void loop() {
    if(myserial.available() > 0){    //if we see that the pH Circuit has sent a character.
     received_from_sensor=myserial.readBytesUntil(13,ph_data,20); //we read the data sent from ph Circuit untill we see a <CR>. We also count how many character have been recived.  
     ph_data[received_from_sensor]=0; //we add a 0 to the spot in the array just after the last character we recived. This will stop us from transmiting incorrect data that may have been left in the buffer. 
     string_received=1;                //a flag used when the arduino is controlling the pH Circuit to let us know that a complete string has been received.

    } 
    
      if (digitalRead(menuPin)== LOW&&flag==1)  // стабилизация кнопки
  {
  flag=0;
  }


  //Обработка нажатия кнопки меню
  if (digitalRead(menuPin)== HIGH&&flag==0)
  {
  m++;//увеличиваем переменную уровня меню
  flag=1;
  if (m>7)//если уровень больше 3
  {
  m=0;// то вернуться к началу
  }

  delay (100);
  lcd.clear();
  
 }
  //Увелечение парметра pHlow
if (digitalRead(upPin)== HIGH && m==1)
  {

  pHlow += 0.1;

  if (pHlow>14)
  {
  pHlow=1;
  }
  delay (100);
  lcd.clear();
  }
  if (digitalRead(setPin)==HIGH && m==1){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SAVE");
  lcd.setCursor(0, 1);
  lcd.blink();
  delay(3000);   // сохранение в энергонезависимую память
  EEPROM.updateDouble(addressPhlow, pHlow); 
  lcd.noBlink();
  lcd.print("complit");
  delay(2000);
  lcd.clear();
  }
  


if (digitalRead(upPin)== HIGH && m==2) //+ для рHhigh
  {
  pHhigh += 0.1;
  if (pHhigh>14)
  {
  pHhigh=14;
  }
  delay (100);
  lcd.clear();
  }
  if (digitalRead(setPin)==HIGH && m==2){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SAVE");
  lcd.setCursor(0, 1);
  lcd.blink();
  delay(3000);   // сохранение в энергонезависимую память
  EEPROM.updateDouble(addressPhhigh, pHhigh); 
  lcd.noBlink();
  lcd.print("complit");
  delay(2000);
  lcd.clear();

  }
  
    //+ для dose
    if (digitalRead(upPin)== HIGH && m==3)
  {
  dose += 0.1;
  if (dose>10)//если дозировка достигла придела в 10 мл
  {
  dose=10;//то возвращаем ее к 0
  }
  delay (100);
  lcd.clear();
  }
  if (digitalRead(setPin)==HIGH && m==3){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SAVE");
  lcd.setCursor(0, 1);
  lcd.blink();
  delay(3000);   // сохранение в энергонезависимую память
  EEPROM.updateDouble(addressDose, dose); 
  lcd.noBlink();
  lcd.print("complit");
  delay(2000);
  lcd.clear();
  }
  //для pHdelay
      if (digitalRead(upPin)== HIGH && m==4)
  {
  pHdelay++;
  if (pHdelay>60)//если переменная достигла придела в 60 минут
  {
  pHdelay=0;//то возвращаем ее к 0
  }
  delay (100);
  lcd.clear();
  }
  if (digitalRead(setPin)==HIGH && m==4){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SAVE");
  lcd.setCursor(0, 1);
  lcd.blink();
  delay(3000);   // сохранение в энергонезависимую память
  EEPROM.updateInt(addressPhdelay, pHdelay); 
  lcd.noBlink();
  lcd.print("complit");
  delay(2000);
  lcd.clear();
  }
  // сигнализация светодиодом
      if (digitalRead(upPin)== HIGH && m==5)
  {
  l++;
  if (l>1)
  {
  l=1;
  }
  
  delay (100);
  lcd.clear();
  }
  if (digitalRead(setPin)==HIGH && m==5){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SAVE");
  lcd.setCursor(0, 1);
  lcd.blink();
  delay(3000);   // сохранение в энергонезависимую память
  EEPROM.updateInt(addressL, l); 
  lcd.noBlink();
  lcd.print("complit");
  delay(2000);
  lcd.clear();
  }
  //каллибровка
        if (digitalRead(upPin)== HIGH && m==6)
  {
  c++;
  if (c>2)
  {
  c=2;
  }
  delay (100);
  lcd.clear();
  }
  
  if (digitalRead(setPin)==HIGH && m==6)
  {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(control[c]);
  lcd.setCursor(0, 1);
  lcd.blink();
  delay(3000);
  if (c==0)
  {
    myserial.write("f\r"); //calibrate to a pH of 4 
  }
    if (c==1)
  {
    myserial.write("s\r"); //calibrate to a pH of 7
  }
      if (c==2)
  {
    myserial.write("t\r"); //calibrate to a pH of 10.00
  }
    
  lcd.noBlink();
  lcd.print("complit");
  delay(2000);
  lcd.clear();
  }
  
  
  // режим работы
          if (digitalRead(upPin)== HIGH && m==7)
  {
  r++;
  if (r>1)
  {
  r=1;
  }
  delay (100);
  lcd.clear();
  }
  if (digitalRead(setPin)==HIGH && m==7){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SAVE");
  lcd.setCursor(0, 1);
  lcd.blink();
  delay(3000);   // сохранение в энергонезависимую память
  EEPROM.updateInt(addressR, r);
  lcd.noBlink();
  lcd.print("complit");
  delay(2000);
  lcd.clear();
  }
  
  
      //уменьшение значений для pH Low
    if (digitalRead(downPin)== HIGH && m==1)
  {
  pHlow -= 0.1;
  if (pHlow<1)
  {
  pHlow=1;

  }
  delay (100);
  lcd.clear();

  }
  //pH High
      if (digitalRead(downPin)== HIGH && m==2)
  {
  pHhigh -= 0.1;
  if (pHhigh<1)
  {
  pHhigh=1;
  }
  delay (100);
  lcd.clear();

  }
      //- для dose
    if (digitalRead(downPin)== HIGH && m==3)
  {
  dose -= 0.1;
  if (dose<0)
  {
  dose=0;
  }
  delay (100);
  lcd.clear();

  }

   if (digitalRead(downPin)== HIGH && m==4)//для задержки
  {
  pHdelay--;
  if (pHdelay<1)
  {
  pHdelay=1;
  }
  delay (100);
  lcd.clear();

  }
if (digitalRead(downPin)== HIGH && m==5) // для светодиода
  {
  l--;
  if (l<0)
  {
  l=0;
  }
  
  delay (100);
  lcd.clear();

  }
  //каллибровка
        if (digitalRead(downPin)== HIGH && m==6)
  {
  c--;
  if (c<0)
  {
  c=0;
  }
  delay (100);
  lcd.clear();
  }
  
  
  
  
  // режим работы
          if (digitalRead(downPin)== HIGH && m==7)
  {
  r--;
  if (r<0)
  {
  r=0;
  }
  delay (100);
  lcd.clear();

  }


  
  //вывод меню
  if (m==0)
  {
  lcd.setCursor(0, 0);
  lcd.print("pH ");
  lcd.print(ph_data);
  lcd.setCursor(0, 1);
  lcd.print("");
  lcd.print(mode[r]);


  } 
  else if (m==1)
  {
  lcd.setCursor(0, 0);
  lcd.print("pH low");
  lcd.setCursor(0, 1);
  lcd.print("");
  lcd.print(pHlow);
  }
  
 else if (m==2)
  {
  lcd.setCursor(0, 0);
  lcd.print("pH high");
  lcd.setCursor(0, 1);
  lcd.print("");
  lcd.print(pHhigh);
  }
 else if (m==3)
  {
  lcd.setCursor(0, 0);
  lcd.print("dose");
  lcd.setCursor(0, 1);
  lcd.print(dose);
  lcd.print(" ml");
  }
  else if (m==4)
  {
  lcd.setCursor(0, 0);
  lcd.print("delay");
  lcd.setCursor(0, 1);
  lcd.print(pHdelay);
  lcd.print(" min");

  }
  else if (m==5)
  {
  lcd.setCursor(0, 0);
  lcd.print("LED");
  lcd.setCursor(0, 1);
  lcd.print(ledString[l]);
  }
   else if (m==6)
  {
  lcd.setCursor(0, 0);
  lcd.print("setting");
  lcd.setCursor(0, 1);
  lcd.print(control[c]);
  }
  else if (m==7)
  {
  lcd.setCursor(0, 0);
  lcd.print("mode");
  lcd.setCursor(0, 1);
  lcd.print(mode[r]);
  }
  
      if (l==1)  // если выбрано "LED off" - выключаем светодиод
    {
  myserial.write("L0\r");
    }
    
    
  unsigned long currentMillis = millis();   // для считывания интервала pH Delay
  if(currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;
if (pHlow>atof(ph_data) && r==0) // если выбран автоматический режим и уровень pH опустился 
                                 // ниже параметра pH Low
{

  digitalWrite (pumpPhup, HIGH); // включаем насос pH UP
  delay (dose*3000);             // выбранная дозировка * на коофициент производительности насоса 
                                 // здесь 1 мл прокачивается за 3 секунды
  digitalWrite (pumpPhup, LOW);  // выключаем насос
  }
       



  if (pHhigh<atof(ph_data) && r==0) // если выбран автоматический режим и уровень pH поднялся 
                                    // выше параметра pH High
{

  digitalWrite (pumpPhdown, HIGH);
  delay (dose*3000);
  digitalWrite (pumpPhdown, LOW);
  }
  if (l==0 && pHhigh<atof(ph_data) || pHlow>atof(ph_data)) // Если выбран параметр "LED on"
                                                           // то при выходе за указанные параметры pH
                                                           // включается светодиод
    {
    myserial.write("L1\r");
    }
    if (pHhigh>atof(ph_data) && pHlow<atof(ph_data) && l==0)  // Если уровень pH находится в заданных пределах, 
                                                              // светодиод выключается
    {
    myserial.write("L0\r");
    }
       


  }


  
}

