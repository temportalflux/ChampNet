using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerReference : MonoBehaviour {

    public Transform sprite;

    private uint playerID;
    
    public void setID(uint id)
    {
        this.playerID = id;
    }

    public uint getID()
    {
        return this.playerID;
    }
 
    public void updateAt(float posX, float posY, float rotZ)
    {
        this.transform.position = new Vector3(posX, posY);
        this.transform.rotation = Quaternion.Euler(0, 0, rotZ);
    }

    virtual public EventNetwork createUpdateEvent()
    {
        return null;
    }

}
