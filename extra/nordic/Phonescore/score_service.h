#pragma once
#include "ble/BLE.h"

class ScoreService {
    private:
        
    public:
        BLE& ble;
        ReadOnlyGattCharacteristic<int>  score_value;
        ReadOnlyArrayGattCharacteristic<uint8_t, 11> read_name_array;
        
        static uint8_t write_value[11];
        WriteOnlyArrayGattCharacteristic<uint8_t, 11> name_array;
    
        const static uint16_t SCORE_SERVICE_UUID = 0xA000;
        const static uint16_t SCORE_VALUE_CHARACTERISTIC_UUID = 0xA001;
        const static uint16_t NAME_CHARACTERISTIC_UUID = 0xA002;
        const static uint16_t NAME_ARRAY_READ_CHARACTERISTIC_UUID = 0xA003;
        
        ScoreService(BLE &ble_, int value, uint8_t* arr) : ble(ble_), score_value(SCORE_VALUE_CHARACTERISTIC_UUID, &value, GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY), name_array(NAME_CHARACTERISTIC_UUID, arr), read_name_array( NAME_ARRAY_READ_CHARACTERISTIC_UUID, arr, GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY) {
            GattCharacteristic *charTable[] = {&score_value, &name_array, &read_name_array};
            GattService scoreService(ScoreService::SCORE_SERVICE_UUID, charTable, sizeof(charTable) / sizeof(GattCharacteristic *));
            ble.gattServer().addService(scoreService);
        }
        
        void UpdateScore(int new_value) {
            ble.gattServer().write(score_value.getValueHandle(), (uint8_t *)&new_value, sizeof(int));
        }
        
        void UpdateName(uint8_t* name_in){
            ble.gattServer().write(read_name_array.getValueHandle(), name_in, 11);
        }
};