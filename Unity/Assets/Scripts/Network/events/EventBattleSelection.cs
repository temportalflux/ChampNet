using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EventBattleSelection : EventBattle
{

    public enum EnumSelection
    {
        ATTACK, SWAP, FLEE,
    }
    
    public uint playerId
    {
        get
        {
            return this.idSender;
        }
    }
    
    public uint playerIdOpponent
    {
        get
        {
            return this.idReceiver;
        }
    }

    [BitSerialize]
    public uint _selection;

    public EnumSelection selection
    {
        set
        {
            this._selection = (uint)value;
        }
    }

    [BitSerialize]
    public uint choice;

    public EventBattleSelection() : base((byte)ChampNetPlugin.MessageIDs.ID_BATTLE_SELECTION)
    {
    }

}
