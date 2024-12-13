#include <WiFi.h>
#include <HTTPClient.h>

#define LED_YELLOW 9  // Pino para LED amarelo
#define LED_GREEN 2   // Pino para LED verde
#define LED_RED 40    // Pino para LED vermelho
#define BUTTON_PIN 18 // Pino do botão
#define LDR_PIN 4     // Pino do sensor LDR

const int threshold = 600; // Limite para identificar claro ou escuro
int buttonState = 0;
int lastButtonState = 0;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50; // Delay para debounce
int buttonPressCount = 0;


const char* ssid = "Wokwi-GUEST";        // Rede Wi-Fi
const char* password = "";               // Senha do Wi-Fi

void setup() {
  Serial.begin(9600);

  // Conectar ao Wi-Fi
  connectWiFi();

  // Configuração dos pinos
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);

  // Inicializar LEDs como desligados
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, LOW);

}

void connectWiFi() {
  Serial.println("Conectando ao WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado!");
}

void loop() {
  int ldrValue = analogRead(LDR_PIN);
  
  if (ldrValue <= threshold) {
    // Modo noturno
    modeNight();
  } else {
    // Modo convencional
    modeDay();
  }
}

void modeNight() {
  static unsigned long previousMillis = 0;
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= 1000) {
    previousMillis = currentMillis;
    digitalWrite(LED_YELLOW, !digitalRead(LED_YELLOW)); // Pisca o LED amarelo
  }
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, LOW);
}

void modeDay() {
  static unsigned long lastChange = 0;
  static int state = 0; // 0 = verde, 1 = amarelo, 2 = vermelho
  unsigned long now = millis();

  if (state == 0 && now - lastChange >= 3000) {
    state = 1;
    lastChange = now;
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_YELLOW, HIGH);
  } else if (state == 1 && now - lastChange >= 2000) {
    state = 2;
    lastChange = now;
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_RED, HIGH);
  } else if (state == 2 && now - lastChange >= 5000) {
    state = 0;
    lastChange = now;
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, HIGH);
  }

  handleButtonPress(state);
}

void handleButtonPress(int state) {
  int reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == HIGH && state == 2) { // Estado vermelho
        buttonPressCount++;
        Serial.print("Button pressed ");
        Serial.print(buttonPressCount);
        Serial.println(" times");

        if (buttonPressCount == 1) {
          delay(1000); // Aguarda 1 segundo
          digitalWrite(LED_RED, LOW);
          digitalWrite(LED_GREEN, HIGH);
        } else if (buttonPressCount == 3) {
          sendAlert();
          buttonPressCount = 0; // Reseta o contador
        }
      }
    }
  }

  lastButtonState = reading;
}

void sendAlert() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin("http://www.google.com.br/");
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}
