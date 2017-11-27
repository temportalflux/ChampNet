using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/**
* Some player's character needs to be updated
 * Client->Server: Request the user update a certain players physics
*/
public class EventRequestMovement : EventWithLocation
{
    
    private float velX;
    private float velY;

    public EventRequestMovement() : base((byte)ChampNetPlugin.MessageIDs.ID_PLAYER_REQUEST_MOVEMENT)
    {
    }

    public EventRequestMovement(uint clientID, uint playerID, float posX, float posY, float velX, float velY) : this()
    {
        this.clientID = clientID;
        this.playerID = playerID;
        this.posX = posX;
        this.posY = posY;
        this.velX = velX;
        this.velY = velY;
    }

    override public int GetSize()
    {
        return base.GetSize() + sizeof(System.Single) * 2; // super + velX,velZ
    }

    override public void Serialize(ref byte[] data, ref int lastIndex)
    {
        base.Serialize(ref data, ref lastIndex);

        WriteTo(ref data, ref lastIndex, System.BitConverter.GetBytes(this.velX));
        WriteTo(ref data, ref lastIndex, System.BitConverter.GetBytes(this.velY));

    }

}
