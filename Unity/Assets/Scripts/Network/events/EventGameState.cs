using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/// <summary>
/// Event: A gamestate update
/// </summary>
/// <remarks>
/// Author: Dustin Yost
/// </remarks>
public class EventGameState : EventNetwork
{

    public EventGameState(ChampNetPlugin.MessageIDs message) : base((byte)message)
    {
    }

    /// <summary>
    /// Returns the size of the packet (the size for a potential byte[])
    /// </summary>
    /// <returns>
    /// the integer length of a byte array to hold this event's data
    /// </returns>
    /// <remarks>
    /// Author: Dustin Yost
    /// </remarks>
    override public int GetSize()
    {
        return base.GetSize() + GameManager.INSTANCE.state.GetSize(); // super + playerID
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
        GameManager.INSTANCE.state.Serialize(ref data, ref lastIndex);
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
        GameManager.INSTANCE.state.Deserialize(data, ref lastIndex);
        
        // TODO: handle players which have been added
        // TODO: handle players which are no longer in existance

    }

    /// <summary>
    /// Processes this event to affect the actual environment
    /// </summary>
    /// <remarks>
    /// Author: Dustin Yost
    /// </remarks>
    override public void Execute()
    {
        
    }

}
