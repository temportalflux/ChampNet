using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EventBattleResultResponse : EventNetwork
{

    [BitSerialize]
    public uint playerID;

    public EventBattleResultResponse() : base((byte)ChampNetPlugin.MessageIDs.ID_BATTLE_RESULT_RESPONSE)
    {

    }

}
