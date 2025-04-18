
//=================================
//   DUAL SENSOR
//=================================
#define DualSensor(i) (_DualSensor[(i)&0x07]+((i)&0xF8))
const byte _DualSensor[]    = {3,2,1,0,6,7,4,5};
//=================================

//===========CURVEFORM ORIGINAL============
// const float _Exp[9]={2.33, 3.85, 6.35, 10.48, 17.28, 28.5, 46.99 , 77.47, 127.74}; //e^((x/256)+0,85)
// const float _Log[9]={0, 83.67, 98.23, 106.74, 112.78, 117.47, 121.30 , 124.53, 127.34}; //21*ln(0,42*x)
// const float _Sigma[9]={2.28, 6.02, 15.13, 34.15, 63.5, 92.84, 111.86 , 120.97, 127.71}; //128/(1+e^((512-x)/128))
// const float _Flat[9]={0, 32.86, 46.42, 55.82, 64.0, 72.17, 81.57 , 95.13, 127}; //64-16*ln((1024/x)-1)

//===== CURVEFORM OTIMIZADO COM 32 PARAMETROS PARA INTERPOLAÇÃO MAIS SUAVE =====
const float _Exp[32] = {2.33, 2.8, 3.5, 4.3, 5.2, 6.35, 7.8, 9.5, 11.3, 13.2, 15.4, 17.9, 20.6, 23.5, 26.7, 30.2, 34.0, 38.1, 42.5, 47.2, 52.2, 57.5, 63.1, 69.0, 75.2, 81.7, 88.5, 95.6, 103.0, 110.7, 118.7, 127.74}; // e^((x/300)+1.0)
const float _Log[32] = {0, 25.1, 35.6, 42.7, 48.3, 53.0, 57.1, 60.8, 64.2, 67.3, 70.2, 72.9, 75.5, 78.0, 80.4, 82.7, 85.0, 87.2, 89.3, 91.4, 93.5, 95.5, 97.4, 99.3, 101.1, 102.9, 104.7, 106.4, 108.1, 109.7, 111.3, 127.9}; // 25*ln(0.5*x)
const float _Sigma[32] = {2.28, 3.2, 4.5, 6.0, 7.8, 10.0, 12.5, 15.3, 18.5, 22.0, 25.9, 30.2, 34.8, 39.7, 44.9, 50.4, 56.2, 62.2, 68.5, 75.0, 81.7, 88.5, 95.4, 102.4, 109.5, 116.6, 123.7, 127.71, 127.71, 127.71, 127.71, 127.71}; // 128/(1+e^((600-x)/150))
const float _Flat[32] = {0, 10.2, 15.8, 20.3, 24.1, 27.6, 30.9, 34.0, 37.0, 39.9, 42.7, 45.4, 48.1, 50.7, 53.3, 55.9, 58.5, 61.1, 63.7, 66.3, 68.9, 71.6, 74.3, 77.0, 79.8, 82.7, 85.6, 88.6, 91.7, 94.9, 98.2, 127}; // 64-16*ln((1200/x)-1)

//===========TYPE============
enum type:byte
{
  Piezo    = 0,
  Switch   = 1,
  HHC      = 2,
  HH       = 3,
  HHs      = 4,
  YSwitch  = 5,
  Disabled = 15
};
//============================

//===========CURVE============
enum curve:byte
{
  Linear  = 0,
  Exp     = 1,
  Log     = 2,
  Sigma   = 3,
  Flat    = 4
};
//============================

//===========TIME============
enum state:byte
{
  Normal_Time     = 0,
  Scan_Time       = 1,
  Mask_Time       = 2,
  Retrigger_Time  = 3,
  Piezo_Time      = 4,
  Switch_Time     = 5,
  Choke_Time      = 6,
  Footsplash_Time = 7,
  Footclose_Time  = 8,
  Scanretrigger_Time = 9
};
//============================

//===========================
//   DEFAULT PIN
//===========================

//
const byte DP_HHC         = 0x00;
const byte DP_HIHAT       = 0x01;
const byte DP_SNARE       = 0x02;
const byte DP_HACKTOM1    = 0x03;
const byte DP_HACKTOM2    = 0x04;
const byte DP_HACKTOM3    = 0x05;
const byte DP_FLOORTOM1   = 0x06;
const byte DP_FLOORTOM2   = 0x07;
const byte DP_KICK        = 0x08;
const byte DP_CRASH1      = 0x09;  
const byte DP_SPLASH      = 0x0A;
const byte DP_CRASH2      = 0x0B;
const byte DP_RIDE        = 0x0C;
const byte DP_CHINA       = 0x0D;
const byte DP_HIHAT2      = 0x0E;

//===========================
//   PIN 
//===========================
class pin
{
  public:  
  //===========PIN============
  pin()
  { //Time;
    MaxReading=-1;    
  }

