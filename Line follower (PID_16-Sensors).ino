#include <EEPROM.h>
#include "analogmuxdemux.h"
#define READPIN A0
#define NO_PINS 8

AnalogMux amux(A1, A2, A3, READPIN);
unsigned long awal, nilai, selisih, waktu = 100, sekarang, ayeuna, kamari, ayeuna2, kamari2, kamari3; 
char buff[100];
int value, pinno = 0, keitung = 0;
int address = 0;
int pin = A0, merah = 0, merah1 = 0,
    a = A1,
    b = A2, s[8], sb[8],
    c = A3,
    d = A5, e = A6, f = A7, pin2 = A4, isib,
    kondisisensor, kondisi[8],
    lasterror, state,
    state2,
    state3,
    setting = 0,
    setting2 = 0,
    simpan,
    sp = 50,
    x, kd, i,
    error,
    moveControl,
    kecepatanMotorKanan,
    kecepatanMotorKiri,
    rate, kp, nkp, nkd, maxs[8], mins[8], maxb[8], minb[8], isi = 0, peka[8], pekab[8], setpoint = 150;
#define ledr 8
#define ledl 7
void setup() {
  EEPROM.write(8, 20);
  EEPROM.write(9, 7);
  Serial.begin(9600);
  for (i = 0; i <= 7; i++) {
    maxs[i] = 0;
    mins[i] = 1023;
    maxb[i] = 0;
    minb[i] = 1023;
    isi = EEPROM.read(i);
    peka[i] = isi * 4;

    isib = EEPROM.read(i + 20);
    pekab[i] = isib * 4;
  }
  kp = EEPROM.read(8);
  kd = EEPROM.read(9);
  for (i = 0; i <= 7; i++) {
    Serial.print("nilai tengah sensor "); Serial.print(i); Serial.print(" : "); Serial.print(peka[i]); Serial.println(" ");
  }
  Serial.print("kp: ");
  Serial.print(EEPROM.read(8));
  Serial.println("");
  Serial.print("kd: ");
  Serial.print(EEPROM.read(9));
  Serial.println("");
  pinMode(ledl, OUTPUT);
  pinMode(ledr, OUTPUT);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  pinMode(pin, INPUT);
  pinMode(a, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(c, OUTPUT);
  pinMode(A7, OUTPUT);
  pinMode(A6, OUTPUT);
  pinMode(A5, OUTPUT);
  pinMode(pin2, INPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  analogWrite(9, LOW);
  analogWrite(5, LOW);
  analogWrite(6, LOW);
  analogWrite(10, LOW);
  delay(100);
  awal = millis();
  kamari = millis();
  kamari2 = millis();kamari3 = millis();
}

void loop() 
{
  cektombol();
  cekstate();
}

void cektombol() 
{
  if ((digitalRead(2) == LOW) && (setting == 0)) 
  {
    state = 4;
    state2 = 0;
    setting = 1;
    setting2 = 0;
  }
  if (digitalRead(3) == LOW) 
  {
    state = 0;
    setting = 1;
    state2 = 3;
    setting2 = 1;
  }
}
void cekstate() 
{
  if ((state == 4) && (setting == 1)) 
  {
    kalibrasi();
  }
  if ((state2 == 3) && (setting2 == 1)) 
  {
    jalankan();
  }
}
void kalibrasi() 
{
  nyalabareng();
  bacasensor();
  for (i = 0; i <= 7; i++) 
  {
    //depan
    if (s[i] > maxs[i]) 
    {
      maxs[i] = s[i];
    }
    if (s[i] < mins[i]) 
    {
      mins[i] = s[i];
    }
    //belakang
    if (sb[i] > maxb[i]) 
    {
      maxb[i] = sb[i];
    }
    if (sb[i] < minb[i]) 
    {
      minb[i] = sb[i];
    }
    peka[i] = (maxs[i] + mins[i]) / 2;//depan
    pekab[i] = (maxb[i] + minb[i]) / 2;//belakang

    //depan
    address = i;
    value = peka[i] / 4;
    EEPROM.write(address, value);
    //belakang
    value = pekab[i] / 4;
    address = i + 20;
    EEPROM.write(address, value);

  }
  
  ledmati();
  tampilkandata();
}
void tampilkandata() 
{
  for (i = 0; i <= 7; i++) 
  {
    Serial.print("nilai tengah sensor ");
    Serial.print(i); Serial.print(" : ");
    Serial.print(peka[i]); Serial.println(" ");
  }
  Serial.print("kp: ");
  Serial.print(EEPROM.read(8));
  Serial.println("");
  Serial.print("kd: ");
  Serial.print(EEPROM.read(9));
  Serial.println("");
}
void bacasensor() 
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  s[0] = analogRead(pin);
  digitalWrite(a, LOW);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  s[1] = analogRead(pin);
  digitalWrite(a, HIGH);
  digitalWrite(b, LOW);
  digitalWrite(c, HIGH);
  s[2] = analogRead(pin);
  digitalWrite(a, LOW);
  digitalWrite(b, LOW);
  digitalWrite(c, HIGH);
  s[3] = analogRead(pin);
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, LOW);
  s[4] = analogRead(pin);
  digitalWrite(a, LOW);
  digitalWrite(b, HIGH);
  digitalWrite(c, LOW);
  s[5] = analogRead(pin);
  digitalWrite(a, HIGH);
  digitalWrite(b, LOW);
  digitalWrite(c, LOW);
  s[6] = analogRead(pin);
  digitalWrite(a, LOW);
  digitalWrite(b, LOW);
  digitalWrite(c, LOW);
  s[7] = analogRead(pin);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  sb[0] = analogRead(pin2);
  digitalWrite(d, LOW);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  sb[1] = analogRead(pin2);
  digitalWrite(d, HIGH);
  digitalWrite(e, LOW);
  digitalWrite(f, HIGH);
  sb[2] = analogRead(pin2);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, HIGH);
  sb[3] = analogRead(pin2);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, LOW);
  sb[4] = analogRead(pin2);
  digitalWrite(d, LOW);
  digitalWrite(e, HIGH);
  digitalWrite(f, LOW);
  sb[5] = analogRead(pin2);
  digitalWrite(d, HIGH);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  sb[6] = analogRead(pin2);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  sb[7] = analogRead(pin2);
  //  int x=7;
  //  for ( pinno=0; pinno < NO_PINS; pinno++){
  //        amux.SelectPin(pinno);
  //        uint16_t reading = amux.AnalogRead();
  //        s[7-pinno]= reading;
  //  }
  for (i = 0; i <= 7; i++) 
  {
    Serial.print(s[i]); Serial.print(" ");
  } Serial.println();
  for (i = 0; i <= 7; i++) 
  {
    Serial.print(sb[i]); Serial.print(" ");
  } Serial.println();
}


