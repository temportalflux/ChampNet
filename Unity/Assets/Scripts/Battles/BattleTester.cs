using System.Collections;
using System.Collections.Generic;
using UnityEngine;

// Author: Jake Ruth
public class BattleTester : MonoBehaviour
{
    public BattleHandler battleHandler;
    public BattleUIController battleUIController;

    public KeeperSystem localPlayerTest;
    public KeeperSystem otherPlayerTest;

    [HideInInspector] public bool isBattleSetup;

    void Start()
    {
        isBattleSetup = false;
    }
}
