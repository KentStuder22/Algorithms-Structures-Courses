/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package serializationjsonexample;

import org.json.simple.JSONObject;
import org.json.simple.JSONValue;

/**
 *
 * @author Professor Wergeles
 */
public class Person implements java.io.Serializable {
    
    private String firstName = "";
    private String lastName = "";
    private Integer age;
    private String gender;
    
    public Person() {
        
    }
    
    public void setFirstName(String firstName) {
        this.firstName = firstName;
    }
    
    public void setLastName(String lastName) {
        this.lastName = lastName;
    }
    
    public void setAge(Integer age) {
        this.age = age;
    }
    
    public void setGender(String gender) {
        this.gender = gender;
    }
    
    public String getFirstName() {
        return firstName;
    }
    
    public String getLastName() {
        return lastName;
    }
    
    public Integer getAge() {
        return age;
    }
    
    public String getGender() {
        return gender;
    }
    
    // http://tutorials.jenkov.com/java-collections/hashcode-equals.html
    // http://stackoverflow.com/questions/7526817/use-of-instance-of-in-java
    @Override
    public boolean equals(Object obj) {
        if (obj == null) {
            return false;
        }
        if (!(obj instanceof Person)) {
            return false;
        }
        
        Person other = (Person) obj;
        
        if (firstName == null) {
            if (!(other.getFirstName() == null)) {
                return false;
            }
        }
        else if (!firstName.equals(other.getFirstName())) {
            return false;
        }
        
        if (lastName == null) {
            if (!(other.getLastName() == null)) {
                return false;
            }
        }
        else if (!lastName.equals(other.getLastName())) {
            return false;
        }
        
        if (age == null) {
             if (!(other.getAge() == null)) {
                return false;
            }           
        } else if (! (age.equals(other.getAge()))) {
            return false;
        }
        
        if (gender == null) {
            if (!(other.getGender() == null)) {
                return false;
            }            
        }
        else if (!gender.equals(other.getGender())) {
            return false;
        }
            
        return true;
    }
    
    public String toJsonString() {
        JSONObject obj=new JSONObject();
        if (firstName != null) obj.put("firstName", firstName);
        if (lastName != null)  obj.put("lastName", lastName);
        if (gender != null) obj.put("gender", gender);
        if (age != null) obj.put("age", age);

        return obj.toJSONString(); 
    }
    
    public void initFromJsonString(String jsonString) {
        firstName = "";
        lastName = "";
        gender = null;
        age = null;
        
        if (jsonString == null || jsonString == "") return;
        
        JSONObject jsonObj;
        try {
            jsonObj = (JSONObject)JSONValue.parse(jsonString);
        } catch (Exception ex) {
            return;
        }
        
        if (jsonObj == null) {
            return;
        }
        
        firstName = (String)jsonObj.getOrDefault("firstName", "");
        lastName = (String)jsonObj.getOrDefault("lastName", "");
        gender = (String)jsonObj.getOrDefault("gender", null);
        
        // get on a number will be a long
        Object ageObj = jsonObj.getOrDefault("age", null);
        
        if (ageObj != null) {
            if (ageObj instanceof Long) {
                Long longAge = (Long)ageObj;
                age = new Integer(longAge.intValue());
            } else {
                age = null;
            } 
        }
    }
}
