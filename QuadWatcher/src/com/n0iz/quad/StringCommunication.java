package com.n0iz.quad;

/*
* Class uses the RXTX Library for Serial Communication
* to send, receive and encode Strings from an Arduino
*
* done by Laurid Meyer
* 28.04.2012
*
* http://www.lauridmeyer.com
*/
 
import gnu.io.CommPortIdentifier;
import gnu.io.SerialPort;
import gnu.io.SerialPortEvent;
import gnu.io.SerialPortEventListener;
 
 
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Enumeration;
 
public class StringCommunication implements SerialPortEventListener{
    SerialPort serialPort;
        /** The port we're normally going to use. */
    private static final String PORT_NAMES[] = {
            "/dev/tty.usbserial-A9007UX1", // Mac OS X
            "/dev/ttyACM0", // Linux
            "COM4", // Windows
            };
    /** Buffered input stream from the port */
    private InputStream input;
    /** The output stream to the port */
    private OutputStream output;
    /** Milliseconds to block while waiting for port open */
    private static final int TIME_OUT = 2000;
    /** Default bits per second for COM port. */
        private static final int DATA_RATE = 115200;
 
        private String inputBuffer="";
 
        public void initialize() {
            CommPortIdentifier portId = null;
            Enumeration<?> portEnum = CommPortIdentifier.getPortIdentifiers();
 
            // iterate through, looking for the port
            while (portEnum.hasMoreElements()) {
                CommPortIdentifier currPortId = (CommPortIdentifier) portEnum.nextElement();
                for (String portName : PORT_NAMES) {
                    if (currPortId.getName().equals(portName)) {
                        portId = currPortId;
                        break;
                    }
                }
            }
 
            if (portId == null) {
                System.out.println("Could not find port " + PORT_NAMES[1]);
        return;
    }else{
    System.out.println("Found port " + PORT_NAMES[1]);
    }
 
       try {
        // open serial port, and use class name for the appName.
        serialPort = (SerialPort) portId.open(this.getClass().getName(),TIME_OUT);
 
        // set port parameters
        serialPort.setSerialPortParams(DATA_RATE,
    		SerialPort.DATABITS_8,
            SerialPort.STOPBITS_1,
            SerialPort.PARITY_NONE);
 
        // open the streams
        input = serialPort.getInputStream();
        output = serialPort.getOutputStream();
 
                // add event listeners
        serialPort.addEventListener(this);
        serialPort.notifyOnDataAvailable(true);
 
            } catch (Exception e) {
                System.err.println(e.toString());
            }
        }
 
 /**
 * This should be called when you stop using the port.
 * This will prevent port locking on platforms like Linux.
 */
public synchronized void close() {
    if (serialPort != null) {
        serialPort.removeEventListener();
        serialPort.close();
    }
}
 
	        /**
	 * This Method can be called to print a String
	 * to the serial connection
	 */
	 public synchronized void sendString(String msg){
	    try {
	        output.write(msg.getBytes());//write it to the serial
	        output.flush();//refresh the serial
	        //System.out.print("<- "+msg);//output for debugging
	    } catch (Exception e) {
	        System.err.println(e.toString());
	    }
	 }
 
        /**
     * This Method is called when Serialdata is recieved
     */
    public synchronized void serialEvent (SerialPortEvent oEvent) {
        if (oEvent.getEventType() == SerialPortEvent.DATA_AVAILABLE) {
            try {
                int available = input.available();
                for(int i=0;i<available;i++){//read all incoming characters
                    int receivedVal=input.read();//store it into an int (because of the input.read method
                    if(receivedVal!=10){//if the character is not a new line "\n" and not a carriage return
                        inputBuffer+=(char)receivedVal;//store the new character into a buffer
                    }else if(receivedVal==10){//if it's a new line character
                        float roll, pitch, yaw;
                        int ping;
                        roll = Float.valueOf(inputBuffer.substring(0, inputBuffer.indexOf(' ')));
                        String temp = inputBuffer.substring(inputBuffer.indexOf(' ')+1, inputBuffer.length());
                        pitch = Float.valueOf(temp.substring(0, temp.indexOf(' ')));
                        temp = temp.substring(temp.indexOf(' ')+1, temp.length());
                        yaw = Float.valueOf(temp.substring(0, temp.indexOf(' ')));
                        temp = temp.substring(temp.indexOf(' ')+1, temp.length());
                        ping = Integer.valueOf(temp);
                        QuadWatcher.setValues(roll, pitch, yaw, ping);
                        inputBuffer="";//clear the buffer
                    }
                }
            } catch (Exception e) {
                System.err.println(e.toString());
            }
        }
    }
}

