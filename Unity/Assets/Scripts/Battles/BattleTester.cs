using System.Collections;
using System.Collections.Generic;
using UnityEngine;

// Author: Jake Ruth
public class BattleTester : MonoBehaviour
{
    public BattleHandler battleHandler;
    public BattleUIController battleUIController;

    public PlayerReference localPlayerTest;
    public PlayerReference otherPlayerTest;

    protected bool isBattleSetup;

    public bool IsBattleSetup
    {
        get { return isBattleSetup; }
        set { isBattleSetup = value; }
    }

    void Start()
    {
        IsBattleSetup = false;
    }
}
