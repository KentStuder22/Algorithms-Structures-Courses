/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package homeworkhub.startup;

import com.google.common.collect.Iterators;
import java.util.Date;
import homeworkhub.startup.startupController;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.net.URL;
import java.util.*;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import java.io.File;
import java.io.FileWriter;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.PrintWriter;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.text.DateFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.time.Instant;
import java.time.LocalDate;
import java.time.ZoneId;
import static java.time.temporal.TemporalQueries.localDate;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.regex.Pattern;
import java.util.stream.Stream;
import javafx.application.Platform;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.event.EventType;
import javafx.scene.Scene;
import javafx.scene.control.Alert;
import javafx.scene.control.ButtonType;
import javafx.scene.control.DatePicker;
import javafx.scene.control.ListView;
import javafx.scene.control.ScrollPane;
import javafx.scene.control.TextArea;
import javafx.scene.control.TextField;
import javafx.scene.control.TextInputDialog;
import javafx.stage.Stage;
import javafx.stage.WindowEvent;

/**
 *
 * @author kentstuder
 */
public class mainController implements Initializable {

    public Stage stage;
    public Scene scene1;
    public startupController startup;
    public String user = "";
    private String filePath;
    File userFile;
    ArrayList<Assignment>assignments = new ArrayList<>();
    
    @FXML
    Label nameLabel;
    @FXML
    TextField assignmentField;
    @FXML
    TextArea desField;
    @FXML
    DatePicker datePicker;
    @FXML
    Button switchButton;
    @FXML
    Button createButton;
    @FXML
    ListView listView;
    
    private TextInputDialog intro;
    public Alert noInfo;
    @FXML
    public void switchBack() {
        listView.getItems().clear();
        stage.setScene(scene1);
    }

    @FXML
    public void create() throws ParseException, IOException {
        if(assignmentField.getText().equals("") || desField.getText().equals("") || datePicker.getValue() == null) {
            noInfo.showAndWait();
        }else{
            Assignment newHW = new Assignment();
            newHW.setName(assignmentField.getText());
            newHW.setDescription(desField.getText());
            LocalDate ld = datePicker.getValue();
            ZoneId defaultZoneId = ZoneId.systemDefault();
            Date date = Date.from(ld.atStartOfDay(defaultZoneId).toInstant());
            DateFormat format = new SimpleDateFormat("MM-dd-yyyy");
            String finalday = format.format(date);
            newHW.setDueDate(finalday);
            
            intro = new TextInputDialog();
            intro.setHeaderText("Set Priority");
            intro.setContentText("1 being lowest 5 being highest");
        
            TextField userInput = intro.getEditor();
            final Button okButton = (Button) intro.getDialogPane().lookupButton(ButtonType.OK);
            okButton.setDisable(true);
        
            userInput.textProperty().addListener(new ChangeListener<String>() {
                @Override
                public void changed(ObservableValue<? extends String> observable, String oldValue, String newValue) {
                    if(Integer.valueOf(newValue) < 1 || Integer.valueOf(newValue) > 5) {
                        okButton.setDisable(true);
                    }
                    else {
                        okButton.setDisable(false);
                    }
                }
            });
            intro.showAndWait();
            assignmentField.setText("");
            desField.setText("");
            
            newHW.setPriority(Integer.parseInt(userInput.getText()));
            createNode(newHW);
            writeDataOut(newHW);
        }
    }
    
    public void displayInfo(Assignment homework) {
        Alert info = new Alert(Alert.AlertType.INFORMATION);
        info.setTitle("Assignment");
        info.setHeaderText(homework.getName());
        SimpleDateFormat day = new SimpleDateFormat("MM-dd-yyyy");
        String finalday = day.format(homework.dueDate);
        info.setContentText(homework.getDescription()+"\n"+homework.getPriority()+"\n"+finalday);
        info.showAndWait();
    }
    public void createNode(Assignment homework) {
        Button hwLabel = new Button();
        hwLabel.setStyle("-fx-background-color: #DEB887");
        hwLabel.setText(homework.getName());
        hwLabel.setOnAction((ActionEvent event) -> {
            displayInfo(homework);
        });
        
        listView.getItems().add(hwLabel);
    }
    public void readDataInto() throws FileNotFoundException, IOException, ParseException{
        
        if(userFile.length() == 0){
        }else{
            try (Stream<String> stream = Files.lines(Paths.get(filePath))) {
                Iterator<List<String>> iterator = Iterators.partition(stream.iterator(), 4);
                do{
                    List<String> temp = iterator.next();
                    Assignment hw = new Assignment();
                    hw.setName(temp.get(0));
                    hw.setDescription(temp.get(1));
                    hw.setPriority(Integer.parseInt(temp.get(2)));
                    hw.setDueDate(temp.get(3));
                    createNode(hw);
                }
                while(iterator.hasNext());
            }catch (IOException e){
            
            }
        }
    }
    public void writeDataOut(Assignment homework) throws IOException {
        FileWriter obj = new FileWriter(filePath, true);
        obj.write(homework.getName()+"\n");
        obj.write(homework.getDescription()+"\n");
        obj.write(String.valueOf(homework.getPriority())+"\n");
        DateFormat format = new SimpleDateFormat("MM-dd-yyyy");
        String finalday = format.format(homework.getDate());
        obj.write(finalday+"\n");
        obj.close();
    }
    @Override
    public void initialize(URL location, ResourceBundle resources) {
        
    }
    public void start(Stage stage) throws IOException, FileNotFoundException, ParseException {
        noInfo = new Alert(Alert.AlertType.WARNING);
        noInfo.setHeaderText("Invalid Information");
        noInfo.setContentText("Please enter an assignment name and description");
        
        nameLabel.setText("NAME: "+user);
        filePath = "./src/homeworkhub/files/"+user+".txt";
        userFile = new File(filePath);
        readDataInto();
        this.stage = stage;
    }
}

