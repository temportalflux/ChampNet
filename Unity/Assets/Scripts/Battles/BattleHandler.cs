﻿using System.Collections;
using System.Collections.Generic;
using JetBrains.Annotations;
using UnityEngine;

/// <summary>
/// 
/// </summary>
/// <author>Jake Ruth</author>
public class BattleHandler : MonoBehaviour
{
    public enum BattleState
    {
        WAITING_ON_INPUT,
        DISPLAYING_TURN
    }

    [Header("Transform Dependencies")]
    public BattleUIController battleUIController;

    /// <summary>
    /// Indicates that the current battle is between networked players
    /// </summary>
    public bool isNetworked;
    
    public BattleParticipant participant1;
    public BattleParticipant participant2;

    /// <summary>
    /// Called To set up the battle scene. Must be called at the start
    /// </summary>
    /// <param name="first">The local player</param>
    /// <param name="second">Other opponent player</param>
    public void SetUpBattle(BattleParticipant first, BattleParticipant second)
    {
        this.participant1 = first;
        this.participant2 = second;
    }

    /// <summary>
    /// Used to send a battle selection
    /// </summary>
    /// <param name="isLocalPlayer">if is <code>true</code> then the selection is from the "local" player (local = main player, not AI, not networked)</param>
    /// <param name="selection">The selection option</param>
    /// <param name="selectionIndex">the index of the selection</param>
    /// <returns>returns <code>False</code> if the selection index is not positive</returns>
    [ToDo("add networked portion here")]
    public bool SendBattleOption(bool isLocalPlayer, GameState.Player.EnumBattleSelection selection, uint selectionIndex)
    {
        if (selectionIndex < 0)
            return false;
        
        // This is a networked battle, so all logic on when players have gone is handled via server
        if (isNetworked)
        {
            // TODO: 1
            //NetInterface.INSTANCE.Dispatch(new EventBattleSelection(-1, -1, selection, selectionIndex));
        }
        // This is a battle between a player and AI, so everything is handled locally
        else
        {
            // The executor is the player
            if (isLocalPlayer)
            {
                this.participant1.selection = selection;
                this.participant1.selectionChoice = (int)selectionIndex - 1;
            }
            // The executor is the AI
            else
            {
                this.participant2.selection = selection;
                this.participant2.selectionChoice = (int)selectionIndex - 1;
            }
            // Both have picked their selection
            if (this.participant1.selectionChoice != -1 && this.participant2.selectionChoice != -1)
            {
                StartCoroutine(HandleResponse(this.participant1, this.participant2));
            }
        }

        return true;
    }

    /// <summary>
    /// Called when both parties have made selections
    /// </summary>
    /// <param name="localSelection">The selection for the local player</param>
    /// <param name="localSelectionIndex">The index for the selection for the local player</param>
    /// <param name="otherSelection">The selection for the other keeper</param>
    /// <param name="otherSelectionIndex">the index for the selection for the other keeper</param>
    [ToDo("handle a death")]
    public IEnumerator HandleResponse(BattleParticipant local, BattleParticipant networkOrAI)
    {
        // Check to see if either selection was to flee
        if (local.selection == GameState.Player.EnumBattleSelection.FLEE)
        {
            // Local keeper fled
            battleUIController.SetFlavorText("You Fled the battle");
            yield return  new WaitForSeconds(2.0f);
        }

        if (networkOrAI.selection == GameState.Player.EnumBattleSelection.FLEE)
        {
            // other keeper fled
            battleUIController.SetFlavorText("Your opponent Fled the battle");
            yield return new WaitForSeconds(2.0f);
        }

        // Check to see if either selection was to switch
        if (local.selection == GameState.Player.EnumBattleSelection.SWAP)
        {
            // local keeper swapped a creiten
            battleUIController.SetFlavorText("You swapped in " + local.currentCretin.GetMonsterName);
            yield return  new WaitForSeconds(2.0f);
        }

        if (networkOrAI.selection == GameState.Player.EnumBattleSelection.SWAP)
        {
            // other keeper swapped a creiten
            battleUIController.SetFlavorText("Your opponent swapped in " + networkOrAI.currentCretin.GetMonsterName);
            yield return new WaitForSeconds(2.0f);
        }

        // Calculate who attacks first
        bool localCreitenIsFaster = local.currentCretin.GetSpeed > networkOrAI.currentCretin.GetSpeed;

        // check to see if either selection was to attack, ordered based on certin speed
        if (localCreitenIsFaster)
        {
            // Check "local" first
            if (local.selection == GameState.Player.EnumBattleSelection.ATTACK)
            {
                ApplyAttack(true, local.selectionChoice);

                battleUIController.SetFlavorText(string.Format("{0} Used {1}", local.currentCretin.GetMonsterName,
                    local.currentCretin.GetAvailableAttacks[local.selectionChoice].attackName));
                yield return new WaitForSeconds(2.0f);

                // check to see if any of the current cretins are dead
                // TODO: 1

            }
            // then check other (network or AI)
            if (networkOrAI.selection == GameState.Player.EnumBattleSelection.ATTACK)
            {
                ApplyAttack(false, networkOrAI.selectionChoice);

                battleUIController.SetFlavorText(string.Format("{0} Used {1}", networkOrAI.currentCretin.GetMonsterName,
                    networkOrAI.currentCretin.GetAvailableAttacks[networkOrAI.selectionChoice].attackName));
                yield return new WaitForSeconds(2.0f);
            }
        }
        else
        {
            if (networkOrAI.selection == GameState.Player.EnumBattleSelection.ATTACK)
            {
                ApplyAttack(false, networkOrAI.selectionChoice);

                battleUIController.SetFlavorText(string.Format("{0} Used {1}", networkOrAI.currentCretin.GetMonsterName,
                    networkOrAI.currentCretin.GetAvailableAttacks[networkOrAI.selectionChoice].attackName));
                yield return new WaitForSeconds(2.0f);
            }

            if (local.selection == GameState.Player.EnumBattleSelection.ATTACK)
            {
                ApplyAttack(true, local.selectionChoice);

                battleUIController.SetFlavorText(string.Format("{0} Used {1}", local.currentCretin.GetMonsterName,
                    local.currentCretin.GetAvailableAttacks[local.selectionChoice].attackName));
                yield return new WaitForSeconds(2.0f);
            }
        }
        
        local.selectionChoice = networkOrAI.selectionChoice = -1;

        battleUIController.menuState = MenuState.MAIN_MENU;

        // Don't need, this doesn't exit the routine, just stalls it
        //yield return null;
    }

