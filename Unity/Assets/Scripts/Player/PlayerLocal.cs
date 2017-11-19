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

    public override void setInfo(GameState.Player info)
    {
        base.setInfo(info);
        this.GetComponent<InputResponse>().inputId = (int)info.localID + 1;
    }

    public void requestMove()
    {
        GameState.Player info = this.getInfo();

        Vector3 position, deltaMove;
        _pic.Move(out position, out deltaMove);

        Vector3 velocity = _pic._input.normalized * _pcc.speed;

        //Debug.Log("Sending " + position);
        NetInterface.INSTANCE.Dispatch(new EventRequestMovement(
            info.clientID, info.playerID,
            this.moveTarget.position.x, this.moveTarget.position.y,
            velocity.x, velocity.y
        ));
    }

}
