using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BitTest2 : BitTest
{

    [BitSerialize]
    public string bitTest2Str;

    protected override void init()
    {
        base.init();
        this.bitTest2Str = "Inehireted string";
    }

    protected override void clear()
    {
        base.clear();

        this.bitTest2Str = null;
    }

    protected override void report()
    {
        base.report();
        Debug.Log(this.bitTest2Str);
    }

}
