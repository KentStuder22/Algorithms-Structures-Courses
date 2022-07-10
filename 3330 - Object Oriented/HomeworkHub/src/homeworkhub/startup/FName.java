/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package homeworkhub.startup;

/**
 *
 * @author kentstuder
 */
public class FName extends Assignment{
    String FirstName;
    String LastName;
    FName(){
        this.FirstName = "";
        this.LastName = "";
    }
    FName(String FirstName) {
        this.FirstName = FirstName;
        this.LastName = "";
    }
    FName(String FirstName, String LastName){
        this.FirstName = FirstName;
        this.LastName = LastName;
    }
    
    public String getFirst(){
        return this.FirstName;
    }
    public String getLast(){
        return this.LastName;
    }
    public void setFirst(String First){
        this.FirstName = First;
    }
    public void setLast(String Last){
        this.LastName = Last;
    }
}
