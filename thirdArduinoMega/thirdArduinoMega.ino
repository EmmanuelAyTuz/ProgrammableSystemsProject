/*
  Sistemas programables
  Domotica
  ITSVA
*/
#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>

//PIN PARA EL LED PISCINA
const int LEDPISC = 22;

//PINES PWM PARA EL MOTOR VENTILADOR
const int VENTI = 2;

//PINES PWM PARA EL MOTOR CORTINA
const int SHORA = 5;
const int SCHORA = 6;

//PIN PARA EL LED PRINCIPAL
const int LEDPRIN = 24;

//PIN PARA EL LED ENTRADA
const int LEDENTRA = 26;

//PIN PWM PARA EL CUARTO GRANDE
const int LEDCUARTOG = 3;

//PIN PARA EL SPEAKER
const int SPK = 28; // Pin de la bocina

//PIN PARA EL SENSOR PIR
const int PINMOV = 30;

//PIN PWM LED ALARMA

int vSensor = -1; // Variable con el valor del sensor

//NOTAS MUSICALES
int c[5] = {131, 262, 523, 1046, 2093}; // Frecuencias 4 octavas de Do
int cs[5] = {139, 277, 554, 1108, 2217}; // Do#
int d[5] = {147, 294, 587, 1175, 2349}; // Re
int ds[5] = {156, 311, 622, 1244, 2489}; // Re#
int e[5] = {165, 330, 659, 1319, 2637}; // Mi
int f[5] = {175, 349, 698, 1397, 2794}; // Fa
int fs[5] = {185, 370, 740, 1480, 2960}; // Fa#
int g[5] = {196, 392, 784, 1568, 3136}; // Sol
int gs[5] = {208, 415, 831, 1661, 3322}; // Sol#
int a[5] = {220, 440, 880, 1760, 3520}; // La
int as[5] = {233, 466, 932, 1866, 3729}; // La#
int b[5] = {247, 494, 988, 1976, 3951}; // Si

#define REQ_BUF_SZ   20 //Tamaño del búfer utilizado para capturar solicitudes HTTP

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //Direccion Fisica MAC
IPAddress ip(192, 168, 0, 110);                      //IP Local a configurar
EthernetServer server(80);                           //Puerto del servidor
File webFile;                                        //Intermediario para los archivos SD

char HTTP_req[REQ_BUF_SZ] = {0}; // buffered HTTP request, stored as null terminated string
char req_index = 0;              // index into HTTP_req buffer

void setup()
{
  //Deshabilitar chip Ethernet
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);

  Serial.begin(9600); //Inicializa el puerto serial

  Serial.println("Iniciando SDCARD..."); //Inicialización de SDCARD
  if (!SD.begin(4)) {
    Serial.println("ERROR -> No se puede iniciar la SDCARD");
    return;
  }

  Serial.println("EXITOSO -> Inicialización de SDCARD");
  if (!SD.exists("index.htm")) { //Verificar si existe el archivo
    Serial.println("ERROR -> Archivo index.htm no encontrado");
    return;
  }

  Serial.println("EXITOSO -> Archivo(s) encontrado(s)");

  Ethernet.begin(mac, ip);  //Inicializa el dispositivo Ethernet
  server.begin();           //Esperar conexiones de los clientes

  Serial.print("El Servidor es: ");
  Serial.println(Ethernet.localIP()); //Imprime la direccion IP Local

  //MODO PINES
  pinMode(SHORA, OUTPUT);
  pinMode(SCHORA, OUTPUT);
  pinMode(VENTI, OUTPUT);
  pinMode(LEDPRIN, OUTPUT);
  pinMode(LEDENTRA, OUTPUT);
  pinMode(LEDCUARTOG, OUTPUT);
  pinMode(LEDPISC, OUTPUT);
  pinMode(PINMOV, INPUT); // Modo del PinSensor a entrada
  pinMode(SPK, OUTPUT); // Modo de la bocina a salida
}

void loop()
{
  pagina();
  vSensor = digitalRead(PINMOV); // Valor del sensor al estar activo / inactivo
  delay (150);
  if (vSensor == HIGH) { // Si es activado el sensor
    //Serial.println("mov-1"); // Escribir texto de advertencia
    tonoEntrada(); // Encender tono
  }

  if (vSensor == LOW) { // En caso contrario
    //Serial.println("mov-0"); // Escribir texto normal
    noTone(SPK);
  }
}

