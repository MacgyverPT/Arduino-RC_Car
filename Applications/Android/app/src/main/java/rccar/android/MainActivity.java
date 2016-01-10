package rccar.android;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.drm.DrmStore;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import java.io.IOException;
import java.io.OutputStream;
import java.util.UUID;

public class MainActivity extends AppCompatActivity {

    private static final String TAG = "RC Car Controller";

    Button btnUp, btnDown, btnLeft, btnRight, btnStop;
    EditText edtxtTemp, edtxtDst;
    MenuItem menuLedsOn, menuLedsOff, menuSaveTemperatureData, menuSaveDistanceData,
             menuBuzzer, menuSpeedMax, menuSpeedMin;

    private static final int REQUEST_ENABLE_BT = 1;
    private BluetoothAdapter btAdapter = null;
    private BluetoothSocket btSocket = null;
    private OutputStream outStream = null;

    // Well known SPP UUID
    private static final UUID MY_UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");

    // Insert your bluetooth devices MAC address
    private static String address = "20:15:10:12:51:65";




    /* Called when the activity is first created. */
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Log.d(TAG, "In onCreate()");
        setContentView(R.layout.activity_main);

        //create all items on screen (buttons, menu items, edit text, etc
        createButtons();

        //check the bluetooth connection
        btAdapter = BluetoothAdapter.getDefaultAdapter();
        checkBTState();

