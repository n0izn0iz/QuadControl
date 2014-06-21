	

package com.n0iz.quad;

import javax.swing.*;        
 
import net.java.games.input.Component;
import net.java.games.input.Controller;
import net.java.games.input.ControllerEnvironment;
 
public class QuadWatcher {
 
    private static JTextArea textArea = new JTextArea("");
       
    /**
     * Create the GUI and show it.  For thread safety,
     * this method should be invoked from the
     * event-dispatching thread.
     */
    static JFrame frame = new JFrame("QuadWatcher");
    private static void createAndShowGUI() {
        //Create and set up the window.
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        //Add the ubiquitous "Hello World" label.
        frame.getContentPane().add(textArea);
        //Display the window.
        frame.pack();
        frame.setVisible(true);
    }
   
    static final double maxthrotle = 1500;
    static final double minthrotle = 800;
    static final double idlethrotle = 950;
    static double throtle = minthrotle;
    static int lastthrotle = (int)throtle;
    static StringCommunication main = null;
    public static void main(String[] args) {
        main = new StringCommunication();
        main.initialize();
        System.out.println("Started");
        javax.swing.SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                createAndShowGUI();
            }
        });
       
        initJInput();
       
        while(true) {
            xbox.poll();
            Component[] components = xbox.getComponents();
            double zAxis = 0.0;
            double xAxis = 0.0;
            double yAxis = 0.0;
            for(int i=0;i<components.length;i++) {
                if(components[i].getName().equals("z")) {
                   double temp = components[i].getPollData();
                   if(Math.abs(temp) >= 0.01) {
                       zAxis -= temp;
                   }
                }
                if(components[i].getName().equals("rz")) {
                    double temp = components[i].getPollData();
                    if(Math.abs(temp) >= 0.01) {
                        zAxis += temp;
                    }
                 }
                if(components[i].getName().equals("x")) {
                    double temp = components[i].getPollData();
                    if(Math.abs(temp) >= 0.01) {
                        xAxis = temp * 10.f;
                    }
                }
                if(components[i].getName().equals("y")) {
                    double temp = components[i].getPollData();
                    if(Math.abs(temp) >= 0.01) {
                        yAxis = -temp * 10.f;
                    }
                }
            }
            double gate = 5.0;
            if(Math.abs(xAxis) < gate) {
                xAxis = 0.0;
            }
            else {
                xAxis = (xAxis > 0.0) ? xAxis - gate : xAxis + gate;
            }
            if(Math.abs(yAxis) < gate) {
                yAxis = 0.0;
            }
            else {
                yAxis = (yAxis > 0.0) ? yAxis - gate : yAxis + gate;
            }
	        throtle += zAxis * 10.0;
	        if(throtle > maxthrotle)
	                throtle = maxthrotle;
	        if(throtle < minthrotle)
	                throtle = minthrotle;
	        textArea.setText(" Throtle: " + (int)throtle + "\nX: " + (int)xAxis + "\nY: " + (int)yAxis 
	        		+ "\nRoll: " + roll + "\nPitch: " + pitch + "\nYaw: " + yaw + "\nPing: " + ping);
	        frame.pack();
	        main.sendString((int)throtle + " " + (int)xAxis + " " + (int)yAxis + ".");
	        lastthrotle = (int)throtle;   
	         
	        try {
	           Thread.sleep(20);
	        } catch (InterruptedException e) {
	           // TODO Auto-generated catch block
	           e.printStackTrace();
	        }
           
        }
    }
   
    static Controller xbox = null;
    static float roll = 0.f, pitch = 0.f, yaw= 0.f;
    static int ping = 0;
    static void setValues(float r, float p, float y, int pp) {
    	roll = r;
    	pitch = p;
    	yaw = y;
    	ping = pp;
    }
    
    public static void initJInput() {
        Controller[] ca = ControllerEnvironment.getDefaultEnvironment().getControllers();
       
        // Search for the first xbox360 controller and assign it to the xbox object
        for(int i =0;i<ca.length;i++){
            if(ca[i].getName().contains("360")) {
                /* Get the name of the controller */
                System.out.println("\n"+ca[i].getName());
               
                System.out.println("Type: "+ca[i].getType().toString());
   
                /* Get this controllers components (buttons and axis) */
                Component[] components = ca[i].getComponents();
                System.out.println("Component Count: "+components.length);
                for(int j=0;j<components.length;j++){
                    /* Get the components name */
                    System.out.print("Component "+j+": "+components[j].getName());
                    System.out.print("    Identifier: "+ components[j].getIdentifier().getName());
                    System.out.print("    ComponentType: ");
                    if (components[j].isRelative()) {
                        System.out.print("Relative");
                    } else {
                        System.out.print("Absolute");
                    }
                    if (components[j].isAnalog()) {
                        System.out.print(" Analog\n");
                    } else {
                        System.out.print(" Digital\n");
                    }
                }
	            System.out.println("Found controller!");
	            xbox = ca[i];
	            break;
            }
        }
       
        if(xbox == null) {
            System.err.println("FAILED TO FIND CONTROLER!");
            System.exit(-1);
        }
    }
}


