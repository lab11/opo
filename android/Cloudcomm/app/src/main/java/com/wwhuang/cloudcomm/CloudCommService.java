package com.wwhuang.cloudcomm;

import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.Service;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattCallback;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattDescriptor;
import android.bluetooth.BluetoothGattService;
import android.bluetooth.BluetoothManager;
import android.bluetooth.BluetoothProfile;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.IBinder;
import android.os.Message;
import android.os.Parcel;
import android.os.PowerManager;
import android.os.Vibrator;
import android.util.Log;
import android.widget.TextView;

import org.apache.http.HttpResponse;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.entity.ByteArrayEntity;
import org.apache.http.entity.StringEntity;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.protocol.HTTP;

import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.List;
import java.util.UUID;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicInteger;

public class CloudCommService extends Service {

    private NotificationManager nm;
    private Notification foregroundNotification;
    private Vibrator mVibrator;
    private PowerManager pm;
    private PowerManager.WakeLock wl;

    private BluetoothManager mBLManager;
    private BluetoothAdapter mBLAdapter;
    private int BT_REQUEST_ENABLE = 1;
    private AtomicBoolean am_i_connecting;
    private AtomicInteger char_setup;
    private HandlerThread BLThread;
    private Handler BLHandler;
    private int debugCounter = 0;

    private HttpClient hc;
    private HttpPost hp;
    ArrayList<Byte> dataBuilder;

    private String CCUUIDSTRING = "00002000-0000-1000-8000-00805F9B34FB";
    private String CCDATAUUIDSTRING = "00002b00-0000-1000-8000-00805F9B34FB";
    private String CCMETAUUIDSTRING = "00002b01-0000-1000-8000-00805F9B34FB";
    private String CCREQUUIDSTRING = "00002b02-0000-1000-8000-00805F9B34FB";
    private String CCINCUUIDSTRING = "00002b03-0000-1000-8000-00805F9B34FB";
    private String CCREADYSTRING = "00002b04-0000-1000-8000-00805F9B34FB";
    private String CCACKSTRING = "00002b05-0000-1000-8000-00805F9B34FB";
    private String CccDescriptorUUIDString = "00002902-0000-1000-8000-00805F9B34FB";

    private UUID CCUUID = UUID.fromString(CCUUIDSTRING);
    private UUID CCDATAUUID = UUID.fromString(CCDATAUUIDSTRING);
    private UUID CCMETAUUID = UUID.fromString(CCMETAUUIDSTRING);
    private UUID CCREQUUID = UUID.fromString(CCREQUUIDSTRING);
    private UUID CCINCUUID = UUID.fromString(CCINCUUIDSTRING);
    private UUID CCREADYUUID = UUID.fromString(CCREADYSTRING);
    private UUID CCACKUUID = UUID.fromString(CCACKSTRING);
    private UUID CCCDESCUUID = UUID.fromString(CccDescriptorUUIDString);

    int CC_START_LE_SCAN = 0;
    int CC_START_BLE_CONNECTION = 1;

    final protected static char[] hexArray = "0123456789ABCDEF".toCharArray();
    public static String bytesToHex(byte[] bytes) {
        char[] hexChars = new char[bytes.length * 2];
        for ( int j = 0; j < bytes.length; j++ ) {
            int v = bytes[j] & 0xFF;
            hexChars[j * 2] = hexArray[v >>> 4];
            hexChars[j * 2 + 1] = hexArray[v & 0x0F];
        }
        return new String(hexChars);
    }
    
    public CloudCommService() {

    }

    @Override
    public IBinder onBind(Intent intent) {
        // TODO: Return the communication channel to the service.
        throw new UnsupportedOperationException("Not yet implemented");
    }