  //===============================
  //   SET
  //===============================
  void set(byte pin)
  {
    this->Time = TIMEFUNCTION + this->MaskTime;    
    this->_pin=pin;  
    

// ======= PAD DEFAULTS GLOBAIS =======
this->Type = Piezo;
this->Note = 0; // vai ser sobrescrito abaixo
this->Thresold = 10;
this->ScanTime = 10;
this->MaskTime = 20;
this->Retrigger = 30;
this->Curve = 2;
this->CurveForm = 70;
this->Xtalk = 15;
this->XtalkGroup = 0; // vai ser sobrescrito abaixo
#if ENABLE_CHANNEL
  this->Channel = 9; //Sempre canal 10 (9+1)
#endif
this->Gain = 80;

// ======= PAD ESPECÍFICOS =======
switch(pin)
{
  case DP_HHC:
    this->Type = HHC;
    this->Note = 4;
    this->XtalkGroup = 1;
    break;

  case DP_HIHAT:
    this->Note = 7;
    this->XtalkGroup = 1;
    break;

  case DP_SNARE:
    this->Note = 38;
    this->XtalkGroup = 1;
    break;

  case DP_HACKTOM1:
    this->Note = 48;
    this->XtalkGroup = 2;
    break;

  case DP_HACKTOM2:
    this->Note = 47;
    this->XtalkGroup = 2;
    break;

  case DP_HACKTOM3:
    this->Note = 45;
    this->XtalkGroup = 2;
    break;

  case DP_FLOORTOM1:
    this->Note = 43;
    this->XtalkGroup = 2;
    break;

  case DP_FLOORTOM2:
    this->Note = 41;
    this->XtalkGroup = 2;
    break;

  case DP_KICK:
    this->Note = 36;
    this->XtalkGroup = 1;
    break;

  case DP_CRASH1:
    this->Note = 49;
    this->XtalkGroup = 3;
    break;

  case DP_SPLASH:
    this->Note = 55;
    this->XtalkGroup = 3;
    break;

  case DP_CRASH2:
    this->Note = 57;
    this->XtalkGroup = 3;
    break;

  case DP_RIDE:
    this->Note = 51;
    this->XtalkGroup = 3;
    break;

  case DP_CHINA:
    this->Note = 52;
    this->XtalkGroup = 3;
    break;

  case DP_HIHAT2:
    this->Note = 26;
    this->XtalkGroup = 1;
    break;
}

    
  }
  
  //===============================
  //   SCAN
  //===============================
  void scan(byte sensor,byte count)
  {
    byte pin = count+(sensor<<3);
    int yn_0 = -1;
    
    //===============================
    //        HHC
    //===============================
    
    if(Type==HHC) { scanHHC(_pin,analogRead(_pin)); return; }    
    
    //===============================
    //        Switch
    //===============================
    if(Type==Switch)
    {      
      yn_0 = analogRead(_pin);      
      //DrawDiagnostic(MulSensor,yn_0/8);
      if(State==Normal_Time) 
      {
        if(yn_0<Thresold*10 && yn_1<Thresold*10 )  
        {
          //DrawDiagnostic(pin,0);
          State=Scan_Time;
          MaxReading=0;
        }
      }
      else if(State==Scan_Time) 
      {      
        if(yn_0<Thresold*10 && yn_1<Thresold*10 ) MaxReading=MaxReading+1;
        else
        {
          if(MaxReading>ScanTime) State=Switch_Time;
          else State=Normal_Time;
        }
      
        if(MaxReading>MaskTime) State=Choke_Time;
      }
      else if(State==Mask_Time)  
      { 
        //if(ZeroCountSensor[MulSensor]>0) DrawDiagnostic(MulSensor,ZeroCountSensor[MulSensor]*16);
        if(MaxReading > 0)
        {
          MaxReading=MaxReading-1;
          //DrawDiagnostic(MulSensor,128);
        }
        else
        {
          if(yn_0>=Thresold*10 && yn_1>=Thresold*10 ) 
            State=Normal_Time;
        }
      }
    }
    //===============================
    //        YSwitch
    //===============================
    else if(Type==YSwitch)
    {
      yn_0 = analogRead(sensor);
      
      if(yn_0<Thresold*4 )
      {
        State=Scan_Time;
      
        if(MaxReading<=MaskTime) MaxReading=MaxReading+1;
      }
      else
      {
        if(MaxReading!=255 && MaxReading>ScanTime) //SwitchTime
        {
          ///DUAL
          //if(yn_0>DualThresoldSensor[MulSensor]*4)
          if(yn_0>CurveForm*4) MaxReading = MaxReading; //???
          else MaxReading = 512+MaxReading;
        }
        State=Switch_Time; 
      }
    }
    //===============================
    //        Piezo, HH
    //===============================
    else
    {
      GlobalTime=TIMEFUNCTION;
    
      if(State==Mask_Time)  
      { 
        if ((GlobalTime-Time)>MaskTime)
        {
          State = Retrigger_Time;
          Time = GlobalTime;
        }
      }      
      yn_0 = 0.5 + ((float)analogRead(sensor)*(float)Gain)/64.0;    
        
      if(State==Retrigger_Time)
      {
        int MaxRetriggerSensor=MaxReading-((GlobalTime-Time)*(Retrigger+1)/16);
        if(MaxRetriggerSensor>0)
        {
          if((yn_0 - yn_1)> Thresold && yn_0 > MaxRetriggerSensor)
          {
            State = Scan_Time;
            Time = GlobalTime;
            MaxReading = 0;
          }
        }
        else
          State = Normal_Time;
      }
      else if(State==Normal_Time) 
      {
        if((yn_0 - yn_1)> Thresold) 
        {
          State = Scan_Time;
          Time = GlobalTime;
          MaxReading = 0;
        }
      }
      else if(State==Scan_Time) 
      {
        if ((GlobalTime-Time) < ScanTime)
        {
          if(yn_0 > MaxReading)
          {
            MaxReading = yn_0;
        
            if(MaxXtalkGroup[XtalkGroup]==-1 || MaxXtalkGroup[XtalkGroup]<yn_0) //MaxGroup
              MaxXtalkGroup[XtalkGroup]=yn_0;
          
            if(MaxMultiplexerXtalk[count]==-1 || MaxMultiplexerXtalk[count]<yn_0)
              MaxMultiplexerXtalk[count]=yn_0;
          }
        }
        else
          State=Piezo_Time;
      }
    }  
    yn_1=yn_0;
  }
  
