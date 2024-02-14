

#define echo A4
#define trig A5

#define ENA 5
#define ENB 6
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11

#define right 10
#define middle 4
#define left 2

void moveForward(int duration) {

  analogWrite(ENA, 150);
  analogWrite(ENB, 150);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  
  delay(duration);
}

void moveBackward(int duration) {

  analogWrite(ENA, 100);
  analogWrite(ENB, 100);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  
  delay(duration);
}

void turnRight(int duration) {

  analogWrite(ENA, 150);
  analogWrite(ENB, 150);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  
  delay(duration);
}

void turnLeft(int duration) {

  analogWrite(ENA, 150);
  analogWrite(ENB, 150);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  
  delay(duration);
}

void stop(int duration) {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  delay(duration);
}

double getDistance(){
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10); 
  digitalWrite(trig, LOW);

  long raw = pulseIn(echo, HIGH);
  double distance = raw * 0.034 / 2;

  return(distance);
}


void setup() {
  // put your setup code here, to run once:
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
 
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(right, INPUT);
  pinMode(middle, INPUT);
  pinMode(left, INPUT);

  Serial.begin(9600);
}

void loop() {
  double distance = getDistance();

  if (distance <=30){
    stop(100);
  }
  
  int right_read = digitalRead(right);
  int middle_read = digitalRead(middle);
  int left_read = digitalRead(left);

  if (right_read + left_read + middle_read ==0){
    stop(3000);
    moveForward(100);
  }
  else if (left_read == 0 && right_read ==1){
    turnLeft(10);
  }
  else if (right_read ==0 && left_read == 1){
    turnRight(10);
  }
  else if (middle_read == 0 && left_read == 1 && right_read == 1){
    moveForward(10);
  }
  else if (right_read + left_read + middle_read == 3){
    moveForward(10);
  }

}



