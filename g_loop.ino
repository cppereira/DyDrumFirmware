//==============================
//    FASTSCAN
//==============================
  #define fastScan(sensor,count) { byte pin=count+(sensor<<3);if(Pin[pin].Type!=Disabled) { Pin[pin].scan(sensor,count); if(Pin[pin].State==Scan_Time) { Pin[pin].scan(sensor,count); Pin[pin].scan(sensor,count); Pin[pin].scan(sensor,count);}}


//==============================
//     LOOP
//==============================
void loop()
 {
  //TESTE PARA DEBUGAR O EXECCOMMAND
  // if (Serial.available()) {
  //   char ch = Serial.peek(); // Espia o próximo byte sem remover

  //   // Comando especial: se digitar % no Serial Monitor
  //   if (ch == '%') {
  //     Serial.read(); // Consumir o %
  //     Serial.println("Comando % recebido: Executando CMD_SEND_ALL_PADS");
  //     ExecCommand(0x25, 0, 0, 0);
  //   }
  //   // Protocolo normal: 4 bytes
  //   else if (Serial.available() >= 4) {
  //     byte CMD = Serial.read();
  //     byte A = Serial.read();
  //     byte B = Serial.read();
  //     byte C = Serial.read();

  //     ExecCommand(CMD, A, B, C);
  //   }
  // }

        //simpleSysex(0xFF,Mode,0x00,0x00);
        Input(); 
        if(Mode==Off)
        {
          delay(100);
          return;
        }

      //---------------------------------------------------------------------------------------
      //      FAZ A LEITURA DOS PINOS DIGITAIS DO ARDUÍNO
      //---------------------------------------------------------------------------------------
        
        // CHOKE CRASH 1
        currentSwitchState = digitalRead(Choke1_Pin);
        if( currentSwitchState == LOW && Choke1_State == HIGH ) // push
          MIDI_TX(0x90 | 144, choke1, 127);
        if( currentSwitchState == HIGH && Choke1_State == LOW ) // release
          //TODO: MIDI_TX(0x80 | 128, choke1, 127);
          // Fazer o teste de NOTEOFF com valor 80, ou enviar para a função a_midi.ino
          MIDI_TX(0x90 | 128, choke1, 127);
          Choke1_State = currentSwitchState;

        // CHOKE CRASH 2
        currentSwitchState = digitalRead(Choke2_Pin);
        if( currentSwitchState == LOW && Choke2_State == HIGH ) // push
          MIDI_TX(0x90 | 144, choke2, 127);
        if( currentSwitchState == HIGH && Choke2_State == LOW ) // release
          //TODO: MIDI_TX(0x80 | 128, choke1, 127);
          // Fazer o teste de NOTEOFF com valor 80, ou enviar para a função a_midi.ino
          MIDI_TX(0x90 | 128, choke2, 127);
          Choke2_State = currentSwitchState;

          // CHOKE SPLASH
        currentSwitchState = digitalRead(Choke3_Pin);
        if( currentSwitchState == LOW && Choke3_State == HIGH ) // push
          MIDI_TX(0x90 | 144, choke3, 127);
        if( currentSwitchState == HIGH && Choke3_State == LOW ) // release
          //TODO: MIDI_TX(0x80 | 128, choke1, 127);
          // Fazer o teste de NOTEOFF com valor 80, ou enviar para a função a_midi.ino
          MIDI_TX(0x90 | 128, choke3, 127);
          Choke3_State = currentSwitchState;

           // CHOKE CHINA
        currentSwitchState = digitalRead(Choke4_Pin);
        if( currentSwitchState == LOW && Choke4_State == HIGH ) // push
          MIDI_TX(0x90 | 144, choke4, 127);
        if( currentSwitchState == HIGH && Choke4_State == LOW ) // release
          //TODO: MIDI_TX(0x80 | 128, choke1, 127);
          // Fazer o teste de NOTEOFF com valor 80, ou enviar para a função a_midi.ino
          MIDI_TX(0x90 | 128, choke4, 127);
          Choke4_State = currentSwitchState;

          // CHOKE RIDE
        currentSwitchState = digitalRead(ChokeRide_Pin);
        if( currentSwitchState == LOW && ChokeRide_State == HIGH ) // push
          MIDI_TX(0x90 | 144, chokeRide, 127);
        if( currentSwitchState == HIGH && ChokeRide_State == LOW ) // release
          //TODO: MIDI_TX(0x80 | 128, choke1, 127);
          // Fazer o teste de NOTEOFF com valor 80, ou enviar para a função a_midi.ino
          MIDI_TX(0x90 | 128, chokeRide, 127);
          ChokeRide_State = currentSwitchState;

      //Lê os pinos analógicos
      // A leitura de todos os PADs é feita aqui...

        for(byte Sensor=0;Sensor<NPin;Sensor++)
        {
        if (Pin[Sensor].Type != Disabled)
        {
          Pin[Sensor].scan(Sensor,0);
          if (Pin[Sensor].State == Scan_Time)
          {
            Pin[Sensor].scan(Sensor,0);
            Pin[Sensor].scan(Sensor,0); 
            Pin[Sensor].scan(Sensor,0);
            }
        }
        Pin[Sensor].play(Sensor,&Pin[DualSensor(Sensor)]);
        }

        //RESET XTALK
        for(int i=0;i<8;i++)
          MaxMultiplexerXtalk[i]=-1;
        for(int i=0;i<NXtalkGroup;i++)
          MaxXtalkGroup[i]=-1;
          //RESET XTALK
        //for(int i=0;i<8;i++)
        // MaxMultiplexerXtalk[i]=MaxXtalkGroup[i]=-1;
}