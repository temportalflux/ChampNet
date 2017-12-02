using System.Collections;
using System.Collections.Generic;
using UnityEngine;

// Author: Jake Ruth
public class BattleHandler : MonoBehaviour
{
    public enum BattleState
    {
        WAITING_ON_INPUT,
        DISPLAYING_TURN
    }

    public KeeperSystem localKeeper;
    public int localCreitenIndex;

    public KeeperSystem otherKeeper;
    public int otherCreitenIndex;

    public void SetUpBattle(KeeperSystem playerA, KeeperSystem playerB)
    {
        localKeeper = playerA;
        localCreitenIndex = 0;

        otherKeeper = playerB;
        otherCreitenIndex = 0;
    }
}
