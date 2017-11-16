using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof( PlayerCharacterController))]
[RequireComponent(typeof(PlayerInputController))]
public class PlayerLocal : PlayerReference
{
    protected PlayerCharacterController _pcc;
    protected PlayerInputController _pic;

    private void Awake()
    {
        _pcc = GetComponent<PlayerCharacterController>();
        _pic = GetComponent<PlayerInputController>();
    }

    void Start()
    {
        //if (GameManager.INSTANCE != null)
        //GameManager.INSTANCE.AddPlayerLocal(this);
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

    /*
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
    //*/

    public override void setInfo(GameState.Player info)
    {
        base.setInfo(info);
        this.GetComponent<InputResponse>().inputId = (int)info.localID + 1;
    }

}