  //===============================
  //   PLAY   
  //===============================
  void play(byte i,pin* dual)
  {
    //===============================
    //        Disabled
    //===============================
    if(Type==Disabled) return;

    //===============================
    //        HHC
    //===============================
    if(Type==HHC) //HH???
    {
      if(State==Footsplash_Time)
      {
        if (Mode==MIDI) fastNoteOn(Channel,HHFootNoteSensor[0],127); // chama a nota 21
        State=Normal_Time;
      }
      else if(State==Footclose_Time)
      {
        if (Mode==MIDI) fastNoteOn(Channel,HHFootNoteSensor[1],127); // chama a nota 23
        State=Normal_Time;
      }
      return;
    }
   
    if(State==Normal_Time || State==Scan_Time || State==Mask_Time || State==Retrigger_Time) return;

    //=============XTALK==================
    
    {
      if(MaxMultiplexerXtalk[i%8]!=-1 && MaxMultiplexerXtalk[i%8]>(2*MaxReading))  //Multiplexer XTalk
      {
        State=Normal_Time;
        return;
      }
      if(MaxXtalkGroup[XtalkGroup]!=-1 && MaxXtalkGroup[XtalkGroup]>(MaxReading+(64-Xtalk)*4))
      {
        State=Normal_Time;
        return;
      }  
      playMIDI(i,dual);

    }
  }
  //===============================
  //   PLAYTOOL
  //===============================
  void playTOOL(byte i,pin* dual)
  {
    //===============================
    //        Switch
    //===============================

    if(Type==Switch)
    { 
      simpleSysex(0x6F,i,MaxReading,0);
    
      if(State==Switch_Time)
      {   
        State=Mask_Time;
        MaxReading = -1;
      }
      return;
    }
    //===============================
    //          YSwitch
    //===============================
    if(Type==YSwitch)
    {
      simpleSysex(0x6F,i,MaxReading,0);
      MaxReading = -1;
      return;
    }
    //===============================
    //        Piezo, HH
    //===============================
    if (/*(Time-TimeSensor[i]) >= ScanTimeSensor[i]*/ State==Piezo_Time)
    {          
      //Piezo
      if(Type==Piezo)
      {
        simpleSysex(0x6F,i,useCurve(),0);
        
        State=Mask_Time;
              
        //Piezo-Switch
        if(dual->Type==Switch && dual->State==Switch_Time )
        {
              simpleSysex(0x6F,DualSensor(i),127,0);

              dual->State=Mask_Time;
              dual->MaxReading = -1;
         }
      }
      else //HH========================================
        simpleSysex(0x6F,i,useCurve(),0);
               
      MaxReading = -1;
    }
  }

