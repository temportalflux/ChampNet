using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerReference : MonoBehaviour {

    public Transform sprite;
    private Animator _anim;

    void Awake()
    {
        _anim = GetComponentInChildren<Animator>();
    }

    private GameState.Player playerInfo;

    /// <summary>
    /// The player identifier
    /// </summary>
    private uint playerID;

    /// <summary>
    /// Sets the identifier.
    /// </summary>
    /// <param name="id">The identifier.</param>
    public void setID(uint id)
    {
        this.playerID = id;
    }

    /// <summary>
    /// Gets the identifier.
    /// </summary>
    /// <returns></returns>
    public uint getID()
    {
        return this.playerID;
    }

    /// <summary>
    /// Update the object to have some transform properties
    /// </summary>
    /// <param name="posX">The position x.</param>
    /// <param name="posY">The position y.</param>
    /// <param name="velX">The rot z.</param>
    /// <param name="velY"></param>
    /// <remarks>
    /// Author: Dustin Yost
    /// </remarks>
    public void updateFromInfo()
    {
        this.transform.position = this.playerInfo.position;
        //this.sprite.rotation = Quaternion.Euler(0, 0, rotZ);

        _anim.SetFloat("velX", this.playerInfo.velocity.x);
        _anim.SetFloat("velY", this.playerInfo.velocity.y);
        _anim.SetBool("walking", Mathf.Abs(this.playerInfo.velocity.x) > 0.001f && Mathf.Abs(this.playerInfo.velocity.y) > 0.001f);

    }

    /// <summary>
    /// Creates the update event (the event for telling other clients where this object is).
    /// </summary>
    /// <returns></returns>
    /// <remarks>
    /// Author: Dustin Yost
    /// </remarks>
    virtual public EventNetwork createUpdateEvent()
    {
        return new EventNetwork.EventUpdatePosition(
            this.getID(),
            this.transform.position.x,
            this.transform.position.y,
            0,0
        );
    }

    public GameState.Player getInfo()
    {
        return this.playerInfo;
    }

    public void initInfo(GameState.Player info)
    {
        this.playerInfo = info;

        // ID already set, init other identification info
        this.playerInfo.name = "Unammed";
        this.playerInfo.color = Color.white;

        this.playerInfo.inBattle = false;

        this.playerInfo.objectReference = this;
    }

}
