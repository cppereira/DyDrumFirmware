
//===========SETTING============
const byte NOTE      = 0x00;
const byte THRESOLD  = 0x01;
const byte SCANTIME  = 0x02;
const byte MASKTIME  = 0x03;
const byte RETRIGGER = 0x04;
const byte CURVE     = 0x05;
const byte XTALK     = 0x06;
const byte XTALKGROUP= 0x07;
const byte CURVEFORM = 0x08;
const byte CHOKENOTE = 0x09;
const byte DUAL      = 0x0A;
const byte TYPE      = 0x0D;
const byte CHANNEL   = 0x0E;
//===============================


// DEBUG PARA DESCOBRIR SE CONSIGO LER OS PINOS SALVOS NA EEPROM CORRETAMENTE, DEU CERTO!
// DEPOIS PRECISO REMOVER O TRECHO ABAIXO PARA CONTINUAR UTILIZANDO O FIRMWARE...
void PrintAllPadSettings() {
  for (int i = 0; i <= 14; i++) {
    Serial.print(F("---- Pad: ")); Serial.print(i); Serial.println(F(" ----"));
    Serial.print(F("Note: ")); Serial.println(Pin[i].Note);
    Serial.print(F("Threshold: ")); Serial.println(Pin[i].Thresold);
    Serial.print(F("ScanTime: ")); Serial.println(Pin[i].ScanTime);
    Serial.print(F("MaskTime: ")); Serial.println(Pin[i].MaskTime);
    Serial.print(F("Retrigger: ")); Serial.println(Pin[i].Retrigger);
    Serial.print(F("Curve: ")); Serial.println(Pin[i].Curve);
    Serial.print(F("CurveForm: ")); Serial.println(Pin[i].CurveForm);
    Serial.print(F("Gain: ")); Serial.println(Pin[i].Gain);
    Serial.println(F("--------------------------------"));    
  }
  Serial.flush();
}

//==============================
//    SETTING
//==============================
void SendPinSetting(byte pin,byte Set)
{
  if(Set==0x7F)//All
  { 
      simpleSysex(0x02,pin,0x00,Pin[pin].Note);//NOTE
      simpleSysex(0x02,pin,0x01,Pin[pin].Thresold);//THRESOLD
      simpleSysex(0x02,pin,0x02,Pin[pin].ScanTime);//SCANTIME
      simpleSysex(0x02,pin,0x03,Pin [pin].MaskTime);//MASKTIME
      simpleSysex(0x02,pin,0x04,Pin[pin].Retrigger);//RETRIGGER
      simpleSysex(0x02,pin,0x05,Pin[pin].Curve);//CURVE
      simpleSysex(0x02,pin,0x06,Pin[pin].Xtalk);//XTALK
      simpleSysex(0x02,pin,0x07,Pin[pin].XtalkGroup);//XTALKGROUP
      simpleSysex(0x02,pin,0x08,Pin[pin].CurveForm);//CURVEFORM
      simpleSysex(0x02,pin,0x09,Pin[pin].ChokeNote);//CHOKE
      simpleSysex(0x02,pin,0x0A,DualSensor(pin));//DUAL
      //simpleSysex(0x02,Pin,0x0B,DualNoteSensor[Pin]);//DUALNOTE
      //simpleSysex(0x02,Pin,0x0C,DualThresoldSensor[Pin]);//DUALTHRESOLD
      simpleSysex(0x02,pin,0x0D,Pin[pin].Type);//TYPE
      #if ENABLE_CHANNEL
      simpleSysex(0x02,pin,0x0E,Pin[pin].Channel);//CHANNEL
      #endif
      return;
  } 
 
  simpleSysex(0x02,pin,Set,GetPinSetting(pin,Set)); 
}

