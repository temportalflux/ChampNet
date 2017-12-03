using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/**
     * Event: Some player requests battle with another
     */
public class EventBattleRequest : EventBattle
{

    public EventBattleRequest() : base((byte)ChampNetPlugin.MessageIDs.ID_BATTLE_REQUEST) { }

    public EventBattleRequest(uint sender, uint receiver) : this()
    {
        this.idSender = sender;
        this.idReceiver = receiver;
    }

    public override void Execute()
    {
        // Some user (requester) has asked us (receiver) to battle
        Debug.Log("Received request to battle from " + this.idSender + "... auto accepting");
        NetInterface.INSTANCE.Dispatch(new EventBattleResponse(this.idReceiver, this.idSender, true));
    }

}
