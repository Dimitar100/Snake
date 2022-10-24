
//За всеки диод в LED матрицата има съответстващо поле в двумерен масив. Ако полето е със стойност 1 съответния диод ще свети 
bool matrix[8][8] = {
  {0, 0, 0, 0, 0, 0, 0, 0}, 
  {0, 0, 0, 0, 0, 0, 0, 0}, 
  {0, 0, 0, 0, 0, 0, 0, 0}, 
  {0, 0, 0, 0, 0, 0, 0, 0}, 
  {0, 0, 0, 0, 0, 0, 0, 0}, 
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}, 
  {0, 0, 0, 0, 0, 0, 0, 0}
}; 


bool sad_matrix[8][8] = {
  {0, 0, 0, 0, 0, 0, 0, 0}, 
  {0, 0, 1, 0, 0, 1, 0, 0}, 
  {0, 0, 1, 0, 0, 1, 0, 0}, 
  {0, 0, 1, 0, 0, 1, 0, 0}, 
  {0, 0, 0, 0, 0, 0, 0, 0}, 
  {0, 0, 1, 1, 1, 1, 0, 0},
  {0, 1, 0, 0, 0, 0, 1, 0}, 
  {0, 0, 0, 0, 0, 0, 0, 0}
}; 

bool snake_matrix[8][8] = {
  {0, 0, 0, 0, 0, 0, 0, 0}, 
  {0, 1, 0, 0, 0, 0, 0, 0}, 
  {0, 1, 0, 1, 0, 1, 1, 0}, 
  {0, 1, 0, 0, 0, 0, 1, 0}, 
  {0, 1, 0, 0, 0, 0, 1, 0}, 
  {0, 1, 1, 1, 1, 1, 1, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}, 
  {0, 0, 0, 0, 0, 0, 0, 0}
}; 


int x = 0;
int y = 0;

bool start = false;
//когато играта приключи (змията умре) стойността става "true"
bool ended = false;

bool gameMenu[3];

void setup() {

  Serial.begin(9600);
  //Чрез пиновете А0, А1, А2 и А3 се обработват сигналите изпращани от джойстика
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);

  pinMode(49, INPUT);

  //Към тези пинове са свързани анодите на на светодиодите
  for(int i = 2; i < 10; i++){
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }

  //Към тези пинове са свързани катодите на на светодиодите
  for(int i = 23; i < 31; i++){
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
  }

  gameMenu[0] = true;
  gameMenu[1] = false;
  gameMenu[2] = false;
}

int startCounter = 0;

void loop() {

  //moveDot();
  snakeMovement();
}

//включва всички светодиоди в матрицата
void turnOnAll(){
  for(int i = 2; i < 10; i++){
    digitalWrite(i, HIGH);
  }
  for(int i = 23; i < 31; i++){
    digitalWrite(i, LOW);
  } 
}

//изключва всички светодиоди в матрицата
void turnOffAll(){
  for(int i = 2; i < 10; i++){
    digitalWrite(i, LOW);
  }
  for(int i = 23; i < 31; i++){
    digitalWrite(i, HIGH);
  } 
}

void clearMatrix(){
  for(int i = 0; i < 8; i++){
    for(int n = 0; n < 8; n++){
      matrix[i][n] = 0;  
    }  
  }  
}

////////////////////Simple dot moving////////////////////////
void moveDot(){
   if(analogRead(A0) == 1023){
    moveLeft();
    Serial.println("LEFT");
  }else if(analogRead(A1) == 1023){
    moveRight();
    Serial.println("RIGHT");
  }else if(analogRead(A2) == 1023){
    moveDown();
    Serial.println("DOWN");
  }else if(analogRead(A3) == 1023){
    moveUp();
    Serial.println("UP");
  }

  delay(200);
}


void turnOnDiode(){
  digitalWrite(y+2, HIGH);
  digitalWrite(x+23, LOW);
  matrix[y][x] = 1;
}


void moveDown(){
  if(y<7){
      digitalWrite(y+2, LOW);
      matrix[y][x] = 0;
      y = y + 1;
      turnOnDiode();
  }  
}

void moveUp(){
  if(y>0){
      digitalWrite(y+2, LOW);
      matrix[y][x] = 0;
      y = y - 1;
      turnOnDiode();
  }  
}

void moveRight(){
  if(x<7){
      digitalWrite(x+23, HIGH);
      matrix[y][x] = 0;
      x = x + 1;
      turnOnDiode();
  }  
}

void moveLeft(){
  if(x>0){
      digitalWrite(x+23, HIGH);
      matrix[y][x] = 0;
      x = x - 1;
      turnOnDiode();
  }  
}


////////////////////Snake////////////////////////

//Взависимост от стойността на velocityX и velocityY се определя посоката в коята се движи змията
int velocityX = 1;
int velocityY = 0;

//Двата масива пазят координатите на всяка точка от змията.
int snakeDotsX[50];
int snakeDotsY[50];

int snakeLength = 0;

//координати на храната
int foodX, foodY;


int delayCounter = 0;


void resetSnake(){
    velocityX = 1;
    velocityY = 0;
    for(int i = 0; i++; i <50){
      snakeDotsX[i] = 0;
      snakeDotsY[i] = 0;
    }
    snakeLength = 0;
    delayCounter = 0;

    for(int i = 0; i<8; i++){
      for(int n = 0; n < 8; n++){
        matrix[i][n] = 0;  
      }  
    }
    startSnake();
    generateFood();
}

