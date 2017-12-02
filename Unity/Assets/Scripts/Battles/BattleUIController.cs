using System.Collections;
using System.Collections.Generic;
using UnityEngine;

// Author: Jake Ruth
public class BattleUIController : MonoBehaviour
{
    public enum MenuState
    {
        MAIN_MENU,
        ATTACK_MENU,
        ITEM_MENU,
        SWITCH_MENU,
        FORFEIT_MENU,
        WAITING
    }

    public MenuState menuState;

}
