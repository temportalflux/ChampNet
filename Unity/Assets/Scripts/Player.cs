using System.Collections;
using System.Collections.Generic;
using UnityEngine;

//using ActionType = InputResponse.GamepadBinding.UpdateEvent;

public class Player : MonoBehaviour {

	// Use this for initialization
	void Start () {
        ChampNetPlugin.Network.Create();
	}

    void OnDestroy()
    {
        ChampNetPlugin.Network.Destroy();
    }

    // Update is called once per frame
    void Update () {
		
	}

    public void onAction(InputResponse.UpdateEvent evt, MappedButton button)
    {
        Debug.Log(evt + ": " + button);
    }

    /*
    public void doAction(ActionType type, string action, float value)
    {
        //Debug.Log(type + ": " + action + " " + value);
        //Debug.Log(ChampNetPlugin.Network.GetRandom() % 1000);

        

    }
    */

}
