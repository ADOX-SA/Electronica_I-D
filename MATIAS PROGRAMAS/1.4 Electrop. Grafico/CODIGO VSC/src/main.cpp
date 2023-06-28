#include <Arduino.h>

int Leer_Pagina(String);
int Preguntar_Pagina();
void Parpadeo(int);
void Enviar_Nextion(String);

#define NEXTION_TX D4 // D4 CON RX DEL SIM808
#define NEXTION_RX D3 // D3 CON TX DEL SIM808
#include <SoftwareSerial.h>
SoftwareSerial Nextion(NEXTION_RX, NEXTION_TX); // RX, TX

void setup()
{
  Nextion.begin(115200);
  pinMode(D5, INPUT_PULLUP);
  pinMode(D6, INPUT_PULLUP);
  pinMode(D7, INPUT_PULLUP);
  pinMode(D0, OUTPUT);
  pinMode(D4, OUTPUT);
  Parpadeo(D0);
  Parpadeo(D4);
}

void loop()
{
  /*
  // Anda bien:
  if (Nextion.available() > 0)
  {
    String datos = Nextion.readString();
    int PaginaActual = Leer_Pagina(datos);
    if (PaginaActual != -1)
    {
      Nextion.print("\n Pagina actual: " + String(PaginaActual));
      Parpadeo(D0);
    }
    else
    {
      Nextion.print("\n No coincide el formato con sendme");
      Parpadeo(D4);
    }
  }
  */

  if (digitalRead(D5) == LOW)
  {
    for(int j=0;j<20;j++){
    String aux = "add 1,0,0";
    Enviar_Nextion(aux);
    }
    /*
    int pag = Preguntar_Pagina();
    if (pag != -1)
    {
      Nextion.print("\n\n Pagina actual: " + String(pag));
      Parpadeo(D0);
    }
    else
    {
      Nextion.print("\n Error!");
      digitalWrite(D4, LOW);
      delay(500);
      digitalWrite(D4, HIGH);
    }
    */
    while (digitalRead(D5) == LOW)
    {
      delay(1);
    }
  }

  //------------------Pulsador d6

  /*
  int cantidad de mediciones
  cada medicion tiene:
      -valor de corriente
      -tiempo desde la ultima medicion
  --------------------------------------
  En nextion:
  el Grafico:
        - s0.gdw = "num"  //configuramos el ancho de la grilla
        - int Cant_Max_grafico = 420 = s0.w //Cantidad maxima de datos que se pueden
        agregar al grafico, que es igual que su ancho.
        - (int) Divisiones de la grilla s0.gdw
        -
  */

  if (digitalRead(D6) == LOW)
  {
    String aux = "";

    int Cant_Mediciones = 54;
    int valores[Cant_Mediciones] = {0, 15, 0, 15, 0, 15,
                                    230, 240, 231, 245, 255, 255,
                                    0, 10, 15, 2, 1, 14,
                                    249, 200, 244, 244, 415, 231,
                                    2, 1, 3, 2, 15, 22,
                                    240, 246, 210, 235, 254, 244,
                                    12, 10, 1, 2, 3, 6,
                                    230, 155, 244, 254, 249, 250,
                                    12, 10, 2, 25, 1, 2};

    int tiempo[Cant_Mediciones] = {10, 10, 10, 10, 10, 10,
                                   25, 11, 10, 15, 10, 70,
                                   11, 13, 14, 14, 12, 35,
                                   11, 150, 16, 14, 10, 44,
                                   15, 20, 15, 75, 12, 20,
                                   10, 5, 14, 15, 16, 25,
                                   1, 2, 18, 100, 14, 12,
                                   10, 140, 10, 15, 2, 23,
                                   5, 6, 10, 100, 10, 1};

    int Maximo_Valor = valores[0];
    for (int x = 0; x < Cant_Mediciones; x++)
    {
      if (valores[x] > Maximo_Valor)
      {
        Maximo_Valor = valores[x];
      }
    }

    int Tiempo_Total = 0;
    int Cant_Max_Grafico_X = 420;
    int Cant_Max_Grafico_Y = Maximo_Valor; // 255?

    // Configuracion de la  grilla
    int Graf_Cant_Div_x = 7;
    int Graf_Cant_Div_y = 7;
    int Graf_Div_x = 60; // s0.gdw
    int Graf_Div_y = 40; // s0.gdh

    for (int i = 0; i < Cant_Mediciones; i++)
    {
      Tiempo_Total += tiempo[i];
    }

    float Escala_Tiempo = (float)Cant_Max_Grafico_X / (float)Tiempo_Total;
    Graf_Div_x = Tiempo_Total / Graf_Cant_Div_x;

    // float Escala_Y = (float)Cant_Max_Grafico_Y / (float)Maximo_Valor;
    Graf_Div_y = Cant_Max_Grafico_Y / Graf_Cant_Div_y;

    Nextion.print("\n\n\n\n");
    Nextion.print("\n Tiempo Total: " + String(Tiempo_Total));
    Nextion.print("\n Escala_Tiempo: " + String(Escala_Tiempo));
    Nextion.print("\n Cant divisiones X: " + String(Graf_Cant_Div_x));
    Nextion.print("\n Division X: " + String(Graf_Div_x));
    Nextion.print("\n Cant divisiones Y: " + String(Graf_Cant_Div_y));
    Nextion.print("\n Division Y: " + String(Graf_Div_y));
    Nextion.print("\n El maximo es: " + String(Maximo_Valor));
    Nextion.print("\n\n\n\n");

    //----------- Configuracion de las etiquetas del eje X
    int aux_int = Graf_Div_x;
    for (int i = 0; i < Graf_Cant_Div_x - 1; i++)
    {
      aux = "x" + String(i + 1) + ".val=" + String(aux_int);
      Enviar_Nextion(aux);
      aux_int += Graf_Div_x;
      // Nextion.print("\t-" + String(i));
    }

    //----------- Configuracion de las etiquetas del eje X
    aux_int = Graf_Div_y;
    for (int i = 0; i < Graf_Cant_Div_y - 1; i++)
    {
      aux = "y" + String(i + 1) + ".val=" + String(aux_int);
      Enviar_Nextion(aux);
      aux_int += Graf_Div_y;
      // Nextion.print("\t-" + String(i));
    }

    // Enviando GRAFICO--------------
    int contador = 0;
    for (int i = Cant_Mediciones; i > 0; i--)
    {
      int lim = (int)(Escala_Tiempo * tiempo[i]);
      float escala = 0;

      // float valor_parcial = (float)valores[i];
      float valor_parcial = (float)(map(valores[i], 0, Maximo_Valor + 1, 0, 255));
      float valor_parcial_sig = (float)(map(valores[i - 1], 0, Maximo_Valor + 1, 0, 255));

      if (valor_parcial > valor_parcial_sig && lim > 0)
        escala = (float)(valor_parcial - valor_parcial_sig) / (float)lim;
      else if (valor_parcial < valor_parcial_sig && lim > 0)
        escala = (float)(valor_parcial_sig - valor_parcial) / (float)lim;
      else if (lim == 0)
        escala = 0;

      Nextion.print("\n\n i: " + String(i) + " -valor: " + String(valores[i]) + " :" + String(valor_parcial) + " -lim: " + String(lim) + " -escala: " + String(escala) + "\n");

      //Este for es para generar la rampa:
      for (int y = 0; y < lim; y++)
      {
        aux = "add 1,0,";
        aux += String((int)valor_parcial);
        Enviar_Nextion(aux);
        contador++;

        if (valor_parcial > valor_parcial_sig)
        {
          if (valor_parcial_sig > 100 || valor_parcial < 50) // Este if es para que no haga la rampa cuando el valor es muy bajo
            valor_parcial -= escala;
        }
        else if (valor_parcial < valor_parcial_sig)
        {
          if ((valor_parcial + escala) < 255)
            valor_parcial += escala;
        }
      }
    }
    Nextion.print("\n\n\n Cantidad de envios: " + String(contador) + "\n\n");

    /*
        for (int i = 0; i < Cant_Mediciones; i++)
        {
          float valor_parcial = (float)map(valores[i], 0, Maximo_Valor + 1, 0, 255);
          Nextion.print("\n Valor: " + String(valores[i]) + "  Mapeado: " + String(valor_parcial));
        }
    */
    while (digitalRead(D6) == LOW)
    {
      delay(1);
    }
  }

  //--------------------------------------------------
  //---------------- Pulsador D7 ---------------------

  if (digitalRead(D7) == LOW)
  {
    for (int x = 0; x < 420; x++)
    {
      Enviar_Nextion("add 1,0,0");
    }

    while (digitalRead(D7) == LOW)
    {
      delay(1);
    }
  }
}

