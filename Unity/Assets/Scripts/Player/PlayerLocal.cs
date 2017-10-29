using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerLocal : PlayerReference
{

    void Start()
    {
        GameManager.INSTANCE.setPlayer(this);
    }

}
