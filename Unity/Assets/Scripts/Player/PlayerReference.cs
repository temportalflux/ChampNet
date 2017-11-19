using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerReference : MonoBehaviour {

    public Transform sprite;
    public SpriteRenderer overlay;
    public Transform moveTarget;

    private Animator _anim;
    //private Vector3 screenPos;

    /// <summary>
    /// Score of the player
    /// </summary>
    public uint score
    {
        get
        {
            return this.playerInfo.wins;
        }
        set
        {
            this.playerInfo.wins = value;
        }
    }

    /// <summary>
    /// Current Rank of the player
    /// </summary>
    public uint rank
    {
        get
        {
            return this.playerInfo.rank;
        }
        set
        {
            this.playerInfo.rank = value;
        }
    }

    void Awake()
    {
        
    }

    protected GameState.Player playerInfo;

    /// <summary>
    /// The player identifier
    /// </summary>
    private uint playerID
    {
        get
        {
            return this.playerInfo.playerID;
        }
        set
        {
            this.playerInfo.playerID = value;
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

    public void setID(uint id)
    {
        this.playerID = id;
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
        //this.screenPos = Camera.main.WorldToScreenPoint(transform.position);
        

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

    public virtual void OnLocalInput(Vector3 position, Vector3 deltaMove)
    {
    }

}
