#include<WiFi.h>
#define trig1 2
#define echo1 18


const char* ssid = "aakash";
const char* password = "12345678";
IPAddress ip(192, 168, 43, 116);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
WiFiServer server(80);


const int trigPin = 2;
const int echoPin = 5;
long duration;
int distance;
String s;

TaskHandle_t Task2;


void Task2code( void * parameter )
{
    for (;;)
    {
      
        // Check if a client has connected
        WiFiClient client = server.available();
        if (!client)
            continue;
        // Wait until the client sends some data
        // Serial.println("new client");
        while(!client.available())
            delay(1);
  
        if(client.available())
        {

            digitalWrite(trigPin, LOW);
           delayMicroseconds(2);
           // Sets the trigPin on HIGH state for 10 micro seconds
            digitalWrite(trigPin, HIGH);
            delayMicroseconds(10);
            digitalWrite(trigPin, LOW);
            duration = pulseIn(echoPin, HIGH);
            // Calculating the distance
            distance= duration*0.034/2;
                 
            s="Distance is:-"+(String)distance;
            Serial.println(s);
            // Return the response
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("");
            client.println("<!DOCTYPE HTML>");
            client.println("<html><body>");
            client.println(s);
            client.println("</body></html>");
            client.stop();                    //close the connection
            delay(1);
            client.flush();
        }
        delay(15);
    }
}
void setup()
{
    Serial.begin(115200);
    pinMode(trigPin, OUTPUT); 
    pinMode(echoPin, INPUT);
    delay(10);
    //Connecting to a WiFi network
    Serial.println("");
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.config(ip, gateway, subnet);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
       Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    server.begin();
    Serial.println("Server started");
    
    
      //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
      xTaskCreatePinnedToCore(
                        Task2code,   /* Task function. */
                        "Task2",     /* name of task. */
                        10000,       /* Stack size of task */
                        NULL,        /* parameter of the task */
                        1,           /* priority of the task */
                        &Task2,      /* Task handle to keep track of created task */
                        1);          /* pin task to core 1 */
    delay(500);
}

void loop()
{
    delay(20) ;
    if(WiFi.status() != WL_CONNECTED)
    {
        Serial.println("WiFi disconnected");
        Serial.print("Reconnecting to ");
        Serial.println(ssid);
        WiFi.config(ip, gateway, subnet);
        WiFi.begin(ssid, password);
    
        while (WiFi.status() != WL_CONNECTED)
        {
            delay(500);
            Serial.print(".");
        }
        Serial.println("WiFi connected");
    }  
}