void tonoEntrada () {
  // Melodias aleatorias
  int melodia = random(1, 4);
  switch (melodia) {
    case 1:
      startwards();
      break;
    case 2:
      harrypotter();
      break;
    case 3:
      cruelangelthesis();
      break;
  }
}

//Melodias
void cruelangelthesis ()
{
  // Do Re# Fa Re#
  nota(c[2], 550); noTone(SPK); delay(50);
  nota(ds[2], 350); noTone(SPK); delay(50);
  nota(f[2], 250); noTone(SPK); delay(50);
  nota(ds[2], 200); noTone(SPK); delay(50);

  // Fa Fa Fa
  nota(f[2], 175); noTone(SPK); delay(50);
  nota(f[2], 175); noTone(SPK); delay(50);
  nota(f[2], 175); noTone(SPK); delay(50);

  // La# Sol# Sol Fa Sol
  nota(as[2], 250); noTone(SPK); delay(50);
  nota(gs[2], 150); noTone(SPK); delay(50);
  nota(g[2], 200); noTone(SPK); delay(50);
  nota(f[2], 150); noTone(SPK); delay(50);
  nota(g[2], 200); noTone(SPK); delay(50);

  // Sol La# Do&quot; Fa
  nota(g[2], 150); noTone(SPK); delay(50);
  nota(as[2], 250); noTone(SPK); delay(50);
  nota(c[3], 550); noTone(SPK); delay(50);
  nota(f[2], 250); noTone(SPK); delay(50);

  // Re# - La# La#
  nota(ds[2], 450); noTone(SPK); delay(50);
  nota(as[2], 250); noTone(SPK); delay(50);
  nota(as[2], 250); noTone(SPK); delay(50);

  // Sol La# La# Do&quot;
  /*nota(g[2], 200); noTone(SPK); delay(50);
  nota(as[2], 250); noTone(SPK); delay(50);
  nota(as[2], 250); noTone(SPK); delay(50);
  nota(c[3], 550); noTone(SPK); delay(50);

  // Do Re# Fa Re#
  nota(c[2], 150); noTone(SPK); delay(50);
  nota(ds[2], 350); noTone(SPK); delay(50);
  nota(f[2], 250); noTone(SPK); delay(50);
  nota(ds[2], 200); noTone(SPK); delay(50);

  // Fa Fa Fa La# Sol# Sol Fa Sol
  nota(f[2], 175); noTone(SPK); delay(50);
  nota(f[2], 175); noTone(SPK); delay(50);
  nota(f[2], 175); noTone(SPK); delay(50);
  nota(as[2], 250); noTone(SPK); delay(50);
  nota(gs[2], 150); noTone(SPK); delay(50);
  nota(g[2], 550); noTone(SPK); delay(50);
  nota(f[2], 175); noTone(SPK); delay(50);
  nota(g[2], 200); noTone(SPK); delay(50);

  // Sol La# Do Fa
  nota(g[2], 100); noTone(SPK); delay(50);
  nota(as[2], 250); noTone(SPK); delay(50);
  nota(c[3], 600); noTone(SPK); delay(50);
  nota(f[2], 175); noTone(SPK); delay(50);

  // Re# - La# La# Sol La# La# Do
  nota(ds[2], 550); noTone(SPK); delay(50);
  nota(as[2], 250); noTone(SPK); delay(50);
  nota(as[2], 250); noTone(SPK); delay(50);
  nota(g[2], 200); noTone(SPK); delay(50);
  nota(as[2], 250); noTone(SPK); delay(50);
  nota(as[2], 250); noTone(SPK); delay(50);
  nota(c[3], 600); noTone(SPK); delay(50);
  delay(1500);*/
}

