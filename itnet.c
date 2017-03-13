#include <LiquidCrystal.h>
#include <SD.h>

int odpocitavani = 0;
float cas = 30;
int heslo = 7755332200;
int alarm = false;
bool sejmuto = false;
bool odpocitavat = false;
bool cistsc = false;
int tlacitko = 0;
int sdo = 13;
int sd = 53;
int sensor = 49;
int piezo = 4;
int cervena = 2;
int zelena = 3;
int bila = 5;
int modra = 6;
int test;
String text;
LiquidCrystal LCD(7, 8, 9, 10, 11, 12);


void setup()
{
  Serial.begin(9600);
  LCD.begin(20, 4);
  LCD.setCursor(0, 0);
  //SD.begin(sd);
  pinMode(sdo, OUTPUT);
  pinMode(sensor, INPUT);
  pinMode(piezo, INPUT);
  pinMode(cervena, OUTPUT);
  pinMode(zelena, OUTPUT);
  pinMode(bila, OUTPUT);
  pinMode(modra, OUTPUT);
  LCD.print("Vitejte!");
  digitalWrite(cervena, HIGH);
}

void loop()
{
  LCD.setCursor(0, 1);
  LCD.print("V poradku.     ");
  int sejmul = digitalRead(sensor);
  if (sejmul)
  {
    sejmuto = true;
  }
  if (!sejmuto)
  {
    LCD.setCursor(0, 2);
    LCD.print(" ");
    digitalWrite(bila, LOW);
    digitalWrite(modra, LOW);
  }
  else
  {
    tone(piezo, 1023);
    delay(1000);
    noTone(piezo);
    odpocitavat = true;
  }
  if (odpocitavat)
  {
    LCD.setCursor(0, 1);
    LCD.print("Odpocitavani: ");
    LCD.setCursor(0, 2);
    LCD.print("Vlozte SD s pinem");
    for (int i = 30;i >= 0;i--)
	{
		if(ctiSD()==true)					//
			{
				odpocitavani=1				//uprava
				odpocitavat = false;		//8.18
				sejmuto = false;
				break;						//
			}
		LCD.setCursor(14, 1);
		LCD.print(i);
		LCD.print(" ");
		delay(1000);
	}

    if (odpocitavani == 0)
      {
        odpocitavat = false;
        sejmuto = false;
        alarm = true;
      }
      
  }
  if (alarm)
  {
    do
    {
    tone(piezo, 900);
    digitalWrite(modra, HIGH);
    digitalWrite(bila, LOW);
    delay(150);
    digitalWrite(modra, LOW);
    digitalWrite(bila, HIGH);
    delay(150);
    } while(alarm == true);
  }
  delay(1000);
}

bool ctiSD()
{
    int tlacitkoHodnota = digitalRead(22);
    if (tlacitkoHodnota == HIGH)
    {
        digitalWrite(sdo, HIGH);

//uprava start
        int timeout = 999;//nastav si jaky chces, ale mel by byt mensi nez 1000 (1sec)
        while(!SD.begin(sd))
        {
            if(--timeout==0)
            {
                LCD.setCursor(0, 3);
                LCD.print("Karta neindikovana  ");
                break;
            }
            delay(1);
        }
        if(timeout==0)return false;//kdyz nenajde kartu vrati se zpet
        //nebo pokracuje
//uprava end
        File soubor = SD.open("pin.txt");
        if (soubor)
        {
            while(soubor.available())
            {
                text = char(soubor.read());
            }
            if (text == heslo)
            {
                digitalWrite(cervena, LOW);
                digitalWrite(zelena, HIGH);
                odpocitavat = false;
                alarm = false;
                sejmuto = false;
                LCD.setCursor(0, 3);
                LCD.print("Karta prijata.     ");
                return true;//uprava
            }
            else
            {
                LCD.setCursor(0, 3);
                LCD.print("Karta odmitnuta.");
                return false;//uprava
            }
        }
    }
}
