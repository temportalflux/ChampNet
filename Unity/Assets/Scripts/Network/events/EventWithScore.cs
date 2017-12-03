using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[System.Obsolete("This should be calculated by server")]
public class EventWithScore : EventWithPlayerID
{
    
    /// <summary>
    /// The player Score
    /// </summary>
    [BitSerialize]
    public uint score;

    public EventWithScore(byte id) : base(id)
    {
    }

}
