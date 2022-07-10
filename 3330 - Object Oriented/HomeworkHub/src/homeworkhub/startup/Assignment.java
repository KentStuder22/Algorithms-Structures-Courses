/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package homeworkhub.startup;

import java.text.DateFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;

/**
 *
 * @author kentstuder
 */
public class Assignment implements ViewChanger{
    
    private String name;
    private String description;
    public int priority;
    Date dueDate;
    
    public void Assignment() {
        name = "";
        description = "";
        priority = 1;
        dueDate = new Date();
    }
    public void Assignment(String name) {
        Assignment();
        this.name = name;
    }
    public void Assignment(String name, String description) {
        Assignment(name);
        this.description = description;
    }
    public void Assignment(String name, String description, int priority) {
        Assignment(name, description);
        this.priority = priority;
    }
    public void Assignment(String name, String description, int priority, Date date) {
        Assignment(name, description, priority);
        this.dueDate = date;
    }
    
    public String getName(){
        return this.name;
    }
    public String getDescription(){
        return this.description;
    }
    public int getPriority(){
        return this.priority;
    }
    public Date getDate(){
        return this.dueDate;
    }
    
    public void setName(String name) {
        this.name = name;
    }
    public void setDescription(String description) {
        this.description = description;
    }
    public void setPriority(int priority) {
        this.priority = priority;
    }
    public void setDueDate(String dueDate) throws ParseException {
        Date date = new SimpleDateFormat("MM-dd-yyyy").parse(dueDate); 
        this.dueDate = date;
    }

    @Override
    public void changeDate(Date newDate) {
        this.dueDate = newDate;
    }

    @Override
    public void changeName(String name) {
        this.name = name;
    }

    @Override
    public void changePriority(int Priority) {
        this.priority = Priority;
    }

    @Override
    public void changeDesc(String description) {
        this.description = description;
    }
}
