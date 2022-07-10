/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package krspcpmvcstopwatchf21;

import java.beans.PropertyChangeListener;
import java.beans.PropertyChangeSupport;

/**
 *
 * @author kento
 */
public abstract class AbstractModel {
    protected PropertyChangeSupport PCS;
    
    public AbstractModel(){
        PCS = new PropertyChangeSupport(this);
    }
    public void addPCL(PropertyChangeListener listener){
        PCS.addPropertyChangeListener(listener);
    }
    public void removePCL(PropertyChangeListener listener){
        PCS.removePropertyChangeListener(listener);
    }
    protected void firePC(String propName, Object oldVal, Object newVal){
        PCS.firePropertyChange(propName, oldVal, newVal);
    }
}
