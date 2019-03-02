void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:

}

void loop() {
  
  // put your main code here, to run repeatedly:

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
