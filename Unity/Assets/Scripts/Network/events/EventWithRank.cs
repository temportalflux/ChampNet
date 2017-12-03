using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[System.Obsolete("This should be calculated by server")]
public class EventWithRank : EventWithPlayerID
{


    /// <summary>
    /// The player Rank
    /// </summary>
    [BitSerialize]
    public uint rank;

    public EventWithRank(byte id) : base(id)
    { }

}
