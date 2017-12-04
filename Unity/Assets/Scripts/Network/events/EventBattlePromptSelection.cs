using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EventBattlePromptSelection : EventNetwork
{

    [BitSerialize]
    public uint playerAID;

    [BitSerialize]
    public int _playerASelection;

    public GameState.Player.EnumBattleSelection playerASelection
    {
        get
        {
            return (GameState.Player.EnumBattleSelection)this._playerASelection;
        }
    }

    [BitSerialize]
    public int playerAChoice;

    [BitSerialize]
    public uint playerBID;

    [BitSerialize]
    public int _playerBSelection;

    public GameState.Player.EnumBattleSelection playerBSelection
    {
        get
        {
            return (GameState.Player.EnumBattleSelection)this._playerBSelection;
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
