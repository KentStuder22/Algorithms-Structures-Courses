/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package mvccpumonitorfxml;

import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.net.URL;
import java.util.ResourceBundle;
import javafx.animation.Animation;
import javafx.animation.KeyFrame;
import javafx.animation.Timeline;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.Button;
import javafx.scene.image.ImageView;
import javafx.scene.text.Text;
import javafx.util.Duration;

/**
 * FXML Controller class
 * @author Professor Wergeles <Professor Wergeles at cs3330@missouri.edu>
 */


// step 2: controller must implement property change listener interface 
public class Controller implements Initializable, PropertyChangeListener {
    
    @FXML
    private Text cpuLabel;
    @FXML
    private ImageView hand;
    @FXML
    private Button startStopButton;

    Model model;
//          digitalModel; 
    
    /**
     * Initializes the controller class.
     */
    @Override
    public void initialize(URL url, ResourceBundle rb) {
        model = new Model(); 
        model.setupMonitor(); 
        hand.setRotate(0);
        cpuLabel.setText("00.00%");
        
        // step 3: connect the model to the property change listener
        model.addPropertyChangeListener(this);
    }

    @FXML
    public void startStopMonitor(ActionEvent event) {
        if ( !(model.isRunnning())) {
            model.start();
            startStopButton.setText("Stop");
        } else {
            model.stop();
            startStopButton.setText("Start");
        }
    }

    @Override
    public void propertyChange(PropertyChangeEvent evt) {
        
        // step 5: Controller updates the display 
        
//        System.out.println(evt.getNewValue().getClass());
        
        
        if(evt.getPropertyName().equals("Analog")){
            hand.setRotate(Double.parseDouble(evt.getNewValue().toString()));
        }
        else if(evt.getPropertyName().equals("Digital")){
            cpuLabel.setText(evt.getNewValue().toString() + "%");
        }
//        else if lap
//        else if record
//        else if timer
//        else if etc.
   
    }
    
   
}
