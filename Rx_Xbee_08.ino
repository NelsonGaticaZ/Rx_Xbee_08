//Este programa es para poder adquirir la información del RSSI desde el coordinador           ///
//Última modificación hecha por Nelson Gatica el Lunes 14 de Septiembre del 2016, 18:01 horas ///
/////////////////////////////////////////////////////////////////////////////////////////////////

//Tarjeta microSD
#include <SPI.h>
#include <SdFat.h>
//Serial virtual
#include <SoftwareSerial.h>
//Reloj RTC del Seeeduino
#include <DS1337.h>
#include <Wire.h>

//Construye la estructura del frame de datos recibido
#include "frame_definition_v04.h" 

//Definición de variables
#define chipSelect  10      //pin para encender la tarjeta SD (protocolo SPI)
unsigned char buf[20]; 
unsigned char buf_send[17]; 
char          fileName[13]; 
int count=0;
unsigned long tiempo;
  unsigned char second;
  unsigned char minute;
  unsigned char hour;
  unsigned char month;
  unsigned char day;
  unsigned int year;
  byte pin_test=5;
  
//Definición de objetos
SdFat SD;
File archivo_datos;
dataframe_receive F_receive;
//SoftwareSerial mySerial(7, 8); // RX, TX
DS1337 RTC; //Create the R8025 object

void setup() {
Serial.begin(57600);//Velocidad de comunicación Serial
pinMode(pin_test,OUTPUT);
//mySerial.begin(57600);//Velocidad de comunicación Serial Virtual
begin_SDcard();
RTC.start(); // ensure RTC oscillator is running
build_fileName();
}

void loop()
{ 
 if (Serial.available()){
  //digitalWrite(pin_test,1);
  delay(10);//CUIDADO CON ESTE RETARDO; ES NECESARIO PARA TERMINAR DE LLENAR EL BUFFER
        while(Serial.available()){               // reading data into char array
            buf[count++]=Serial.read();      // writing data into array
            if(count == 20)break;
        }
  //         mySerial.write(buf,10);
  
  save_SD();
  //digitalWrite(pin_test,0);  
 }

 clearBufferArray();                         // call clearBufferArray function to clear the stored data from the array 
 count = 0;                                  // set counter of while loop to zero
}

void clearBufferArray(){// function to clear buffer array
    for (int i=0; i<count;i++){ 
     buf[i]=NULL;
     }
}

void begin_SDcard(){
 pinMode(chipSelect, OUTPUT);
 byte estatus_SD=0;
 
 while(estatus_SD==0){
  estatus_SD=SD.begin(chipSelect);

  if (estatus_SD){
    //mySerial.println("microSD Inicializada.");
   }
   else{
    //mySerial.println("Error microSD, o no esta presente");
   }
 }   

   delay(500);
}

void build_fileName(){
  tiempo=RTC.getTime();
  fileName[0]='N';
  fileName[1 ] = (tiempo/10000000)%10  +'0';
  fileName[2 ] = (tiempo/1000000)%10   +'0';
  fileName[3 ] = (tiempo/100000)%10    +'0';
  fileName[4 ] = (tiempo/10000)%10     +'0';
  fileName[5 ] = (tiempo/1000)%10      +'0';
  fileName[6 ] = (tiempo/100)%10       +'0';
  fileName[7 ] = (tiempo/10)%10        +'0';
  fileName[8 ] = '.';
  fileName[9 ] = 'T';
  fileName[10] = 'X';
  fileName[11] = 'T';
  fileName[12] = '\0';
  //mySerial.println("Nombre del archivo creado");

        second = RTC.getSeconds();
        minute = RTC.getMinutes();
        hour = RTC.getHours();        
        day = RTC.getDays();
        month = RTC.getMonths();        
        year = RTC.getYears();

        archivo_datos = SD.open(fileName, FILE_WRITE);
        
        if (archivo_datos){  
         archivo_datos.print("Fecha: ");archivo_datos.print(year);archivo_datos.print("-");
         archivo_datos.print(month);archivo_datos.print("-");
         archivo_datos.println(day);
         archivo_datos.print("Hora: ");archivo_datos.print(hour);archivo_datos.print(":");
         archivo_datos.print(minute);archivo_datos.print(":");
         archivo_datos.print(second);
         archivo_datos.println("");
         
         archivo_datos.close();
        }  
        else {
         //mySerial.println("Error opening SD");
        }
  delay(500);      
}

void save_SD(){  
dataframe_receive *p_F_receive = &F_receive;
archivo_datos = SD.open(fileName, FILE_WRITE);
F_receive.start    = buf[0];//Recibe 13 Byte porque solo se envia 3 byte de datos
F_receive.f_length = buf[1]<<8 | buf[2];
F_receive.type     = buf[3];

F_receive.address1 = buf[5]<<8 | buf[4];
F_receive.address2 = buf[7]<<8 | buf[6];
F_receive.address3 = buf[9]<<8 | buf[8];
F_receive.address4 = buf[11]<<8 | buf[10];

F_receive.rssi     = buf[12];
F_receive.options  = buf[13];
F_receive.data1    = buf[14];
F_receive.data2    = buf[15];
F_receive.Checksum = buf[16];

byte RSSI_valor = F_receive.rssi;
byte RSSI_data1 = F_receive.data1;
byte RSSI_data2 = F_receive.data2;

       if (archivo_datos){  
         archivo_datos.print(RSSI_valor);archivo_datos.print(",");
         archivo_datos.print(RSSI_data1);archivo_datos.print(",");
         archivo_datos.print(RSSI_data2);archivo_datos.print(",");archivo_datos.println(String(micros()));
         //archivo_datos.write(p_F_receive,18);
         archivo_datos.close();
       }  
       else{
       //mySerial.println("Error opening SD");
       }
}

/*void make_frame(){


}*/



