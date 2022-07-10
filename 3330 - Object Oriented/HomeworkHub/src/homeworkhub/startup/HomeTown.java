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
public class HomeTown extends UserInfo{
    public String townName;
    public String state;
    
    HomeTown(String townName, String extra){
        super(extra);
        this.townName = townName;
    }

    @Override
    public void setHomeTown(String town) {
        this.townName = town;
    }

    @Override
    public void setState(String State) {
        this.state = state;
    }
}
