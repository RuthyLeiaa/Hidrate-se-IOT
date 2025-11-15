#Hidrate-se IoT – Lembrete Inteligente de Hidratação (Simulado no Wokwi)

📌 1. Descrição do Projeto

O Hidrate-se IoT é um protótipo desenvolvido para lembrar o usuário de beber água ao longo do dia. Ele utiliza um ESP32, um botão, um LED (atuador) e o sensor DHT22, que ajusta automaticamente o tempo entre os lembretes conforme a temperatura ambiente.
O projeto foi totalmente desenvolvido e testado no ambiente virtual Wokwi, com comunicação em nuvem via MQTT, utilizando o HiveMQ Cloud.

Sempre que o usuário aperta o botão, o sistema registra que ele bebeu água. Se passar tempo demais sem consumo, o LED acende como alerta. As informações são enviadas para o broker MQTT, permitindo acompanhamento em tempo real.

📁 2. Arquivos incluídos no repositório

✔️ Código completo (hidratese.ino)
✔️ Imagens da montagem no Wokwi
✔️ Documentação dos componentes utilizados

⚙️ 3. Hardware utilizado

Como o projeto foi executado de forma virtual, os componentes listados correspondem à versão simulada:

ESP32 DevKit v1 (microcontrolador)

Responsável pelo processamento, leitura de sensores, acionamento do LED e envio dos dados via Wi-Fi/MQTT.

DHT22 (sensor de temperatura)

Mede temperatura ambiente.

O tempo de lembrete muda conforme a temperatura (mais quente → lembrete mais rápido).

Botão (entrada de usuário)

Simula o registro da ingestão de água.

LED + resistor (atuador visual)

Acende quando o usuário fica muito tempo sem beber água.

Protótipo desenvolvido no Wokwi

Circuito virtual sem protoboard física.

🌐 4. Comunicação, Interfaces e Protocolos

A comunicação entre o ESP32 e a internet ocorre via TCP/IP, por meio do Wi-Fi do simulador.

MQTT:

O sistema usa o protocolo MQTT, ideal para IoT pela leveza e baixa latência.

HiveMQ Cloud:

Broker online gratuito

Suporte a TLS

Visualização das mensagens em tempo real através do console

💻 5. Como reproduzir o projeto
Passo 1 – Acessar o simulador

👉 https://wokwi.com

Passo 2 – Criar um projeto ESP32

Selecione: ESP32 DevKit V1

Passo 3 – Montar o circuito

Ligações recomendadas:

Componente	Pino
Botão	GPIO 5
LED	GPIO 2 (com resistor)
DHT22	GPIO 4

Alimente com 3.3V e GND.

Passo 4 – Copiar o código para o Wokwi

O código completo estará no arquivo sketch.ino do repositório.

Passo 5 – Criar a conta no HiveMQ Cloud

https://console.hivemq.cloud/

Criar cluster

Criar usuário MQTT

Anotar host e porta

Passo 6 – Testar no cliente websocket

https://www.hivemq.com/demos/websocket-client/

Verifique se:

O ESP32 conecta

As mensagens chegam nos tópicos

Passo 7 – Rodar o simulador

O LED irá acender automaticamente quando o tempo limite for atingido.

🧪 6. Funcionamento resumido

O botão registra a ingestão de água

O ESP32 reinicia o tempo entre lembretes

O DHT22 envia temperatura e ajusta o intervalo

O LED acende quando o limite é ultrapassado

Todos os dados são enviados via MQTT para o HiveMQ Cloud
