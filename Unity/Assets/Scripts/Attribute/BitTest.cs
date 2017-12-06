using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BitTest : MonoBehaviour {

    [BitSerialize(0)]
    public int intVar;

    [BitSerialize(1)]
    public float floatVar;

    [BitSerialize(2)]
    public bool[] arrayTest;

    [BitSerialize(3)]
    public string testString;

    [BitSerialize(4)]
    public GameState.Player player;

    protected virtual void Start()
    {
        this.init();

        byte[] data = BitSerializing.Serialize(this);

        this.clear();

        BitSerializing.Deserialize(this, data);

        this.report();

    }

    protected virtual void init()
    {

        this.intVar = 22;
        this.floatVar = 30.5f;
        this.arrayTest = new bool[] { false, true, true, false, true };
        this.testString = "hello world";
        this.player = new GameState.Player();
        this.player.clientID = 25;
        this.player.color = Color.red;

    }

    protected virtual void clear()
    {

        this.intVar = 0;
        this.floatVar = 0;
        this.arrayTest = null;
        this.testString = null;
        this.player = new GameState.Player();

    }

    protected virtual void report()
    {

        Debug.Log(this.intVar);
        Debug.Log(this.floatVar);
        Debug.Log(this.testString);
        Debug.Log(this.player.clientID);
        Debug.Log(this.player.color);

    }
	
}
