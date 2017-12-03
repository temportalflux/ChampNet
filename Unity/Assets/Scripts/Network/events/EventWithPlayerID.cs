using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EventWithPlayerID : EventNetwork
{

    [BitSerialize]
    public uint clientID;

    [BitSerialize]
    public uint playerID;

    public EventWithPlayerID(byte id) : base(id)
    {
    }

}
