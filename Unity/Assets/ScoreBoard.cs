using System.Collections;
using System.Collections.Generic;
using UnityEngine.UI;
using UnityEngine;

public class ScoreBoard : MonoBehaviour {

    public GameObject NameHeader;
    public GameObject WinHeader;

    // will remove this when the Dictionary of all units is created
    public PlayerLocal LocalPlayerScript;

    public Camera theCamera;
    
    //private Text name;
    private Text win;
    private uint currentText = 1;

    [System.Serializable]
    public class RankText
    {
        [Tooltip("Rank setting (sets itself on start) (don't touch)")]
        public uint Rank;

        [Tooltip("Size of font used for text created")]
        public int fontSize;

        [Tooltip("Style of the font used for text created")]
        public FontStyle fontStyle;

        [Tooltip("Font of the font used for text created")]
        public Font font;

        [Tooltip("Text Object Name")]
        public GameObject NameObject;

        [Tooltip("Text Object Wins")]
        public GameObject WinObject;

        [Tooltip("Name for current owner")]
        public string name;

        [Tooltip("Wins for current owner")]
        public uint score;
    }

    [Tooltip("The list of listeners for buttons")]
    public RankText[] rankText;

    /// <summary>
    /// Creates all text for number of ranks created
    /// </summary>
    void Start ()
    {
        // give GameManager the scoreboard
        GameManager.INSTANCE.grabScoreBoard();
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

            // init name
            text.name = "N/A";

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

            // init score
            text.score = 0;

            currentText += 1;
        }
        currentText = 1;
    }

    /// <summary>
    /// used to reset score board if window size is changed mid game
    /// Also there was a bug where Z was not set properly and this allows it to be set properly
    /// </summary>
    private void Update()
    {
        DisplayScoreBoard();
        foreach (RankText text in rankText)
        {
            // set game object location
            text.NameObject.GetComponent<RectTransform>().transform.position = this.theCamera.ScreenToWorldPoint(new Vector3(
                (text.NameObject.GetComponent<RectTransform>().rect.width / 2) + 5,
                ((Screen.height - (text.NameObject.GetComponent<RectTransform>().rect.height / 2)) - (15 * text.Rank)), 0));

            // set the z position
            text.NameObject.GetComponent<RectTransform>().transform.position = new Vector3(text.NameObject.transform.position.x, text.NameObject.transform.position.y, theCamera.nearClipPlane);

            // repeate above with modifications for wins

            text.WinObject.GetComponent<RectTransform>().transform.position = this.theCamera.ScreenToWorldPoint(new Vector3(
                (text.WinObject.GetComponent<RectTransform>().rect.width / 3) + text.WinObject.GetComponent<RectTransform>().rect.width,
                ((Screen.height - (text.WinObject.GetComponent<RectTransform>().rect.height / 2)) - (15 * text.Rank)), 0));

            text.WinObject.GetComponent<RectTransform>().transform.position = new Vector3(text.WinObject.transform.position.x, text.WinObject.transform.position.y, theCamera.nearClipPlane);

            // set Header

            // set game object location
            NameHeader.GetComponent<RectTransform>().transform.position = this.theCamera.ScreenToWorldPoint(new Vector3(
                (NameHeader.GetComponent<RectTransform>().rect.width / 2) + 5,
                ((Screen.height - (NameHeader.GetComponent<RectTransform>().rect.height / 2)) - 5), 0));

            // set the z position
            NameHeader.GetComponent<RectTransform>().transform.position = new Vector3(NameHeader.transform.position.x, NameHeader.transform.position.y, theCamera.nearClipPlane);

            // repeate above with modifications for wins

            WinHeader.GetComponent<RectTransform>().transform.position = this.theCamera.ScreenToWorldPoint(new Vector3(
                (WinHeader.GetComponent<RectTransform>().rect.width / 3) + WinHeader.GetComponent<RectTransform>().rect.width,
                ((Screen.height - (WinHeader.GetComponent<RectTransform>().rect.height / 2)) - 5), 0));

            WinHeader.GetComponent<RectTransform>().transform.position = new Vector3(WinHeader.transform.position.x, WinHeader.transform.position.y, theCamera.nearClipPlane);
        }
    }

    /// <summary>
    /// updates scoreboard on player win
    /// </summary>
    /// left to do: remove player from board when they leave // call function on player disconnect notification
    /// <remarks>
    /// Author: Christopher Brennan
    /// </remarks>
    public void DisplayScoreBoard()
    {
        //Debug.Log("update scoreboard");

        foreach (RankText scoreboard in rankText)
        {
            findNextHighestScore(scoreboard);

            // set name info
            scoreboard.NameObject.GetComponent<Text>().text = scoreboard.name.ToString();

            // set win info
            scoreboard.WinObject.GetComponent<Text>().text = scoreboard.score.ToString();
        }
    }

    /// <summary>
    /// recieve current score on board to check if a change is needed to be made
    /// </summary>
    /// <param name="scoreBoard"></param>
    /// <remarks>
    /// Author: Christopher Brennan
    /// </remarks>
    private void findNextHighestScore(RankText scoreBoard)
    {
        // checks both local and connected players
        foreach (KeyValuePair<uint, GameState.Player> player in GameManager.INSTANCE.state.players)
        {
            //if (player.Value.objectReference.getID() == scoreBoard.name)
            //{
            //    temp = player.Value.objectReference; // if nothing changes then stay with current player information
            //}
            if (player.Value.objectReference.score > scoreBoard.score)
            {
                // check if you are lower in the rankings (higher number means lower in ranking) or check if you are even ranked
                if (player.Value.objectReference.rank > scoreBoard.Rank || player.Value.objectReference.rank == 0)
                {
                    // check if there was already a player there and check if it is the current player
                    if (scoreBoard.name == " " || scoreBoard.name == player.Value.objectReference.name) // == 100 is the set id for if no player exists // can change later just current placeholder
                    {
                        // set player on to the scoreboard
                        // no one is currently holding that spot on the board. so take it
                        player.Value.objectReference.rank = scoreBoard.Rank;
                        scoreBoard.score = player.Value.objectReference.score;
                        scoreBoard.name = player.Value.objectReference.name;
                        // return player.Value.objectReference; // return player Reference
                    }
                    else
                    {
                        // You just kicked someone off of above you
                        // switch places with the individual that you kicked out
                        foreach (KeyValuePair<uint, GameState.Player> defeatedPlayer in GameManager.INSTANCE.state.players) // search for individual to move down a rank
                        {
                            if (defeatedPlayer.Value.rank == scoreBoard.Rank) // check for match
                            {
                                defeatedPlayer.Value.objectReference.rank = player.Value.objectReference.rank; // move player down
                                player.Value.objectReference.rank = scoreBoard.Rank; // move new player up
                                scoreBoard.score = player.Value.objectReference.score;
                                scoreBoard.name = player.Value.objectReference.name;
                                //return player.Value.objectReference; // return player Reference
                            }
                        }
                    }
                }
            }
        }
    }
    public void removePlayerOnLeave(string removeName) // currently using ID
    {
        foreach (RankText scoreboard in rankText)
        {
            if(scoreboard.name == removeName)
            {
                scoreboard.name = "N/A";
                scoreboard.score = 0;
            }
        }
    }

}
