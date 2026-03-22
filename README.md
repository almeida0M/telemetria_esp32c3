# telemetria_esp32c3

Este projeto consiste em um sistema de monitoramento em tempo real para o microcontrolador esp32c3. O objetivo é fornecer diagnósticos de rede e hardware de forma independente, utilizando um servidor web nativo.

## Características Técnicas

  Processamento de Sinal: Implementação de Filtro de Média Móvel (janela de 5 pontos) para suavização do RSSI e eliminação de ruídos de alta frequência.
  
  Métricas de Hardware:Monitoramento da temperatura interna do SoC e disponibilidade de memória RAM (Heap).
  
  Interface: Dashboard priorizando baixa latência e legibilidade.
  
  Gestão de Energia: Ajuste manual de TxPower para garantir estabilidade operacional em barramentos de alimentação limitados.

## Variáveis Monitoradas

Sinal Wi-Fi (dBm): Intensidade de sinal filtrada para diagnóstico de conectividade.

Temperatura (°C): Dados do sensor interno para acompanhamento térmico do chip.

Memória Livre (KB): Supervisão de integridade do sistema e detecção de memory leaks.

Tempo de Atividade (Uptime): Contador de persistência do sistema.

## Tecnologias

C++ (Framework Arduino)

HTML5 / CSS3 / JavaScript (Fetch API)

Chart.js 4.x

Desenvolvido em ambiente Arch Linux

##Imagens

<img width="1918" height="949" alt="image" src="https://github.com/user-attachments/assets/9a89f1f2-e527-4236-b114-dcb66e6b9689" />
