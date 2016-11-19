package baugen.scoreboard;


import android.Manifest;
import android.bluetooth.le.BluetoothLeScanner;
import android.bluetooth.le.ScanCallback;
import android.bluetooth.le.ScanResult;
import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;
import android.bluetooth.*;
import java.nio.charset.Charset;
import java.nio.charset.StandardCharsets;
import java.util.UUID;

public class frontpage extends AppCompatActivity {

    // All the member objects of the class. All the objects will be initialized onCreate.
    private BluetoothAdapter bluetoothAdapter;
    private boolean isScanningForBLEDevice;
    private BluetoothLeScanner BLEScanner;

    TextView statusLabel;
    EditText userName;

    BluetoothManager bluetoothManager;
    BluetoothDevice BLESlave;
    BluetoothGatt subscription;
    BluetoothGattService service;
    BluetoothGattCharacteristic readCharacteristic;
    BluetoothGattCharacteristic writeCharacteristic;

    UUID serviceUuid;
    UUID readUuid;
    UUID writeUuid;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_frontpage);

        // Get the elements from the view
        statusLabel = (TextView) findViewById(R.id.logo);
        userName = (EditText) findViewById(R.id.name);
        statusLabel.setText("Initialized Application");

        // Initializes Bluetooth adapter.
        bluetoothManager = (BluetoothManager) getSystemService(getApplicationContext().BLUETOOTH_SERVICE);
        bluetoothAdapter = bluetoothManager.getAdapter();

        // Request permissions from the user to use BLE
        requestPermissions(new String[]{Manifest.permission.ACCESS_COARSE_LOCATION}, 1);
        requestPermissions(new String[]{Manifest.permission.ACCESS_FINE_LOCATION}, 4);

        if (bluetoothAdapter == null || !bluetoothAdapter.isEnabled()) {
            statusLabel.setText("ERROR");
            Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableBtIntent, 1);
        }
    }

    public void ScanForDevices(View view){

        if(isScanningForBLEDevice){
            statusLabel.setText("Stopped Scanning");
            scanLeDevice(false);
        } else {
            statusLabel.setText("Started Scanning");
            scanLeDevice(true);
        }
    }

    public void GetNameFromBLE(View view){
        subscription.readCharacteristic(readCharacteristic);
    }

    public void SendName(View view){
        // Get the name (in ascii) from the view
        byte[] name_score = userName.getText().toString().getBytes(StandardCharsets.US_ASCII);

        // A way to generate a new byte array on runtime
        byte[] dummy = {0,0,0,0,0,0,0,0,0,0};
        for(int i = 0; i < name_score.length; i++){
            dummy[i] = name_score[i];
        }
        byte[] lengthByte = {0};
        lengthByte[0] = (byte) name_score.length;
        byte[] toSend = new byte[11];
        System.arraycopy(dummy, 0, toSend, 0, dummy.length);
        System.arraycopy(lengthByte, 0, toSend, 10, lengthByte.length);

        // Write to the BLE Characteristic
        writeCharacteristic.setValue(toSend);
        subscription.writeCharacteristic(writeCharacteristic);
        statusLabel.setText("Sent name");
    }

    private void scanLeDevice(final boolean enable) {
        if (enable) {
            isScanningForBLEDevice = true;
            BLEScanner = bluetoothAdapter.getBluetoothLeScanner();
            BLEScanner.startScan(mLeScanCallback);
        } else {
            isScanningForBLEDevice = false;
            BLEScanner = bluetoothAdapter.getBluetoothLeScanner();
            BLEScanner.stopScan(mLeScanCallback);
        }
    }

    private ScanCallback mLeScanCallback = new ScanCallback() {

        @Override
        public void onScanResult(int callbackType, ScanResult result) {

            super.onScanResult(callbackType, result);
            BLESlave = result.getDevice();

            if(BLESlave.getName() == null){
                return;
            }

            // Check if it is the right Device. Fairly simple, the device's name is Baugen so it just looks for that...
            // Would've implemented better security etc. if we had time.
            if(BLESlave.getName().startsWith("Ba")){
                subscription = BLESlave.connectGatt(getApplicationContext(), true, joinGatt);
                statusLabel.setText(BLESlave.getName());
                scanLeDevice(false);
            }
        }
    };

    private BluetoothGattCallback joinGatt = new BluetoothGattCallback() {

        @Override
        public void onConnectionStateChange(BluetoothGatt gatt, int status, int newState) {
            if (newState == BluetoothProfile.STATE_CONNECTED) {
                subscription.discoverServices();
            }
        }

        @Override
        public void onCharacteristicRead(BluetoothGatt gatt, BluetoothGattCharacteristic characteristic, int status) {
            if(characteristic == null){
                statusLabel.setText("ERROR");
            }
            final byte[] val = characteristic.getValue();
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    String namestr = new String(val, Charset.forName("UTF-8"));
                    statusLabel.setText(namestr);
                }
            });
        }

        @Override
        public void onServicesDiscovered(BluetoothGatt gatt, int status) {
            // The UUID of the service. Hardcoded
            serviceUuid = serviceUuid.fromString("0000A000-0000-1000-8000-00805F9B34FB");
            service = subscription.getService(serviceUuid);
            if(service == null){
                statusLabel.setText("ERROR");
            }

            // The UUID of the characteristics. Hardcoded
            writeUuid = writeUuid.fromString("0000A002-0000-1000-8000-00805F9B34FB");
            readUuid =  readUuid.fromString("0000A003-0000-1000-8000-00805F9B34FB");
            readCharacteristic = service.getCharacteristic(readUuid);
            writeCharacteristic = service.getCharacteristic(writeUuid);
            scanLeDevice(false);
        }
    };
}
