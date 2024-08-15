#include <WiFi.h>

const char* ssid = "CAMADA-FÍSICA-EXEMPLO";
const char* password = "12345678";

WiFiServer server(80);

void setup() {
  Serial.begin(9600);

  WiFi.softAP(ssid, password);
  Serial.println();
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());  

  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("Novo cliente conectado");
    String currentLine = "";
    String receivedText = "";

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);

        if (c == '\n') {
          if (currentLine.length() == 0) {
            if (receivedText.length() > 0) {
              Serial.println("Mensagem recebida: " + receivedText);
            }

            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            client.println("<!DOCTYPE HTML>");
            client.println("<html>");
            client.println("<h1>Ola, Cliente!</h1>");
            client.println("<p>Envie uma mensagem pela URL para que ele seja impresso no terminal.</p>");
            client.println("<a>192.168.4.1/?name=suamensagem</a>");
            client.println("</html>");

            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    client.stop();
    Serial.println("Cliente desconectado");
  }
}