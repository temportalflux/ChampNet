using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EventBattlePromptSelection : EventNetwork
{

    [BitSerialize(1)]
    public uint playerAID;

    [BitSerialize(2)]
    public int _playerASelection;

    public GameState.Player.EnumBattleSelection playerASelection
    {
        get
        {
            return (GameState.Player.EnumBattleSelection)this._playerASelection;
        }
    }

    [BitSerialize(3)]
    public int playerAChoice;

    [BitSerialize(4)]
    public uint playerBID;

    [BitSerialize(5)]
    public int _playerBSelection;

    public GameState.Player.EnumBattleSelection playerBSelection
    {
        get
        {
            return (GameState.Player.EnumBattleSelection)this._playerBSelection;
        }
    }

    [BitSerialize(6)]
    public int playerBChoice;

    public EventBattlePromptSelection() : base((byte)ChampNetPlugin.MessageIDs.ID_BATTLE_PROMPT_SELECTION)
    {
    }

    public override void Execute()
    {
        base.Execute();
        
    }

}