void startwards ()
{
  nota(d[1], 150); noTone(SPK); delay(50);
  nota(d[1], 150); noTone(SPK); delay(50);
  nota(d[1], 150); noTone(SPK); delay(50);
  nota(g[1], 900); noTone(SPK); delay(150);
  nota(d[2], 900); noTone(SPK); delay(50);
  nota(c[2], 150); noTone(SPK); delay(50);
  nota(b[1], 150); noTone(SPK); delay(50);
  nota(a[1], 150); noTone(SPK); delay(50);
  nota(g[2], 900); noTone(SPK); delay(150);
  nota(d[2], 900); noTone(SPK); delay(100);
  /*nota(c[2], 150); noTone(SPK); delay(50);
  nota(b[1], 150); noTone(SPK); delay(50);
  nota(a[1], 150); noTone(SPK); delay(50);
  nota(g[2], 900); noTone(SPK); delay(150);
  nota(d[2], 900); noTone(SPK); delay(100);
  nota(c[2], 150); noTone(SPK); delay(50);
  nota(b[1], 150); noTone(SPK); delay(50);
  nota(c[2], 150); noTone(SPK); delay(50);
  nota(a[1], 1200); noTone(SPK); delay(2000);*/
  noTone(SPK);
  delay(1000);
}

void harrypotter () {
  nota(b[2], 500);
  nota(e[3], 1000);
  nota(g[3], 250);
  nota(fs[3], 250);
  nota(e[3], 1000);
  nota(b[3], 500);
  nota(a[3], 1250);
  nota(fs[3], 1000);
  nota(b[2], 500);
  nota(e[3], 1000);
  nota(g[3], 250);
  nota(fs[3], 250);
  nota(d[3], 1000);
  nota(e[3], 500 );
  nota(b[2], 1000 );
  noTone(SPK); delay(1000);
  /*nota(b[2], 500);
  nota(e[3], 1000);
  nota(g[3], 250);
  nota(fs[3], 250);
  nota(e[3], 1000);
  nota(b[3], 500);
  nota(d[4], 1000);
  nota(cs[4], 500);
  nota(c[4], 1000);
  nota(a[3], 500);
  nota(c[4], 1000);
  nota(b[3], 250);
  nota(as[3], 250);
  nota(b[2], 1000);
  nota(g[3], 500);
  nota(e[3], 1000);
  noTone(SPK);
  delay(2000);*/
}

void nota(int frec, int t)
{
  tone(SPK, frec);     // suena la nota frec recibida
  delay(t);            // para despues de un tiempo t
}

