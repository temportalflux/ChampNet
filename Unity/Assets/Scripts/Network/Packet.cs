using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Packet {

    // The packet identifier
    public uint id;

    /** Author: Dustin Yost
     * Returns the total size of all data in this packet
     */
    public uint getSize()
    {
        return sizeof(uint);
    }

    /** Author: Dustin Yost
     * Writes all data to the data stream, starting at start
     * When finished, end will equal start + getSize()
     */
    public void write(byte[] data, int start, out int end)
    {
        end = start;

        // Write the int to the byte array (increments end to the new end of the data)
        this.write(data, System.BitConverter.GetBytes(this.id), ref end);

    }

    /** Author: Dustin Yost
     * Writes a byte[] of data starting at 'end'
     */
    private void write(byte[] data, byte[] toWrite, ref int end)
    {
        // copy the source to the data
        // copies all of toWrite [0, length) to data [end, end + length)
        System.Array.Copy(toWrite, 0, data, end, toWrite.Length);
        end += toWrite.Length;
    }

    /** Author: Dustin Yost
     * Reads all data for this packet from data, starting at start.
     * When finished, end will equal start + getSize()
     */
    public void read(byte[] data, int start, out int end)
    {
        end = start;

        this.id = System.BitConverter.ToUInt32(data, end);
        end += sizeof(uint);

    }

    /** Author: Dustin Yost
     * Saves all data to a new byte[], and returns that byte array
     */
    public byte[] save()
    {
        // create an area to store data
        byte[] data = new byte[this.getSize()];
        int end;
        // Write data
        this.write(data, 0, out end);
        // Return the data
        return data;
    }

    /** Author: Dustin Yost
     * Reads data from start to end of the byte array
     */
    public void load(byte[] data)
    {
        int end;
        this.read(data, 0, out end);
    }
	
}
