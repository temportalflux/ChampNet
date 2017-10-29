using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerReference : MonoBehaviour {
    
    private uint playerID;
    
    public void setID(uint id)
    {
        this.playerID = id;
    }

    public uint getID()
    {
        return this.playerID;
    }
 
    public void updateAt(float posX, float posY)
    {
        this.transform.position = new Vector3(posX, posY);
    }

}
