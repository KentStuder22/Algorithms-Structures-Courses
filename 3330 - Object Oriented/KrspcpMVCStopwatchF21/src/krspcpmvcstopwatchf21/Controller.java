/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package krspcpmvcstopwatchf21;

import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.net.URL;
import java.util.ResourceBundle;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.chart.AreaChart;
import javafx.scene.chart.LineChart;
import javafx.scene.chart.XYChart;
import javafx.scene.control.Alert;
import javafx.scene.control.Button;
import javafx.scene.control.ButtonType;
import javafx.scene.control.Label;
import javafx.scene.control.TextField;
import javafx.scene.control.TextInputDialog;
import javafx.scene.image.ImageView;
import javafx.scene.text.Text;

/**
 *
 * @author kento
 */
public class Controller implements Initializable, PropertyChangeListener {
    
    // FXML Variable Declarations
    @FXML
    private ImageView handImage;
    @FXML
    private Button startStop;
    @FXML
    private Button recordReset;
    @FXML
    private Text middleTime;
    @FXML
    private Text timerText;
    @FXML
    private Text lapText;
    @FXML
    private Text avgLapText;
    @FXML
    private LineChart lineChart;
    @FXML
    private AreaChart areaChart;
    
    Alert timesUp;
    
    // Model Declarations
    AnalogModel analog;
    DigitalModel digital;
    
    private XYChart.Series<String, Number> recordSeries;
    private XYChart.Series<String, Number> avgSeries;
    
    private TextInputDialog intro;
    public int startTime;
    
    @Override
    public void initialize(URL url, ResourceBundle rb) {
        // intro interface for user
        intro = new TextInputDialog();
        intro.setHeaderText("Set up start time");
        intro.setContentText("Please set up start time (integer)");
        
        TextField userInput = intro.getEditor();
        final Button okButton = (Button) intro.getDialogPane().lookupButton(ButtonType.OK);
        okButton.setDisable(true);
        
        userInput.textProperty().addListener(new ChangeListener<String>() {
            @Override
            public void changed(ObservableValue<? extends String> observable, String oldValue, String newValue) {
                if(!allValid(newValue)) {
                    okButton.setDisable(true);
                }
                if(allValid(newValue)) {
                    okButton.setDisable(false);
                }
            }
        });
        intro.showAndWait();
        startTime = Integer.parseInt(userInput.getText());
        
        analog = new AnalogModel();
        digital = new DigitalModel(startTime);
        analog.setupTimer();
        digital.setUpTimer();
        
        handImage.setRotate(0);
        
        middleTime.setText("--:--:--.--");
        avgLapText.setText("--:--:--.--");
        
        startStop.setStyle("-fx-background-color:#00ff00");
        
        timesUp = new Alert(Alert.AlertType.WARNING);
        timesUp.setTitle("Time's Up!");
        timesUp.setContentText("No more records, time has expired");
        
        lineChart.setAnimated(false);
        areaChart.setAnimated(false);
        
        recordSeries = new XYChart.Series();
        avgSeries = new XYChart.Series();
        
        lineChart.getData().add(recordSeries);
        areaChart.getData().add(avgSeries);
        
        analog.addPCL(this);
        digital.addPCL(this);
    } 
    @Override
    public void propertyChange(PropertyChangeEvent pce) {
        if(pce.getPropertyName().equals("Analog")){
            handImage.setRotate(Double.parseDouble(pce.getNewValue().toString()));
        }else if(pce.getPropertyName().equals("Digital")){
            middleTime.setText(pce.getNewValue().toString());
        }else if(pce.getPropertyName().equals("Timer")){
            timerText.setText(pce.getNewValue().toString());
        }else if(pce.getPropertyName().equals("Lap")){
            lapText.setText(pce.getNewValue().toString());
        }else if(pce.getPropertyName().equals("Average")){
            avgLapText.setText(pce.getNewValue().toString());
        }else if(pce.getPropertyName().equals("LineChart")){
            recordSeries.getData().add(new XYChart.Data(Integer.toString(digital.recordCount), pce.getNewValue()));
        }else if(pce.getPropertyName().equals("AreaChart")){
            avgSeries.getData().add(new XYChart.Data(Integer.toString(digital.recordCount), pce.getNewValue()));
        }else if(pce.getPropertyName().equals("LineChartClear")){
            lineChart.getData().clear();
        }else if(pce.getPropertyName().equals("AreaChartClear")){
            areaChart.getData().clear();
        }else if(pce.getPropertyName().equals("Alert")){
            timesUp.showAndWait();
        }
    }
        
    @FXML
    public void handleStartStop(ActionEvent event){ // handles button press of Start/Stop
        if(!(analog.isRunning())){
            analog.start();
            digital.start();
            
            startStop.setText("Stop");
            startStop.setStyle("-fx-background-color:#ff0000");
            
            recordReset.setText("Record");
        }else{
            analog.stop();
            digital.stop();
            
            startStop.setText("Start");
            startStop.setStyle("-fx-background-color:#00ff00");
            
            recordReset.setText("Reset");
        }
    }
    @FXML
    public void handleRecordReset(ActionEvent event){ // handles button press of Record/Reset
        if(!(analog.isRunning())){
            analog.reset();
            digital.reset();
            recordSeries.getData().clear();
            avgSeries.getData().clear();
        }else{
            digital.record();
        }
    }
    private boolean allValid(String input) {
        int start = 0;
        try {
            start = Integer.parseInt(input);
        } catch(NumberFormatException e){
            intro.setContentText("Please enter only digits");
            return false;
        }
        if(input.equals("")) {
            intro.setContentText("Please enter a value");
            return false;
        } else if(start < 0) {
            intro.setContentText("Please enter valid clock value");
            return false;
        }
        return true;
    }
}