void jalankan() 
{
  bacapekaPD();
  bacastate();
  bacakondisisensor();
  cekerror();
  jalankanwaktu(); 
    if (keitung == 2 ) 
    {
    error=-6;
  }
  putarkanmotor();
//  if (sekarang - kamari3 >= 100) {
//    kamari3 = sekarang;
//    if ((s[0] >= 390) && (s[0] <= 400)) {
//      digitalWrite(9, 0);
//      digitalWrite(10, 0);
//      digitalWrite(5, 0);
//      digitalWrite(6, 0);
//      state2 = 0;
//    }
//  }

  if (sekarang - kamari >= 100) 
  {
    kamari = sekarang;
    cekcabang();
  }

  if (keitung == 2) 
  {
    error = -6;
  }
  //  if (keitung == 11) {
  //    error = 6;
  //  }
  Serial.print(" keitung = ");
  Serial.print(keitung); Serial.println();

  ceklampu();
  lasterror = error;

  // printdata();
}
void ceklampu() 
{
  if (error <= -1) 
  {
    ledkanan();
  } else if (error >= 1) 
  {
    ledkiri();
  } else 
  {
    nyalabareng();
  }
}

void bacakondisisensor() 
{
  //s[0] = analogRead(A3);
  //  for ( pinno=0; pinno < NO_PINS; pinno++){
  //        amux.SelectPin(pinno);
  //        uint16_t reading = amux.AnalogRead();
  //        s[pinno]= reading;
  //  }

  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  s[0] = analogRead(pin);
  digitalWrite(a, LOW);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  s[1] = analogRead(pin);
  digitalWrite(a, HIGH);
  digitalWrite(b, LOW);
  digitalWrite(c, HIGH);
  s[2] = analogRead(pin);
  digitalWrite(a, LOW);
  digitalWrite(b, LOW);
  digitalWrite(c, HIGH);
  s[3] = analogRead(pin);
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, LOW);
  s[4] = analogRead(pin);
  digitalWrite(a, LOW);
  digitalWrite(b, HIGH);
  digitalWrite(c, LOW);
  s[5] = analogRead(pin);
  digitalWrite(a, HIGH);
  digitalWrite(b, LOW);
  digitalWrite(c, LOW);
  s[6] = analogRead(pin);
  digitalWrite(a, LOW);
  digitalWrite(b, LOW);
  digitalWrite(c, LOW);
  s[7] = analogRead(pin);

  for (i = 0; i <= 7; i++)
  {
    if (s[i] >= peka[i]) 
    {
      kondisi[i] = 0;
    }
    else 
    {
      kondisi[i] = 1;
    }
  }
  kondisisensor = 0;
  for (i = 0; i <= 7; i++) 
  {
    kondisisensor += kondisi[i] * (1 << i);
  }
  sprintf(buff, "%3d %3d %3d %3d %3d %3d %3d %3d ", s[7], s[6], s[5], s[4], s[3], s[2], s[1], s[0]);
  Serial.println(buff);
  sprintf(buff, "%d%d%d%d%d%d%d%d ", kondisi[7], kondisi[6], kondisi[5], kondisi[4], kondisi[3], kondisi[2], kondisi[1], kondisi[0]);
  Serial.println(buff);

}

