/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package krspcpgpacalculatorf21;

import javafx.application.Application;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.TextArea;
import javafx.scene.control.TextField;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.StackPane;
import javafx.scene.layout.VBox;
import javafx.scene.text.Font;
import javafx.scene.text.FontWeight;
import javafx.scene.text.Text;
import javafx.stage.Stage;
import java.lang.Math;
import javafx.scene.control.Alert;
import javafx.scene.control.Alert.AlertType;

/**
 *
 * @author kent studer krspcp
 * @references:
 *      1) w3schools.com/java/ref_string_equals.asp
 *      2) https://docs.oracle.com/javase/tutorial/uiswing/components/button.html
 *      3) Lecture 8
 */
public class KrspcpGPACalculatorF21 extends Application {
    
    private final String title = "GPA Calculator";
    private final String course1Label = "Course 1: ";
    private final String course2Label = "Course 2: ";
    private final String course3Label = "Course 3: ";
    private final String course4Label = "Course 4: ";
    private final String infoLabel = "Information: ";
    private final String fontStyle = "Times New Roman";
    private final String gpaButtonText = "Calculate GPA Score";
    private final String statButtonText = "Show Statistics";
    private final String alertButtonText = "Alert";
    private final String clearButtonText = "Clear All";
    private final double textWidth = 350;
    private final int width = 500;
    private final int height = 425;
    
