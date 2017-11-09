using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof( PlayerCharacterController))]
[RequireComponent(typeof(PlayerInputController))]
public class PlayerLocal : PlayerReference
{
    private PlayerCharacterController _pcc;
    private PlayerInputController _pic;

    private void Awake()
    {
        _pcc = GetComponent<PlayerCharacterController>();
        _pic = GetComponent<PlayerInputController>();
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

    public void OnKeyInput(InputDevice device, char c)
    {
        MappedAxis axis = MappedAxis.Horizontal;
        AxisDirection dir = AxisDirection.Centered;
        bool found = true;
        switch (c)
        {
            case 'o':
                axis = MappedAxis.Vertical;
                dir = AxisDirection.Positive;
                break;
            case 'k':
                axis = MappedAxis.Horizontal;
                dir = AxisDirection.Negative;
                break;
            case 'l':
                axis = MappedAxis.Vertical;
                dir = AxisDirection.Negative;
                break;
            case ';':
                axis = MappedAxis.Horizontal;
                dir = AxisDirection.Positive;
                break;
            default:
                found = false;
                break;
        }
        if (found)
        {
            _pic.onAxis(device, InputResponse.UpdateEvent.DOWN, axis, dir);
            _pic.onAxis(device, InputResponse.UpdateEvent.TICK, axis, 1.0f);
        }
    }

}