        //car movement
        rcCarMove();
    }


    @Override
    public void onResume() {
        super.onResume();

        Log.d(TAG, "...In onResume - Attempting client connect...");

        // Set up a pointer to the remote node using it's address.
        BluetoothDevice device = btAdapter.getRemoteDevice(address);

        // Two things are needed to make a connection:
        //  1. A MAC address, which we got above.
        //  2. A Service ID or UUID.  In this case we are using the UUID for SPP.
        try {
            btSocket = device.createRfcommSocketToServiceRecord(MY_UUID);
        } catch (IOException e) {
            errorExit("Fatal Error", "In onResume() and socket create failed: " + e.getMessage() + ".");
        }

        // Discovery is resource intensive.  Make sure it isn't going on
        // when you attempt to connect and pass your message.
        btAdapter.cancelDiscovery();

        // Establish the connection.  This will block until it connects.
        Log.d(TAG, "...Connecting to Remote...");
        try {
            btSocket.connect();
            Log.d(TAG, "...Connection established and data link opened...");
        } catch (IOException e) {
            try {
                btSocket.close();
            } catch (IOException e2) {
                errorExit("Fatal Error", "In onResume() and unable to close socket during connection failure" + e2.getMessage() + ".");
            }
        }

        // Create a data stream so we can talk to server.
        Log.d(TAG, "...Creating Socket...");

        try {
            outStream = btSocket.getOutputStream();
        } catch (IOException e) {
            errorExit("Fatal Error", "In onResume() and output stream creation failed:" + e.getMessage() + ".");
        }
    }

    @Override
    public void onPause() {
        super.onPause();

        Log.d(TAG, "...In onPause()...");

        if (outStream != null) {
            try {
                outStream.flush();
            } catch (IOException e) {
                errorExit("Fatal Error", "In onPause() and failed to flush output stream: " + e.getMessage() + ".");
            }
        }

        try     {
            btSocket.close();
        } catch (IOException e2) {
            errorExit("Fatal Error", "In onPause() and failed to close socket." + e2.getMessage() + ".");
        }
    }

    /*
    * Check for Bluetooth support and then check to make sure it is turned on
    */
    private void errorExit(String title, String message) {
        Toast msg = Toast.makeText(getBaseContext(), title + " - " + message, Toast.LENGTH_LONG);
        msg.show();
        finish();
    }

    /*
    * Check the bluetooth state
    */
    private void checkBTState() {
        // Emulator doesn't support Bluetooth and will return null
        if(btAdapter==null) {
            errorExit("Fatal Error", "Bluetooth Not supported. Aborting.");
        } else {
            if (btAdapter.isEnabled()) {
                Log.d(TAG, "...Bluetooth is enabled...");
            } else {
                //Prompt user to turn on Bluetooth
                Intent enableBtIntent = new Intent(btAdapter.ACTION_REQUEST_ENABLE);
                startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
            }
        }
    }

    public void sendData(String message) {
        byte[] msgBuffer = message.getBytes();

        Log.d(TAG, "...Sending data: " + message + "...");

        try {
            outStream.write(msgBuffer);
        } catch (IOException e) {
            String msg = "In onResume() and an exception occurred during write: " + e.getMessage();
            //if (address.equals("00:00:00:00:00:00"))
            if(address.equals(MY_UUID))
                msg = msg + ".\n\nUpdate your server address from " + MY_UUID.toString() + " to the correct address on line 37 in the java code";
            msg = msg +  ".\n\nCheck that the SPP UUID: " + MY_UUID.toString() + " exists on server.\n\n";

            errorExit("Fatal Error", msg);
        }
    }

    private void createButtons() {
        btnUp = (Button) findViewById(R.id.btnUp);
        btnDown = (Button) findViewById(R.id.btnDown);
        btnLeft = (Button) findViewById(R.id.btnLeft);
        btnRight = (Button) findViewById(R.id.btnRight);
        btnStop = (Button) findViewById(R.id.btnStop);
        edtxtTemp = (EditText) findViewById(R.id.edtTxtTemperatura);
        edtxtDst = (EditText) findViewById(R.id.edtTxtDistancia);
        menuLedsOn = (MenuItem) findViewById(R.id.menuLedsOn);
        menuLedsOff = (MenuItem) findViewById(R.id.menuLedsOff);
        menuSaveTemperatureData = (MenuItem) findViewById(R.id.menuSaveTemperature);
        menuSaveDistanceData = (MenuItem) findViewById(R.id.menuSaveDistance);
        menuBuzzer = (MenuItem) findViewById(R.id.menuBuzzer);
        menuSpeedMax = (MenuItem) findViewById(R.id.menuSpeedMax);
        menuSpeedMin = (MenuItem) findViewById(R.id.menuSpeedMin);

        edtxtDst.setEnabled(false);
        edtxtTemp.setEnabled(false);
    }


    // ==================== MENUS ==================

    public boolean onCreateOptionsMenu(Menu menu){
        MenuInflater inflater = getMenuInflater();

        inflater.inflate(R.menu.menu_main, menu);
        return true;
    }

    public boolean onOptionsItemSelected(MenuItem item){
        int id = item.getItemId();

        switch(id){

            case R.id.menuBuzzer:
                Toast.makeText(getApplicationContext(), "Buzzer", Toast.LENGTH_SHORT).show(); //Delete this line
                //sendData("9"); //delete this line
                buzzerBuzz();
                break;

            case R.id.menuLedsOn:
                ledsOn();
                break;

            case R.id.menuLedsOff:
                //FAZER
                break;

            case R.id.menuSaveDistance:
                menuSaveDistanceToFile();
                break;

            case R.id.menuSpeedMax:
                setCarMaxSpeed();
                break;

            case R.id.menuSpeedMin:
                setCarMinSpeed();
                break;
        }


        return super.onOptionsItemSelected(item);
    }




    // ==================== ACTIONS FOR MENUS ===============
    private void buzzerBuzz(){
        sendData("9");
    }

    private void ledsOn(){
        sendData("8");
    }

    private void setCarMaxSpeed() {
        sendData("7");
    }

    private void setCarMinSpeed() {
        sendData("6");
    }

    private void menuSaveDistanceToFile(){

    }

    // ==================== MOVEMENT ===============
    private void rcCarMove() {
        moveForward();
        moveBackward();
        moveLeft();
        moveRight();
        moveStop();
    }
    /*
    *Move RC Car to FORWARD
    * */
    public void moveForward(){
        btnUp.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                sendData("1");
                Toast msg = Toast.makeText(getBaseContext(), "You have clicked MOVER FRENTE", Toast.LENGTH_SHORT);
                msg.show();
                edtxtDst.append("FRENTE\n"); //apagar
            }
        });
    }

    /*
    *Move RC Car to BACKWARD
    * */
    public void moveBackward(){
        btnDown.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                sendData("2");
                Toast msg = Toast.makeText(getBaseContext(), "You have clicked MOVER TRAS", Toast.LENGTH_SHORT);
                msg.show();
                edtxtDst.append("TRAS\n"); //apagar
            }
        });
    }

    /*
    *Move RC Car to LEFT
    * */
    public void moveLeft(){
        btnLeft.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                sendData("3");
                Toast msg = Toast.makeText(getBaseContext(), "You have clicked MOVER ESQUERDA", Toast.LENGTH_SHORT);
                msg.show();
                edtxtDst.append("ESQUERDA\n"); //apagar
            }
        });
    }

    /*
    *Move RC Car to RIGHT
    * */
    public void moveRight(){
        btnRight.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                sendData("4");
                Toast msg = Toast.makeText(getBaseContext(), "You have clicked MOVER DIREITA", Toast.LENGTH_SHORT);
                msg.show();
                edtxtDst.append("DIREITA\n"); //apagar
            }
        });
    }

    /*
    *Move RC Car to STOP
    * */
    public void moveStop(){
        btnStop.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                sendData("5");
                Toast msg = Toast.makeText(getBaseContext(), "You have clicked STOP", Toast.LENGTH_SHORT);
                msg.show();
                edtxtDst.append("STOP\n"); //apagar
            }
        });
    }


}
