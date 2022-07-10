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
public abstract class UserInfo {
    String extraUserInfo = "";
    UserInfo(String extra){
        this.extraUserInfo = extra;
    }
    abstract public void setHomeTown(String town);
    abstract public void setState(String State);
}
