/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package homeworkhub.startup;

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
import java.text.ParseException;
import java.util.logging.Level;
import java.util.logging.Logger;
import javafx.application.Platform;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Alert;
import javafx.scene.control.ButtonType;
import javafx.scene.control.Dialog;
import javafx.scene.control.DialogPane;
import javafx.scene.control.TextField;
import javafx.scene.control.TextInputDialog;
import javafx.scene.text.Text;
import javafx.stage.Stage;
import javafx.stage.WindowEvent;

/**
 *
 * @author kento
 */
public class startupController implements Initializable {
    
    @FXML
    private Button s1Button;
    
    @FXML
    private Button s2Button;
    
    @FXML
    private Button s3Button;
    
    @FXML
    private Button exitButton;
    
    @FXML
    private Button deleteButton;
    
    private final String dirName = "./src/homeworkhub/files/";
    private final String userListDir = "./src/homeworkhub/files/users.txt";
    private String user;
    
    private TextInputDialog newUser;
    private TextInputDialog deleteUser;
    private TextField userInput;
    private TextField delInput;
    
    private Alert outOfUsers;
    private Alert alreadyExists;
    private Alert validUser;
    
    private Stage stage;
    public Scene scene1;
    public Scene scene2;
    public mainController mainController;
    private Text about;
    private Dialog aboutProject;
    private DialogPane DP;
    
    
    ArrayList<String> users = new ArrayList<>(3);
    PrintWriter out;
    @FXML
    public void aboutProj(){
        aboutProject = new Dialog();
        DP = aboutProject.getDialogPane();
        DP.getButtonTypes().addAll(ButtonType.CLOSE);
        about = new Text("This Project is a homework organizer inspired by Canvas's lack of organizability\n"
                + " while certainly it is not 100% fully implemented the Homework Hub allows for the creation\n"
                + " of 3 user profiles, homework assignments with names, date, priorities, etc. On top of this\n"
                + " there is some user customization with the ability to store your first and last name, along with your\n"
                + " hometown and state! - Kent Studer (krspcp)");
        DP.setContent(about);
        aboutProject.showAndWait();
    }
    @FXML
    public void handleS1 (ActionEvent event) throws IOException, FileNotFoundException, ParseException {
       if(!s1Button.getText().equals("Student 1")) {
           user = s1Button.getText();
           goToMain();
       }else{
           handleButton(1);
       }
    }
    @FXML
    public void handleS2 (ActionEvent event) throws IOException, FileNotFoundException, ParseException {
       if(!s2Button.getText().equals("Student 2")) {
           user = s2Button.getText();
           goToMain();
       }else{
           handleButton(2);
       }
    }
    @FXML
    public void handleS3 (ActionEvent event) throws IOException, FileNotFoundException, ParseException {
       if(!s3Button.getText().equals("Student 3")) {
           user = s3Button.getText();
           goToMain();
       }else{
           handleButton(3);
       }
    }
    @FXML
    public void handleExit (ActionEvent event) throws IOException {
       //First we clear the file, to ensure we are left with accurate user data after insertions and deletions
       FileWriter clearer = new FileWriter(userListDir);
       clearer.write("");
       clearer.close();
       
       FileWriter update = new FileWriter(userListDir);
       int i = 0;
       for(String str: users) {
           update.write(str+"\n");
       }
       update.close();
       out.close();
       System.exit(0);
    }
    @FXML
    public void deleteStudent (ActionEvent event) throws IOException {
        deleteUser.showAndWait();
        String delUser = delInput.getText();
        delInput.setText("");
        if(users.contains(delUser)) { //the user exists
            
            //First lets take care of removing the username from our users.txt file
            //Creat temp file that will be a copy of the users.txt minus the deleted user
            File temp = new File(dirName+"tempFile.txt");
            File tempUser = new File(userListDir);
            
            BufferedReader reader = new BufferedReader(new FileReader(tempUser));
            BufferedWriter writer = new BufferedWriter(new FileWriter(temp, true));            
            String currentLine;
            while((currentLine = reader.readLine()) != null) { //copying every line in users.txt
                String trim = currentLine.trim();
                if(trim.equals(delUser)) { //if we found our user to be deleted we skip
                }else{
                    writer.write(currentLine+"\n"); //copy each line
                }
            }
            writer.close();
            reader.close();
            boolean success = temp.renameTo(tempUser); //rename file 
            
            int index = users.indexOf(delUser); //snag index for button change
            users.remove(delUser);
            //users.add(index, "0");
            
            switch (index) { //case for which button to change to default
                case 0:
                    s1Button.setText("Student 1");
                    break;
                case 1:
                    s2Button.setText("Student 2");
                    break;
                case 2:
                    s3Button.setText("Student 3");
                    break;
                default:
                    break;
            }
            
            File delFile = new File(dirName+delUser+".txt"); //delete the users file
            delFile.delete();
        }else{
            validUser.showAndWait();
        }
    }
    public void start(Stage stage) {
        this.stage = stage;
        scene1 = stage.getScene();
    }
        
