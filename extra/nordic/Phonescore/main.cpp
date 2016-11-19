#include "ble/BLE.h"
#include "ble/GattCharacteristic.h"
#include "score_service.h"
#include "mbed.h"

DigitalOut TestLED(LED3, 0);
DigitalOut TestLED2(LED2, 0);
DigitalOut StartTransmission(P0_13, 0);
SPISlave slave(P0_25, P0_28, P0_29, P0_24);

bool score_received = false;
uint8_t* name_in_bytes;
int len;

// BLE settings
const static char     DEVICE_NAME[] = "Baugen";
static const uint16_t uuid16_list[] = {ScoreService::SCORE_SERVICE_UUID};
static ScoreService *ScoreServicePtr;

void InitializeSlave(){
    slave.format(8, 1);
}

void SendDataSPI(uint8_t data){
    slave.reply(data);
    StartTransmission.write(1);
    wait_ms(100);
    StartTransmission.write(0);
    wait_ms(100);
}

void NameWrittenCallback(const GattWriteCallbackParams *params) {
    
    // check to see what characteristic was written, by handle
    if(params->handle == ScoreServicePtr->name_array.getValueHandle()) {
        
        if(params->len == 1) {
            uint8_t* temp = const_cast<uint8_t*>(params->data);
            ScoreServicePtr->UpdateName(temp);
            ScoreServicePtr->UpdateScore(params->data[0]);
        } else {
            uint8_t* temp = const_cast<uint8_t*>(params->data);
            len = params->data[10]; // Set length here eventually
            memcpy(name_in_bytes, temp, len);
            ScoreServicePtr->UpdateName(temp);
            ScoreServicePtr->UpdateScore(params->data[10]);
        }
    }
    
    score_received = true;
}

void CallbackButton(){
    
    if(len == 3){
        TestLED = !TestLED;
    }
}

void CallbackBLE(BLE::InitializationCompleteCallbackContext *params){
    
    BLE& Bluetooth   = params->ble;
    Bluetooth.onDataWritten(NameWrittenCallback);
    uint8_t arr[11];
    
    ScoreServicePtr = new ScoreService(Bluetooth, 0, arr);
    
    /* setup advertising */
    Bluetooth.gap().accumulateAdvertisingPayload(GapAdvertisingData::BREDR_NOT_SUPPORTED | GapAdvertisingData::LE_GENERAL_DISCOVERABLE);
    Bluetooth.gap().accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_16BIT_SERVICE_IDS, (uint8_t *)uuid16_list, sizeof(uuid16_list));
    Bluetooth.gap().accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LOCAL_NAME, (uint8_t *)DEVICE_NAME, sizeof(DEVICE_NAME));
    Bluetooth.gap().setAdvertisingType(GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED);
    Bluetooth.gap().setAdvertisingInterval(100); /* 1000ms. */
    Bluetooth.gap().startAdvertising();
}

int main(void)
{
    // Initialize the value on the heap
    name_in_bytes = (uint8_t*) malloc(10);
    InitializeSlave();
    InterruptIn Button(BUTTON1);
    Button.rise(CallbackButton);
    
    // Singleton of BLE and setup BLE
    BLE &Bluetooth = BLE::Instance();
    Bluetooth.init(CallbackBLE);
    while (Bluetooth.hasInitialized()  == false) { /* spin loop */ }
    
    while (true) {
        Bluetooth.waitForEvent();
        if(score_received){
            for(int i = 0; i < len; i++){
                SendDataSPI(name_in_bytes[i]);
            }
            SendDataSPI(0x33);
            score_received = false;
        }
    }
}
