using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EventBattlePromptSelection : EventNetwork
{

    [BitSerialize]
    public uint playerAID;

    [BitSerialize]
    public int _playerASelection;

    public EventBattleSelection.EnumSelection playerASelection
    {
        get
        {
            return (EventBattleSelection.EnumSelection)this._playerASelection;
        }
    }

    [BitSerialize]
    public int playerAChoice;

    [BitSerialize]
    public uint playerBID;

    [BitSerialize]
    public int _playerBSelection;

    public EventBattleSelection.EnumSelection playerBSelection
    {
        get
        {
            return (EventBattleSelection.EnumSelection)this._playerBSelection;
        }
    }

    [BitSerialize]
    public int playerBChoice;

    public EventBattlePromptSelection() : base((byte)ChampNetPlugin.MessageIDs.ID_BATTLE_PROMPT_SELECTION)
    {
    }

    public override void Execute()
    {
        base.Execute();

    }
}