//записва първите две точки от змията
void startSnake(){
  x = 0;
  y = 0;
  matrix[y][x] = 1;
  snakeDotsX[1] = x;
  snakeDotsY[1] = y;
  x = x + 1;

  matrix[y][x] = 1;
  snakeDotsX[0] = x;
  snakeDotsY[0] = y;

  snakeLength++;
}

void snakeMovement(){
  
  //възоснова на това кои полета в двумерния масив са със стойност 1, включва диодите от LED матрицата
  if(!ended){
    for(int i = 0; i < 8; i++){
      digitalWrite(i+2, HIGH);
      for(int n = 0; n < 8; n++){
        if(matrix[i][n] == 1){
           //digitalWrite(i+2, HIGH);
           digitalWrite(n+23, LOW);
        }    
      }
      turnOffAll();
    }
  }else if(ended && !start){
    for(int i = 0; i < 8; i++){
      digitalWrite(i+2, HIGH);
      for(int n = 0; n < 8; n++){
        if(sad_matrix[i][n] == 1){
           //digitalWrite(i+2, HIGH);
           digitalWrite(n+23, LOW);
        }    
      }
      turnOffAll();
    } 
  } 
  
  delayCounter++;
  if(delayCounter == 150){
    delayCounter = 0;

    //Обработване на сигналите подавани от джойстика
    if(analogRead(A0) == 1023){
      if(velocityX == 0 && ended == false){
        velocityX = -1;
        velocityY = 0;
      }  
      Serial.println("LEFT");
    }else if(analogRead(A1) == 1023){
      if(velocityX == 0 && ended == false){
        velocityX = 1;
        velocityY = 0;
      }
      Serial.println("RIGHT");
    }else if(analogRead(A2) == 1023){
      if(velocityY == 0 && ended == false){
        velocityY = 1;
        velocityX = 0;
      }  
      Serial.println("DOWN");
    }else if(analogRead(A3) == 1023){
      if(velocityY == 0 && ended == false){
        velocityY = -1;
        velocityX = 0;
      }
      Serial.println("UP");
    }

    //Движение на змията
    matrix[snakeDotsY[snakeLength]][snakeDotsX[snakeLength]] = 0;
   
    for(int i = snakeLength; i > 0; i--){
      snakeDotsX[i] = snakeDotsX[i-1];
      snakeDotsY[i] = snakeDotsY[i-1];
    }

    //мести главата на змията по X 
    if(snakeDotsX[0] + velocityX < 8 && snakeDotsX[0] + velocityX > -1){
      snakeDotsX[0] = snakeDotsX[0] + velocityX;
    }else if(snakeDotsX[0] + velocityX == 8){
         snakeDotsX[0] = 0;
    }else if(snakeDotsX[0] + velocityX == -1){
         snakeDotsX[0] = 7;
    }

    //мести главата на змията по Y 
    if(snakeDotsY[0] + velocityY < 8 && snakeDotsY[0] + velocityY > -1){
      snakeDotsY[0] = snakeDotsY[0] + velocityY;
    }else if(snakeDotsY[0] + velocityY == 8){
         snakeDotsY[0] = 0;
    }else if(snakeDotsY[0] + velocityY == -1){
         snakeDotsY[0] = 7;
    }

    //засича колизии на змията със храна или с нейното тяло
    if(matrix[snakeDotsY[0]][snakeDotsX[0]] == 1){
      if(snakeDotsY[0]==foodY && snakeDotsX[0]==foodX){
        matrix[snakeDotsY[0]][snakeDotsX[0]] = 1;  
        addDotToSnake();
        generateFood();
      }else{
        ended = true;
        start = false;
        resetSnake();
        clearMatrix(); 
      }  
    }else{
      matrix[snakeDotsY[0]][snakeDotsX[0]] = 1;
    }
  }
}

//генерира храна за змията
void generateFood(){
  int x1 = random(7);
  int y1 = random(7);
  
  if(matrix[y1][x1]==0){
    foodX = x1;
    foodY = y1;
    matrix[foodY][foodX] = 1;  
  }else{
    generateFood();  
  }

}

//удължава змията като добавя нова точка към нея
void addDotToSnake(){
  snakeLength++;
  if(snakeDotsX[snakeLength-1] == snakeDotsX[snakeLength-2]){
    snakeDotsX[snakeLength] = snakeDotsX[snakeLength-1];
    if(snakeDotsY[snakeLength-1] < snakeDotsY[snakeLength-2]){
      snakeDotsY[snakeLength] = snakeDotsY[snakeLength-1] - 1;
    }else{
      snakeDotsY[snakeLength] = snakeDotsY[snakeLength-1] + 1;
    }
  }else if (snakeDotsY[snakeLength-1] == snakeDotsY[snakeLength-2]){
    snakeDotsY[snakeLength] = snakeDotsY[snakeLength-1];
    if(snakeDotsX[snakeLength-1] < snakeDotsX[snakeLength-2]){
      snakeDotsX[snakeLength] = snakeDotsX[snakeLength-1] - 1;
    }else{
      snakeDotsX[snakeLength] = snakeDotsX[snakeLength-1] + 1;
    }
  }

  matrix[snakeDotsY[snakeLength]][snakeDotsX[snakeLength]] = 1; 
}
