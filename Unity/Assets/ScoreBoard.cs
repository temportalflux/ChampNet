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

        public GameObject NameObject;

        public GameObject WinObject;
    }

    [Tooltip("The list of listeners for buttons")]
    public RankText[] rankText;


    void Start ()
    {
        GameObject test = new GameObject();
        foreach (RankText text in rankText)
        {
            // set rank for later use
            text.Rank = currentText;

            // create new game object
            text.NameObject = new GameObject(("PlayerName (" + currentText + ")"), typeof(RectTransform));
            // set RectWidth and RectHeight
            text.NameObject.GetComponent<RectTransform>().sizeDelta = new Vector2(160, 30);

            text.NameObject.GetComponent<RectTransform>().anchorMin = new Vector2(0.5f, 0.5f);
            text.NameObject.GetComponent<RectTransform>().anchorMax = new Vector2(0.5f, 0.5f);
            text.NameObject.GetComponent<RectTransform>().pivot = new Vector2(0.5f, 0.5f);

            //// set game object location
            //text.NameObject.GetComponent<RectTransform>().transform.position = this.camera.ScreenToWorldPoint(new Vector3(
            //    (text.NameObject.GetComponent<RectTransform>().rect.width / 2) + 5,
            //    ((Screen.height - (text.NameObject.GetComponent<RectTransform>().rect.height / 2)) - (15 * text.Rank)), camera.nearClipPlane));

            //// set the z position
            //text.NameObject.GetComponent<RectTransform>().transform.position = new Vector3(text.NameObject.transform.position.x, text.NameObject.transform.position.y, text.NameObject.transform.position.z);

            // add text component to the new game object
            var newNameText = text.NameObject.AddComponent<Text>();

            // set text info
            newNameText.text = ("Empty rank: " + currentText);
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
            // set text allignment
            newNameText.alignment = TextAnchor.MiddleLeft;
            // set unt to be child of ScoreBoard
            newNameText.transform.SetParent(transform);
            // set game object scale
            text.NameObject.transform.localScale = new Vector3(1, 1, 1);

            // repeate above with modifications for wins

            text.WinObject = new GameObject(("PlayerWins (" + currentText + ")"), typeof(RectTransform));
            text.WinObject.GetComponent<RectTransform>().sizeDelta = new Vector2(160, 30);

            //text.WinObject.GetComponent<RectTransform>().transform.position = this.camera.ScreenToWorldPoint(new Vector3(
            //    (text.WinObject.GetComponent<RectTransform>().rect.width / 3) + text.WinObject.GetComponent<RectTransform>().rect.width,
            //    ((Screen.height - (text.WinObject.GetComponent<RectTransform>().rect.height / 2)) - (15 * text.Rank)), camera.nearClipPlane));

            //text.WinObject.GetComponent<RectTransform>().transform.position = new Vector3(text.WinObject.transform.position.x, text.WinObject.transform.position.y, text.WinObject.transform.position.z);

            var newWinText = text.WinObject.AddComponent<Text>();

            // set this to N/A as their is no one there to display a win
            newWinText.text = "N/A";
            newWinText.alignment = TextAnchor.MiddleCenter;
            newWinText.fontSize = text.fontSize;
            newWinText.fontStyle = text.fontStyle;
            newWinText.font = Resources.GetBuiltinResource(typeof(Font), "Arial.ttf") as Font; // place holder unitl I can find how to add it from inspector
            newWinText.color = Color.black;
            newWinText.transform.SetParent(transform);
            newWinText.alignment = TextAnchor.MiddleLeft;
            text.WinObject.transform.localScale = new Vector3(1, 1, 1);

            currentText += 1;
        }
        currentText = 1;

        GameManager_GameObject = GameObject.Find("GameManager");
        GameManager_Script = GameManager_GameObject.GetComponent<GameManager>();
    }

    /// <summary>
    /// used to reset score board if window size is changed mid game
    /// Also there was a bug where Z was not set properly and this allows it to be set properly
    /// </summary>
    private void Update()
    {
        foreach (RankText text in rankText)
        {
            // set game object location
            text.NameObject.GetComponent<RectTransform>().transform.position = this.camera.ScreenToWorldPoint(new Vector3(
                (text.NameObject.GetComponent<RectTransform>().rect.width / 2) + 5,
                ((Screen.height - (text.NameObject.GetComponent<RectTransform>().rect.height / 2)) - (15 * text.Rank)), 0));

            // set the z position
            text.NameObject.GetComponent<RectTransform>().transform.position = new Vector3(text.NameObject.transform.position.x, text.NameObject.transform.position.y, camera.nearClipPlane);

            // repeate above with modifications for wins

            text.WinObject.GetComponent<RectTransform>().transform.position = this.camera.ScreenToWorldPoint(new Vector3(
                (text.WinObject.GetComponent<RectTransform>().rect.width / 3) + text.WinObject.GetComponent<RectTransform>().rect.width,
                ((Screen.height - (text.WinObject.GetComponent<RectTransform>().rect.height / 2)) - (15 * text.Rank)), 0));

            text.WinObject.GetComponent<RectTransform>().transform.position = new Vector3(text.WinObject.transform.position.x, text.WinObject.transform.position.y, camera.nearClipPlane);
        }
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
