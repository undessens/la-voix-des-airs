/*
          MACHINE A ECRIRE
          WILD LUBERON
          github.com/undessens

          Author : Aurelien Conil
*/
#define DIGITALIN 39  // Nombre de bouton
#define DIGITALOUT 0  // Nombre de leds
#define DEBUG 0

// ------------   BOUTONS  ------------------
int digitalinValue[DIGITALIN];
// --------------- AIMANTS -----------------
int spacePin = 53;
int spaceValue = 0;
int capsPin = 52;
int capsValue = 0;
int slashEndPin = 50;
int slashEndValue = 0;


// --------------- Keyboard To Pin Layout -----------
//
// Caps Keyboard Layout - Key(Pin)
// A(28) 2(29) Z(30) 3(31) E(32) 4(41) R(42) 5(43) T(44) 6(45) Y(33) 7(34) U(35) 8(36) I(37) 9(38) O(39) &(40) P(46) °() +()
//   Q (9) W(8) S(7) X(6) D(5) C(49) F(3) V(2) G(14) B(15) H(16) N(17) J(18) ?(19) K(20) .(21) L(22) /(23) M(24) §(25) %()
//
// Lower Keyboard Layout
//  a é z " e ' r ( t - y è u _ i ç o à p )
//   q w s x d c f v g b h n j , k ; l : m ! ù

int digitalinPin[] =  {
  28, 29, 30, 31, 32, 41, 42, 43, 44, 45, 33, 34, 35, 36, 37, 38, 39, 40, 46,
    9, 8, 7, 6, 5, 49, 3, 2, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
};

String capsKeyboard[] = {
  "A", "2", "Z", "3", "E", "4", "R", "5", "T", "6", "Y", "7", "U", "8", "I", "9", "O", "&", "P", 
    "Q", "W", "S", "X", "D", "C", "F", "V", "G", "B", "H", "N", "J", "?", "K", ".", "L", "/", "M", "%"
};



String lowerKeyboard[] = {
  "a", "é", "z", "\"", "e", "'", "r", "(", "t", "-", "y", "è", "u", "_", "i", "ç", "o", "à", "p", 
    "q", "w", "s", "x", "d", "c", "f", "v", "g", "b", "h", "n", "j", ",", "k", ";", "l", ":", "m", "!"
};


void setup() {
  Serial.begin(38400);
  while (!Serial);

  for (int i = 0 ; i < DIGITALIN ; i++ ) {
    pinMode(digitalinPin[i], INPUT);
    digitalWrite(digitalinPin[i], HIGH);
    digitalinValue[i] = !digitalRead(digitalinPin[i]);
  }

  pinMode(spacePin, INPUT);
  pinMode(capsPin, INPUT);
  pinMode(slashEndPin, INPUT);
  digitalWrite(slashEndPin, HIGH);

}
  
void loop() {
  
  //READ digital pin and write to serial  
  for (int i = 0; i < DIGITALIN; i++) {
    int newValueDi = !digitalRead(digitalinPin[i]);
    if (newValueDi != digitalinValue[i]) {
      digitalinValue[i] = newValueDi;
      if (newValueDi == HIGH ) {
        if(capsValue){
          sendMessage(digitalinPin[i], capsKeyboard[i]);
        }
        else{
          sendMessage(digitalinPin[i], lowerKeyboard[i]);
        }
        
      }
    }
  }

  //SPECIAL NON ASCII : SPACE
  int newValueDi = digitalRead(spacePin);
  if (newValueDi != spaceValue) {
    spaceValue = newValueDi;
    if (newValueDi == HIGH ) {
      sendMessage(spacePin, " ");
     }
  }

   //SPECIAL NON ASCII : CAPS MAJ
  capsValue = digitalRead(capsPin);

  //SPECIAL NON ASCII Slash End
  int newSlashEndValue = !digitalRead(slashEndPin);
  if( newSlashEndValue != slashEndValue){
    slashEndValue = newSlashEndValue;
    if (slashEndValue == HIGH ) {
      sendMessage(slashEndPin, "\n");
     }
    
  }


  delay(2); // ms 
}

void sendMessage(int pin, String key) {
  String msg ;
  if(DEBUG){
    msg = "Pin:"+ String(pin) + " - Key:" + key +  "\n";    
  } else {
    msg = key;
  }
  
  Serial.print(msg);
}
