//Two-Way communication ESP32 to Control LED
//Load libraries
#include <esp_now.h>
#include <WiFi.h>


//Defines PIN Button and PIN LED.
#define LED_Pin1   1
#define LED_Pin2   2
#define LED_Pin3   3
#define LED_Pin4   4

#define BTN_Pin1   5
#define BTN_Pin2   6
#define BTN_Pin3   7
#define BTN_Pin4   8

int BTN_State1;
int BTN_State2;
int BTN_State3;
int BTN_State4;

uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; //--> REPLACE WITH THE MAC Address of your receiver.

int LED_State_Send1 = 0; //--> Variable to hold the data to be transmitted to control the LEDs on the paired ESP32.
int LED_State_Send2 = 0;
int LED_State_Send3 = 0;
int LED_State_Send4 = 0;

int LED_State_Receive1; //--> Variable to receive data to control the LEDs on the ESP32 running this code.
int LED_State_Receive2;
int LED_State_Receive3;
int LED_State_Receive4;

String success; //--> Variable to store if sending data was successful

//Structure example to send data
//Must match the receiver structure
typedef struct struct_message {
  //change if needed
    int led1;
    int led2;
    int led3;
    int led4;
} struct_message_send;

struct_message send_Data; // Create a struct_message to send data.

struct_message receive_Data; // Create a struct_message to receive data.

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if (status ==0){
    success = "Delivery Success :)";
  }
  else{
    success = "Delivery Fail :(";
  }
  Serial.println(">>>>>");
}

//Callback when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&receive_Data, incomingData, sizeof(receive_Data));
  Serial.println();
  Serial.println("<<<<< Receive Data:");
  Serial.print("Bytes received: ");
  Serial.println(len);

  //change if needed
  LED_State_Receive1 = receive_Data.led1;
  LED_State_Receive2 = receive_Data.led2;
  LED_State_Receive3 = receive_Data.led3;
  LED_State_Receive4 = receive_Data.led4;

  Serial.print("Receive Data: ");
  Serial.println(LED_State_Receive1);
  Serial.println("<<<<<");
  Serial.println(LED_State_Receive2);
  Serial.println("<<<<<");
  Serial.println(LED_State_Receive3);
  Serial.println("<<<<<");
  Serial.println(LED_State_Receive4);
  Serial.println("<<<<<");

  digitalWrite(LED_Pin1, LED_State_Receive1);
  digitalWrite(LED_Pin2, LED_State_Receive2);
  digitalWrite(LED_Pin3, LED_State_Receive3);
  digitalWrite(LED_Pin4, LED_State_Receive4);
}
//VOID SETUP
void setup() {
  Serial.begin(115200);

  pinMode(LED_Pin1, OUTPUT);
  pinMode(LED_Pin2, OUTPUT);
  pinMode(LED_Pin3, OUTPUT);
  pinMode(LED_Pin4, OUTPUT);

  pinMode(BTN_Pin1, INPUT);
  pinMode(BTN_Pin2, INPUT);
  pinMode(BTN_Pin3, INPUT);
  pinMode(BTN_Pin4, INPUT);
  
  WiFi.mode(WIFI_STA); //Set device as a Wi-Fi Station

  //Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }  
  //Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);

  //Register peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
 
  //Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  
  esp_now_register_recv_cb(OnDataRecv); // Register for a callback function that will be called when data is received
}

void loop() {
  //change if needed
  BTN_State1 = digitalRead(BTN_Pin1); // Reads and holds button states.
  BTN_State2 = digitalRead(BTN_Pin2);
  BTN_State3 = digitalRead(BTN_Pin3);
  BTN_State4 = digitalRead(BTN_Pin4);

  //the else if bs
  //When the button is pressed it will send data to control the LED on the ESP32 Target.
  if(BTN_State1 == 1) {
    LED_State_Send1 = !LED_State_Send1;
    send_Data.led1 = LED_State_Send1;

    Serial.println();
    Serial.print(">>>>> ");
    Serial.println("Send data");
  
    //Send message via ESP-NOW
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &send_Data, sizeof(send_Data));
     
    if (result == ESP_OK) {
      Serial.println("Sent with success");
    }
    else {
      Serial.println("Error sending the data");
    }
    // Wait for the button to be released. Release the button first to send the next data.
    while(BTN_State1 == 1) {
      BTN_State1 = digitalRead(BTN_Pin1);
      delay(10);
    }
  }
  if(BTN_State2 == 1) {
    LED_State_Send2 = !LED_State_Send2;
    send_Data.led2 = LED_State_Send2;

    Serial.println();
    Serial.print(">>>>> ");
    Serial.println("Send data");
  
    //Send message via ESP-NOW
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &send_Data, sizeof(send_Data));
     
    if (result == ESP_OK) {
      Serial.println("Sent with success");
    }
    else {
      Serial.println("Error sending the data");
    }
    // Wait for the button to be released. Release the button first to send the next data.
    while(BTN_State2 == 1) {
      BTN_State2 = digitalRead(BTN_Pin2);
      delay(10);
    }
  }
  if(BTN_State3 == 1) {
    LED_State_Send3 = !LED_State_Send3;
    send_Data.led = LED_State_Send3;

    Serial.println();
    Serial.print(">>>>> ");
    Serial.println("Send data");
  
    //Send message via ESP-NOW
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &send_Data, sizeof(send_Data));
     
    if (result == ESP_OK) {
      Serial.println("Sent with success");
    }
    else {
      Serial.println("Error sending the data");
    }
    // Wait for the button to be released. Release the button first to send the next data.
    while(BTN_State3 == 1) {
      BTN_State3 = digitalRead(BTN_Pin3);
      delay(10);
    }
  }
  if(BTN_State4 == 1) {
    LED_State_Send4 = !LED_State_Send4;
    send_Data.led = LED_State_Send4;

    Serial.println();
    Serial.print(">>>>> ");
    Serial.println("Send data");
  
    //Send message via ESP-NOW
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &send_Data, sizeof(send_Data));
     
    if (result == ESP_OK) {
      Serial.println("Sent with success");
    }
    else {
      Serial.println("Error sending the data");
    }
    // Wait for the button to be released. Release the button first to send the next data.
    while(BTN_State4 == 1) {
      BTN_State4 = digitalRead(BTN_Pin4);
      delay(10);
    }
  }
}