byte GetPinSetting(byte pin,byte Set)
{
  byte Value=0;
  switch(Set)
  {
    case NOTE:
      Value=Pin[pin].Note;
    break;
    case THRESOLD:
      Value=Pin[pin].Thresold;
    break;
    case SCANTIME:
      Value=Pin[pin].ScanTime;
    break;
    case MASKTIME:
      Value=Pin[pin].MaskTime;
    break;
    case RETRIGGER:
      Value=Pin[pin].Retrigger;
    break;
    case CURVE:
      Value=Pin[pin].Curve;
    break;
    case XTALK:
      Value=Pin[pin].Xtalk;
    break;
    case XTALKGROUP:
      Value=Pin[pin].XtalkGroup;
    break;
    case CURVEFORM:
      Value=Pin[pin].CurveForm;
    break;       
    case TYPE:
      Value=Pin[pin].Type;
      break;
    case CHANNEL:    
      Value=Pin[pin].Channel;    
      break; 
  } 
 
  return Value;
}

void SendHHSetting(byte Set)
{
    byte Value=0;
    if(Set==0x7F)
    {
      simpleSysex(0x02,0x4C,0x00,HHNoteSensor[0]);//HH NOTE A
      simpleSysex(0x02,0x4C,0x01,HHNoteSensor[1]);//HH NOTE B
      simpleSysex(0x02,0x4C,0x02,HHNoteSensor[2]);//HH NOTE C
      simpleSysex(0x02,0x4C,0x03,HHNoteSensor[3]);//HH NOTE D
      simpleSysex(0x02,0x4C,0x04,HHThresoldSensor[0]);//HH THRESOLD A
      simpleSysex(0x02,0x4C,0x05,HHThresoldSensor[1]);//HH THRESOLD B
      simpleSysex(0x02,0x4C,0x06,HHThresoldSensor[2]);//HH THRESOLD C
      simpleSysex(0x02,0x4C,0x07,HHThresoldSensor[3]);//HH THRESOLD D
      
      simpleSysex(0x02,0x4C,0x08,HHFootNoteSensor[0]);//HH FOOTSPLASH NOTE
      simpleSysex(0x02,0x4C,0x09,HHFootNoteSensor[1]);//HH FOOTCLOSE NOTE
      simpleSysex(0x02,0x4C,0x0A,HHFootThresoldSensor[0]);//HH FOOTSPLASH THRESOLD
      simpleSysex(0x02,0x4C,0x0B,HHFootThresoldSensor[1]);//HH FOOTCLOSE THRESOLD
      return;
    }
   else if(Set<4)Value=HHNoteSensor[Set];
   else if(Set<8)Value=HHThresoldSensor[Set-4];
   else if(Set<10) Value=HHFootNoteSensor[Set-8];
   else Value=HHFootThresoldSensor[Set-10];
  
   simpleSysex(0x02,0x4C,Set,Value);
}

void SendGeneralSetting(byte Set)
{
  byte Value=0;
   switch(Set)
  {
    case 0x00: //Delay
      Value=delayTime/2;
    break;
    
    /*case 0x01: //HHC1
      Value=GetHHC();
      break;*/
    
    case 0x02: //NSensor
      Value=NSensor;
      break;
    
    case 0x03: //GeneralXtalk
      Value=GeneralXtalk;
     break;
     
     case 0x7F://All 
       simpleSysex(0x02,0x7E,0x00,(byte)(delayTime/2));//Delay
       //simpleSysex(0x02,0x7E,0x01,(byte)GetHHC());//HHC1
       simpleSysex(0x02,0x7E,0x02,(byte)NSensor);//NSensor
       simpleSysex(0x02,0x7E,0x03,(byte)GeneralXtalk);//GeneralXtalk
       return;
     break;
  }
  simpleSysex(0x02,0x7E,Set,Value);
}
 
 // Envia sinal de fim de transmissão após enviar todos os parâmetros
void sendEndOfTransmission()
{
    Serial.write(0xF0);     // Start of SysEx
    Serial.write(0x77);     // Manufacturer ID (padrão do projeto)
    Serial.write(0x7F);     // Código especial de "fim da transmissão"
    Serial.write(0xF7);     // End of SysEx
}

