using System.Collections;
using System.Collections.Generic;
using UnityEngine;


/// <summary>
/// Only used to increment a player score by 1 to prove that scoreboard works
/// </summary>
[System.Obsolete("This should be calculated by server")]
public class EventIncrementScore : EventWithScore
{

    // THIS IS BROKEN

    public EventIncrementScore() : base((byte)ChampNetPlugin.MessageIDs.ID_CLIENT_SCORE_UP)
    {
        //this.setIDs(0, 0);
    }

    public EventIncrementScore(uint clientID, uint playerID, uint score) : this()
    {
        //this.setIDs(clientID, playerID);
        this.score = score;
    }

}