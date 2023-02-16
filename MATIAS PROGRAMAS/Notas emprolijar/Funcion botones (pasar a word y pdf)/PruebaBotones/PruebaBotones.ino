  
  #define CANT_BOTONES 4
  int BOTONES[CANT_BOTONES]={D5,D7,D6,D4};
  #define BOTON_SUBIR 0 //a
  #define BOTON_BAJAR 1 //c
  #define BOTON_ON 2 //b
  #define BOTON_OFF 3 //d
  int litros=0;
  int comenzar=0;
  int subir=0;
  int bajar=0;

void setup() {
    
Serial.begin(9600);

for (int i=0;i<CANT_BOTONES-1;i++){
  pinMode(BOTONES[i],INPUT);
}

}

void loop() {


      //SUBIR
      subir = Funcion_Leer_Boton(BOTON_SUBIR);
      if (subir==1){
        //--- Lo que quiero hacer al presionar BOTON_SUBIR
        Serial.println("SUBIR");
        litros +=2;
        Serial.println(litros);
        //---- Fin de lo que quiero hacer
              }
        
        //BAJAR
        bajar = Funcion_Leer_Boton(BOTON_BAJAR);
        if (bajar==1){
             //--- Lo que quiero hacer al presionar BOTON_SUBIR
             Serial.println("BAJAR");
             litros -=2;
             Serial.println(litros);
             //---- Fin de lo que quiero hacer
              }
          
  } 



/*------------ FUNCION
 la funcion devuelve 1 si se presiona el boton
 devuelve 0 si no se presiona el boton
 se necesita definir:
 int BOTONES[CANT_BOTONES]={D5,D7,D6,D4};
 y debemos enviarle la posicion del puerto que queremos leer
 */
int Funcion_Leer_Boton (int codigo_boton){
  int boton_presionado=0,retorno=0;
  //******
  if (digitalRead(BOTONES[codigo_boton]) == LOW)     
      {
        boton_presionado = 1;     
      while(digitalRead(BOTONES[codigo_boton]) == LOW)
        delay (5);
        }
      if (digitalRead(BOTONES[codigo_boton]) == HIGH && boton_presionado == 1)
      {
        boton_presionado = 0;    
        retorno = 1;
       } 
  return retorno; 
  }