void sendAllParameters(byte padIndex)
{
  Serial.print("Enviando pad: "); Serial.println(padIndex);
  sendSysEx(padIndex, 0x00, Pin[padIndex].Note);delay(1);
  sendSysEx(padIndex, 0x01, Pin[padIndex].Thresold);delay(1);
  sendSysEx(padIndex, 0x02, Pin[padIndex].ScanTime);delay(1);
  sendSysEx(padIndex, 0x03, Pin[padIndex].MaskTime);delay(1);
  sendSysEx(padIndex, 0x04, Pin[padIndex].Retrigger);delay(1);
  sendSysEx(padIndex, 0x05, Pin[padIndex].Curve);delay(1);
  sendSysEx(padIndex, 0x06, Pin[padIndex].CurveForm);delay(1);
  sendSysEx(padIndex, 0x07, Pin[padIndex].Gain);delay(1);
  sendSysEx(padIndex, 0x08, Pin[padIndex].Xtalk);delay(1);
  sendSysEx(padIndex, 0x0D, Pin[padIndex].XtalkGroup);delay(1);
  sendSysEx(padIndex, 0x0E, Pin[padIndex].Channel);delay(1);
  sendSysEx(padIndex, 0x0F, Pin[padIndex].Type);delay(1);
  Serial.print("Fim pad: "); Serial.println(padIndex);
}


void sendSysEx(byte padIndex, byte paramId, byte value)
{
  Serial.write(0xF0);       // Início da SysEx
  Serial.write(0x77);       // Manufacturer ID (fixo no seu protocolo)
  Serial.write(0x02);       // Identificador de mensagem de parâmetro
  Serial.write(padIndex);   // Índice do pad
  Serial.write(paramId);    // ID do parâmetro
  Serial.write(value);      // Valor do parâmetro
  Serial.write(0xF7);       // Fim da SysEx
  Serial.flush();
}

