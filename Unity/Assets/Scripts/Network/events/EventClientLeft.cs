using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EventClientLeft : EventWithID
{

    public EventClientLeft() : base((byte)ChampNetPlugin.MessageIDs.ID_CLIENT_LEFT)
    {
    }

    public EventClientLeft(uint clientID) : this()
    {
        this.id = clientID;
    }

}
