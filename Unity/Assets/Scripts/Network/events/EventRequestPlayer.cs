using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EventRequestPlayer : EventWithPlayerID
{

    public EventRequestPlayer() : base((byte)ChampNetPlugin.MessageIDs.ID_CLIENT_REQUEST_PLAYER)
    { }

    public EventRequestPlayer(uint clientID, uint localID) : this()
    {
        this.clientID = clientID;
        this.playerID = localID;
    }

}