//----- Funcion Leer pagina -----
//----- Hay que enviarle el string leido del puerto.
//----- Si el string no tiene el formato del comando sendme, devuelve -1.
//----- Si es el formato del sendme retorna el numero de pagina.
int Leer_Pagina(String datosRecibidos)
{
  int retorno = -1;
  if (datosRecibidos[0] == 102)
    if (datosRecibidos[2] == 255)
      if (datosRecibidos[3] == 255)
        if (datosRecibidos[4] == 255)
        {
          int pagina = datosRecibidos[1];
          retorno = pagina;
        }
  return retorno;
}
//----- Fin de funcion

/* Funcion PREGUNTAR PAGINA */
// Devuelve el pageID
int Preguntar_Pagina()
{
  // Enviamos 0xFF 0xFF 0xFF y despues vaciamos el puerto!
  Nextion.write(0xff);
  Nextion.write(0xff);
  Nextion.write(0xff);
  while (Nextion.available() > 0)
    Nextion.read();

  // Declaracion de variables:
  int retorno = -1;
  String str = "";

  // Enviamos el comando:
  Nextion.print("sendme");
  Nextion.write(0xff);
  Nextion.write(0xff);
  Nextion.write(0xff);

  // Mientras no haya una respuesta esperamos:
  while (Nextion.available() == 0)
    delay(1);

  // Usando Nextion.readString() funciona bien!
  str = Nextion.readString();

  // Verificamos que el formato sea el correcto!
  if (str[0] == 102)
    if (str[2] == 255)
      if (str[3] == 255)
        if (str[4] == 255)
        {
          int pagina = str[1];
          retorno = pagina;
        }
  return retorno;
}
/* Fin de la funcion */

/* Funcion Parpadeo */
void Parpadeo(int port)
{
  for (int x = 0; x < 3; x++)
  {
    digitalWrite(port, LOW);
    delay(50);
    digitalWrite(port, HIGH);
    delay(50);
  }
}

/* Funcion ENVIAR COMANDO A NEXTION */
void Enviar_Nextion(String comando)
{
  // Enviamos 0xFF 0xFF 0xFF y despues vaciamos el puerto!
  Nextion.write(0xff);
  Nextion.write(0xff);
  Nextion.write(0xff);
  while (Nextion.available() > 0) // Aunque lo saquemos no envia mas rapido
    Nextion.read();

  // Enviamos el comando:
  Nextion.print(comando);
  Nextion.write(0xff);
  Nextion.write(0xff);
  Nextion.write(0xff);
}