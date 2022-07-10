/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package serializationjsonexample;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.net.URL;
import java.util.Optional;
import java.util.ResourceBundle;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.Alert;
import javafx.scene.control.ButtonType;
import javafx.scene.control.ComboBox;
import javafx.scene.control.Label;
import javafx.scene.control.TextArea;
import javafx.scene.control.TextField;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.Priority;
import javafx.stage.FileChooser;
import javafx.stage.Stage;
import javafx.stage.WindowEvent;

/**
 * FXML Controller class
 *
 * @author Professor Wergeles
 * 
 * @references
 *      1)  http://www.tutorialspoint.com/java/java_serialization.htm
 * 
 */
public class UserInterfaceController implements Initializable {

    private Stage stage;
    
    @FXML
    private TextField firstNameTextField;
    @FXML
    private TextField lastNameTextField;
    @FXML
    private TextField ageTextField;
    @FXML
    private ComboBox genderComboBox;
    
    Person person;
    
    @Override
    public void initialize(URL url, ResourceBundle rb) {
        person = new Person();
    }
    
    public void ready(Stage stage) {
        this.stage = stage;
        
        genderComboBox.getItems().add("female");
        genderComboBox.getItems().add("male");
        
        stage.setOnCloseRequest(new EventHandler<WindowEvent>() {
            @Override
            public void handle(WindowEvent we) {
                Person formPerson = createPersonFromFormData();
                if (person != null) {
                    if (!person.equals(formPerson)) {
                        if (!confirmContinueOnUnsavedData()) {
                            we.consume();  // this stops the closing from happening
                        }
                    }
                }
            }
        });
    }
    
    // If you want to shows only files with a specific extension
    // FileChooser can be setup with filters
    // http://docs.oracle.com/javafx/2/ui_controls/file-chooser.htm
    
    @FXML
    public void handleOpen(ActionEvent event) {
        Person formPerson = createPersonFromFormData();
        if (person != null) {
            if (!person.equals(formPerson)) {
                if (!confirmContinueOnUnsavedData()) {
                    return;
                }
            }
        }       
        
        FileChooser fileChooser = new FileChooser();
        File file = fileChooser.showOpenDialog(stage);
        if (file != null) {
            try
            {
                FileReader fileReader = new FileReader(file.getPath());
                BufferedReader bufferedReader = new BufferedReader(fileReader);
 
                String json = "";
                String line = null;
                while((line = bufferedReader.readLine()) != null) {
                    json += line;
                }
                bufferedReader.close();
                fileReader.close();
                
                person.initFromJsonString(json);
               
                fillFormFromPerson(person);
            }catch(IOException ioex)
            {
               String message = "Exception occurred while opening " + file.getPath();
               displayExceptionAlert(message, ioex);
            }
        }
    }
    
    @FXML
    public void handleSave(ActionEvent event) {
        person = createValidatedPersonFromFormData();
        if (person == null) {
            return;
        }
        
        FileChooser fileChooser = new FileChooser();
        File file = fileChooser.showSaveDialog(stage);
        if (file != null) {
            try
            {
               String jsonString = person.toJsonString();
               PrintWriter out = new PrintWriter(file.getPath());
               out.print(jsonString);
               out.close();
            }catch(IOException ioex)
            {
                String message = "Exception occurred while saving to " + file.getPath();
                displayExceptionAlert(message, ioex);
            } 
        }        
    }
    
    @FXML
    public void handleClose(ActionEvent event) {
        Person formPerson = createPersonFromFormData();
        if (person != null) {
            if (!person.equals(formPerson)) {
                if (!confirmContinueOnUnsavedData()) {
                    return;
                }
            }
        }
        
        stage.close();
    }
    
    @FXML
    public void handleAbout(ActionEvent event) {
        Alert alert = new Alert(Alert.AlertType.INFORMATION);
        alert.setTitle("About");
        alert.setHeaderText("Serialization Example");
        alert.setContentText("This application was developed by Wergeles for CS3330 at the University of Missouri.");
        
        TextArea textArea = new TextArea("This example illustrates how to serialize and deserialze objects.");
        textArea.setEditable(false);
        textArea.setWrapText(true);
        textArea.setMaxWidth(Double.MAX_VALUE);
        textArea.setMaxHeight(Double.MAX_VALUE);

        GridPane expContent = new GridPane();
        expContent.setMaxWidth(Double.MAX_VALUE);
        expContent.add(textArea, 0, 0);
        alert.getDialogPane().setExpandableContent(expContent);
        
        alert.showAndWait();
    }
    
