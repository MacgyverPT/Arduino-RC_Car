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

//variavéis para os motores/rodas
const byte motorAAtrasIN1 = 3;
const byte motorAAtrasIN2 = 5;
const byte motorBAtrasIN3 = 6;
const byte motorBAtrasIN4 = 9;
const byte motorAFrenteIN1 = 4;
const byte motorAFrenteIN2 = 7;
const byte motorBFrenteIN3 = 8;
const byte motorBFrenteIN4 = 13;
const byte motorSpeed = 255; //255 is the maximum > 0v low, 5v high

//variaveis: pinos digitais
const byte buzzerSpeaker = 2;
const byte led = 10;
const byte echoUltraSounds = 11;
const byte trigUltraSounds = 12;

//variaveis: pinos analogicos
byte analogSensorLDR = 0;

//variaveis:
int delayTime = 1000; //this is in microseconds
byte dataFromBT;




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
  pinMode(led, OUTPUT);
  pinMode(trigUltraSounds, OUTPUT);
  pinMode(echoUltraSounds, INPUT);
}



//put your main code here, to run repeatedly:
void loop() {

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
  }

  showTemperature();
  delayTime;
  showDistance();

}



///////////// FUNCTIONS /////////////

/*
   Show temperature in clesius degree
*/
void showTemperature() {
  unsigned int tempC = 0;
  unsigned int readTempC = 0;

  readTempC = analogRead(analogSensorLDR);
  tempC = ( 5.0 * readTempC * 100.0 ) / 1024.0;

  Serial.print( (float)tempC );
  Serial.println("ºC");
}



/*
   Mostra a distancia do objecto que tem à sua frente
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
  distance = (duration / 2) / 29.1;

  Serial.print(distance - 2); //-2cm para contar a partir do para-choques
  Serial.println(" cm");
}



///////////// MOVIMENTO /////////////

void moveForward() {
  // MOTOR A/B TRAS
  analogWrite(motorAAtrasIN1, LOW);
  analogWrite(motorAAtrasIN2, motorSpeed);
  analogWrite(motorBAtrasIN3, LOW);
  analogWrite(motorBAtrasIN4, motorSpeed);

  // MOTOR A/B FRENTE
  analogWrite(motorAFrenteIN1, motorSpeed);
  analogWrite(motorAFrenteIN2, LOW);
  analogWrite(motorBFrenteIN3, motorSpeed);
  analogWrite(motorBFrenteIN4, LOW);
}

void moveBackward() {
  // MOTOR A/B TRAS
  analogWrite(motorAAtrasIN1, motorSpeed);
  analogWrite(motorAAtrasIN2, LOW);
  analogWrite(motorBAtrasIN3, motorSpeed);
  analogWrite(motorBAtrasIN4, LOW);

  // MOTOR A/B FRENTE
  analogWrite(motorAFrenteIN1, LOW);
  analogWrite(motorAFrenteIN2, motorSpeed);
  analogWrite(motorBFrenteIN3, LOW);
  analogWrite(motorBFrenteIN4, motorSpeed);
}

void moveLeft() {
  // MOTOR A/B TRAS
  analogWrite(motorAAtrasIN1, LOW);
  analogWrite(motorAAtrasIN2, motorSpeed);
  analogWrite(motorBAtrasIN3, motorSpeed);
  analogWrite(motorBAtrasIN4, LOW);

  // MOTOR A/B FRENTE
  analogWrite(motorAFrenteIN1, LOW);
  analogWrite(motorAFrenteIN2, LOW);
  analogWrite(motorBFrenteIN3, LOW);
  analogWrite(motorBFrenteIN4, LOW);
}

void moveRight() {
  // MOTOR A/B TRAS
  analogWrite(motorAAtrasIN1, motorSpeed);
  analogWrite(motorAAtrasIN2, LOW);
  analogWrite(motorBAtrasIN3, LOW);
  analogWrite(motorBAtrasIN4, motorSpeed);

  // MOTOR A/B FRENTE
  analogWrite(motorAFrenteIN1, LOW);
  analogWrite(motorAFrenteIN2, LOW);
  analogWrite(motorBFrenteIN3, LOW);
  analogWrite(motorBFrenteIN4, LOW);
}

void moveStop() {
  // MOTOR A/B TRAS
  analogWrite(motorAAtrasIN1, LOW);
  analogWrite(motorAAtrasIN2, LOW);
  analogWrite(motorBAtrasIN3, LOW);
  analogWrite(motorBAtrasIN4, LOW);

  // MOTOR A/B FRENTE
  analogWrite(motorAFrenteIN1, LOW);
  analogWrite(motorAFrenteIN2, LOW);
  analogWrite(motorBFrenteIN3, LOW);
  analogWrite(motorBFrenteIN4, LOW);
}
