/*
          MACHINE A ECRIRE
          WILD LUBERON
          github.com/undessens

          Author : Arturo Mondragón
*/
#define DEBUG 0


String special_char[] = {
  "été ", "à ",  "moi, ", "rêve ", "du ", "mois ", "d'aoüt", "\n",
  "l'amour est comme le £ ça va, ça part\n"
};

int len_char = sizeof(special_char) / sizeof(String);

void setup() {
  Serial.begin(38400);
  while (!Serial);
  
  Serial.println("\n+++++++++++++++++++++++++++++++++++++++++");
  Serial.println("Use a serial terminal that supports UTF-8");
  Serial.println("+++++++++++++++++++++++++++++++++++++++++\n");
}
  
void loop() {
  //READ digital pin and write to serial  
  for (int i = 0; i < len_char; i++) {
    sendMessageForPin(i);
    delay(600); // ms 
  }
  delay(1000); // ms
}

void sendMessageForPin(int pin) {
  String msg ;
  
  if(DEBUG){
    msg = "Pin:"+ String(pin) + " - Key:" + special_char[pin] +  "\n";    
  } else {
    msg = special_char[pin];
  }
  
  Serial.print(msg);
}
