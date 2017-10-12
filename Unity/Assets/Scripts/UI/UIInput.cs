using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using GamepadBinding = InputResponse.GamepadBinding;

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
	
	public void down(GamepadBinding.UpdateEvent type, string action, float value)
    {
        Debug.Log("Down");
    }

    public void up(GamepadBinding.UpdateEvent type, string action, float value)
    {
        Debug.Log("Up");
    }

}
