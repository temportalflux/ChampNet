using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/**
 * Event: A battle has finished and the entire world needs to know about it
 */
public class EventBattleResult : EventBattle
{

    [BitSerialize(3)]
    public uint playerIDWinner;

    public EventBattleResult() : base((byte)ChampNetPlugin.MessageIDs.ID_BATTLE_RESULT)
    {
    }

    public override void Execute()
    {
        Debug.Log("Battle between " + this.idSender + " and " + this.idReceiver + " was won by " + this.playerIDWinner);
        GameState.Player player = GameManager.INSTANCE.state.players[this.playerIDWinner];
        player.wins++;
    }

}