    /// <summary>
    /// Apply a move to the current battle
    /// </summary>
    /// <param name="isLocalCretin">if <code>true</code> then the attack came from the local player, as oppossed to a network player or AI</param>
    /// <param name="attackIndex">the attack index to be used</param>
    private void ApplyAttack(bool isLocalCretin, int attackIndex)
    {
        MonsterDataObject localKeeperMonster = this.participant1.currentCretin;
        MonsterDataObject otherKeeperMonster = this.participant2.currentCretin;

        // get the attack
        AttackObject attack = isLocalCretin
            ? localKeeperMonster.GetAvailableAttacks[attackIndex]
            : otherKeeperMonster.GetAvailableAttacks[attackIndex];

        // The damage the attack will ultimately do
        float damage;

        // The level of the monster
        // for our sake, every monster will be level 50 for now
        float level = 50;

        // the power of the attack being used
        float power = attack.power;

        // the effective attack stat of the user
        float attackStat = isLocalCretin
            ? localKeeperMonster.GetMonsterAttackStat(attack.physicalOrSpecial, true)
            : otherKeeperMonster.GetMonsterAttackStat(attack.physicalOrSpecial, true);

        // the effective defense stat of the reciever
        float defenseStat = isLocalCretin
            ? localKeeperMonster.GetMonsterDefenseStat(attack.physicalOrSpecial, true)
            : otherKeeperMonster.GetMonsterDefenseStat(attack.physicalOrSpecial, true);

        // a modifier if the attack would target multiple monsters (I.E. a doubles battle)
        // for our sake, because there is always one target so the value will be 1
        float targets = 1;

        // a modifier if the attack type matches a particlular weather pattern
        // for our sake, the weather is always clear so the value will be 1
        float weather = 1;

        // a modifier if the attack was a critical hit
        // for our sake, the critical hit will be 1
        float critical = 1;

        // a modifier between 0.85 and 1
        // for our sake, the value will be 1
        float random = 1;

        // a modifier for the Same-Type Attack Bonus (STAB)
        // 1.5 if the type of the attack matches one of the user's types, otherwise 1
        float stab = 1;
        foreach (MonsterType type in isLocalCretin ? localKeeperMonster.GetTypes : otherKeeperMonster.GetTypes)
        {
            if (type == attack.type)
            {
                stab = 1.5f;
                break;
            }
        }

        // a modifier to the attack's type effectiveness, the default is 1
        // the value is halfed, doubled, or multiplied by 0 depending on type match ups from the attack type and the other's types.
        float typeAdvantage = 1;
        foreach (MonsterType type in isLocalCretin ? localKeeperMonster.GetTypes : otherKeeperMonster.GetTypes)
        {
            typeAdvantage *= GetTypeEffectivenes(attack.type, type);
        }
        
        // a modifier to half the attack if the user is burned, and is a physical move
        // for our sake, there is no burning so the value is 1
        float burn = 1;

        // a modifier that is affected by very specific attacks, abilities, or items
        // for our sake, the value is 1
        float other = 1;

        float modifier = targets * weather * critical * random * stab * typeAdvantage * burn * other;

        damage = ((2 * level / 5 + 2) * power * attackStat * defenseStat / 50 + 2) * modifier;
        damage = Mathf.Floor(damage);

        // apply damage after calculations
        if (isLocalCretin)
        {
            otherKeeperMonster.CurrentHP -= (int) damage;
            
        }
        else
        {
            localKeeperMonster.CurrentHP -= (int) damage;
            
        }


    }

    private void CretinHasDied(bool isLocalCretin)
    {

    }

    // Type effectiveness match up 2d array hard coded here. 2d arrays dont look nice in unity inspector
    // [attack type][monster type]
    public readonly float[][] typeMatchUp =
    {
        // monster:  normal fire    water   grass    attack 
        new float[] {1.0f,  1.0f,   1.0f,   1.0f}, // normal
        new float[] {1.0f,  1.0f,   0.5f,   2.0f}, // fire
        new float[] {1.0f,  2.0f,   1.0f,   0.5f}, // water
        new float[] {1.0f,  0.5f,   2.0f,   1.0f}, // grass
    };

    /// <summary>
    /// Used to get the type effectiveness match up
    /// </summary>
    /// <param name="attackType"></param>
    /// <param name="monsterType"></param>
    /// <returns></returns>
    private float GetTypeEffectivenes(MonsterType attackType, MonsterType monsterType)
    {
        return typeMatchUp[(int) attackType][(int) monsterType];
    }
}
