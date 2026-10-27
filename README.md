# NotifyReachedOffice
The repo has about creating backend service which will integrate with IFTT and Arduino based IOT ecosystem

```mermaid
flowchart LR
    A[IFTTT Mobile App] --> B[IFTTT Webhook]
    B --> C[Backend AWS lambda Service]
    C --> D[AWS IOT Hub]
    D --> E[ESP8266]
