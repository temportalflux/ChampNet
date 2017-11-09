using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ScoreBoard : MonoBehaviour {

    public GameObject GameManager_GameObject;
    public GameManager GameManagerScript;

	// Use this for initialization
	void Start ()
    {
        GameManager_GameObject = GameObject.Find("GameManager");
        GameManagerScript = GameManager_GameObject.GetComponent<GameManager>();

        GameManagerScript.networkPlayerMap
    }

}
