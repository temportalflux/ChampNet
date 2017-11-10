using System.Collections;
using System.Collections.Generic;
using UnityEngine.UI;
using UnityEngine;

public class ScoreBoard : MonoBehaviour {

    public GameObject GameManager_GameObject;
    public GameManager GameManager_Script;

    public Camera camera;

    private Text name;
    private Text win;

    private int currentText = 1;
    // The listener class for handling all button effects
    [System.Serializable]
    public class RankText
    {
        [Tooltip("Rank setting (sets itself on start) (don't touch)")]
        public int Rank;

        //[Tooltip("Name text created")]
        //public Text newName;

        //[Tooltip("Wins text created")]
        //public Text newWin;

        [Tooltip("Size of font used for text created")]
        public int fontSize;

        [Tooltip("Style of the font used for text created")]
        public FontStyle fontStyle;

        [Tooltip("font of the font used for text created")]
        public Font font;
    }

    [Tooltip("The list of listeners for buttons")]
    public RankText[] rankText;


    void Start ()
    {
        foreach (RankText text in rankText)
        {
            // set rank for later use
            text.Rank = currentText;

            // create new game object
            GameObject newNameObject = new GameObject(("PlayerName (" + currentText + ")"), typeof(RectTransform));
            // set game object location
            newNameObject.transform.position = this.camera.ViewportToWorldPoint(new Vector3(1, 1, camera.nearClipPlane));
            // add text component to the new game object
            var newNameText = newNameObject.AddComponent<Text>();

            // set text info
            newNameText.text = ("Empty rank: " + currentText );
            // set alignment for RectTransform
            newNameText.alignment = TextAnchor.MiddleCenter;
            // set font size to what we wanted
            newNameText.fontSize = text.fontSize;
            // set font style
            newNameText.fontStyle = text.fontStyle;
            // set font type
            newNameText.font = Resources.GetBuiltinResource(typeof(Font), "Arial.ttf") as Font; // place holder unitl I can find how to add it from inspector
            // set font color
            newNameText.color = Color.black;
            // set unt to be child of ScoreBoard
            newNameText.transform.SetParent(transform);
            // set game object scale
            newNameObject.transform.localScale = new Vector3(1, 1, 1);

            // repeate above for wins

            GameObject newWinObject = new GameObject(("PlayerWins (" + currentText + ")"), typeof(RectTransform));
            var newWinText = newWinObject.AddComponent<Text>();
            newWinObject.transform.position = this.camera.ScreenToWorldPoint(new Vector3(0 + 20, Screen.height, camera.nearClipPlane));
            
            // set this to N/A as their is no one there to claim a win
            newWinText.text = "N/A";
            newWinText.alignment = TextAnchor.MiddleCenter;
            newWinText.fontSize = text.fontSize;
            newWinText.fontStyle = text.fontStyle;
            newWinText.font = Resources.GetBuiltinResource(typeof(Font), "Arial.ttf") as Font; // place holder unitl I can find how to add it from inspector
            newWinText.color = Color.black;
            newWinText.transform.SetParent(transform);
            newWinObject.transform.localScale = new Vector3(1, 1, 1);

            currentText += 1;
        }

        GameManager_GameObject = GameObject.Find("GameManager");
        GameManager_Script = GameManager_GameObject.GetComponent<GameManager>();
    }

    void DisplayScoreBoard()
    {
        Debug.Log("yo it me, da score Board!");


        foreach (RankText textNames in rankText)
        {
            this.name = GameObject.Find("PlayerName (" + textNames.Rank + ")").GetComponent<Text>();
            {
                this.name.text = "hi";
            }

            if (this.win = GameObject.Find("PlayerWins (" + textNames.Rank + ")").GetComponent<Text>())
            {
                this.win.text = "yo";
            }
        }
    }

}