    @Override
    public void onCreate() {
        nm = (NotificationManager)getSystemService(NOTIFICATION_SERVICE);
        mBLManager = (BluetoothManager) getSystemService(BLUETOOTH_SERVICE);
        mBLAdapter = mBLManager.getAdapter();
        am_i_connecting = new AtomicBoolean(false);
        char_setup = new AtomicInteger(0);
        mVibrator = (Vibrator) getSystemService(Context.VIBRATOR_SERVICE);
        BLThread = new HandlerThread("CcBLHandlerThread");
        BLThread.start();
        BLHandler = new Handler(BLThread.getLooper(), ccBLHandlerCallback);
        dataBuilder = new ArrayList<Byte>();
        hc = new DefaultHttpClient();
        hp = new HttpPost("https://gbns1w57rtvk.runscope.net/");
        //hp.setHeader(HTTP.CONTENT_TYPE, HTTP.OCTET_STREAM_TYPE);
        pm = (PowerManager) getSystemService(POWER_SERVICE);
        wl = pm.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK, "CCPartialWakeLock");
        // In this sample, we'll use the same text for the ticker and the expanded notification
        CharSequence text = "CC Service Test";

        Intent intent = new Intent(this, MainActivity.class);
        PendingIntent pIntent = PendingIntent.getActivity(this, 0, intent, 0);

        // Set the icon, scrolling text and timestamp
        foregroundNotification = new Notification.Builder(this)
                .setContentTitle("Cloudcomm")
                .setContentText(text)
                .setContentIntent(pIntent)
                .setSmallIcon(R.drawable.abc_ab_share_pack_holo_light)
                .build();

