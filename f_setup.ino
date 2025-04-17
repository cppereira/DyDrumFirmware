
//==============================
//     SETUP
//==============================
void setup()
{
//  Serial.begin(115200);
//   while (!Serial);

//   // SavePresetsToEEPROM(); // Agora comenta ISSO
//   LoadAllEEPROM(); 


  pinMode(13, OUTPUT);   //LED
  
//------------------------------------
//    MULTIPLEX RETIRADO SE MEGA
//------------------------------------ 

//Ativa os pinos digitais
 pinMode(Choke1_Pin, INPUT_PULLUP);
 pinMode(Choke2_Pin, INPUT_PULLUP);
 pinMode(Choke3_Pin, INPUT_PULLUP);
 pinMode(Choke4_Pin, INPUT_PULLUP);
 pinMode(ChokeRide_Pin, INPUT_PULLUP);

//------------------------------------

  GlobalTime=TIMEFUNCTION;
  for (int count=0; count<NPin; count++)
  {
    Pin[count].set(count);
    //Pin[count].Time=Time+Pin[count].MaskTime;
    //MaxReadingSensor[count]=-1;
    //MaxRetriggerSensor[count]=0;//0xFF;
  }  
  
  #if SERIALSPEED
    Serial.begin(115200);    //Serial
  #else
    Serial.begin(31250);      // MIDI
  #endif
  Serial.flush();  
        
  #if defined(__AVR__) 
    analogReference(DEFAULT);
  #endif
  
  LoadAllEEPROM();
  
  #if defined(__AVR__)
  
  #if FASTADC
    setPrescaler(Prescaler_16);
  #endif
  
  #if VERYFASTADC
    setPrescaler(Prescaler_8);
  #endif
  
  #if VERYVERYFASTADC
    setPrescaler(Prescaler_4);
  #endif
  
  #elif defined(__arm__) 
    REG_ADC_MR = (REG_ADC_MR & 0xFFF0FFFF) | 0x00030000; //0x00020000;
  #endif 
    
  fastWrite(3,0);
  fastWrite(4,0);

  
}
