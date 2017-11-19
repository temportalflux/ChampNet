﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerReference : MonoBehaviour {

    public Transform sprite;
    public SpriteRenderer overlay;
    public Transform moveTarget;

    private Animator _anim;
    private Vector3 screenPos;

    /// <summary>
    /// Score of the player
    /// </summary>
    private uint score;

    /// <summary>
    /// Current Rank of the player
    /// </summary>
    private uint rank;

    void Awake()
    {
        
    }

    private GameState.Player playerInfo;

    /// <summary>
    /// The player identifier
    /// </summary>
    private uint playerID
    {
        get
        {
            return this.playerInfo.playerID;
        }
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
    /// <remarks>
    /// Author: Christopher Brennan
    /// </remarks>
    public void setScore(uint newScore)
    {
        this.score = newScore;
    }

    /// <summary>
    /// Gets the identifier.
    /// </summary>
    /// <returns> the current number of wins for the player </returns>
    /// <remarks>
    /// Author: Christopher Brennan
    /// </remarks>
    public uint getScore()
    {
        return this.score;
    }
    /// <summary>
    /// sets the new rank of the player for the scoreboard
    /// </summary>
    /// <param name="newRank"></param>
    /// <remarks>
    /// Author: Christopher Brennan
    /// </remarks>
    public void setRank(uint newRank)
    {

    }

    /// <summary>
    /// Gets the rank of the player 
    /// </summary>
    /// <returns> rank of player on scoreboard </returns>
    /// <remarks>
    /// Author: Christopher Brennan
    /// </remarks>
    public uint getRank()
    {
        return rank;
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
    public void integrateInfo(GameState.Player playerInfo, bool forceSnap = false)
    {
        this.playerInfo = playerInfo;
        
        this.moveTarget.position = this.playerInfo.position;
        if (forceSnap)
        {
            this.transform.position = this.moveTarget.position;
        }
        
        //this.sprite.rotation = Quaternion.Euler(0, 0, rotZ);

        this.overlay.color = this.playerInfo.color;

        if (this._anim != null) // can happen during instantiation of object (via setInfo)
        {
            _anim.SetFloat("velX", this.playerInfo.velocity.x);
            _anim.SetFloat("velY", this.playerInfo.velocity.y);
            _anim.SetBool("walking", Mathf.Abs(this.playerInfo.velocity.x) > 0.001f || Mathf.Abs(this.playerInfo.velocity.y) > 0.001f);
        }
        else
        {
            _anim = GetComponentInChildren<Animator>();
        }
    }

    public GameState.Player getInfo()
    {
        return this.playerInfo;
    }

    virtual public void setInfo(GameState.Player info)
    {
        this.integrateInfo(info, true);
    }

    protected virtual void Update()
    {
        this.screenPos = Camera.main.WorldToScreenPoint(transform.position);
        

    }

    private void OnGUI()
    {
        //GUI.Label(new Rect(screenPos.x, screenPos.y, 100, 50), this.playerInfo.name);
    }

    private void FixedUpdate()
    {

        // Integrate all physics
        this.playerInfo.integratePhysics(Time.deltaTime);
        this.moveTarget.position = this.playerInfo.position;

        this.transform.position = Vector3.Lerp(this.transform.position, this.moveTarget.position, 0.1f);

    }

}