        startForeground(27, foregroundNotification);
    }


    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        if(!mBLAdapter.isEnabled()) {
            mBLAdapter.enable();
        }
        BLHandler.sendEmptyMessage(0);
        //mBLAdapter.startLeScan(mLeScanCallback);
        return Service.START_STICKY;
    }

    @Override
    public void onDestroy() {

    }

    Handler.Callback ccBLHandlerCallback = new Handler.Callback() {
       @Override
        public boolean handleMessage(Message msg) {
           BluetoothManager ccBLManager = (BluetoothManager) getSystemService(BLUETOOTH_SERVICE);
           BluetoothAdapter ccBLAdapter = ccBLManager.getAdapter();
           if(msg.what == 0) {
               ccBLAdapter.startLeScan(mLeScanCallback);
           }
           else if(msg.what == 1) {
               ccBLAdapter.stopLeScan(mLeScanCallback);
               BluetoothDevice blDevice = (BluetoothDevice) msg.obj;
               blDevice.connectGatt(getApplicationContext(), false, mGattCallback);
           }
           else if(msg.what == 2) {
               BluetoothGatt mGatt = (BluetoothGatt) msg.obj;
               if(!mGatt.discoverServices()) {
                   Log.v("BLEServices", "failed");
               }
           }
           else if(msg.what == 3) {
               BluetoothGatt mGatt = (BluetoothGatt) msg.obj;
               BluetoothGattCharacteristic ccReadyChar = mGatt.getService(CCUUID).getCharacteristic(CCREADYUUID);
               if(ccReadyChar != null) {
                   ccReadyChar.setValue(1, BluetoothGattCharacteristic.FORMAT_UINT8, 0);
                   ccReadyChar.setWriteType(BluetoothGattCharacteristic.WRITE_TYPE_DEFAULT);
                   mGatt.writeCharacteristic(ccReadyChar);
               }
           }
           else if(msg.what == 4) {
               BluetoothGatt mGatt = (BluetoothGatt) msg.obj;
               BluetoothGattCharacteristic ccDataChar = mGatt.getService(CCUUID).getCharacteristic(CCDATAUUID);
               mGatt.readCharacteristic(ccDataChar);
           }
           else if(msg.what == 5) {
               BluetoothGatt mGatt = (BluetoothGatt) msg.obj;
               BluetoothGattCharacteristic ccReadyChar = mGatt.getService(CCUUID).getCharacteristic(CCREADYUUID);
               ccReadyChar.setValue(msg.arg1, BluetoothGattCharacteristic.FORMAT_UINT8, 0);
               ccReadyChar.setWriteType(BluetoothGattCharacteristic.WRITE_TYPE_DEFAULT);
               if(!(mGatt.writeCharacteristic(ccReadyChar))) {
                   Log.v("BleCharWrite", "failed");
               }
           }
           return true;
       }
    };


    private final BluetoothAdapter.LeScanCallback mLeScanCallback = new BluetoothAdapter.LeScanCallback() {
        // Find a cloudcomm device, connect to it, and stop scanning for other devices
        @Override
        public void onLeScan(final BluetoothDevice bluetoothDevice, int i, byte[] bytes) {
            String dname = bluetoothDevice.getName();
            if(dname != null && dname.matches("Cloudcomm")) {
                //mBLAdapter.stopLeScan(mLeScanCallback);
                wl.acquire();
                Message m = Message.obtain(BLHandler, 1, bluetoothDevice);
                BLHandler.sendMessage(m);
                //bluetoothDevice.connectGatt(getApplicationContext(), false, mGattCallback);
            }
        }
    };

    private final BluetoothGattCallback mGattCallback = new BluetoothGattCallback() {
            @Override
            public void onConnectionStateChange(BluetoothGatt gatt, int status, int newState) {
                super.onConnectionStateChange(gatt, status, newState);
                if (newState == BluetoothProfile.STATE_CONNECTED) {
                    Message m = Message.obtain(BLHandler, 2, gatt);
                    BLHandler.sendMessage(m);
                }
                else if (newState == BluetoothProfile.STATE_DISCONNECTED) {
                    gatt.close();
                    if (!mBLAdapter.isEnabled()) {
                        mBLAdapter.enable();
                    }
                    wl.release();
                    BLHandler.sendEmptyMessage(0);
                }
            }

            @Override
            public void onServicesDiscovered(BluetoothGatt gatt, int status) {
                super.onServicesDiscovered(gatt, status);
                if (status == BluetoothGatt.GATT_SUCCESS) {
                    BluetoothGattService ccService = gatt.getService(CCUUID);
                    if (ccService != null) {
                        BluetoothGattCharacteristic ccData = ccService.getCharacteristic(CCDATAUUID);
                        if (ccData != null) {
                            BluetoothGattDescriptor dataDesc = ccData.getDescriptor(CCCDESCUUID);
                            gatt.setCharacteristicNotification(ccData, true);
                            dataDesc.setValue(BluetoothGattDescriptor.ENABLE_INDICATION_VALUE);
                            gatt.writeDescriptor(dataDesc);
                        }
                    }
                }
            }

            @Override
            public void onCharacteristicRead(BluetoothGatt gatt, BluetoothGattCharacteristic characteristic, int status) {
                super.onCharacteristicRead(gatt, characteristic, status);
                if (characteristic.getUuid().equals(CCDATAUUID)) {
                    byte[] cData = characteristic.getValue();
                    int seq_num = cData[0];
                    if(seq_num < 0) {
                        seq_num += 256;
                    }

                    for(int i=1;i<cData.length;i++) {
                        dataBuilder.add(cData[i]);
                    }

                    if(seq_num == 255) {
                        Log.v("ccDataSend", "cloudsend");
                        byte[] finalData = new byte[dataBuilder.size()];
                        for(int i=0;i<finalData.length;i++) {
                            finalData[i] = (byte) dataBuilder.get(i);
                        }
                        String fd = bytesToHex(finalData);
                        Log.v("ccDataString", fd);
                        try{
                            hp.setEntity(new StringEntity(fd));
                            HttpResponse r = hc.execute(hp);
                            r.getEntity().consumeContent();
                        }catch (ClientProtocolException e) {
                            Log.v("ccHpException", "Client protocol exception");
                        } catch (IOException e) {
                            Log.v("ccHpException", "IO exception");
                        }
                        dataBuilder.clear();
                    }
                    Log.v("seqnum", Integer.toString(seq_num));
                    Message m = Message.obtain(BLHandler, 5, seq_num, 0, gatt);
                    BLHandler.sendMessage(m);
                }
                else if (characteristic.getUuid().equals(CCMETAUUID)) {

                }
                else if (characteristic.getUuid().equals(CCREQUUID)) {

                }
            }

            @Override
            public void onCharacteristicWrite(BluetoothGatt gatt, BluetoothGattCharacteristic characteristic, int status) {
                super.onCharacteristicWrite(gatt, characteristic, status);
                if (characteristic.getUuid().equals(CCREADYUUID)) {
                    if(status != BluetoothGatt.GATT_SUCCESS) {
                        Log.v("ccOnCharWrite", "CCREADY write failed");
                    }
                }
                if (characteristic.getUuid().equals(CCACKUUID)) {}
            }

            @Override
            public void onCharacteristicChanged(BluetoothGatt gatt, BluetoothGattCharacteristic characteristic) {
                super.onCharacteristicChanged(gatt, characteristic);
                if(characteristic.getUuid().equals(CCDATAUUID)) {
                    Message m = Message.obtain(BLHandler, 4, gatt);
                    BLHandler.sendMessage(m);
                }

            }

            @Override
            public void onDescriptorRead(BluetoothGatt gatt, BluetoothGattDescriptor descriptor, int status) {
                super.onDescriptorRead(gatt, descriptor, status);
            }

            @Override
            public void onDescriptorWrite(BluetoothGatt gatt, final BluetoothGattDescriptor descriptor, int status) {
                super.onDescriptorWrite(gatt, descriptor, status);
                if (descriptor.getUuid().equals(CCCDESCUUID) && status == BluetoothGatt.GATT_SUCCESS) {
                    BluetoothGattService s = gatt.getService(CCUUID);
                    if (descriptor.getCharacteristic().getUuid().equals(CCDATAUUID)) {
                        BluetoothGattCharacteristic c = s.getCharacteristic(CCREQUUID);
                        BluetoothGattDescriptor dataDesc = c.getDescriptor(CCCDESCUUID);
                        gatt.setCharacteristicNotification(c, true);
                        dataDesc.setValue(BluetoothGattDescriptor.ENABLE_INDICATION_VALUE);
                        gatt.writeDescriptor(dataDesc);

                    } else if (descriptor.getCharacteristic().getUuid().equals(CCREQUUID)) {
                        BluetoothGattCharacteristic c = s.getCharacteristic(CCMETAUUID);
                        BluetoothGattDescriptor dataDesc = c.getDescriptor(CCCDESCUUID);
                        gatt.setCharacteristicNotification(c, true);
                        dataDesc.setValue(BluetoothGattDescriptor.ENABLE_INDICATION_VALUE);
                        gatt.writeDescriptor(dataDesc);

                    } else if (descriptor.getCharacteristic().getUuid().equals(CCMETAUUID)) {
                        Message m = Message.obtain(BLHandler, 3, gatt);
                        BLHandler.sendMessage(m);
                    }
                }
            }

            @Override
            public void onReliableWriteCompleted(BluetoothGatt gatt, int status) {
                super.onReliableWriteCompleted(gatt, status);
            }
    };


    /**
     * Show a notification while this service is running.
     */
    private void showNotification() {
        // In this sample, we'll use the same text for the ticker and the expanded notification
        CharSequence text = "Test Test 2";

        Intent intent = new Intent(this, MainActivity.class);
        PendingIntent pIntent = PendingIntent.getActivity(this, 0, intent, 0);

        // Set the icon, scrolling text and timestamp
        Notification notification = new Notification.Builder(this)
                .setContentTitle("WilLTest")
                .setContentText(text)
                .setContentIntent(pIntent)
                .setSmallIcon(R.drawable.abc_ab_share_pack_holo_light)
                .build();

        // Send the notification.
        nm.notify(0, notification);
    }

}
