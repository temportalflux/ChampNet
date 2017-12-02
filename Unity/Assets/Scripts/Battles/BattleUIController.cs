// Author: Jake Ruth
using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public enum MenuState
{
    MAIN_MENU,
    ATTACK_MENU,
    ITEM_MENU,
    SWITCH_MENU,
    FORFEIT_MENU,
    WAITING
}

public class BattleUIController : MonoBehaviour
{
    private MenuState _menuState;

    public MenuState menuState
    {
        get { return _menuState; }
        set
        {
            _menuState = value;

            mainMenuGameObject.SetActive(_menuState == MenuState.MAIN_MENU);
            attackMenuGameObject.SetActive(_menuState == MenuState.ATTACK_MENU);
            itemsMenuGameObject.SetActive(_menuState == MenuState.ITEM_MENU);
            switchMenuGameObject.SetActive(_menuState == MenuState.SWITCH_MENU);
            forfeitMenuGameObject.SetActive(_menuState == MenuState.FORFEIT_MENU);
            waitingGameObject.SetActive(_menuState == MenuState.WAITING);

            switch (_menuState)
            {
                case MenuState.MAIN_MENU:
                    break;
                case MenuState.ATTACK_MENU:
                    List<AttackObject> availableAttacks = battleHandler.localKeeper.monsters[battleHandler.localCreitenIndex].GetAvailableAttacks;
                    for (int i = 0; i < attackButtons.Length; i++)
                    {
                        bool hasMatchingAttack = i < availableAttacks.Count;
                        attackButtons[i].interactable = hasMatchingAttack;
                        if (hasMatchingAttack)
                        {
                            attackButtons[i].GetComponentInChildren<Text>().text = availableAttacks[i].attackName;
                        }
                    }
                    break;
                case MenuState.ITEM_MENU:
                    break;
                case MenuState.SWITCH_MENU:
                    List<MonsterDataObject> availableMonsters;
                    availableMonsters = battleHandler.localKeeper.monsters;
                    for (int i = 0; i < creitenButtons.Length; i++)
                    {
                        bool hasMatchingMonster = i < availableMonsters.Count;
                        creitenButtons[i].interactable = hasMatchingMonster;
                        if (hasMatchingMonster)
                        {
                            creitenButtons[i].GetComponentInChildren<Text>().text = availableMonsters[i].GetMonsterName;
                        }
                        else
                        {
                            creitenButtons[i].GetComponentInChildren<Text>().text = "N/A";
                        }
                    }
                    break;
                case MenuState.FORFEIT_MENU:
                    break;
                case MenuState.WAITING:
                    break;
                default:
                    throw new ArgumentOutOfRangeException();
            }
        }
    }

    [Header("Transform Dependencies")]
    public BattleHandler battleHandler;

    [Header("Base menu holders")]
    public GameObject mainMenuGameObject;
    public GameObject attackMenuGameObject;
    public GameObject itemsMenuGameObject;
    public GameObject switchMenuGameObject;
    public GameObject forfeitMenuGameObject;
    public GameObject waitingGameObject;

    [Header("Attack Variables")]
    public Button[] attackButtons;

    [Header("Switch Variables")]
    public Button[] creitenButtons;

    //[Header("Items Variables")]
    // coming soon. HA....

    //[Header("Forfeit Variables")]

    [Header("Waiting Variables")]
    public Text waitingText;

    void Start()
    {
        menuState = MenuState.MAIN_MENU;
    }

    public void ButtonClicked(int buttonIndex)
    {
        switch (menuState)
        {
            case MenuState.MAIN_MENU:

                switch (buttonIndex)
                {
                    case 1:
                        menuState = MenuState.ATTACK_MENU;
                        break;
                    case 2:
                        menuState = MenuState.SWITCH_MENU;
                        break;
                    case 3:
                        menuState = MenuState.ITEM_MENU;
                        break;
                    case 4:
                        menuState = MenuState.FORFEIT_MENU;
                        break;
                }

                break;
            case MenuState.ATTACK_MENU:
                // **** selected an attack
                menuState = MenuState.WAITING;

                break;
            case MenuState.ITEM_MENU:
                // Eventually do something... maybe.

                break;
            case MenuState.SWITCH_MENU:
                // **** selected a creiten to switch to
                menuState = MenuState.WAITING;

                break;
            case MenuState.FORFEIT_MENU:
                switch (buttonIndex)
                {
                    case 1:
                        break;
                    case 2:
                        BackButtonClicked();
                        break;
                }
                break;
            case MenuState.WAITING:
                break;
            default:
                throw new ArgumentOutOfRangeException();
        }
    }

    public void BackButtonClicked()
    {
        if (menuState != MenuState.WAITING)
            menuState = MenuState.MAIN_MENU;
    }
}
