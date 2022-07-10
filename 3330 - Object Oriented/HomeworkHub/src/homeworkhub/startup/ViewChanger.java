/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package homeworkhub.startup;

import java.util.Date;

/**
 *
 * @author kentstuder
 */
public interface ViewChanger {
    public void changeDate(Date newDate);
    public void changeName(String name);
    public void changePriority(int Priority);
    public void changeDesc(String description);   
}