    private Person createValidatedPersonFromFormData() {
        Person p = new Person();
        
        p.setFirstName(firstNameTextField.getText());
        p.setLastName(lastNameTextField.getText());
        
        String ageStr = ageTextField.getText();
        Integer age;
        try {
            age = Integer.parseInt(ageStr);
        } catch (Exception ex) {
            displayErrorAlert("The age value must be an integer.");
            return null;
        }
        p.setAge(age);
        
        if (genderComboBox.getValue() != null &&  !genderComboBox.getValue().toString().isEmpty()) {
            p.setGender(genderComboBox.getValue().toString());
        } else {
            displayErrorAlert("A gender must be selected.");
            return null;
        }

        return p;
    }
    
    private Person createPersonFromFormData() {
        Person p = new Person();
        
        p.setFirstName(firstNameTextField.getText());
        p.setLastName(lastNameTextField.getText());
        
        String ageStr = ageTextField.getText();
        Integer age;
        try {
            age = Integer.parseInt(ageStr);
        } catch (Exception ex) {
            age = null;
        }
        p.setAge(age);
        
        if (genderComboBox.getValue() != null &&  !genderComboBox.getValue().toString().isEmpty()) {
            p.setGender(genderComboBox.getValue().toString());
        } else {
            p.setGender(null);
        }

        return p;
    }
    
    private void fillFormFromPerson(Person person) {  
        if (person.getFirstName() != null) {
            firstNameTextField.setText(person.getFirstName());
        } else {
            firstNameTextField.setText("");
        }
        
        if (person.getLastName() != null) {
            lastNameTextField.setText(person.getLastName());
        } else {
            lastNameTextField.setText("");
        }
        
        if (person.getAge() != null) {
            ageTextField.setText(Integer.toString(person.getAge()));
        } else {
            ageTextField.setText("");
        }
        
        if (person.getGender() != null) {
            genderComboBox.setValue(person.getGender());
        } else {
            genderComboBox.getSelectionModel().clearSelection();
        }  
    }
    
    private void displayErrorAlert(String message) {
        Alert alert = new Alert(Alert.AlertType.ERROR);
        alert.setTitle("Error");
        //alert.setHeaderText("Error!");
        alert.setContentText(message);
        alert.showAndWait();
    }
    
    private void displayExceptionAlert(String message, Exception ex) {
        Alert alert = new Alert(Alert.AlertType.ERROR);
        alert.setTitle("Exception Dialog");
        alert.setHeaderText("Exception!");
        alert.setContentText(message);

        // Create expandable Exception.
        StringWriter sw = new StringWriter();
        PrintWriter pw = new PrintWriter(sw);
        ex.printStackTrace(pw);
        String exceptionText = sw.toString();

        Label label = new Label("The exception stacktrace was:");

        TextArea textArea = new TextArea(exceptionText);
        textArea.setEditable(false);
        textArea.setWrapText(true);

        textArea.setMaxWidth(Double.MAX_VALUE);
        textArea.setMaxHeight(Double.MAX_VALUE);
        GridPane.setVgrow(textArea, Priority.ALWAYS);
        GridPane.setHgrow(textArea, Priority.ALWAYS);

        GridPane expContent = new GridPane();
        expContent.setMaxWidth(Double.MAX_VALUE);
        expContent.add(label, 0, 0);
        expContent.add(textArea, 0, 1);

        // Set expandable Exception into the dialog pane.
        alert.getDialogPane().setExpandableContent(expContent);

        alert.showAndWait();
    }
    
    private boolean confirmContinueOnUnsavedData() {
        Alert alert = new Alert(Alert.AlertType.CONFIRMATION);
        alert.setTitle("Unsaved Data");
        alert.setHeaderText("Changes have not been saved.");
        alert.setContentText("Are you sure you want to continue?");

        Optional<ButtonType> result = alert.showAndWait();
        if (result.get() == ButtonType.OK){
            // ... user chose OK
            return true;
        } else {
            // ... user chose CANCEL or closed the dialog
            return false;
        }
    }  
    
}
