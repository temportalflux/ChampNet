using System.Collections;
using System.Collections.Generic;
using UnityEngine.UI;
using UnityEngine;

public class ScoreBoard : MonoBehaviour {

    public GameObject GameManager_GameObject;
    public GameManager GameManagerScript;

    private Text name;
    private Text win;

	// Use this for initialization
	void Start ()
    {
        GameManager_GameObject = GameObject.Find("GameManager");
        GameManagerScript = GameManager_GameObject.GetComponent<GameManager>();
    }

    void Update()
    {
        Debug.Log("yo it me, da score Board!");

        for (int i = 1; i <= GameManagerScript.getSize() + 1; ++i)
        {
            this.name = GameObject.Find("PlayerName (" + i + ")").GetComponent<Text>();
            {
                this.name.text = "hi";
            }

            if (this.win = GameObject.Find("PlayerWins (" + i + ")").GetComponent<Text>())
            {
                this.win.text = "yo";
            }
        }
    }

}
