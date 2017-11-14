using System.Collections;
using System.Collections.Generic;
using UnityEngine.UI;
using UnityEngine;

public class ScoreBoard : MonoBehaviour {

    public GameObject GameManager_GameObject;
    public GameManager GameManager_Script;

    public GameObject NameHeader;
    public GameObject WinHeader;

    // will remove this when the Dictionary of all units is created
    public PlayerLocal LocalPlayerScript;

    public Camera camera;

    private Text name;
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
        public uint name;

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
            text.name = 100;

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

        //GameManager_GameObject = GameObject.Find("GameManager");
        //GameManager_Script = GameManager_GameObject.GetComponent<GameManager>();
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

            // set Header

            // set game object location
            NameHeader.GetComponent<RectTransform>().transform.position = this.camera.ScreenToWorldPoint(new Vector3(
                (NameHeader.GetComponent<RectTransform>().rect.width / 2) + 5,
                ((Screen.height - (NameHeader.GetComponent<RectTransform>().rect.height / 2)) - 5), 0));

            // set the z position
            NameHeader.GetComponent<RectTransform>().transform.position = new Vector3(NameHeader.transform.position.x, NameHeader.transform.position.y, camera.nearClipPlane);

            // repeate above with modifications for wins

            WinHeader.GetComponent<RectTransform>().transform.position = this.camera.ScreenToWorldPoint(new Vector3(
                (WinHeader.GetComponent<RectTransform>().rect.width / 3) + WinHeader.GetComponent<RectTransform>().rect.width,
                ((Screen.height - (WinHeader.GetComponent<RectTransform>().rect.height / 2)) - 5), 0));

            WinHeader.GetComponent<RectTransform>().transform.position = new Vector3(WinHeader.transform.position.x, WinHeader.transform.position.y, camera.nearClipPlane);
        }
    }

    /// <summary>
    /// updates scoreboard on player win
    /// </summary>
    /// left to do: remove player from board when they leave // call function on player disconnect notification
    /// <remarks>
    /// Author: Christopher Brennan
    /// </remarks>
    void DisplayScoreBoard()
    {
        Debug.Log("yo it me, da score Board!");

        foreach (RankText scoreboard in rankText)
        {
            PlayerReference recordInfo; // player info for board
            recordInfo = findNextHighestScore(scoreboard);

            // get and set all name info
            scoreboard.name = recordInfo.getID();
            scoreboard.NameObject.GetComponent<Text>().text = scoreboard.name.ToString();

            // get and set all win info
            scoreboard.score = recordInfo.getScore();
            scoreboard.WinObject.GetComponent<Text>().text = scoreboard.score.ToString();
        }
    }

    /// <summary>
    /// recieve current score on board to check if a change is needed to be made
    /// </summary>
    /// <param name="scoreBoard"></param>
    /// 
    /// Modify so then the networked player and local player are all in one Dictionary and search that instead of having two searches
    /// 
    /// <remarks>
    /// Author: Christopher Brennan
    /// </remarks>
    PlayerReference findNextHighestScore(RankText scoreBoard)
    {
        // created incase no new players are on scoreboard
        PlayerReference temp = new PlayerReference();
        temp.setID(0);
        temp.setScore(0);

        //local player
        if (LocalPlayerScript.getID() == scoreBoard.name)
        {
            temp = LocalPlayerScript; // if nothing changes then stay with current player information
        }
        if (LocalPlayerScript.getScore() > scoreBoard.score)
        {
            if (LocalPlayerScript.getRank() > scoreBoard.Rank || LocalPlayerScript.getRank() == 0)
            {
                // check if there was already a player there and check if true check if it was the same player
                if (scoreBoard.name == 100 || scoreBoard.name == LocalPlayerScript.getID()) // == 100 is the set id for if no player exists
                {
                    // set player on to the scoreboard
                    // no one is currently holding that spot on the board. so take it
                    LocalPlayerScript.setRank(scoreBoard.Rank);
                    return LocalPlayerScript; // return player Reference
                }
                else
                {
                    // You just kicked someone off of above you
                    // switch places with the individual that you kicked out
                    foreach (KeyValuePair<uint, PlayerReference> defeatedPlayer in GameManager_Script.networkPlayerMap) // search for individual to move down a rank
                    {
                        if (defeatedPlayer.Value.getRank() == scoreBoard.Rank) // check for match
                        {
                            defeatedPlayer.Value.setRank(LocalPlayerScript.getRank()); // move player down
                            LocalPlayerScript.setRank(scoreBoard.Rank); // move new player up
                            return LocalPlayerScript; // return player Reference
                        }
                    }
                }
            }
        }

        // networked players
        foreach (KeyValuePair<uint, PlayerReference> player in GameManager_Script.networkPlayerMap)
        {
            if(player.Value.getID() == scoreBoard.name)
            {
                temp = player.Value; // if nothing changes then stay with current player information
            }
            if(player.Value.getScore() > scoreBoard.score)
            {
                if(player.Value.getRank() > scoreBoard.Rank || player.Value.getRank() == 0)
                {
                    // check if there was already a player there and check if true check if it was the same player
                    if(scoreBoard.name == 100 || scoreBoard.name == player.Value.getID()) // == 100 is the set id for if no player exists
                    {
                        // set player on to the scoreboard
                        // no one is currently holding that spot on the board. so take it
                        player.Value.setRank(scoreBoard.Rank);
                        return player.Value; // return player Reference
                    }
                    else
                    {
                        // You just kicked someone off of above you
                        // switch places with the individual that you kicked out
                        foreach (KeyValuePair<uint, PlayerReference> defeatedPlayer in GameManager_Script.networkPlayerMap) // search for individual to move down a rank
                        {
                            if(defeatedPlayer.Value.getRank() == scoreBoard.Rank) // check for match
                            {
                                defeatedPlayer.Value.setRank(player.Value.getRank()); // move player down
                                player.Value.setRank(scoreBoard.Rank); // move new player up
                                return player.Value; // return player Reference
                            }
                        }
                    }
                }
            }
        }
        // if no new info has changed then return the current player/ no player on the scoreboard
        return temp;

    }

}
