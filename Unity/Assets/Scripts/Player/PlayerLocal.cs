using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerLocal : PlayerReference
{

    void Start()
    {
        GameManager.INSTANCE.setPlayer(this);
    }

    public void challengeRandomPlayer()
    {
        PlayerReference player = GameManager.INSTANCE.getRandomPlayer();
        if (player != null)
        {
            Debug.Log("Requesting battle from player " + player.getID());
            this.requestBattle(player);
        }

    }

    public void requestBattle(PlayerReference player)
    {
        NetInterface.INSTANCE.getEvents().Dispatch(new EventNetwork.EventBattleRequest(this.getID(), player.getID()));
    }

}
