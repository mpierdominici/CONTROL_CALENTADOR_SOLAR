//Gui para gestion de calentador solar,
//a traves de un sitio web
//
//
//
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#define TEMPERATURA 1
#define POSITION 5
//credenciales de la red que se mostrara
const char* ssid = "Calentador solar";  // Nombre de la red
const char* password = "12345678";  //Contrasenia de la red
/* Put IP Address details */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);
String dataRecived="";
String tempSet="40";
String tempNow="30";
String poss="30";
ESP8266WebServer server(80);// abro el puerto 80


void setup() {
  Serial.begin(115200);

  WiFi.softAP(ssid, password);//creo la red
  WiFi.softAPConfig(local_ip, gateway, subnet);//configuro la red
  delay(100);
//se setean los calback de cada boton  
  server.on("/", handle_OnConnect);
  server.on("/st", handle_st);
  server.on("/bt", handle_bt);
  server.on("/sp", handle_sp);
  server.on("/bp", handle_bp);
  server.onNotFound(handle_NotFound);
 //***********fin callback
  server.begin();
  
}
void loop() {
  server.handleClient();
  if((dataRecived=getSerialData())!=""){//chequeo si llego la informacion por serial
    if(dataRecived.length()>2)
    {
      switch ((dataRecived.c_str())[0]){
      case 'a':
      tempSet="";
      tempSet+=String((dataRecived.c_str())[1]);
      tempSet+=String((dataRecived.c_str())[2]);
     
      
      break;
      case 'b':
      tempNow="";
      tempNow+=String((dataRecived.c_str())[1]);
      tempNow+=String((dataRecived.c_str())[2]);
     
      break;
      case 'c':
      poss="";
      poss+=String((dataRecived.c_str())[1]);
      poss+=String((dataRecived.c_str())[2]);
     
      break;
    }
    }
    
    
  }

}

void handle_OnConnect() { // se llama a esta funcion cuando se abre el sitio web
  
  Serial.println("Se conectaromn");
  server.send(200, "text/html", SendHTML()); 
}

void handle_st() { // se llama a esta funcion cuando se presiona el boton subir temperatura 

  Serial.println("a"+String(tempSet.toInt()+TEMPERATURA));
  server.send(200, "text/html", SendHTML()); 
 
}
void handle_bt() { // se llama a esta funcion cuando se presiona el boton bajar temperatura 
  Serial.println("a"+String(tempSet.toInt()-TEMPERATURA));
  server.send(200, "text/html", SendHTML()); 
}
void handle_sp() { // se llama a esta funcion cuando se presiona el boton subir persiana 
  Serial.println("c"+String(poss.toInt()+POSITION));
  server.send(200, "text/html", SendHTML()); 
}
void handle_bp() { // se llama a esta funcion cuando se presiona el boton bajar persiana 
  Serial.println("c"+String(poss.toInt()-POSITION));
  server.send(200, "text/html", SendHTML()); 
}



void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(void){
  String ptr = "<!DOCTYPE html> <html>\n";
  //ptr+="\n";
  ptr+="<head>\n";
  ptr+="<title>Termoranque Solar</title>\n";
  ptr+="<h1>Termotanque solar</h1>\n";
   ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +="</style>\n";
  ptr+="</head>\n";
  ptr+="<body>\n";
  ptr+="<h3>Temperatura actual "+tempNow+"</h3>\n";
  ptr+="<h3>Temperatura seteada "+tempSet+"</h3>\n";
  ptr+="<h3>Estado de apaertura de la persiana "+poss+"%</h3>\n";
  ptr +="<a class=\"button\" href=\"/st\">Subir Temp.</a>\n";
  ptr +="<a class=\"button\" href=\"/bt\">Bajar Temp.</a>\n";
  ptr +="<a class=\"button\" href=\"/sp\">Subir Persiana</a>\n";
  ptr +="<a class=\"button\" href=\"/bp\">Bajar Persiana</a>\n";

  ptr+="</body>\n";
  ptr+="</html>\n";

  return ptr;
}

String getSerialData()
{
  String recibedData="";
  while(Serial.available())
  {
    String temp=Serial.readString();
    recibedData=recibedData+temp;
  }
  return recibedData;
}
