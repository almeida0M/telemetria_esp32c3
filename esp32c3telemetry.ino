#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "rede";
const char* password = "senha";

WebServer server(80);

// filtro de media
const int NUM_LEITURAS = 5;
int leituras[NUM_LEITURAS];
int indiceLeitura = 0;

int calcularMediaRSSI() {
  leituras[indiceLeitura] = WiFi.RSSI();
  indiceLeitura = (indiceLeitura + 1) % NUM_LEITURAS;
  long soma = 0;
  for(int i = 0; i < NUM_LEITURAS; i++) soma += leituras[i];
  return (int)(soma / NUM_LEITURAS);
}

void handleData() {
  String json = "{";
  json += "\"rssi\":" + String(calcularMediaRSSI()) + ",";
  json += "\"temp\":" + String(temperatureRead(), 0) + ",";
  json += "\"heap\":" + String(ESP.getFreeHeap() / 1024) + ",";
  json += "\"uptime\":" + String(millis() / 1000); // tempo em segundos
  json += "}";
  server.send(200, "application/json", json);
}

void handleRoot() {
  String html = "<html><head><meta charset='utf-8' name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<script src='https://cdn.jsdelivr.net/npm/chart.js'></script>";
  html += "<style>";

  html += "body{background:#0f1114; color:#e9ecef; font-family:-apple-system, system-ui, sans-serif; margin:0; padding:20px;}";
  html += ".header{margin-bottom:30px; border-left: 3px solid #3b82f6; padding-left: 15px;}";
  html += "h1{font-size:1.2rem; margin:0; letter-spacing:0.5px; color:#f8f9fa;} p.status{margin:5px 0 0 0; font-size:0.8rem; color:#10b981; text-transform:uppercase; letter-spacing:1px;}";
  
  
  html += ".stats{display:grid; grid-template-columns: repeat(auto-fit, minmax(140px, 1fr)); gap:20px; margin-bottom:30px;}";
  html += ".unit{font-size:0.75rem; color:#6c757d; text-transform:uppercase; font-weight:600; margin-bottom:5px;}";
  html += ".value{font-size:1.8rem; font-weight:600; color:#ffffff;}";
  html += ".value span{font-size:0.9rem; margin-left:4px; color:#6c757d; font-weight:400;}";
  
  
  html += ".chart-box{background:#161b22; padding:20px; border-radius:8px; border: 1px solid #30363d;}";
  html += "</style></head><body>";

  html += "<div class='header'><h1>Network & System Monitor</h1><p class='status'>Active Device: ESP32-C3</p></div>";

  html += "<div class='stats'>";
  html += "  <div><div class='unit'>Temperature</div><div class='value' id='tempVal'>--<span>&deg;C</span></div></div>";
  html += "  <div><div class='unit'>Signal Strength</div><div class='value' id='rssiVal'>--<span>dBm</span></div></div>";
  html += "  <div><div class='unit'>Free Memory</div><div class='value' id='heapVal'>--<span>KB</span></div></div>";
  html += "  <div><div class='unit'>System Uptime</div><div class='value' id='uptimeVal'>--<span>s</span></div></div>";
  html += "</div>";

  html += "<div class='chart-box'><canvas id='grafico'></canvas></div>";

  html += "<script>";
  html += "var ctx = document.getElementById('grafico').getContext('2d');";
  html += "var chart = new Chart(ctx, {type:'line', data:{labels:[], datasets:[{data:[], borderColor:'#3b82f6', borderWidth:2, fill:false, tension:0.4, pointRadius:0}]},";
  html += "options:{ plugins:{legend:{display:false}}, scales:{y:{min:-100, max:-20, grid:{color:'#30363d', drawBorder:false}, ticks:{color:'#6c757d'}}, x:{display:false}} } });";

  html += "setInterval(function(){";
  html += "  fetch('/data').then(res => res.json()).then(d => {";
  html += "    document.getElementById('tempVal').childNodes[0].nodeValue = d.temp;";
  html += "    document.getElementById('rssiVal').childNodes[0].nodeValue = d.rssi;";
  html += "    document.getElementById('heapVal').childNodes[0].nodeValue = d.heap;";
  html += "    document.getElementById('uptimeVal').childNodes[0].nodeValue = d.uptime;";
  html += "    if(chart.data.labels.length > 30){ chart.data.labels.shift(); chart.data.datasets[0].data.shift(); }";
  html += "    chart.data.labels.push(''); chart.data.datasets[0].data.push(d.rssi); chart.update('none');";
  html += "  });";
  html += "}, 500);";
  html += "</script></body></html>";

  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.setSleep(false);
  WiFi.setTxPower(WIFI_POWER_11dBm); // limitador 
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  
  server.on("/", handleRoot);
  server.on("/data", handleData);
  server.begin();
}

void loop() { server.handleClient(); }