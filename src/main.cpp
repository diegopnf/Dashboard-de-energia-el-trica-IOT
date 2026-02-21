#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>
#include <WiFiManager.h>

// --- CONFIGURAÇÕES DE IDENTIDADE ---
// DEVE ser idêntico ao 'device_id' no variables.tf do Terraform
const char* DEVICE_ID = "medidor-esp32c6-01"; 
const char* FW_VERSION = "1.0.2"; 

// Protótipos das funções
void sendTelemetry();

void setup() {
    Serial.begin(115200);
    delay(2000);
    Serial.println("--- Monitor de Energia IoT Iniciado ---");
    WiFiManager wm;
    bool res = wm.autoConnect("ESP32 Medidor de energia");

    if(!res) {
        Serial.println("Falha na conexão. Reiniciando...");
        ESP.restart();
    } else {
        Serial.println("Conectado com sucesso à rede WiFi!");
    }
    Serial.printf("ID: %s | Versao: %s\n", DEVICE_ID, FW_VERSION);
}

void loop() {
    sendTelemetry();
    delay(5000); // Frequência de envio para o seu teste de 1h
}

void sendTelemetry() {
    // Buffer ajustado para incluir o ID do dispositivo e versão
    StaticJsonDocument<384> doc;

    // Simulação de valores elétricos realistas
    float voltage = 215.0 + (rand() % 100) / 10.0; 
    float current = (rand() % 500) / 100.0; 
    float pf = 0.85 + (rand() % 15) / 100.0;
    float frequency = 59.8 + (rand() % 4) / 10.0;
    float power = voltage * current * pf;

    // --- MAPEAMENTO PARA DYNAMODB E DASHBOARD ---
    // Chave de Partição obrigatória conforme Terraform
    doc["device_id"] = DEVICE_ID; 
    
    // Chave de Classificação (Sort Key)
    doc["timestamp"] = millis(); 

    // Dados padronizados para os gráficos do site
    doc["voltage"] = voltage;
    doc["current"] = current;
    doc["power"] = power;
    doc["frequency"] = frequency;
    doc["pf"] = pf;
    
    // Campo para a sua tela de inventário/dispositivos
    doc["fw_version"] = FW_VERSION;

    serializeJson(doc, Serial);
    Serial.println();
}