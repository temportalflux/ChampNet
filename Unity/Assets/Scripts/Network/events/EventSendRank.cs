using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/// <summary>
/// Only used to increment a player score by 1 to prove that scoreboard works
/// </summary>
public class EventSendRank : EventWithRank
{

    // TODO: THIS IS BROKEN

    public EventSendRank() : base((byte)ChampNetPlugin.MessageIDs.ID_CLIENT_RANK_UPDATE)
    {
        //this.setIDs(0, 0);
    }

    public EventSendRank(uint clientID, uint playerID, uint rank) : this()
    {
        //this.setIDs(clientID, playerID);
        this.rank = rank;
    }

}
