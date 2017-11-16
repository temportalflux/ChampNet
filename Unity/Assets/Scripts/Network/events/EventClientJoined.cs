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

}
