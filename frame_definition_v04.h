//Estrutura para los paquetes recibidos, Nelson Gatica, 05-09-2016

typedef struct dataframe_receive{
  int8_t start; //1B
  
  int16_t f_length;//2B
  
  int8_t  type;//1B
  
  int16_t address1;//2B 
  int16_t address2;//2B 
  int16_t address3;//2B 
  int16_t address4;//2B 

  int8_t rssi;//1B
  int8_t options;//1B
  int8_t data1;//1B
  int8_t data2;//1B
  int8_t Checksum;//1B  (TOTAL 18 Bytes)
};

/*union{
  int16_t b[2];
  uint32_t w;
}IMU_time;//antes solo int32_t
*/
