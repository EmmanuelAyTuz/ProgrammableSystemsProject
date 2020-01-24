/*
  Sistemas programables
  Domotica
  ITSVA
*/
// Notas musicales
int spk = 7; // Pin de la bocina
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

const int PINMOV = 12; // Pin del Sensor de Presencia
const int PINLED = 3; // Pin PWM del LED Rojo x2
//const int PINSPK = 8; // Pin de la Bocina

int duracion = 500; // Duración del sonido
int fMin = 2000; // Frecuencia más baja a emitir
int fMax = 4000; // Frecuencia más alta a emitir

int vSensor = -1; // Variable con el valor del sensor

float temp; // Variable para almacenar el valor obtenido del sensor (0 a 1023)
int PINTEMP = A0; // Variable del pin de entrada del sensor (A0)

int PINLEDLUZ = 5; //Pin LED PWM

const long A = 1000;     //Resistencia en oscuridad en KΩ
const int B = 15;        //Resistencia a la luz (10 Lux) en KΩ
const int Rc = 10;       //Resistencia calibracion en KΩ
const int LDRPin = A1;   //Pin del LDR

int V;
int ilum;

void setup() {
  Serial.begin(9600); // Serial
  pinMode(PINLED, OUTPUT); // Modo del PinLED a salida
  pinMode(PINMOV, INPUT); // Modo del PinSensor a entrada
  pinMode(spk, OUTPUT); // Modo de la bocina a salida
}

