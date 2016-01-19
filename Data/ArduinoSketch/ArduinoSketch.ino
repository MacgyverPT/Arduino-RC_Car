/* Programa: Arduino RC Car

   Descrição:
   Neste sketch estão reunidos todas as funcionalidade do RC Car, tal como recolha de informação
   para o utilizador, movimento do carro, conectar via Bluetooth, etc.


   Autor: Miguel Rosa
   Data: Dezembro 2015 / Janeiro 2016
   Instituto Politécnico de Beja - Escola Superior Tecnologia e Gestão
   Ano Lectivo 2015-16
   Disciplina: Física Aplicada à Computação
   Docente: Nuno Pereira
*/

//motores/rodas
#define motorAAtrasIN1 3
#define motorAAtrasIN2 5
#define motorBAtrasIN3 6
#define motorBAtrasIN4 9
#define motorAFrenteIN1 4
#define motorAFrenteIN2 7
#define motorBFrenteIN3 8
#define motorBFrenteIN4 13
//byte motorSpeed = 250; //255 is the maximum > 0v low, 5v high

//pinos digitais
#define buzzerSpeaker 2
#define ledPin 10
#define echoUltraSounds 11
#define trigUltraSounds 12

//pinos analogicos
#define analogSensorLM35 A0
#define analogSensorLDR A1

//variaveis:
#define delayTime 1000 //this is in microseconds
#define LDRMaxValueToTurnLEDON 100 //Max value for LDR to turn LED ON
unsigned long distanceInCm = 0;
unsigned int sensorLDRReading = 0;
int tempC;
int dataFromBT;


enum DIRECTION {
  STOP,
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT
};

byte direction = STOP;