    private void goToMain() throws IOException, FileNotFoundException, ParseException {
        try {
            if(scene2 == null){
                FXMLLoader loader = new FXMLLoader(getClass().getResource("mainView.fxml"));
                Parent page2Root = (Parent) loader.load();
                mainController = loader.getController();
                mainController.user = user;
                mainController.scene1 = scene1; 
                mainController.startup = this; 
                scene2 = new Scene(page2Root);
            }
        } catch (Exception ex){
        }
        mainController.user = user;
        mainController.scene1 = scene1;
        stage.setScene(scene2); 
        mainController.start(stage);
    }
    private void handleButton (int i) throws IOException, FileNotFoundException, ParseException {
       //creating user slots
       //we have an open slot for a user
       newUser.showAndWait();
       user = userInput.getText(); //getting the string entered by the user
       userInput.setText("");

       if(users.contains(user)) { //if we have a profile with the new name
           alreadyExists.showAndWait(); 
       }else{ //if we have an open slot AND a valid username

           //Lets add to our collection and write the name to the file for loading the profiles back in
           users.add(user);

           switch (i) {
                   case 1:
                       s1Button.setText(user);
                       break;
                   case 2:
                       s2Button.setText(user);
                       break;
                   case 3:
                       s3Button.setText(user);
                       break;
                   default:
                       break;
           }

           String fileName = user+".txt";
           File file = new File(dirName+fileName);

           boolean exists = file.exists();
           if(exists){
           }else{
               try{
                   file.createNewFile();
                   FileWriter writer = new FileWriter(userListDir, true);
                   writer.write(user+"\n");
                   writer.close();
                   goToMain();
               } catch (IOException ex) {
                   Logger.getLogger(startupController.class.getName()).log(Level.SEVERE, null, ex);
               }
           }
       }
    }
    @Override
    public void initialize(URL url, ResourceBundle rb) {
        users.clear();
        int i = 0;
        
        try {
            File userFile = new File(userListDir);
            Scanner userScan = new Scanner(userFile);
            out = new PrintWriter(new FileWriter(userListDir, true));
            
            while(userScan.hasNext()) {
                users.add(userScan.next());
                i++;
            }
        } catch (FileNotFoundException ex) {
            Logger.getLogger(startupController.class.getName()).log(Level.SEVERE, null, ex);
        } catch (IOException ex) {
            Logger.getLogger(startupController.class.getName()).log(Level.SEVERE, null, ex);
        }
        
        switch (i) {
            case 0:
                s1Button.setText("Student 1");
                s2Button.setText("Student 2");
                s3Button.setText("Student 3");
                break;
            case 1:
                if("0".equals(users.get(0))){
                    s1Button.setText("Student 1");
                }else{
                    s1Button.setText(users.get(0));
                }
                s2Button.setText("Student 2");
                s3Button.setText("Student 3");
                break;
            case 2:
                if("0".equals(users.get(0))){
                    s1Button.setText("Student 1");
                }else{
                    s1Button.setText(users.get(0));
                }
                if("0".equals(users.get(1))){
                    s2Button.setText("Student 2");
                }else{
                    s2Button.setText(users.get(1));
                }
                s3Button.setText("Student 3");
                break;
            case 3:
                if("0".equals(users.get(0))){
                    s1Button.setText("Student 1");
                }else{
                    s1Button.setText(users.get(0));
                }
                if("0".equals(users.get(1))){
                 s2Button.setText("Student 2");
                }else{
                    s2Button.setText(users.get(1));
                }
                if("0".equals(users.get(2))){
                    s3Button.setText("Student 3");
                }else{
                    s3Button.setText(users.get(2));
                }
                break;
            default:
                break;
        }
            
        
        newUser = new TextInputDialog();
        newUser.setHeaderText("New User");
        newUser.setContentText("Please enter your name");
        
        deleteUser = new TextInputDialog();
        deleteUser.setTitle("Delete User");
        deleteUser.setContentText("Which user would you like to delete?");
        
        userInput = new TextField();
        userInput = newUser.getEditor();
        final Button okButton = (Button) newUser.getDialogPane().lookupButton(ButtonType.OK);
        okButton.setDisable(true);
        
        userInput.textProperty().addListener(new ChangeListener<String>() {
            @Override
            public void changed(ObservableValue<? extends String> observable, String oldValue, String newValue) {
                if(newValue.isEmpty() || users.contains(newValue)) {
                    okButton.setDisable(true);
                }
                if(!newValue.isEmpty()) {
                    okButton.setDisable(false);
                }
            }
        });
        
        delInput = new TextField();
        delInput = deleteUser.getEditor();
        final Button delOK = (Button) deleteUser.getDialogPane().lookupButton(ButtonType.OK);
        delOK.setDisable(true);
        delInput.textProperty().addListener(new ChangeListener<String>() {
            @Override
            public void changed(ObservableValue<? extends String> observable, String oldValue, String newValue) {
                if(newValue.isEmpty() || users.contains(newValue)) {
                    delOK.setDisable(true);
                }
                if(!newValue.isEmpty()) {
                    delOK.setDisable(false);
                }
            }
        });
        
        outOfUsers = new Alert(Alert.AlertType.WARNING);
        outOfUsers.setTitle("Out of User Spaces!");
        outOfUsers.setContentText("Please use existing user or delete user for more space");
        
        alreadyExists = new Alert(Alert.AlertType.WARNING);
        alreadyExists.setTitle("User Already Exists");
        alreadyExists.setContentText("Please enter a valid username");
        
        validUser = new Alert(Alert.AlertType.WARNING);
        validUser.setTitle("Invalid username");
        validUser.setContentText("Please enter a valid, existing username");
    }
}
//"C:\Users\kento\OneDrive\Desktop\GitHub Repo\OO-Java\HomeworkHub\src\homeworkhub\files"