using System.Collections;
using System.Collections.Generic;
using UnityEngine;

//using GamepadBinding = InputResponse.GamepadBinding;

public class UIInput : MonoBehaviour {

    public UIInputField[] fields;

    private int selected;
    
	void Start () {
        this.selected = 0;


        for (int i = 0; i < fields.Length; i++)
        {
            this.fields[i].index = i;
        }

	}
	
	public void down(InputDevice device, InputResponse.UpdateEvent type, MappedAxis action, float value)
    {
        
    }

}
