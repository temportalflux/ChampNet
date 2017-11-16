using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/**
 * Client->Server: Tell server that we joined
 * Server->Client: Tell the client of its ID and first playerID
 */
public class EventClientJoined : EventNetwork
{

    public EventClientJoined() : base((byte)ChampNetPlugin.MessageIDs.ID_CLIENT_JOINED)
    {
    }

    public override void Deserialize(byte[] data, ref int lastIndex)
    {
        base.Deserialize(data, ref lastIndex);
        GameManager.INSTANCE.state.Deserialize(data, ref lastIndex);
    }

}
