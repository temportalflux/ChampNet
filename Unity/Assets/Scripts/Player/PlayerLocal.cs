using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof( PlayerCharacterController))]
public class PlayerLocal : PlayerReference
{
    private PlayerCharacterController _pcc;

    private void Awake()
    {
        _pcc = GetComponent<PlayerCharacterController>();
    }

    void Start()
    {
        if (GameManager.INSTANCE != null)
        GameManager.INSTANCE.setPlayer(this);
    }

    /// <summary>
    /// Challenges the random player to a networked battle
    /// </summary>
    /// <remarks>
    /// Author: Dustin Yost
    /// </remarks>
    public void challengeRandomPlayer()
    {
        PlayerReference player = GameManager.INSTANCE.getRandomPlayer();
        if (player != null)
        {
            //Debug.Log("Requesting battle from player " + player.getID());
            this.requestBattle(player);
        }

    }

    /// <summary>
    /// Notifies the server of a battle request
    /// </summary>
    /// <param name="player">The player.</param>
    /// <remarks>
    /// Author: Dustin Yost
    /// </remarks>
    public void requestBattle(PlayerReference player)
    {
        NetInterface.INSTANCE.Dispatch(new EventNetwork.EventBattleRequest(this.getID(), player.getID()));
    }

    public override EventNetwork createUpdateEvent()
    {
        return new EventNetwork.EventUpdatePosition(
            this.getID(),
            this.transform.position.x,
            this.transform.position.y,
            _pcc.deltaMove.x,
            _pcc.deltaMove.y
        );
    }
}
