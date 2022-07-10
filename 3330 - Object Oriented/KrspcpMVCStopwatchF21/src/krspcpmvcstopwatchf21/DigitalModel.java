/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package krspcpmvcstopwatchf21;

import javafx.animation.Animation;
import javafx.animation.KeyFrame;
import javafx.animation.Timeline;
import javafx.event.ActionEvent;
import javafx.scene.chart.XYChart;
import javafx.scene.control.Alert;
import javafx.util.Duration;

/**
 *
 * @author kento
 */
public class DigitalModel extends AbstractModel {
    private Timeline timeline;
    private KeyFrame keyFrame;
    
    private int startTime;
    private double tickTime;
    private int digitalInc;
    private int digitalDec;
    
    private String middleString;
    private String timerString;
    private String lapString;
    private String avgString;
    
    private boolean isOut = false;
    
    private double currentTime;
    private double pastTime;
    public int recordCount;
    private double sumLap;
    
    DigitalModel(int start){
        startTime = start;
        tickTime = 0.01;
        digitalInc = 0;
        digitalDec = start * 100;
        timerString = getTimerString(digitalDec);
        currentTime = 0;
        pastTime = 0;
        recordCount = 0;
        sumLap = 0;
    }
    
    public void setUpTimer(){
        keyFrame = new KeyFrame(Duration.millis(1000 * tickTime), (ActionEvent actionEvent) -> {
            updateDigital();
        });
        timeline = new Timeline(keyFrame);
        timeline.setCycleCount(Animation.INDEFINITE);
    }
    
    public void updateDigital(){
        digitalInc++;
        
        String middleTemp = middleString; //store old string value in temp
        middleString = getTimerString(digitalInc);
        firePC("Digital", middleTemp, middleString);
        
        if(digitalDec > 0){
            String startTemp = timerString;
            timerString = getSubTimerString(digitalInc + 99); //+99 because our incTimer starts on top of the start (i.e, 10 = 10.99)
            firePC("Timer", startTemp, timerString);
        }else{
            firePC("Timer", null, "Time's Up!");
            isOut = true;
        }      
    }
    public String getTimerString(int digitalInc){
        String hours = String.format("%02d", (Math.floorDiv(digitalInc, (6000*60))));
        String minutes = String.format("%02d", (Math.floorDiv(digitalInc, 6000)));
        String seconds = String.format("%02d", (Math.floorDiv(digitalInc, 100) % 60));
        String milliseconds = String.format("%02d", digitalInc % 100);
        return hours + ":" + minutes + ":" + seconds + "." + milliseconds;
    }
    public String getSubTimerString(int digitalInc){ //need separate function for subtracting from the start
        int minutes = Math.floorDiv(digitalInc, 6000);
        int seconds = Math.floorDiv(digitalInc, 100) % 60;
        int milliseconds = digitalInc % 100;
        
        int subSeconds = startTime - seconds;
        int subMilliseconds = 99 - milliseconds;
        digitalDec--;
        
        String secondsString = Integer.toString(subSeconds);
        String millisecondsString = Integer.toString(subMilliseconds);
        
        return secondsString + "." + millisecondsString;
    }
    public void start(){
        timeline.play();
    }
    public void stop(){
        timeline.pause();
    }
    public void reset(){
        digitalInc = 0;
        digitalDec = startTime * 100;
        isOut = false;
        recordCount = 0;
        pastTime = 0;
        currentTime = 0;
        sumLap = 0;
        
        firePC("Digital", null, "--:--:--.--");
        firePC("Timer", null, "--:--");
        firePC("Lap", null, "Lap -: --:--:--.--");
        firePC("Average", null, "--:--:--.--");
    }
    public void record(){
        if(isOut){
            firePC("Alert", false, isOut);
        }else{
            recordCount++;
            
            pastTime = currentTime;
            currentTime = digitalInc;
            double lapTime = currentTime - pastTime;
            sumLap += lapTime;
            double avgLapTime = sumLap / recordCount;
           
            firePC("LineChart", null, lapTime / 100);
            firePC("AreaChart", null, avgLapTime / 100);
            
            lapString = getTimerString((int) lapTime);
            avgString = getTimerString((int) avgLapTime);
            firePC("Lap", null, ("Lap:" + recordCount + " " + lapString));
            firePC("Average", null, avgString);
        }
    }
    
}