void ExecCommand(int Cmd,int Data1,int Data2,int Data3)
{
  
  switch(Cmd)
      {
        case 0x25: // CMD_SEND_ALL_PADS
          for (byte i = 0; i < 15; i++) {
            sendAllParameters(i); // <- ESSENCIAL!
          }
          sendEndOfTransmission(); // <- Encerra tudo com estilo
        break;

        case 0x10: // Comando especial de debug para imprimir os dados da caixa
          PrintAllPadSettings();
        break;
        case 0x00: //AskMode
          simpleSysex(0x00,Mode,0x00,0x00);
        break;
        
        case 0x01: //SetMode
          Serial.flush();
          switch(Data1)
          {
            case Off: Mode=Off; break;
            case Standby: Mode=Standby; /*CheckLicense();*/ break;
            case MIDI: Mode=MIDI; break;
            case Tool: Mode=Tool; break;
          } 
          simpleSysex(0x01,Mode,0x00,0x00);
        break;
        
        case 0x02: // AskSetting
    if(Data1 == 0x7F)
    {
      SendGeneralSetting(Data2);
      SendHHSetting(Data2);
      for(int s = 0; s < NPin; s++)
        SendPinSetting(s, Data2);

      simpleSysex(0x02, 0x7F, 0x7F, 0x7F); // Envia FIM só uma vez
    }
    else if(Data1 == 0x7E)
      SendGeneralSetting(Data2);
    else if(Data1 == 0x4C)
      SendHHSetting(Data2);
    else
    {
      SendPinSetting(Data1, Data2);
      simpleSysex(0x02, 0x7F, 0x7F, 0x7F); // ← se for só UM pad, manda fim aqui
    }
    break;
        
        case 0x03: //SetSetting: Data1: general, pad ecc... Data2:delay, note, ecc.. Data3:value
          if(Data1==0x7F) //Non utilizzato
          {

          }
          else if(Data1==0x7E) //General
          {
            switch(Data2)
            {
              //case 0x00: delayTime=Data3*2; break; //DELAY
              //case 0x01: int h=GetHHC(); if(h!=127) TypeSensor[h]=0; TypeSensor[Data3]=2; /*HHControl1=Data3;*/ break; //HHC1
              case 0x02: NSensor=Data3; break; //MaxNSensor
              case 0x03: GeneralXtalk=Data3; break; //GeneralXtalk
            }
          }
          else if(Data1==0x4C) //HH
          {
            switch(Data2)
            {
              case 0x00: HHNoteSensor[0]=Data3;break; //HH NOTE A
              case 0x01: HHNoteSensor[1]=Data3;break; //HH NOTE B
              case 0x02: HHNoteSensor[2]=Data3;break; //HH NOTE C
              case 0x03: HHNoteSensor[3]=Data3;break; //HH NOTE D
              case 0x04: HHThresoldSensor[0]=Data3;break; //HH Thresold A
              case 0x05: HHThresoldSensor[1]=Data3;break; //HH Thresold B
              case 0x06: HHThresoldSensor[2]=Data3;break; //HH Thresold C
              case 0x07: HHThresoldSensor[3]=Data3;break; //HH Thresold D
              
              case 0x08: HHFootNoteSensor[0]=Data3;break;//FootSplash Note
              case 0x09: HHFootNoteSensor[1]=Data3;break;//FootClose Note
              case 0x0A: HHFootThresoldSensor[0]=Data3;break;//FootSplash Thresold
              case 0x0B: HHFootThresoldSensor[1]=Data3;break;//FootClose Thresold              
            }
          }
          else
          {
            switch(Data2)
            {
              case NOTE: Pin[Data1].Note=Data3; break;
              case THRESOLD: Pin[Data1].Thresold=Data3; break;
              case SCANTIME: Pin[Data1].ScanTime=Data3; break;
              case MASKTIME: Pin[Data1].MaskTime=Data3; break;
              case RETRIGGER: Pin[Data1].Retrigger=Data3; break;
              case CURVE: Pin[Data1].Curve=(curve)Data3; break;
              case XTALK: Pin[Data1].Xtalk=Data3; break;
              case XTALKGROUP: Pin[Data1].XtalkGroup=Data3; break;
              case CURVEFORM: Pin[Data1].CurveForm=Data3; break;
              case CHOKENOTE: Pin[Data1].ChokeNote=Data3; break;
              //case 0x0A: DualSensor[Data1]=Data3; break; //DUAL
              case TYPE: Pin[Data1].Type=(type)Data3; break;
              #if ENABLE_CHANNEL
              case CHANNEL: Pin[Data1].Channel=Data3; break;
              #endif
                          
            }
          }
        break;
        case 0x04: //SaveSetting: Data1: general, pad ecc... Data2:delay, note, ecc.. Data3:value
          if(Data1==0x7F) //Non utilizzato
          {

          }
          else if(Data1==0x7E) //General
          {
            SaveGeneralEEPROM(Data2,Data3);
          }
          else if(Data1==0x4C)
          {
            SaveHHEEPROM(Data2,Data3); 
          }
          else
          {
            SaveEEPROM(Data1,Data2,Data3);
          }
        break;
        case 0x6D:
        #if USE_PROFILER
          if(Data1==0)
          {
            TimeProf=0;
            NProf=0;
          }
          else if (Data1==1)
          {
          SendProfiling(); 
          }
        #endif
        break;   
      
        case 0x61:
          #if defined(__AVR__)
          simpleSysex(0x61,Data1,Data2,EEPROM.read((Data1*256)+Data2));
          #endif
        break;
        
        case 0x7F: //RESET
          Serial.flush();
          Mode=Off;
          softReset();
          //simpleSysex(0x7F,0x00,0x00,0x00);
        break;
    }
}

//==============================
//    INPUT
//==============================
void Input()
{
  //===HANDSHAKE======
  while(Serial.peek()>=0 && Serial.peek()!=0xF0) Serial.read();
  //===HANDSHAKE======
  
  if (Serial.available() > 6)
  {
    byte Start=Serial.read();
    byte ID=Serial.read(); 
    int Cmd=Serial.read();
    int Data1=Serial.read();
    int Data2=Serial.read();
    int Data3=Serial.read();
    byte End=Serial.read();
    
    ExecCommand(Cmd,Data1,Data2,Data3);
  }
}