// put your setup code here, to run once:
void setup() {
  Serial.begin(9600);

  pinMode(motorAAtrasIN1, OUTPUT);
  pinMode(motorAAtrasIN2, OUTPUT);
  pinMode(motorBAtrasIN3, OUTPUT);
  pinMode(motorBAtrasIN4, OUTPUT);
  pinMode(motorAFrenteIN1, OUTPUT);
  pinMode(motorAFrenteIN2, OUTPUT);
  pinMode(motorBFrenteIN3, OUTPUT);
  pinMode(motorBFrenteIN4, OUTPUT);

  pinMode(buzzerSpeaker, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(trigUltraSounds, OUTPUT);
  pinMode(echoUltraSounds, INPUT);
}



//put your main code here, to run repeatedly:
void loop() {
  dataFromBT = 0;

  //read from serial port
  if ( Serial.available() ) {
    dataFromBT = Serial.read();
  }

  switch (dataFromBT) {
    case '1': //mover frente
      moveForward();
      break;
    case '2': //mover tras
      moveBackward();
      break;
    case '3': //mover esquerda
      moveLeft();
      break;
    case '4': //mover direita
      moveRight();
      break;
    case '5': //parar
      moveStop();
      break;
    case '6':
      turnAllLedsON();
      break;
    case '7':
      turnAllLedsOFF();
      break;
    case '8':
      playBuzzer();
      break;
    case '9':
      //
      break;
  }


  if (direction == FORWARD && distanceInCm <= 60) {
    moveStop();
  }


  showAllOutputs();
  delay(delayTime);
}




///////////// FUNCTIONS /////////////

/* Mostra o output no terminal
*/
void showAllOutputs() {

  showTemperature();
  showDistance();
  lightSensor();

  Serial.print("Obstacle at distance: ");
  Serial.print(distanceInCm);
  Serial.print("cm. | Temperature: ");
  Serial.print( (float)tempC );
  Serial.print("ºc | Light Sensor Value: ");
  Serial.println( sensorLDRReading );
}

/* Show temperature in clesius degree
*/
void showTemperature() {
  unsigned int readTempC = 0;

  readTempC = analogRead(analogSensorLM35);
  tempC = ( 5.0 * readTempC * 100.0 ) / 1024.0;
}

/* Mostra a distancia do objecto que tem à sua frente
*/
void showDistance() {
  unsigned long duration = 0;
  unsigned long distance = 0;

  digitalWrite(trigUltraSounds, LOW);
  delay(5);
  digitalWrite(trigUltraSounds, HIGH);
  delay(10);
  digitalWrite(trigUltraSounds, LOW);

  duration = pulseIn(echoUltraSounds, HIGH);
  distance = ((duration / 2) / 29.1) - 2;
  distanceInCm = distance;
}

/* Acende os leds caso nao haja luz no local onde o carro esta
*/
void lightSensor() {
  sensorLDRReading = analogRead(analogSensorLDR);

  if (sensorLDRReading < LDRMaxValueToTurnLEDON ) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
}

/* Play SuperMario theme
*/
void playBuzzer() {
  int melodia[] = {660, 660, 660, 510, 660, 770, 380};
  int duracaoDasNotas[] = {100, 100, 100, 100, 100, 100, 100};
  int pausaDepoisDasNotas[] = {150, 300, 300, 100, 300, 550, 575};

  for (int nota = 0; nota < 7; nota++) {
    int duracaoDaNota = duracaoDasNotas[nota];
    tone(buzzerSpeaker, melodia[nota], duracaoDaNota);
    delay(pausaDepoisDasNotas[nota]);
  }

  noTone(buzzerSpeaker);
}

/* Liga todos os leds (isto se nao tivesse acendido automaticamente)
*/
void turnAllLedsON() {
  if ( sensorLDRReading > LDRMaxValueToTurnLEDON) {
    digitalWrite(ledPin, HIGH);
  }
}

/* Desliga todos os Leds (isto se nao tivesse acendido automaticamente)
*/
void turnAllLedsOFF() {
  if ( sensorLDRReading > LDRMaxValueToTurnLEDON) {
    digitalWrite(ledPin, LOW);
  }
}



///////////// MOVIMENTO /////////////

void moveForward() {
  direction = FORWARD;

  // MOTOR A/B TRAS
  digitalWrite(motorAAtrasIN1, LOW);
  digitalWrite(motorAAtrasIN2, HIGH);
  //analogWrite(motorAAtrasIN2, motorSpeed);
  digitalWrite(motorBAtrasIN3, LOW);
  digitalWrite(motorBAtrasIN4, HIGH);
  //analogWrite(motorBAtrasIN4, motorSpeed);

  // MOTOR A/B FRENTE
  digitalWrite(motorAFrenteIN2, LOW);
  digitalWrite(motorBFrenteIN4, LOW);
  digitalWrite(motorAFrenteIN1, HIGH);
  //analogWrite(motorAFrenteIN1, motorSpeed);
  //analogWrite(motorBFrenteIN3, motorSpeed);
  digitalWrite(motorBFrenteIN3, HIGH);

}

void moveBackward() {
  direction = BACKWARD;

  // MOTOR A/B TRAS
  digitalWrite(motorAAtrasIN1, HIGH);
  //analogWrite(motorAAtrasIN1, motorSpeed);
  digitalWrite(motorAAtrasIN2, LOW);
  digitalWrite(motorBAtrasIN3, HIGH);
  //analogWrite(motorBAtrasIN3, motorSpeed);
  digitalWrite(motorBAtrasIN4, LOW);

  // MOTOR A/B FRENTE
  digitalWrite(motorAFrenteIN1, LOW);
  //analogWrite(motorAFrenteIN2, motorSpeed);
  digitalWrite(motorBFrenteIN3, LOW);
  digitalWrite(motorAFrenteIN2, HIGH);
  digitalWrite(motorBFrenteIN4, HIGH);
  //analogWrite(motorBFrenteIN4, motorSpeed);
}

void moveLeft() {
  direction = LEFT;

  // MOTOR A/B TRAS
  digitalWrite(motorAAtrasIN1, LOW);
  digitalWrite(motorAAtrasIN2, HIGH);
  //analogWrite(motorAAtrasIN2, motorSpeed);
  digitalWrite(motorBAtrasIN3, HIGH);
  //analogWrite(motorBAtrasIN3, motorSpeed);
  digitalWrite(motorBAtrasIN4, LOW);

  // MOTOR A/B FRENTE
  digitalWrite(motorAFrenteIN1, LOW);
  digitalWrite(motorAFrenteIN2, LOW);
  digitalWrite(motorBFrenteIN3, LOW);
  digitalWrite(motorBFrenteIN4, LOW);
}

void moveRight() {
  direction = RIGHT;

  // MOTOR A/B TRAS
  digitalWrite(motorAAtrasIN1, HIGH);
  //analogWrite(motorAAtrasIN1, motorSpeed);
  digitalWrite(motorAAtrasIN2, LOW);
  digitalWrite(motorBAtrasIN3, LOW);
  digitalWrite(motorBAtrasIN4, HIGH);
  //analogWrite(motorBAtrasIN4, motorSpeed);

  // MOTOR A/B FRENTE
  digitalWrite(motorAFrenteIN1, LOW);
  digitalWrite(motorAFrenteIN2, LOW);
  digitalWrite(motorBFrenteIN3, LOW);
  digitalWrite(motorBFrenteIN4, LOW);
}

void moveStop() {
  direction = STOP;

  // MOTOR A/B TRAS
  digitalWrite(motorAAtrasIN1, LOW);
  digitalWrite(motorAAtrasIN2, LOW);
  digitalWrite(motorBAtrasIN3, LOW);
  digitalWrite(motorBAtrasIN4, LOW);

  // MOTOR A/B FRENTE
  digitalWrite(motorAFrenteIN1, LOW);
  digitalWrite(motorAFrenteIN2, LOW);
  digitalWrite(motorBFrenteIN3, LOW);
  digitalWrite(motorBFrenteIN4, LOW);
}
