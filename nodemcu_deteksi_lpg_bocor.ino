#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

ESP8266WiFiMulti WiFiMulti;
void setup()
{
    Serial.begin(9600);
    
    // set Wifi SSID dan passwordnya
    WiFiMulti.addAP("NAMA WIFI", "PASSWORD WIFI");
}

void loop()
{ 
    float sensor_volt;
    float RS_gas; // Get value of RS in a GAS
    float ratio; // Get ratio RS_GAS/RS_air
    int sensorValue = analogRead(A0);
    sensor_volt=(float)sensorValue/1024*5.0;
    RS_gas = (5.0-sensor_volt)/sensor_volt; // omit *RL
    ratio = RS_gas/0.9;
  
    String sensorGas  = String(ratio);
       
    // tunggu koneksi Wifi
    if((WiFiMulti.run() == WL_CONNECTED))
    {
        HTTPClient http;

        String url_api  = "MASUKKAN LINK SENDING DATA TO CHANEL DARI AGNOSTHINGS LALU GANTI {value} dengan {lpgval}";
        url_api.replace("{lpgval}", sensorGas);
        
        char url_api_char[100];
        url_api.toCharArray(url_api_char, 100);

        // ganti dengan URL API Last Feed punyamu sendiri
        http.begin(url_api_char);

        // mulai koneksi dan ambil HTTP Header
        int httpCode = http.GET();

        // httpCode akan bernilai negatif bila error
        if(httpCode > 0)
        {
            // cetak httpCode ke Serial
            Serial.printf("[HTTP] GET... code: %d\n", httpCode);

            // bila nilai dari server diterima
            if(httpCode == HTTP_CODE_OK)
            {
                // cetak string json dari server
                String json = http.getString();
                Serial.println(json);
            }

        } else {
            Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
        // tutup koneksi HTTP
        http.end();
    }

    delay(5000);
}
