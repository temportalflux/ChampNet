using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EventWithPlayerID : EventWithID
{

    /// <summary>
    /// The player identifier
    /// </summary>
    [BitSerialize]
    public uint playerID;

    public EventWithPlayerID(byte id) : base(id)
    { }

}