  //===============================
  //   PLAYMIDI
  //===============================
  void playMIDI(byte i,pin* dual)
  {
    //===============================
    //        Single Switch
    //===============================
    if(Type==Switch)
    { 
      
      if(State==Switch_Time)
      {
        fastNoteOn(Channel,Note,127);
          
        State=Mask_Time;
        MaxReading = Retrigger;
      }
      else if(State==Choke_Time) //Choke
      {
        fastNoteOn(Channel,ChokeNote,127);
            
        State = Mask_Time;
        MaxReading = Retrigger;
      }
      return;
    }
    //===============================
  
    GlobalTime=TIMEFUNCTION;
  
    //===============================
    //          YSwitch
    //===============================
    if(Type==YSwitch)
    {
      if(MaxReading <= 512)
      {
        fastNoteOn(Channel,Note,min(127,MaxReading*8));
      }
      else
        fastNoteOn(Channel,DualSensor(i),min(127,(MaxReading-512)*8)); //Dual??? ChokeNote
          
      if(DualSensor(i)!=127)//Dual???
      {
        dual->MaxReading=-1;
        dual->Time=GlobalTime - dual->ScanTime;
      }
    
      MaxReading = -1;
      return;
    }
    //===============================
  
    //====================================================================
    if (State==Piezo_Time)
    {          
      //Piezo
      if(Type==Piezo)
      {
        byte v=useCurve();        
                 
        fastNoteOn(Channel,Note,v);
        
        State=Mask_Time;
              
        //Piezo-Switch
        if(dual->Type==Switch && dual->State==Switch_Time )
        {
          fastNoteOn(dual->Channel,dual->Note,127); 
          dual->State=Mask_Time;
         }
         
      }
      else //HH========================================
      {
      	//??? dual != hhc
        byte note=Note;
        //if(ZeroCountSensor[HHC]>DualThresoldSensor[i])//DUAL
        if(dual->MaxReading>dual->Thresold)
          note=ChokeNote;
        else if(dual->MaxReading>HHThresoldSensor[3])
          note=HHNoteSensor[3];
        else if(dual->MaxReading>HHThresoldSensor[2])
          note=HHNoteSensor[2];
        else if(dual->MaxReading>HHThresoldSensor[1])
          note=HHNoteSensor[1];
        else if(dual->MaxReading>HHThresoldSensor[0])
          note=HHNoteSensor[0];

        fastNoteOn(Channel,note,useCurve());
      }//HH=======================
    }
  }
  //=======================
  byte Type:4; //type
  byte State:4; //state
  byte _pin;
    
  byte Note;
  union
  {
    byte ChokeNote;
    byte Gain;
  };
  
  #if ENABLE_CHANNEL
  byte Channel;
  #endif
  
  byte Thresold;
  byte ScanTime;
  byte MaskTime;
  byte Retrigger;
  
  byte CurveForm;
  byte Curve:4;
  byte XtalkGroup:4;
  byte Xtalk;

  unsigned long Time;
  int MaxReading;
  int yn_1;

  //===============================
  //   USECURVE
  //===============================
  byte useCurve()
  {
    int ret=0;
    //float Xn=(float)Value;
    float f=((float)CurveForm)/32.0;//[1;127]->[0.;4.0]
    
    if(Curve==Linear)
    {
      ret=0.5 + ((float)MaxReading*f/8.0);
    }
    else
    {
      int i=MaxReading/128;
      int m=MaxReading % 128;
    
      switch(Curve)
      {
        case Exp: ret = 0.5 + (((float)m*(_Exp[i+1]-_Exp[i])/128.0) + _Exp[i])*f; break;
        case Log: ret = 0.5 + (((float)m*(_Log[i+1]-_Log[i])/128.0) + _Log[i])*f; break;
        case Sigma: ret = 0.5 + (((float)m*(_Sigma[i+1]-_Sigma[i])/128.0) + _Sigma[i])*f; break;
        case Flat: ret = 0.5 + (((float)m*(_Flat[i+1]-_Flat[i])/128.0) + _Flat[i])*f; break;
      
        default: ret = i*16; break;//Test mode
      }
    }
  
    if(ret<=0) return 0;
    if(ret>=127) return 127;//127
    return ret;
  }

  //===============================
  //   SCANHHC
  //===============================
  void scanHHC(byte pin,byte sensorReading)
  {
    if ((GlobalTime-Time) > MaskTime)
    {
      if(sensorReading>(/*yn_1*/MaxReading+Thresold) || sensorReading<(/*yn_1*/MaxReading-Thresold))
      {
        if (Mode==MIDI)
        {        
          fastMidiCC(Channel,Note,sensorReading);
        }        
        
        float m=(((float)MaxReading-(float)sensorReading)/((float)Time-(float)GlobalTime))*100;

        MaxReading=sensorReading;//LastReading
        
        //Foot Splash
        if(m>0 && m>HHFootThresoldSensor[0])
          State=Footsplash_Time;

        //FootClose
        else if(m<0 && -m>HHFootThresoldSensor[1])
          State=Footclose_Time;

        Time=GlobalTime;
      }
    }
  }
} Pin[NPin];
//===========================