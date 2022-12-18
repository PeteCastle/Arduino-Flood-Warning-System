#include "SIMManager.h"

SIMManager::SIMManager(const int SIM_RX_PIN, const int SIM_TX_PIN, const int SIM_RESET_PIN) :
    _SIM_RX_PIN(SIM_RX_PIN),
    _SIM_TX_PIN(SIM_TX_PIN),
    _SIM_RESET_PIN(SIM_RESET_PIN){
        this.SIM = SIM(_SIM_RX_PIN, SIM_TX_PIN);
        _buffer.reserve(255);
    }

String Sim800l::_readSerial(){
    _timeout=0;
    while(!SIM.available() && _timeout<12000) {
        delay(13);
        _timeout++;
    }
    if (SIM.available()) {
        return SIM.readString();
    }
}

void Sim800l::reset(){
    digitalWrite(RESET_PIN,1);
    delay(1000);
    digitalWrite(RESET_PIN,0);
    delay(1000);

    SIM.print(F("AT\r\n"));
    while (_readSerial().indexOf("OK")==-1 ){
        SIM.print(F("AT\r\n"));
    }
    
    while (_readSerial().indexOf("SMS")==-1 ){}
}

bool Sim800l::sendSms(char number, char text){
    SIM.print (F("AT+CMGF=1\r")); //set sms to text mode  
    _buffer=_readSerial();
    SIM.print (F("AT+CMGS=\""));  // command to send sms
    SIM.print (number);           
    SIM.print(F("\"\r"));       
    _buffer=_readSerial(); 
    SIM.print (text);
    SIM.print ("\r"); //change delay 100 to readserial	
    _buffer=_readSerial();
    SIM.print((char)26);
    _buffer=_readSerial();
    //expect CMGS:xxx   , where xxx is a number,for the sending sms.
    _buffer.indexOf("CMGS")  != -1 ? return True: return False;
}