    @Override
    public void start(Stage primaryStage){

        GridPane root = new GridPane();
        root.setAlignment(Pos.BASELINE_CENTER);
        root.setVgap(10);
        root.setHgap(10);
        root.setPadding(new Insets(15, 15, 15, 15));
        
        Scene scene = new Scene(root, width, height);
        
        Label label1 = new Label(course1Label);
        label1.setFont(Font.font(fontStyle, FontWeight.NORMAL, 18));
        root.add(label1, 0, 0, 1, 1);
        
        TextField course1TextField = new TextField();
        course1TextField.setPrefWidth(textWidth);
        root.add(course1TextField, 1, 0, 1, 1);
        
        Label label2 = new Label(course2Label);
        label2.setFont(Font.font(fontStyle, FontWeight.NORMAL, 18));
        root.add(label2, 0, 1, 1, 1);
        
        TextField course2TextField = new TextField();
        course2TextField.setPrefWidth(textWidth);
        root.add(course2TextField, 1, 1, 1, 1);
        
        Label label3 = new Label(course3Label);
        label3.setFont(Font.font(fontStyle, FontWeight.NORMAL, 18));
        root.add(label3, 0, 2, 1, 1);
        
        TextField course3TextField = new TextField();
        course3TextField.setPrefWidth(textWidth);
        root.add(course3TextField, 1, 2, 1, 1);
        
        Label label4 = new Label(course4Label);
        label4.setFont(Font.font(fontStyle, FontWeight.NORMAL, 18));
        root.add(label4, 0, 3, 1, 1);
        
        TextField course4TextField = new TextField();
        course4TextField.setPrefWidth(textWidth);
        root.add(course4TextField, 1, 3, 1, 1);
        
        Label label5 = new Label(infoLabel);
        label5.setFont(Font.font(fontStyle, FontWeight.NORMAL, 18));
        root.add(label5, 0, 4, 2, 1);
        
        TextArea infoTextArea = new TextArea();
        infoTextArea.setPrefWidth(textWidth);
        infoTextArea.setPrefRowCount(3);
        infoTextArea.setWrapText(true);
        infoTextArea.setEditable(false);
        root.add(infoTextArea, 0, 5, 2, 1);
        
        VBox buttonBox = new VBox();
        buttonBox.setAlignment(Pos.CENTER);
        buttonBox.setSpacing(10);
        root.add(buttonBox, 0, 6, 2, 1);
        
        Button calcButton = new Button();
        calcButton.setText(gpaButtonText);
        calcButton.setFont(Font.font(fontStyle, FontWeight.NORMAL, 18));
        calcButton.setMaxWidth(Double.MAX_VALUE);
        calcButton.setOnAction(new EventHandler<ActionEvent>() {
            @Override
            public void handle(ActionEvent event) {
                //initializing these variables to -1 is wonky but will allow us to do the try catch on the parseing
                //they will also just fall into another alert if anything else happens
                double score1 = -1;
                double score2 = -1;
                double score3 = -1;
                double score4 = -1;
                try{
                    score1 = Double.parseDouble(course1TextField.getText());
                    score2 = Double.parseDouble(course2TextField.getText());
                    score3 = Double.parseDouble(course3TextField.getText());
                    score4 = Double.parseDouble(course4TextField.getText());
                }catch(NumberFormatException x){
                    Alert charAlert = new Alert(AlertType.ERROR);
                    charAlert.setContentText("Please enter valid inputs into the text field");
                    charAlert.show();
                    return;
                }
                
                if(course1TextField.getText().equals("") || course2TextField.getText().equals("") || course3TextField.getText().equals("")
                        || course4TextField.getText().equals("")){
                    Alert missingAlert = new Alert(AlertType.ERROR);
                    missingAlert.setContentText("Please fill each course with a numeric value");
                    missingAlert.show();
                }
                else if((score1 < 0 || score1 > 100) || (score2 < 0 || score2 > 100) || (score3 < 0 || score3 > 100) || (score4 < 0 || score4 > 100)){
                    Alert invalidNumber = new Alert(AlertType.ERROR);
                    invalidNumber.setContentText("Please only enter numbers between 0 & 100");
                    invalidNumber.show();
                }else{
                    double avgGPA = calcGPA(score1, score2, score3, score4);
                    char letterGrade = getLetterGrade(avgGPA);
                    infoTextArea.setText("Your average score is ((" + course1TextField.getText() + "+ " + course2TextField.getText() + "+ " + course3TextField.getText()
                        + "+ " + course4TextField.getText() + ")/4) = " + String.format("%.2f", avgGPA) + "\n" + "Your GPA is: " + letterGrade);
                }
            }
        });
             
        Button statButton = new Button();
        statButton.setText(statButtonText);
        statButton.setFont(Font.font(fontStyle, FontWeight.NORMAL, 18));
        statButton.setMaxWidth(Double.MAX_VALUE);
        statButton.setOnAction(new EventHandler<ActionEvent>() {
            @Override
            public void handle(ActionEvent event) {
                double score1 = -1;
                double score2 = -1;
                double score3 = -1;
                double score4 = -1;
                try{
                    score1 = Double.parseDouble(course1TextField.getText());
                    score2 = Double.parseDouble(course2TextField.getText());
                    score3 = Double.parseDouble(course3TextField.getText());
                    score4 = Double.parseDouble(course4TextField.getText());
                }catch(NumberFormatException x){
                    Alert charAlert = new Alert(AlertType.ERROR);
                    charAlert.setContentText("Please enter only numbers into the course fields");
                    charAlert.show();
                    return;
                }
                
                if(course1TextField.getText().equals("") || course2TextField.getText().equals("") || course3TextField.getText().equals("")
                        || course4TextField.getText().equals("")){
                    Alert missingAlert = new Alert(AlertType.ERROR);
                    missingAlert.setContentText("Please fill each course with a numeric value");
                    missingAlert.show();
                }
                else if((score1 < 0 || score1 > 100) || (score2 < 0 || score2 > 100) || (score3 < 0 || score3 > 100) || (score4 < 0 || score4 > 100)){
                    Alert invalidNumber = new Alert(AlertType.ERROR);
                    invalidNumber.setContentText("Please only enter numbers between 0 & 100");
                    invalidNumber.show();
                }
                else{
                double Max = getMax(score1, score2, score3, score4);
                double Min = getMin(score1, score2, score3, score4);
                
                infoTextArea.setText("Your highest score is: " + Max + "\nYour lowest score is: " + Min);
                }
            } 
        });
        
        Button alertButton = new Button();
        alertButton.setText(alertButtonText);
        alertButton.setFont(Font.font(fontStyle, FontWeight.NORMAL, 18));
        alertButton.setMaxWidth(Double.MAX_VALUE);
        alertButton.setOnAction(new EventHandler<ActionEvent>() {
            @Override
            public void handle(ActionEvent event) {
                Alert statAlert = new Alert(AlertType.INFORMATION);
                if(infoTextArea.getText().equals("")){
                    statAlert.setContentText("There is nothing to display");
                }else{
                    statAlert.setContentText(infoTextArea.getText());
                }
                statAlert.show();
            } 
        });
        
        Button clearButton = new Button();
        clearButton.setText(clearButtonText);
        clearButton.setFont(Font.font(fontStyle, FontWeight.NORMAL, 18));
        clearButton.setMaxWidth(Double.MAX_VALUE);
        clearButton.setOnAction(new EventHandler<ActionEvent>(){
            @Override
            public void handle(ActionEvent event) {
                course1TextField.setText("");
                course2TextField.setText("");
                course3TextField.setText("");
                course4TextField.setText("");
                infoTextArea.setText("");
            }  
        });
        
        buttonBox.getChildren().add(calcButton);
        buttonBox.getChildren().add(statButton);
        buttonBox.getChildren().add(alertButton);
        buttonBox.getChildren().add(clearButton);
        
        primaryStage.setScene(scene);
        primaryStage.setTitle(title);
        
        primaryStage.show();
        
    }
    
    public double calcGPA(double score1, double score2, double score3, double score4){
     
        return (score1 + score2 + score3 + score4) / 4.0;
    }
    public char getLetterGrade(double avgGPA){
        if(avgGPA < 60){
            return 'F';
        }else if(avgGPA >= 60 && avgGPA < 67){
            return 'D';
        }else if(avgGPA >= 67 && avgGPA < 77){
            return 'C';
        }else if(avgGPA >= 77 && avgGPA < 87){
            return 'B';
        }else if(avgGPA >= 87 && avgGPA <= 100){
            return 'A';
        }else{
            return 'Z';
        }
    }
    public double getMax(double score1, double score2, double score3, double score4){
        
        double Max1 = Math.max(score1, score2);
        double Max2 = Math.max(score3, score4);
        double finalMax = Math.max(Max1, Max2);
        
        return finalMax;
    }
    public double getMin(double score1, double score2, double score3, double score4){
        
        double Min1 = Math.min(score1, score2);
        double Min2 = Math.min(score3, score4);
        double finalMin = Math.min(Min1, Min2);
        
        return finalMin;
    }
    
    public static void main(String[] args) {
        launch(args);
    }
    
}
