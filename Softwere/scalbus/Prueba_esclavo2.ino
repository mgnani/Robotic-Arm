#include <Servo.h>
#define pull LOW
#define CantServos 6

typedef enum {nulo, moviendo, grabar, unica, multiple, borrando, calibrando} ejecucion;
ejecucion funcion;

int boton = 9;                                                                                                             //Pulsadores asociados a los pines de la placa

int tiempo2;
int tiempo1;
int angulo[CantServos];
bool activo = false;
int MovGuardado[20][CantServos];                                                                                                      //Array de 20 filas y 4 columnas para guardar las posiciones
//Variables para poder movernos por el array
int muestra = 0;
int muestraActual = 0;
int iReproducir = 0;
//Variables para añadir los vales del potenciometro al array
int buzzer = 10;
int ultimaFila = 0;                                                                                                         //para saber cual es la ultima fila utilizada
int anguloREP[CantServos];

int posicion[4];                                                                          //angulos para saber la posicion que esta en ese momento el servo
int entradaAnal[5];
Servo servoMotor[CantServos];                                                                                                                 //servomotores utilizados
const int arduino = 11;

void movimiento();
void grabando();
void repitiendo() ;
void monitoreo();
void borrar();
void calibrar();

void setup() {
  Serial.begin(9600);
  pinMode(arduino, OUTPUT);                                                                                                       //Comunicacion entre la placa y el ordenador mediante el "monitor serie"
  pinMode(boton, INPUT_PULLUP);                                                                                                   //Los pines utilizados por los pulsadores lo utilizamos como entradas
  pinMode(buzzer, OUTPUT);

  //servomotores utilizados
  servoMotor[0].attach(2);                                                                                                //Asociamos los servos a los pines donde estan colocados
  servoMotor[1].attach(3);
  servoMotor[2].attach(4);
  servoMotor[3].attach(5);
  servoMotor[4].attach(6);
  servoMotor[5].attach(7);

  for (int i = 0; i < 20; i++)                                                                                                  //inicializamos a 0 todos las celdas del array
  {
    for (int j = 0; j < 5; j++)
    {
      MovGuardado[i][j] = 0;
    }

  }
  Serial.println("Inicializado: ");
  delay(200);
  digitalWrite(buzzer, HIGH);
}
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
void loop() {
  Serial.print(funcion);
  digitalWrite(arduino, LOW);

  digitalWrite(arduino, HIGH);
  if (Serial.available() > 0) {
    funcion = Serial.read();
  }
  if(funcion!=multiple){
    activo = false;
  }
  switch (funcion) {
    case nulo: {
        //case de seguridad        
      }
      break;
    case moviendo: {
        movimiento();
      }
      break;
    case grabar: {
        grabando();
        movimiento();
      }
      break;
    case unica: {
        repitiendo();
      }
      break;
    case multiple: {
        activo = true;
        repitiendo();
      }
      break;
    case borrando: {
        borrar();
      }
      break;
    case calibrando: {
        calibrar();
      }
      break;
  }
}
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
void movimiento() {
  entradaAnal[0] = analogRead(A1);
  entradaAnal[1] = analogRead(A2);
  entradaAnal[2] = analogRead(A3);
  entradaAnal[3] = analogRead(A4);
  entradaAnal[4] = analogRead(A5);
  //---------------------------------------------
  for (int i = 0; i < 5; i++) {
    angulo[i] = map(entradaAnal[i], 0, 1023, 0, 180);
    servoMotor[i].write(angulo[i]);
    if (i == 4) {
      angulo[5] = map(entradaAnal[4], 0, 1023, 180, 0);
      servoMotor[5].write(angulo[5]);
    }
  }
  //-------------------------------------------
  Serial.println(angulo[0]);
  Serial.print(angulo[1]);
  Serial.print(angulo[2]);
  Serial.print(angulo[3]);
  Serial.print(angulo[4]);
}
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
void grabando() {
  if ((digitalRead(boton) == pull ) && (ultimaFila != 19))  //Cuando se de al pulsador de guardar posicion y no se haya excedido el numero de filas del array
  {
    Serial.println("Grabando posicion");
    for (muestra; muestra <= ultimaFila; muestra++)                                                                      //Primero iremos recorriendo cada fila
    {
      for (int servoSelec = 0; servoSelec < CantServos; servoSelec++)                                                                           //Entonces al estar en cada fila iremos pasando columna por columna
      {
        MovGuardado[muestra][servoSelec] = angulo[servoSelec];
        Serial.println(" guardada");

      }
    }
    ultimaFila += 1;                                                                                                     //Ira sumando uno cada vez que se cree una fila nueva
    //-------------------------------------------
    if (ultimaFila == 19)                                                                                               //Si llega a la ultima fila, le saldra el siguiente dialogo
    {
      Serial.println("Maximo de posiciones guardadas, no se pueden añadir mas posiciones");
    }
    for (int i = 0; i < 25; i++) {
      Serial.println("KJHKJH]");
      digitalWrite(buzzer, LOW);

    }
    digitalWrite(buzzer, HIGH);
  }
}
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
void repitiendo() {
  int SerSelect = 0;
  if (digitalRead(boton) == pull or activo == true)                                                                                  //AL pulsar el pulsador de reproducir
  {
    while (digitalRead(boton) == pull) {}
    Serial.println("Reproduciendo posicion");
    posicion[0] = angulo[0];                                                                                                 //copia los angulos que tienen los servos en el momento de reproducir, para saber donde empezar a cambiar angulos
    posicion[1] = angulo[1];
    posicion[2] = angulo[2];
    posicion[3] = angulo[3];
    posicion[4] = angulo[4];
    //--------------------------------------------------------------------------------------------------------------------------------------------------------------
    for (iReproducir = 0; iReproducir < muestra; iReproducir++)                                                          //Empezaremos a reproducir cada fila hasta que lleguemos al numero de filas que hemos guardado
    {
      for (SerSelect = 0; SerSelect < CantServos; SerSelect++)                                                               //Iremos columna por colmuna en cada fila
      {
        Serial.print("Reproduciendo base posicion: "); Serial.println(iReproducir);
        if (posicion[SerSelect] < MovGuardado[iReproducir][SerSelect])                                                             //si el angulo que hay guardado en el array es mayor que el angulo que tiene en ese momento, ocurrira esta secuencia, aumentando en grados
        {
          for (anguloREP[SerSelect] = posicion[SerSelect]; anguloREP[SerSelect] <= MovGuardado[iReproducir][SerSelect]; anguloREP[SerSelect] ++)         //ira escribiendo los valores de reproducir al servo y despues se guardara la ultima posicion donde se quedo
          {
            servoMotor[SerSelect].write(anguloREP[SerSelect]);
            posicion[SerSelect] = anguloREP[SerSelect];

          }
        }
        //--------------------------------------------------------------------------------------------------------------------------------------------------------------
        Serial.print("Reproduciendo base posicion: "); Serial.println(iReproducir);
        if (posicion[SerSelect] > MovGuardado[iReproducir][SerSelect])                                                             //si por el contrario es menos, ira disminuyendo grados
        {
          for (anguloREP[SerSelect] = posicion[SerSelect]; anguloREP[SerSelect] >= MovGuardado[iReproducir][SerSelect]; anguloREP[SerSelect] --)
          {
            servoMotor[SerSelect].write(anguloREP[SerSelect]);
            posicion[SerSelect] = anguloREP[SerSelect];
          }
        }
        //--------------------------------------------------------------------------------------------------------------------------------------------------------------
      }
    }                                                                                                         //Cuando termina de reproducir toda la secuendia guardada, ira a la posicion de los potenciometros que estan colocados en ese momento
    for (int i = 0; CantServos > i; i++) {
      if (angulo[i] > posicion[i])                                                                                           //si el angulo del potenciometro es mayor, la base ira aumentando en grados
      {
        Serial.println("posicion inicial Base");
        for (posicion[i]; posicion[i] <= angulo[i]; posicion[i] ++)
        {
          servoMotor[0].write(posicion[i]);
        }
      }

      if (angulo[i] < posicion[i])                                                                                           //si por el contrario es menor, ira disminuyendo grados
      {
        Serial.println("posicion inicial Base");
        for (posicion[i]; posicion[i] >= angulo[i]; posicion[i] --)
        {
          servoMotor[0].write(posicion[i]);
        }
      }
    }
  }
}
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
void borrar() {
  if (digitalRead(boton) == pull)                //Al pulsar resetear, ira colocando 0 a todo el array para borrar los valores existentes
  {
    Serial.println("Borrando, espere...");
    for (int i = 0; i < 20; i++)
    {
      for (int j = 0; j < CantServos; j++)
      {
        MovGuardado[i][j] = 0;
      }
      Serial.print("Borrado: ");  Serial.println(i);
    }
    Serial.println("Borrado Total");
    muestra = 0;                                                    //Se pone a 0 estas variables ya que no hay valores guardados
    ultimaFila = 0;

    for (int i = 0; i < 50; i++) {
      Serial.println("DASDASDASDASDA");
      digitalWrite(buzzer, LOW);
    }
    digitalWrite(buzzer, HIGH);
    while (digitalRead(boton) == pull) {}
  }
}

void calibrar() {
  servoMotor[0].write(90);                                                                                                //Asociamos los servos a los pines donde estan colocados
  servoMotor[1].write(90);
  servoMotor[2].write(90);
  servoMotor[3].write(90);
  servoMotor[4].write(90);
  servoMotor[5].write(90);
}
