/*
          MACHINE A ECRIRE
          WILD LUBERON
          github.com/undessens

          Author : Aurelien Conil
*/
#define ANALOGIN 0    // Nombre de potentiometre
#define ANALOG_THRESH 10

#define DIGITALIN 39  // Nombre de boutons
#define DIGITALOUT 0  // Nombre de leds

// ------------   POTENTIOMETRE --------------
// int analogValue[ANALOGIN];
// int analogPin[] = { A0, A1, A2};

// ------------   BOUTONS  ------------------
int digitalinValue[DIGITALIN];


// --------------- Keyboard To Pin Layout -----------
//
// Caps Keyboard Layout - Key(Pin)
// A(28) 2(29) Z(30) 3(31) E(32) 4(41) R(42) 5(43) T(44) 6(45) Y(33) 7(34) U(35) 8(36) I(37) 9(38) O(39) &(40) P(46) °() +()
// Q (9) W(8) S(7) X(6) D(5) C(4) F(3) V(2) G(14) B(15) H(16) N(17) J(18) ?(19) K(20) .(21) L(22) /(23) M(24) §(25) %()
//
// Lower Keyboard Layout
//  a é z " e ' r ( t - y è u _ i ç o à p )
//   q w s x d c f v g b h n j , k ; l : m ! ù

int digitalinPin[] =  {
   9, 8, 7, 6, 5, 4, 3, 2, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
   28, 29, 30, 31, 32, 41, 42, 43, 44, 45, 33, 34, 35, 36, 37, 38, 39, 40, 46
};

char capsKeyboard[] = {
 'Q', 'W', 'S', 'X', 'D', 'C', 'F', 'V', 'G', 'B', 'H', 'N', 'J', '?', 'K', '.', 'L', '/', 'M', '§',
   'A', '2', 'Z', '3', 'E', '4', 'R', '5', 'T', '6', 'Y', '7', 'U', '8', 'I', '9', 'O', '&', 'P', 
};


//char lowerKeyboard[] = {
//  
//};


void setup() {
  Serial.begin(38400);
  while (!Serial);

  for (int i = 0 ; i < DIGITALIN ; i++ ) {
    pinMode(digitalinPin[i], INPUT_PULLUP);
    digitalinValue[i] = digitalRead(digitalinPin[i]);
  }

}

void loop() {
  //READ digital pin and write to serial
  int newValueDi = 0;
  
  for (int i = 0; i < DIGITALIN; i++) {
    newValueDi = !digitalRead(digitalinPin[i]);
    if (newValueDi != digitalinValue[i]) {
      digitalinValue[i] = newValueDi;
      
      if (newValueDi == LOW ) {
        sendMessage(i , capsKeyboard[i]);
      }
    }
  }

  // Serial receive message
  while (Serial.available() > 2) {

    int channel = (Serial.read()) ;
    int value = (Serial.read()) ;
    int finalvalue = value * 255;
    char end_of_line = Serial.read();

    //change Value because of led light power
    if (end_of_line == 13 ) {
      switch (channel) {
        case 0:
          break;
        case 1:
          break;
      }
    }

  }

  delay(5); // ms
}

void sendMessage(int pin, char key) {
  printf("Sending the key %c (pin %d)", pin, key);
  
  // Normal way to send a message
  Serial.println("pouet");
//  Serial.print(char(key));
}