void printdata() 
{
  Serial.print(" lasterror = "); Serial.print(lasterror);
  Serial.println(" ");
  Serial.print(" kanan = "); Serial.print(kecepatanMotorKanan);
  Serial.println(" ");
  Serial.print(" kiri = "); Serial.print(kecepatanMotorKiri);
  Serial.println(" ");
  Serial.print(" waktu awal = "); Serial.print(awal);
  Serial.println(" ");
  Serial.print(" waktu cek error = "); Serial.print(nilai);
  Serial.println(" ");
  Serial.print(" selisih waktu = "); Serial.print(selisih);
  Serial.println(" ");
}
void putarkanmotor() 
{
  if ((selisih > waktu) && (error == 0)) 
  {
    sp = 250; awal = sekarang;
  }

  if ((selisih > waktu) && (error == 1)) 
  {
    sp = 250; awal = sekarang;
  }
  if ((selisih > waktu) && (error == -1)) 
  {
    sp = 250; awal = sekarang;
  }
  if (error > 1 && error < -1) 
  {
    sp = 150;
    selisih = 0;
  }
  Serial.print(" error = "); Serial.print(error); Serial.println("");
  Serial.print(kondisisensor); Serial.println("");
  rate = (lasterror) - (error);
  moveControl = (kp * error) + (kd * rate);
  kecepatanMotorKanan = sp + (moveControl);
  kecepatanMotorKiri = sp - (moveControl);

  if (kecepatanMotorKanan > 141) 
  {
    kecepatanMotorKanan = 141;
  } else if (kecepatanMotorKanan < -141) 
  {
    kecepatanMotorKanan = -141;
  }
  if (kecepatanMotorKanan < 0) 
  {
    kecepatanMotorKanan *= -1;
    analogWrite(9, kecepatanMotorKanan);
    analogWrite(10, 0);
  } else 
  {
    if ((error == -5) || (error == -6)) 
    {
      kecepatanMotorKanan = 0;
    }
    analogWrite(9, 0);
    analogWrite(10, kecepatanMotorKanan);
  }
  if (kecepatanMotorKiri > 141) 
  {
    kecepatanMotorKiri = 141;
  } else if (kecepatanMotorKiri < -141) 
  {
    kecepatanMotorKiri = -141;
  }
  if (kecepatanMotorKiri < 0) 
  {
    kecepatanMotorKiri *= -1;
    analogWrite(5, kecepatanMotorKiri);
    analogWrite(6, 0);
  } else 
  {
    if ((error == 5)||(error==6)) 
    {
      kecepatanMotorKiri = 0;
    }
    analogWrite(5, 0);
    analogWrite(6, kecepatanMotorKiri);
  }

  if (keitung == 2 ) 
  {
    delay(200);analogWrite(6, 200);analogWrite(10, 200);delay(300);
  }

}

unsigned long jalankanwaktu() 
{
  sekarang = millis();
  selisih = sekarang - awal;
}

