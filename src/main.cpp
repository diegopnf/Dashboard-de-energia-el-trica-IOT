#include <Arduino.h>
#include <ArduinoJson.h>

// Protótipos das funções
void sendTelemetry();

void setup() {
    Serial.begin(115200);
    // Aqui no futuro entrará a config de WiFi e AWS
}

void loop() {
    sendTelemetry();
    delay(5000); // Envia a cada 5 segundos para o teste de 1h
}

void sendTelemetry() {
    StaticJsonDocument<256> doc;

    // Simulação de valores reais (220V com variação)
    float tensao = 215.0 + (rand() % 100) / 10.0; 
    float frequencia = 59.8 + (rand() % 4) / 10.0;
    float fator_potencia = 0.85 + (rand() % 15) / 100.0;
    float corrente = (rand() % 500) / 100.0; // 0 a 5 Amperes
    float potencia = tensao * corrente * fator_potencia;

    doc["tensao"] = tensao;
    doc["corrente"] = corrente;
    doc["potencia"] = potencia;
    doc["frequencia"] = frequencia;
    doc["fp"] = fator_potencia;
    doc["timestamp"] = millis(); // Provisório enquanto não temos NTP

    serializeJson(doc, Serial);
    Serial.println();
}