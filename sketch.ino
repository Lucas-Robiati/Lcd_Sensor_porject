#include<DHT.h>
#include<LiquidCrystal.h>

#define DHTPIN 3
#define DHTTYPE 22
DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal lcd(12, 11, 7, 6, 5, 4);

#define buzzer 10
float temp = 0;
boolean press = false;

void setup() 
{
  DDRD = DDRD & ~(1 << DDD2);
  PORTD = PORTD | (1 << PD2);
  EICRA = (EICRA | (1 << ISC01)) & ~(1 << ISC00);
  EIMSK = EIMSK | (1 << INT0);

  Serial.begin(115200);
  lcd.begin(16,2);
  dht.begin();
  DDRB = 0X1B;

}

void loop() 
{
  float t = dht.readTemperature();
  imprimeT(t);
  imprimeLCD(t);
  perigo(t);
}

//funções

ISR(INT0_vect)
{
  press = !press;
}

void imprimeT(float t)
{
  if(isnan(t))
  {
    Serial.println("Falha na leitura do DHT22");
    delay(1000);
  }
  else
  {
    Serial.print("Temperatura: ");

    if(press)
    {
      Serial.print(t*1.8+32);
      Serial.print("*F \n");
      delay(1000);
    }
    else
    {
      Serial.print(t);
      Serial.print("*C \n");
      delay(1000);
    }
  }
 
}

void imprimeLCD(float t)
{
  
  if(press)
  {
    float fahrenheit = (t*1.80)+32.00;
    temp = 0;

    if(fahrenheit != temp)
    {
      lcd.clear();
      lcd.print(fahrenheit);
      lcd.print("\xDF");
      lcd.print("F");
      temp = fahrenheit;
    }
  }
  else
  {
    if(t != temp)
    {
      lcd.clear();
      lcd.print(t);
      lcd.print("\xDF");
      lcd.print("C");
      temp = t;
    }
  }
}

void perigo(float t)
{
  if((t > 45.00)||(t < 0))
  {
    PORTB = PORTB & ~(1 << PB1);
    PORTB = PORTB | (1 << PB0);
    tone(buzzer, 330);
  }
  else
  {
    PORTB = PORTB & ~(1 << PB0);
    PORTB = PORTB | (1 << PB1);
    noTone(buzzer);
  }
}
