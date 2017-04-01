//#include <SoftwareSerial.h>

#define logListSize 2
#define commandListSize 2

char incomingChar;
char commandChar = '=';
char echoOffChar = ';';

String startCommandLayout = "ol+";
String serialBuffer="";
String commandList[] = {"dooropen", "do"}; 
String logList[] = {"doorstatus", "ds"}; 
int commandIndexStartValue = 1000;
int logIndexStartValue = 2000;

//functions prototypes
bool isMessageEchoOff(String);
bool parsMessage(String, bool, int&, int&);

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}

void loop() { // run over and over
  bool isEchoOff;
  int messageIndex;
  int commandValue;
  
  if (Serial.available()) {
        incomingChar = Serial.read();
        serialBuffer.concat(incomingChar);
        if(incomingChar == 10){
          serialBuffer.toLowerCase();
          isEchoOff = isMessageEchoOff(serialBuffer);
          if(parsMessage(serialBuffer,isEchoOff,messageIndex,commandValue)){//with error
            //TODO
          }else{
            //TODO
            Serial.print("messageIndex");
            Serial.println(messageIndex);
            Serial.print("commandValue");
            Serial.println(commandValue);
            if(isEchoOff){
              Serial.println("silent command");
            }
          }
          serialBuffer="";
        }
  }
}

bool isMessageEchoOff(String message){
  bool isEchoOff=false;
  if(message.length()>2){
    isEchoOff = message.charAt(message.length()-2) == echoOffChar;  
  }
  return isEchoOff;
}

bool parsMessage(String message ,bool messageEchoOff ,int& messageIndex ,int& commandValue){
  bool isAnyError = false;
  int commandCharIndex = -1;
  int endOfMessageIndex = -1;
  String startMessage;
  String commandValueString;
  String commandString;
  String logString;
  int i;
  int sizeOfLogList;
  
  endOfMessageIndex = message.length()-1-messageEchoOff;
  commandCharIndex=message.indexOf('=');
  
  //init output variables
  commandValue=-1;
  messageIndex=-1;
  if(message.length()>3){
    startMessage = message.substring(0,3);
    if(startMessage.equals(startCommandLayout)){
      if(commandCharIndex==-1){//log message
        logString = message.substring(3,message.length()-1);
        for(i=0;i<logListSize;i++){
          if(logList[i].equals(logString)){
            messageIndex = logIndexStartValue + i;
            break;
          }
        }
  
      }else{//command message
        commandString = message.substring(3,commandCharIndex);
        for(i=0;i<commandListSize;i++){
          if(commandList[i].equals(commandString)){
            messageIndex = commandIndexStartValue + i;
            break;
          }
        }
        commandValueString = message.substring(commandCharIndex+1,endOfMessageIndex);
        commandValue = commandValueString.toInt();
      }
    }
  }
  return isAnyError;
}