void pagina() {
  EthernetClient client = server.available();  //Verificar si esta disponible el servidor

  if (client) { //Cliente Ethernet disponible
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {   //Datos del cliente disponibles para leer
        char c = client.read(); //Leer 1 byte (caracter) del cliente
        //Primera parte del búfer de la solicitud HTTP en el arreglo HTTP_req (cadena)
        //Dejar el último elemento en la matriz como 0 para terminar la cadena nula (REQ_BUF_SZ - 1)
        if (req_index < (REQ_BUF_SZ - 1)) {
          HTTP_req[req_index] = c;          //Guardar el carácter de solicitud HTTP
          req_index++;
        }

        Serial.print (c); //Imprime el carácter de solicitud HTTP al monitor en serie

        if (c == '\n' && currentLineIsBlank) {

          if (StrContains(HTTP_req, "GET / ")) { //Cargar index <--StrContains(HTTP_req, "GET / ") || StrContains(HTTP_req, "GET /index.htm")
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            //client.println("Connnection: Keep-Alive");
            loadpage(client);
          }

          if (StrContains(HTTP_req, "GET /js/script.js")) {
            webFile = SD.open("js/script.js");
            if (webFile) {
              client.println("HTTP/1.1 200 OK");
              client.println("Content-Type: application/javascript");
              client.println();
            }
          }

          if (StrContains(HTTP_req, "GET /css/style.css")) { //Cargar estilos
            webFile = SD.open("css/style.css");
            if (webFile) {
              client.println("HTTP/1.1 200 OK");
              client.println("Content-Type: text/css");
              client.println();
            }
          }

          if (StrContains(HTTP_req, "GET /img/i.png")) { //Cargar estilos
            webFile = SD.open("img/i.png");
            if (webFile) {
              client.println("HTTP/1.1 200 OK");
              client.println();
            }
          }

          //Acciones
          if (StrContains(HTTP_req, "GET /?")) {
            String req = HTTP_req;
            int valor;
            String acciont;
            for (int i = 0; i < req.length(); i++) {
              if (req.substring(i, i + 1) == "=") {
                acciont = req.substring(0, i);
                valor = req.substring(i + 1).toInt();
                break;
              }
            }
            Serial.println();
            Serial.println(req);
            Serial.println(valor);
            Serial.println(acciont);
            Serial.println();
            switch (valor) {
              case 0: //Cortina
                if (acciont == "GET /?on") {
                  cortina(1, 650); //<---Abrir
                }
                if (acciont == "GET /?off") {
                  cortina(2, 250); //<---Cerrar
                }
                break;
              case 1://Pisicina - Patio
                if (acciont == "GET /?on") {
                  digitalWrite(LEDPISC, HIGH);
                }
                if (acciont == "GET /?off") {
                  digitalWrite(LEDPISC, LOW);
                }
                break;
              case 2://Principal
                if (acciont == "GET /?on") {
                  digitalWrite(LEDPRIN, HIGH);
                }
                if (acciont == "GET /?off") {
                  digitalWrite(LEDPRIN, LOW);
                }
                break;
              case 3://Entrada
                if (acciont == "GET /?on") {
                  digitalWrite(LEDENTRA, HIGH);
                }
                if (acciont == "GET /?off") {
                  digitalWrite(LEDENTRA, LOW);
                }
                break;
            }
            if (acciont == "GET /?venti") {
              controlbasico(VENTI, getValor(valor, 0, 240)); //HTTP_req<---Encender v 0, 30, 60, 90
            }
            if (acciont ==  "GET /?light") {
              controlbasico(LEDCUARTOG, getValor(valor, 0, 250)); //<---Encender v 0, 30, 60, 90
            }
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            //client.println("Connnection: Keep-Alive");
            client.println();
            loadpage(client);
          }

          if (webFile) { //Cargar página web al cliente
            while (webFile.available()) {
              client.write(webFile.read());
            }
            webFile.close();
          }

          //Restablecer el índice del búfer y todos los elementos del búfer a 0
          req_index = 0;
          StrClear(HTTP_req, REQ_BUF_SZ);
          break;
        }

        if (c == '\n') {
          //EL último carácter en la línea del texto recibido
          //Comienza una nueva línea con el siguiente carácter leído
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          //Se recibió un carácter de texto del cliente
          currentLineIsBlank = false;
        }
      }
    }
    delay(50);      //Tiempo al navegador web para recibir los datos
    client.stop();   //Cerrar conexion
  }
}

// sets every element of str to 0 (clears array)
void StrClear(char *str, char length)
{
  for (int i = 0; i < length; i++) {
    str[i] = 0;
  }
}

// searches for the string sfind in the string str
// returns 1 if string found
// returns 0 if string not found
char StrContains(char *str, char *sfind)
{
  char found = 0;
  char index = 0;
  char len;

  len = strlen(str);

  if (strlen(sfind) > len) {
    return 0;
  }
  while (index < len) {
    if (str[index] == sfind[found]) {
      found++;
      if (strlen(sfind) == found) {
        return 1;
      }
    }
    else {
      found = 0;
    }
    index++;
  }

  return 0;
}

void loadpage(EthernetClient client) {
  /*client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connnection: close");
    client.println();*/
  webFile = SD.open("index.htm");        // Abrir index
}

void cortina(int accion, int tiempo) {
  if (accion == 1) {
    // ABRIR CORTINA
    analogWrite(SHORA, 255);
    analogWrite(SCHORA, 0);

  } else if (accion == 2) {
    // CERRAR CORTINA
    analogWrite(SHORA, 0);
    analogWrite(SCHORA, 255);
  }
  delay(tiempo);
  analogWrite(SHORA, 0);
  analogWrite(SCHORA, 0);
}

void controlbasico(int PIN, int nivelpwm) {
  analogWrite(PIN, nivelpwm);
}

int getValor(int n, int minimo, int maximo) {
  /*int valor;
    for (int i = 0; i < texto.length(); i++) {
    if (texto.substring(i, i + 1) == "=") {
      valor = texto.substring(i + 1).toInt();
      break;
    }
    }*/
  if (n >= minimo && n <= maximo) {
    return n;
  }
  return 0;
  Serial.println(n);
}


