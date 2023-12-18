#include <Wire.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
char mensaje[30];

void setup()
{
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);

  lcd.begin(16, 2);
  lcd.print("Equipo 06");
  lcd_conf();
  recibir_datos();
  Serial.begin(9600);
  Serial.println("Arduino Esclavo #1\n");
  analogWrite(13, 255);
  analogWrite(12, 0);
  analogWrite(11, 255);
  delay(1000);
  analogWrite(13, 0);
  analogWrite(12, 0);
  analogWrite(11, 0);
}

void loop()
{
}

void recepcionEvento(int num)
{
  for (int i = 0; i < num; i++)
  {
    mensaje[i] = Wire.read();
  }
  mensaje[num] = '\0';
  // este if chequea si se recibe un =, si pasa esto lo muestra en el display, pasa a la linea 2 y prende el led en cyan
  if (mensaje[0] == '=')
  {
    lcd.print(mensaje);
    lcd.setCursor(0, 1);
    analogWrite(13, 0);
    analogWrite(12, 255);
    analogWrite(11, 255);
    delay(100);
  }
  else if (mensaje[0] == 'Infinito')
  {
    lcd.print("Infinito");
    lcd.setCursor(0, 1);
    analogWrite(13, 0);
    analogWrite(12, 255);
    analogWrite(11, 255);
    delay(100);
  }
  else if (mensaje[0] == 'C')
  { // si se recibe C se borra lo que haya en el display y se apaga el led
    lcd.clear();
    analogWrite(13, 0);
    analogWrite(12, 0);
    analogWrite(11, 0);
  }
  else
  {
    lcd.print(mensaje);
    delay(100);
  }
  analogWrite(13, 255); // cada vez que se imprime algo se prende el led en rojo
  delay(100);
  analogWrite(13, 0);
}

void recibir_datos()
{
  Wire.begin(1);
  Wire.onReceive(recepcionEvento);
}

void lcd_conf()
{ // configuracion del led
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.blink();
}
