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

    /// <summary>
    /// The player identifier
    /// </summary>
    private uint playerID;

    /// <summary>
    /// Score of the player
    /// </summary>
    private uint score;

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
    /// Sets the identifier.
    /// </summary>
    /// <param name="newScore">The identifier.</param>
    public void setScore(uint newScore)
    {
        this.score = newScore;
    }

    /// <summary>
    /// Gets the identifier.
    /// </summary>
    /// <returns></returns>
    public uint getScore()
    {
        return this.score;
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
    public void updateAt(float posX, float posY, float velX, float velY)
    {
        this.transform.position = new Vector3(posX, posY);
        //this.sprite.rotation = Quaternion.Euler(0, 0, rotZ);

        _anim.SetFloat("velX", velX);
        _anim.SetFloat("velY", velY);
        _anim.SetBool("walking", Mathf.Abs(velX) > 0.001f && Mathf.Abs(velY) > 0.001f);

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

}
