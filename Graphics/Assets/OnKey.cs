using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Events;

public class OnKey : MonoBehaviour {

    public KeyCode key;
    public UnityEvent evt;
	
	void Update ()
    {
		if (Input.GetKeyDown(this.key))
        {
            this.evt.Invoke();
        }
	}

}