void cekcabang() 
{
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  sb[0] = analogRead(pin2);
  digitalWrite(d, LOW);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  sb[1] = analogRead(pin2);
  digitalWrite(d, HIGH);
  digitalWrite(e, LOW);
  digitalWrite(f, HIGH);
  sb[2] = analogRead(pin2);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, HIGH);
  sb[3] = analogRead(pin2);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, LOW);
  sb[4] = analogRead(pin2);
  digitalWrite(d, LOW);
  digitalWrite(e, HIGH);
  digitalWrite(f, LOW);
  sb[5] = analogRead(pin2);
  digitalWrite(d, HIGH);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  sb[6] = analogRead(pin2);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  sb[7] = analogRead(pin2);
  
  if ( (sb[1] <= pekab[1]) && (sb[2] <= pekab[2]) && (sb[5] <= pekab[5]) ) 
  {  
    keitung += 1;
    digitalWrite(13,HIGH);
  }
  else{digitalWrite(13,LOW);}
}

void bacastate() 
{
  if (digitalRead(3) == LOW) 
  {
    kp = kp + 1;
    EEPROM.write(8, kp);
    ledmati(); delay(10);
    ledkiri(); delay(100); ledmati();
  }
  if (digitalRead(2) == LOW) 
  {
    kp = kp - 1;
    EEPROM.write(8, kp);
    ledmati(); delay(10);
    ledkiri(); delay(100); ledmati();
  }

  if (digitalRead(12) == LOW) 
  {
    kd = kd - 1;
    EEPROM.write(9, kd);
    ledmati(); delay(10);
    ledkanan(); delay(100); ledmati();
  }

  if (digitalRead(11) == LOW) 
  {
    kd = kd + 1;
    EEPROM.write(9, kd);
    ledmati(); delay(10);
    ledkanan(); delay(100); ledmati();
  }
}

void ledkanan() 
{
  digitalWrite(ledr, HIGH); digitalWrite(ledl, LOW);
}

void ledkiri() 
{
  digitalWrite(ledl, HIGH); digitalWrite(ledr, LOW);
}

void ledmati() 
{
  digitalWrite(ledr, LOW); digitalWrite(ledl, LOW);
}

void nyalabareng() 
{
  digitalWrite(ledr, HIGH); digitalWrite(ledl, HIGH);
}
void cekerror() 
{
  switch (kondisisensor) 
  {
    case  0b10000000: error = -6; break;
    case  0b11000000: error = -5; break;
    case  0b11100000: error = -5; break;
    case  0b01000000: error = -4; break;
    case  0b01100000: error = -3; break;
    case  0b01110000: error = -3; break;
    case  0b00100000: error = -2; break;
    case  0b00110000: error = -1; break;
    case  0b00111000: error = -1; break;
    case  0b00010000: error = 0; break;
    case  0b00011000: error = 0; break;
    case  0b00011100: error = 0; break;
    case  0b00001000: error = 0; break;
    case  0b00001100: error = 1; break;
    case  0b00001110: error = 1; break;
    case  0b00000100: error = 2; break;
    case  0b00000110: error = 3; break;
    case  0b00000111: error = 3; break;
    case  0b00000010: error = 4; break;
    case  0b00000011: error = 5; break;
    case  0b00000001: error = 6; break;
    case  0b00000000: error = 6; break;
    //case  0b11010001: error = -6; break;
    //    case  0b11110001: error = -5; break;
    //    case  0b01110001: error = -4; break;
    //    case  0b01111111: error = -3; break;
    //    case  0b01011111: error = -2; break;
    //    case  0b01010101: error = -1; break;
    //    case  0b00010000: error = 0; break;
    //    //case  0b01010101: error = 0; break;
    //    case  0b01010111: error = 2; break;
    ////    case  0b01010111: error = 1; break;
    ////    case  0b01010111: error = 2; break;
    ////    case  0b01010111: error = 3; break;
    //    case  0b11110011: error = 4; break;
    //   // case  0b11110011: error = 5; break;
    ////    case  0b11110001: error = 6; break;
    default : error = lasterror; break;
  }
}

void bacapekaPD() 
{
  //  for (i = 0; i <= 7; i++) {
  //    isi = EEPROM.read(i);
  //    peka[i] = isi * 4;
  //  }
  kp = EEPROM.read(8);
  kd = EEPROM.read(9);
}