void loop() {
  sensorLuz();
  vSensor = digitalRead(PINMOV); // Valor del sensor al estar activo / inactivo
  //vSensor = HIGH;
  delay (150);
  if (vSensor == HIGH) { // Si es activado el sensor
    //Serial.println("mov-1"); // Escribir texto de advertencia
    tonoEntrada(); // Encender tono
  }

  if (vSensor == LOW) { // En caso contrario
    //Serial.println("mov-0"); // Escribir texto normal
    noTone(spk);
  }
  sensorTemp(50);
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

void alarma() {
  int i;
  for (i = fMin; i <= fMax; i++) { // Sonido más agudo
    tone(spk, i, duracion);
  }

  for (i = fMax; i >= fMin; i--) { // Sonido más grave
    tone(spk, i, duracion);
  }

  analogWrite(PINLED, 255);
  delay(500);
  analogWrite(PINLED, 0);
  delay(500);
}


void sensorTemp(float tempPeligro) {
  temp = analogRead(PINTEMP); // Con analogRead leemos el sensor, recuerda que es un valor de 0 a 1023
  temp = (5.0 * temp * 100.0) / 1024.0; // Calculamos la temperatura con la fórmula
  //Serial.println(temp); // Envia el dato al puerto serial
  if (temp > tempPeligro) {
    //digitalWrite(PINLED, HIGH); // Encender LED de advertencia
    alarma();
  } else {
    //digitalWrite(PINLED, LOW); // Apagar LED
    analogWrite(PINLED, 0);
    noTone(spk);
  }
  delay(750);
}

void sensorLuz() {
  V = analogRead(LDRPin);
  //ilum = ((long)(1024-V)*A*10)/((long)B*Rc*V);  //usar si LDR entre GND y A0
  ilum = ((long)V * A * 10) / ((long)B * Rc * (1024 - V)); //usar si LDR entre A0 y Vcc (como en el esquema anterior)

  Serial.println(ilum);
  if (ilum < 50) {
    analogWrite(PINLEDLUZ, 255);
  } else {
    analogWrite(PINLEDLUZ, 0);
  }
  delay(100);
}


//Melodias
void cruelangelthesis ()
{
  // Do Re# Fa Re#
  nota(c[2], 550); noTone(spk); delay(50);
  nota(ds[2], 350); noTone(spk); delay(50);
  nota(f[2], 250); noTone(spk); delay(50);
  nota(ds[2], 200); noTone(spk); delay(50);

  // Fa Fa Fa
  nota(f[2], 175); noTone(spk); delay(50);
  nota(f[2], 175); noTone(spk); delay(50);
  nota(f[2], 175); noTone(spk); delay(50);

  // La# Sol# Sol Fa Sol
  nota(as[2], 250); noTone(spk); delay(50);
  nota(gs[2], 150); noTone(spk); delay(50);
  nota(g[2], 200); noTone(spk); delay(50);
  nota(f[2], 150); noTone(spk); delay(50);
  nota(g[2], 200); noTone(spk); delay(50);

  // Sol La# Do&quot; Fa
  nota(g[2], 150); noTone(spk); delay(50);
  nota(as[2], 250); noTone(spk); delay(50);
  nota(c[3], 550); noTone(spk); delay(50);
  nota(f[2], 250); noTone(spk); delay(50);

  // Re# - La# La#
  nota(ds[2], 450); noTone(spk); delay(50);
  nota(as[2], 250); noTone(spk); delay(50);
  nota(as[2], 250); noTone(spk); delay(50);

  // Sol La# La# Do&quot;
  nota(g[2], 200); noTone(spk); delay(50);
  nota(as[2], 250); noTone(spk); delay(50);
  nota(as[2], 250); noTone(spk); delay(50);
  nota(c[3], 550); noTone(spk); delay(50);

  // Do Re# Fa Re#
  nota(c[2], 150); noTone(spk); delay(50);
  nota(ds[2], 350); noTone(spk); delay(50);
  nota(f[2], 250); noTone(spk); delay(50);
  nota(ds[2], 200); noTone(spk); delay(50);

  // Fa Fa Fa La# Sol# Sol Fa Sol
  nota(f[2], 175); noTone(spk); delay(50);
  nota(f[2], 175); noTone(spk); delay(50);
  nota(f[2], 175); noTone(spk); delay(50);
  nota(as[2], 250); noTone(spk); delay(50);
  nota(gs[2], 150); noTone(spk); delay(50);
  nota(g[2], 550); noTone(spk); delay(50);
  nota(f[2], 175); noTone(spk); delay(50);
  nota(g[2], 200); noTone(spk); delay(50);

  // Sol La# Do Fa
  nota(g[2], 100); noTone(spk); delay(50);
  nota(as[2], 250); noTone(spk); delay(50);
  nota(c[3], 600); noTone(spk); delay(50);
  nota(f[2], 175); noTone(spk); delay(50);

  // Re# - La# La# Sol La# La# Do
  nota(ds[2], 550); noTone(spk); delay(50);
  nota(as[2], 250); noTone(spk); delay(50);
  nota(as[2], 250); noTone(spk); delay(50);
  nota(g[2], 200); noTone(spk); delay(50);
  nota(as[2], 250); noTone(spk); delay(50);
  nota(as[2], 250); noTone(spk); delay(50);
  nota(c[3], 600); noTone(spk); delay(50);
  delay(1500);
}

void startwards ()
{
  nota(d[1], 150); noTone(spk); delay(50);
  nota(d[1], 150); noTone(spk); delay(50);
  nota(d[1], 150); noTone(spk); delay(50);
  nota(g[1], 900); noTone(spk); delay(150);
  nota(d[2], 900); noTone(spk); delay(50);
  nota(c[2], 150); noTone(spk); delay(50);
  nota(b[1], 150); noTone(spk); delay(50);
  nota(a[1], 150); noTone(spk); delay(50);
  nota(g[2], 900); noTone(spk); delay(150);
  nota(d[2], 900); noTone(spk); delay(100);
  nota(c[2], 150); noTone(spk); delay(50);
  nota(b[1], 150); noTone(spk); delay(50);
  nota(a[1], 150); noTone(spk); delay(50);
  nota(g[2], 900); noTone(spk); delay(150);
  nota(d[2], 900); noTone(spk); delay(100);
  nota(c[2], 150); noTone(spk); delay(50);
  nota(b[1], 150); noTone(spk); delay(50);
  nota(c[2], 150); noTone(spk); delay(50);
  nota(a[1], 1200); noTone(spk); delay(2000);
  noTone(spk);
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
  noTone(spk); delay(1000);
  nota(b[2], 500);
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
  noTone(spk);
  delay(2000);
}

void nota(int frec, int t)
{
  tone(spk, frec);     // suena la nota frec recibida
  delay(t);            // para despues de un tiempo t
}
