using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/**
     * A base event to (de)serialize a (float,float) location
     */
public class EventWithLocation : EventWithPlayerID
{

    protected float posX, posY;

    public EventWithLocation(byte id) : base(id)
    {
    }

    override public int GetSize()
    {
        return base.GetSize() + (sizeof(System.Single) * 2); // super + posX + posY
    }

    override public void Deserialize(byte[] data, ref int lastIndex)
    {
        base.Deserialize(data, ref lastIndex);

        // https://msdn.microsoft.com/en-us/library/system.bitconverter(v=vs.110).aspx
        // float(single) is 4 bytes (c++ float is 4 bytes)
        this.posX = System.BitConverter.ToSingle(data, lastIndex);
        lastIndex += sizeof(System.Single);
        this.posY = System.BitConverter.ToSingle(data, lastIndex);
        lastIndex += sizeof(System.Single);


    }

    override public void Serialize(ref byte[] data, ref int lastIndex)
    {
        base.Serialize(ref data, ref lastIndex);

        WriteTo(ref data, ref lastIndex, System.BitConverter.GetBytes(this.posX));
        WriteTo(ref data, ref lastIndex, System.BitConverter.GetBytes(this.posY));

    }

}
