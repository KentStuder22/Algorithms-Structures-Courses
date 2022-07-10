/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package krspcpCPUMonitorF21;

import java.lang.management.ManagementFactory;
import java.lang.management.OperatingSystemMXBean;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.net.URL;
import java.util.ResourceBundle;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;
import javafx.animation.Animation;
import javafx.animation.KeyFrame;
import javafx.animation.Timeline;
import javafx.application.Platform;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.chart.AreaChart;
import javafx.scene.chart.LineChart;
import javafx.scene.chart.XYChart;
import javafx.scene.control.*;
import javafx.scene.image.ImageView;
import javafx.scene.layout.*;
import javafx.scene.text.Text;
import javafx.util.Duration;

/**
 *
 * @author Kent Studer
 */
public class Controller {
    
    private double usagePercent = 0.0;
    private final double minAngle = 195.3;
    private final double maxAngle = 135.0;
    private final double tickPercentage = .3;
    private final double angleDeltaPerPercent = 3.0;
    private double peakArray[] = {0,0};
    private double sum = 0;
    private double average = 0;
    private int counter = 0;
    private Timeline timeline;
    private XYChart.Series<Number, Number> recordSeries = new XYChart.Series();
    private XYChart.Series<Number, Number> meanSeries = new XYChart.Series();
    private int chartCounter = 0;
    
    @FXML
    private Button startStopButton;
    @FXML
    private Button recordResetButton;
    @FXML
    private ImageView handImage;
    @FXML
    private Text mainPercentLabel;
    @FXML
    private Text peakPercent;
    @FXML
    private Text meanPercent;
    @FXML
    private LineChart lineChart;
    @FXML
    private AreaChart areaChart;
    
    @FXML
    private void handleStartStop(){
        if(!isRunning()){ //if the animation is not running, the button click will start the
                            // animation & our buttons will change
            setUpGauge();
            start();
            startStopButton.setText("Stop");
            startStopButton.setStyle("-fx-background-color:#ff0000");
            recordResetButton.setText("Record");
        }else{ //if we are running our the button click will stop the animation & our buttons
                  //will change to start and reset
            stop();
            startStopButton.setText("Start");
            startStopButton.setStyle("-fx-background-color:#00ff00");
            recordResetButton.setText("Reset");
        }
    }
    
    @FXML
    private void handleResetRecord(){
        if(!isRunning()){ //if function is not running the button click resets the data and UI
            reset();
        }else{ //it records data
            record();
        }
    }
    
    private void reset(){
        usagePercent = 0;
        handImage.setRotate(minAngle);
        mainPercentLabel.setText("0.00%");
        peakArray[0] = peakArray[1] = 0;
        peakPercent.setText("0.00%");
        meanPercent.setText("0.00%");
        counter = 0;
        sum = 0;
        average = 0;
        chartCounter = 0;
        recordSeries.getData().clear();
        lineChart.getData().clear();
        meanSeries.getData().clear();
        areaChart.getData().clear();
    }
    private void record(){
        chartCounter++;
        float percentRecord = (float) usagePercent;
        recordSeries.getData().add(new XYChart.Data(Integer.toString(chartCounter), percentRecord));
        lineChart.getData().add(recordSeries);
    }
    private void setUpGauge(){
        timeline = new Timeline(
        new KeyFrame(Duration.millis(1000 * tickPercentage), 
            e -> {
                update();
        }));
        timeline.setCycleCount(Animation.INDEFINITE);
    }
    private void update(){
        usagePercent = getCPUUsage() * 100;
        
        if(sum > 100000 || counter > 1000){ //so we dont endlessly store a larger and larger number
            sum = 0;
            counter = 0;
        }
        counter++; 
        sum += usagePercent;
        
        average = sum / counter;
        
        //Handles error exception and updates the digital display appropriately
        if(usagePercent <= 0 || Double.isNaN(usagePercent)){ //on Windows the CPUUsage function returns 0 or a negative number (Mac returns Nan)
            mainPercentLabel.setText("--.--%");
            handImage.setRotate(minAngle);
            return;
        }else{
            String formattedPercentMain = String.format("%2.2f", usagePercent);
            mainPercentLabel.setText(formattedPercentMain + "%");
            //moving hand based on percent
            double rotation = usagePercent * angleDeltaPerPercent;
            handImage.setRotate(minAngle + rotation);
        
            //Handling the max cpu load percent
            peakArray[0] = usagePercent;
            if(peakArray[0] > peakArray[1]){
                peakArray[1] = peakArray[0];
            }
        
            String formattedPercentPeak = String.format("%2.2f", peakArray[1]);
            peakPercent.setText(formattedPercentPeak + "%");  
            String formattedPercentMean = String.format("%2.2f", average);
            meanPercent.setText(formattedPercentMean + "%");     
       
            meanSeries.getData().add(new XYChart.Data(Integer.toString(counter-1), average));
            areaChart.getData().add(meanSeries);
        }
    }
    
    //Functions to handle our timeline and its stop/start functionality
    private void start(){
        timeline.play();
    }
    private void stop(){
        timeline.pause();
    }
    //determines if our gauge is running so we can use our buttons properly
    private boolean isRunning(){
        if(timeline != null){
            if(timeline.getStatus() == Animation.Status.RUNNING){
                return true;
            }
        }
        return false;
    }
    
    //returns a value in between 0.0 and 1.0 which corresponds to CPU Usage
    private double getCPUUsage(){ 
        OperatingSystemMXBean operatingSystemMXBean = ManagementFactory.getOperatingSystemMXBean();
        
        double value = 0;
        for(Method method : operatingSystemMXBean.getClass().getDeclaredMethods()) {
            method.setAccessible(true);
            if (method.getName().startsWith("getSystemCpuLoad") && Modifier.isPublic(method.getModifiers())) {
                try{
                    value = (double) method.invoke(operatingSystemMXBean);
                }catch (Exception e){
                    value = 0;
                }
                return value;
            }
        }
        return value;
    }
}
   
