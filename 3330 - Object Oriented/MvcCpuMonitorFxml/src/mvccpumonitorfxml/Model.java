/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package mvccpumonitorfxml;

import java.lang.management.ManagementFactory;
import java.lang.management.OperatingSystemMXBean;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.text.DecimalFormat;
import javafx.animation.Animation;
import javafx.animation.KeyFrame;
import javafx.animation.Timeline;
import javafx.event.ActionEvent;
import javafx.util.Duration;

/**
 *
 * @author Professor Wergeles <Professor Wergeles at cs3330@missouri.edu>
 */
public class Model extends AbstractModel { 
    // step 1: extends abstractmodel 
    
    private Timeline timeline;
    private KeyFrame keyFrame;
    
    private double tickTimeInSeconds; //Change resolution
    private double cpu;
    private double angle; 
    private String cpuString; 
    
    public Model(){
//        super(); 
        tickTimeInSeconds = 0.1;
        cpu = 0;
    }
    
    public void setupMonitor() {        
        keyFrame = new KeyFrame(Duration.millis(tickTimeInSeconds * 1000), (ActionEvent event) -> {
                updateMonitor(); 
        });
        timeline = new Timeline(keyFrame);
        timeline.setCycleCount(Animation.INDEFINITE);
    }
    
    private void updateMonitor() {
        cpu = getCpu();
        updateAnalog(); 
        updateDigital(); 
//        hand.setRotate(model.calculateRotation(cpu));
//        cpuLabel.setText(model.getCPUString(cpu) + "%");
    }
    
    public boolean isRunnning(){
        if(timeline != null){
            if(timeline.getStatus() == Animation.Status.RUNNING){
                return true;
            }
        }
        return false;
    }
   
    public void updateAnalog(){
        double oldAngle = angle; 
        angle = calculateRotation(cpu);   
        
        // step 4: everytime a value is updated, let the controller know
        
        firePropertyChange("Analog", oldAngle, angle); 
        
//        firePropertyChange("Analog", null, angle);
    }
    public void updateDigital(){
        String oldString = cpuString; 
        cpuString = getCPUString(cpu); 

        //step 4
        
        firePropertyChange("Digital", oldString, cpuString); 
    }
    
    public String getCPUString(double cpu) {
        DecimalFormat formatter = new DecimalFormat("00.00");
        return formatter.format(cpu * 100);
    }
    
    public double calculateRotation(double cpu){ 
        return cpu * 360;
    }
    
    public void start() {
        timeline.play();
    }
    
    public void stop() {
        timeline.stop();
    }
    
    public void reset() {
        timeline.stop();
        cpu = 0.0; 
    }
    
    public double getCpu(){
        cpu = getCPUUsage(); 
        return cpu; 
    }
    
    public Timeline getTimeLine(){
        return timeline; 
    }
    public void setTimeLine(Timeline timeline){
        this.timeline = timeline; 
    }
    public KeyFrame getKeyFrame(){
        return keyFrame; 
    }
    public void setKeyFrame(KeyFrame keyFrame){
        this.keyFrame = keyFrame; 
    }
    public double getTickTimeInSeconds(){
        return tickTimeInSeconds; 
    }
        
    private static double getCPUUsage() {
        OperatingSystemMXBean operatingSystemMXBean = ManagementFactory.getOperatingSystemMXBean();
        double value = 0;
        for (Method method : operatingSystemMXBean.getClass().getDeclaredMethods()) {
            method.setAccessible(true);
            if (method.getName().startsWith("getSystemCpuLoad")
                    && Modifier.isPublic(method.getModifiers())) {
                try {
                    value = (double) method.invoke(operatingSystemMXBean);
                } catch (Exception e) {
                    value = 0;
                }
                
                if(Double.isNaN(value)) value = Math.random(); 
                
                return value;
            }
        }
        return value;
    } 
}