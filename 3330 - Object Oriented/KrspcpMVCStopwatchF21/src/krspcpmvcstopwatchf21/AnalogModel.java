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
import javafx.util.Duration;

/**
 *
 * @author kento
 */
public class AnalogModel extends AbstractModel{
    
    private Timeline timeline;
    private KeyFrame keyFrame;
    
    private double secondsElapsed;
    private final double resolution;
    private final double angleDelta;
    private double rotation;
    
    // Constructor the gives our analog object the proper starting values
    public AnalogModel(){
        secondsElapsed = 0.0;
        resolution = 0.01;
        angleDelta = 6.0;
    }
    
    public void setupTimer(){
        keyFrame = new KeyFrame(Duration.millis(1000 * resolution), (ActionEvent actionEvent) -> {
            updateAnalog();
        });
        timeline = new Timeline(keyFrame);
        timeline.setCycleCount(Animation.INDEFINITE);
    }
    public void updateAnalog(){
        double oldRotation = rotation;
        secondsElapsed += resolution;
        rotation = secondsElapsed * angleDelta;
        firePC("Analog", oldRotation, rotation);
    }
    public boolean isRunning(){
        if(timeline != null){
            if(timeline.getStatus() == Animation.Status.RUNNING){
                return true;
            }
        }
        return false;
    }
 
    public void start(){
        timeline.play();
    }
    public void stop(){
        timeline.pause();
    }
    public void reset(){
        timeline.stop();
        secondsElapsed = 0.0;
        rotation = 0.0;
        firePC("Analog", null, rotation);
    }
    //Getters and setters for controller
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
    public double getRotation(){
        return rotation;
    }
    public double getResolution(){
        return resolution;
    }
}
