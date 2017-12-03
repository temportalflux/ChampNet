using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EventBattle : EventNetwork
{

    [BitSerialize]
    public uint idSender;

    [BitSerialize]
    public uint idReceiver;

    public EventBattle(byte id) : base(id)
    {
    }

}

