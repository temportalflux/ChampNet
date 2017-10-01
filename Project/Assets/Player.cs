using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using ActionType = InputResponse.GamepadBinding.UpdateEvent;

public class Player : MonoBehaviour {

	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		
	}

    public void doAction(ActionType type, string action, float value)
    {
        Debug.Log(type + ": " + action + " " + value);
    }

}
