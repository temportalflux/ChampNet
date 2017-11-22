using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EventWithScore : EventWithPlayerID
{


    /// <summary>
    /// The player Score
    /// </summary>
    protected uint score;

    public EventWithScore(byte id) : base(id)
    { }

    /// <summary>
    /// Returns the size of the packet (the size for a potential byte[])
    /// </summary>
    /// <returns>
    /// the integer length of a byte array to hold this event's data
    /// </returns>
    /// <remarks>
    /// Author: Dustin Yost
    /// Modifier: Christopher Brennan
    /// </remarks>
    override public int GetSize()
    {
        return base.GetSize() + sizeof(System.UInt32); // super + score
    }

    /// <summary>
    /// Deserializes data from a byte array into this event's data
    /// </summary>
    /// <param name="data">The data.</param>
    /// <param name="lastIndex">The last index.</param>
    /// <remarks>
    /// Author: Dustin Yost
    /// </remarks>
    override public void Deserialize(byte[] data, ref int lastIndex)
    {
        base.Deserialize(data, ref lastIndex);

        // https://msdn.microsoft.com/en-us/library/system.bitconverter(v=vs.110).aspx
        // uint is 4 bytes (c++ uint is 4 bytes)
        this.playerID = System.BitConverter.ToUInt32(data, lastIndex);
        lastIndex += sizeof(System.UInt32);

    }

    /// <summary>
    /// Serializes data from this event into a byte array
    /// </summary>
    /// <param name="data">The data.</param>
    /// <param name="lastIndex">The last index.</param>
    /// <remarks>
    /// Author: Dustin Yost
    /// </remarks>
    override public void Serialize(ref byte[] data, ref int lastIndex)
    {
        base.Serialize(ref data, ref lastIndex);

        // Write the bytes of the score
        WriteTo(ref data, ref lastIndex, System.BitConverter.GetBytes(this.score));

    }

}
