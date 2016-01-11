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
#define motorAAtrasIN1 3
#define motorAAtrasIN2 5
#define motorBAtrasIN3 6
#define motorBAtrasIN4 9
#define motorAFrenteIN1 4
#define motorAFrenteIN2 7
#define motorBFrenteIN3 8
#define motorBFrenteIN4 13
byte motorSpeed = 250; //255 is the maximum > 0v low, 5v high

//variaveis: pinos digitais
#define buzzerSpeaker 2
#define ledPin 10
#define echoUltraSounds 11
#define trigUltraSounds 12

//variaveis: pinos analogicos
#define analogSensorLM35 A0
int analogSensorLDR;

//variaveis:
#define delayTime 100 //this is in microseconds
unsigned long distanceInCm = 0;
int dataFromBT;
int tempC;


enum DIRECTION{
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

  playBuzzer();
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

    
    case '9':
      playBuzzer();
      break;
  }

  if (direction == FORWARD && distanceInCm <= 10) {
    moveStop();
  }

  
  
  showAllOutputs();
  delay(delayTime);
}



///////////// FUNCTIONS /////////////

void showAllOutputs(){

  showTemperature();
  showDistance();
  //lightSensor();
  
  Serial.print("Next obstacle at distance: ");
  Serial.print(distanceInCm);
  Serial.print(" cm. Temperature: ");
  Serial.println( (float)tempC );

  
  //light sensor
  /*Serial.print("Light Sensor: ");
  Serial.println(analogSensorLDR); 
  //temperature
  Serial.print(" Temperature: ");
  Serial.print( (float)tempC );
  Serial.println("ºC");
  //distance
  Serial.print("    Distance: ");
  Serial.print(distanceInCm);
  Serial.println(" cm");*/
}

/*
   Show temperature in clesius degree
*/
void showTemperature() {
  //unsigned int tempC = 0;
  unsigned int readTempC = 0;

  readTempC = analogRead(analogSensorLM35);
  tempC = ( 5.0 * readTempC * 100.0 ) / 1024.0;

  //Serial.print( (float)tempC );
  //Serial.println("ºC");
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
  distance = ((duration / 2) / 29.1) - 2;
  distanceInCm = distance;

  //Serial.print(distance); //-2cm para contar a partir do para-choques
  //Serial.println(" cm");
}

/*
 * Acende os leds caso nao haja luz no local onde o carro esta
 */
void lightSensor(){
  analogSensorLDR = analogRead(1);

  if(analogSensorLDR < 15){
    digitalWrite(ledPin, HIGH);
  }else{
    digitalWrite(ledPin, LOW);
  }

  
}


/*
   Play SuperMario theme
*/
void playBuzzer() {
  digitalWrite(buzzerSpeaker, HIGH);
  int melodia[] = {660, 660, 660, 510, 660, 770, 380};
  int duracaoDasNotas[] = {100, 100, 100, 100, 100, 100, 100};
  int pausaDepoisDasNotas[] = {150, 300, 300, 100, 300, 550, 575};

  for (int nota = 0; nota < 7; nota++) {
    int duracaoDaNota = duracaoDasNotas[nota];
    tone(buzzerSpeaker, melodia[nota], duracaoDaNota);
    delay(pausaDepoisDasNotas[nota]);
  }

  noTone(buzzerSpeaker);
  digitalWrite(buzzerSpeaker, LOW);
}


void turnAllLedsON() {
  digitalWrite(ledPin, HIGH);
}

void turnAllLedsOFF(){
  digitalWrite(ledPin, LOW);
}




///////////// MOVIMENTO /////////////

void moveForward() {
  direction = FORWARD;
  
  // MOTOR A/B TRAS
  digitalWrite(motorAAtrasIN1, LOW);
  digitalWrite(motorBAtrasIN3, LOW);

  digitalWrite(motorAAtrasIN2, HIGH);
  //analogWrite(motorAAtrasIN2, motorSpeed);
  //analogWrite(motorBAtrasIN4, motorSpeed);
  digitalWrite(motorBAtrasIN4, HIGH);

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
  //analogWrite(motorAAtrasIN1, motorSpeed);
  digitalWrite(motorAAtrasIN2, LOW);
  //analogWrite(motorBAtrasIN3, motorSpeed);
  digitalWrite(motorBAtrasIN4, LOW);

  digitalWrite(motorAAtrasIN1, HIGH);
  digitalWrite(motorBAtrasIN3, HIGH);

  // MOTOR A/B FRENTE
  digitalWrite(motorAFrenteIN1, LOW);
  //analogWrite(motorAFrenteIN2, motorSpeed);
  digitalWrite(motorBFrenteIN3, LOW);
  //analogWrite(motorBFrenteIN4, motorSpeed);

  digitalWrite(motorAFrenteIN2, HIGH);
  digitalWrite(motorBFrenteIN4, HIGH);
}

void moveLeft() {
  direction = LEFT;
  
  // MOTOR A/B TRAS
  digitalWrite(motorAAtrasIN1, LOW);
  //analogWrite(motorAAtrasIN2, motorSpeed);
  //analogWrite(motorBAtrasIN3, motorSpeed);
  digitalWrite(motorBAtrasIN4, LOW);

  // MOTOR A/B FRENTE
  digitalWrite(motorAFrenteIN1, LOW);
  digitalWrite(motorAFrenteIN2, LOW);
  digitalWrite(motorBFrenteIN3, LOW);
  digitalWrite(motorBFrenteIN4, LOW);

  digitalWrite(motorAAtrasIN2, HIGH);
  digitalWrite(motorBAtrasIN3, HIGH);
}

void moveRight() {
  direction = RIGHT;
  
  // MOTOR A/B TRAS
  //analogWrite(motorAAtrasIN1, motorSpeed);
  digitalWrite(motorAAtrasIN2, LOW);
  digitalWrite(motorBAtrasIN3, LOW);
  //analogWrite(motorBAtrasIN4, motorSpeed);

  // MOTOR A/B FRENTE
  digitalWrite(motorAFrenteIN1, LOW);
  digitalWrite(motorAFrenteIN2, LOW);
  digitalWrite(motorBFrenteIN3, LOW);
  digitalWrite(motorBFrenteIN4, LOW);

  digitalWrite(motorAAtrasIN1, HIGH);
  digitalWrite(motorBAtrasIN4, HIGH);
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
