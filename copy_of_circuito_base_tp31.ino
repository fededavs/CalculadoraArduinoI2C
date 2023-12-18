#include <Wire.h>
#include <Keypad.h>

#define filas 4
#define columnas 4

byte pinFilas[] = {11, 10, 9, 8}; // configuracion de pines de filas
byte pinCol[] = {7, 6, 5, 4};     // configuracion de pines de columnas
char teclas[4][4] = {             // configuracion de caracteres del teclado
    {'1', '2', '3', '+'},         // A: suma
    {'4', '5', '6', '-'},         // B: resta
    {'7', '8', '9', '*'},         // C: multiplicacion
    {'C', '0', '=', '/'}};        // D: division, #: resultado, *: borrar

Keypad teclado = Keypad(makeKeymap(teclas), pinFilas, pinCol, filas, columnas); // asignacion de teclas

char pulsacion; // pulsacion de la tecla
char operacion_ing;
char num_ing1[6];
char num_ing2[6];
char numero_resultado[11], rta[6] = "Rta: ";
char numero_div[11], inf[9] = "Infinito";
long numero1, numero2;
int contador = 0;
bool aux = LOW;
float resultado = 0;
float resultado_div = 0;

void setup()
{
  Wire.begin(); // inicializar I2C como maestro
  Serial.begin(9600);
  Serial.println("Arduino Maestro\n");
}

void loop()
{
  pulsacion = teclado.getKey(); // se obtiene lo presionado en el keypad
  if (pulsacion != NO_KEY)
  {
    if (pulsacion >= '0' && pulsacion <= '9')
    { // este if se fija que lo ingresado este entre 1 o 9, despues un else if se fija si lo ingresado es un = o una operacion
      if (contador < 4)
      {
        Wire.beginTransmission(1);
        Wire.write(pulsacion); // si el contador es menor a 4 y es un numero, se envia al arduino esclavo para mostrarlo por el display
        Wire.endTransmission();

        if (!aux)
        { // esto llena un string de los numeros ingresados y cuando el aux cambia se llena otro string para poder separar los 2 numeros ingresados
          num_ing1[contador] = pulsacion;
        }
        else
        {
          num_ing2[contador] = pulsacion;
        }
        contador++;
        delay(100);
        Serial.print(pulsacion);
      }
    }
    else if (pulsacion == '=')
    { // este if se fija si se ingreso un =
      Wire.beginTransmission(1);
      Wire.write(pulsacion); // si se ingreso lo manda al esclavo para que este cambie a la linea 2 y se prenda el led en color cyan
      Wire.endTransmission();
      numero1 = atoi(num_ing1); // pasar los enteros a caracteres
      numero2 = atoi(num_ing2);

      switch (operacion_ing)
      {
      case '+':
        aux = LOW;
        resultado = numero1 + numero2;
        dtostrf(resultado, 10, 0, numero_resultado); // funcion para pasar flotantes a string
        Wire.beginTransmission(1);                   // enviar resultado al esclavo
        Wire.write(rta);
        Wire.write(numero_resultado); // marcar como resultado
        Wire.endTransmission();
        Serial.println("\n");
        Serial.println(rta);
        Serial.println(numero_resultado);
        Serial.print("\n");
        break;
      case '-':
        aux = LOW;
        resultado = numero1 - numero2;
        dtostrf(resultado, 10, 0, numero_resultado);
        Wire.beginTransmission(1);
        Wire.write(rta);
        Wire.write(numero_resultado);
        Wire.endTransmission();
        Serial.println("\n");
        Serial.println(rta);
        Serial.println(numero_resultado);
        Serial.print("\n");
        break;
      case '*':
        aux = LOW;
        resultado = numero1 * numero2;
        dtostrf(resultado, 10, 0, numero_resultado);
        Wire.beginTransmission(1);
        Wire.write(rta);
        Wire.write(numero_resultado);
        Wire.endTransmission();
        Serial.println("\n");
        Serial.println(rta);
        Serial.println(numero_resultado);
        Serial.print("\n");
        break;
      case '/':
        aux = LOW;
        resultado_div = (float)numero1 / (float)numero2; // convertir enteros a flotantes
        if (numero2 == 0)
        {
          Wire.beginTransmission(1);
          Wire.write(inf);
          Wire.endTransmission();
          Serial.println("\n");
          Serial.println(inf);
          Serial.print("\n");
        }
        else
        {
          dtostrf(resultado_div, 10, 2, numero_div);
          Wire.beginTransmission(1);
          Wire.write(rta);
          Wire.write(numero_div);
          Wire.endTransmission();
          Serial.println("\n");
          Serial.println(rta);
          Serial.println(resultado_div);
          Serial.print("\n");
        }
        break;
      }
    }
    else if (pulsacion == 'C')
    {
      aux = LOW;
      contador = 0;
      reiniciarString(); // llamada a la funcion para reiniciar el string
      Wire.beginTransmission(1);
      Wire.write(pulsacion);
      Wire.endTransmission();
      Serial.println("--Nueva cuenta--");
    }
    else
    { // este else solo se activa si lo que se apreto en el keypad no es ni un numero ni un igual, osea una operacion
      contador = 0;
      operacion_ing = pulsacion; // guardo la operacion para despues usarla en el switch case
      aux = HIGH;
      Serial.print(operacion_ing);
      Wire.beginTransmission(1);
      Wire.write(pulsacion); // mando la operacion ingresada para que se muestre en el display
      Wire.endTransmission();
    }
  }
}

void reiniciarString()
{
  operacion_ing = '0';                   // reiniciar la variable de operacion
  memset(num_ing1, 0, sizeof(num_ing1)); // reiniciar num_ing1
  memset(num_ing2, 0, sizeof(num_ing2)); // reiniciar num_ing2
}