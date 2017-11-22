using System.Collections;
using System.Collections.Generic;
using UnityEngine;


/// <summary>
/// Only used to increment a player score by 1 to prove that scoreboard works
/// </summary>
public class EventIncrementScore : EventWithScore
{

    public EventIncrementScore() : base((byte)ChampNetPlugin.MessageIDs.ID_CLIENT_SCORE_UP)
    {
    }

    public EventIncrementScore(uint clientID, uint playerID, uint score) : this()
    {
        this.clientID = clientID;
        this.playerID = playerID;
        this.score = score;
    }

    override public void Serialize(ref byte[] data, ref int lastIndex)
    {
        base.Serialize(ref data, ref lastIndex);
    }

}