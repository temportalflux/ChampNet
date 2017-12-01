using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/// <summary>
/// Only used to increment a player score by 1 to prove that scoreboard works
/// </summary>
public class EventSendRank : EventWithRank
{

    public EventSendRank() : base((byte)ChampNetPlugin.MessageIDs.ID_CLIENT_RANK_UPDATE)
    {
    }

    public EventSendRank(uint clientID, uint playerID, uint rank) : this()
    {
        this.clientID = clientID;
        this.playerID = playerID;
        this.rank = rank;
    }

    override public void Serialize(ref byte[] data, ref int lastIndex)
    {
        base.Serialize(ref data, ref lastIndex);
    }

}
