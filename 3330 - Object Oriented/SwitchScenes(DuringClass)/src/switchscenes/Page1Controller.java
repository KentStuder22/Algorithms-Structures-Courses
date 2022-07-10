/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package switchscenes;

import java.io.IOException;
import java.net.URL;
import java.util.Optional;
import java.util.ResourceBundle;
import java.util.logging.Level;
import java.util.logging.Logger;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.fxml.Initializable;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Label;
import javafx.scene.control.TextInputDialog;
import javafx.stage.Stage;

/**
 * FXML Controller class
 *
 * @author Professor Wergeles
 */
public class Page1Controller implements Initializable {
    private Stage stage;
    private Scene page1Scene;
    private Scene page2Scene;
    private Page2Controller page2Controller;
    
    @FXML
    private Label label;
    
    /**
     * Initializes the controller class.
     */
    @Override
    public void initialize(URL url, ResourceBundle rb) {
        // TODO
    }
    
    public void start(Stage stage) {
        System.out.println("Page 1 has started");
        this.stage = stage; 
           
    }
    
    @FXML
    private void goToPage2(ActionEvent event) {
        System.out.println("Going to page 2.");
        String result = getDataToTransfer();
        
         if (page2Scene == null) {
            try {
                System.out.println("page2scene is null");
                
                FXMLLoader loader = new FXMLLoader(getClass().getResource("Page2.fxml"));
                Parent page2Root = loader.load(); 
                page2Controller = loader.getController();
                
                
            } catch (IOException ex) {
                Logger.getLogger(Page1Controller.class.getName()).log(Level.SEVERE, null, ex);
            }
        }

    }
    
    public void doThisThing(String info) {
        label.setText(info);
    }
    
    private String getDataToTransfer(){
        TextInputDialog dialog = new TextInputDialog("Enter something");
        dialog.setTitle("Text Input Dialog");
        dialog.setHeaderText("Look, a Text Input Dialog");
        dialog.setContentText("Please enter the data to send:");

        // Traditional way to get the response value.
        Optional<String> result = dialog.showAndWait();
        if (result.isPresent()){
            System.out.println("Your name: " + result.get());
            return result.get(); 
        }
     
        return null; 
    }   
}