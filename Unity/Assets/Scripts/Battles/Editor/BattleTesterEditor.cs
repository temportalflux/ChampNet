using System.Collections;
using System.Collections.Generic;
using NUnit.Framework.Internal.Commands;
using UnityEditor;
using UnityEngine;

// Author: Jake Ruth
[CustomEditor(typeof(BattleTester))]
public class BattleTesterEditor : Editor
{
    public override void OnInspectorGUI()
    {
        base.OnInspectorGUI();
        BattleTester t = target as BattleTester;

        GUILayout.Space(10f);

        if (t == null)
            return;

        if (!t.IsBattleSetup)
        {
            if (GUILayout.Button("Setup Battle Handler"))
            {
                if (t.localPlayerTest == null || t.otherPlayerTest == null)
                    return;

                foreach (MonsterDataObject o in t.localPlayerTest.monsters)
                {
                    o.Heal();
                }

                foreach (MonsterDataObject o in t.otherPlayerTest.monsters)
                {
                    o.Heal();
                }

                t.battleHandler.SetUpBattle(t.localPlayerTest, t.otherPlayerTest);
                t.IsBattleSetup = true;
            }
        }
        else
        {
            if (GUILayout.Button("Button 1"))
            {
                t.battleUIController.ButtonClicked(1);
            }

            if (GUILayout.Button("Button 2"))
            {
                t.battleUIController.ButtonClicked(2);
            }

            if (GUILayout.Button("Button 3"))
            {
                t.battleUIController.ButtonClicked(3);
            }

            if (GUILayout.Button("Button 4"))
            {
                t.battleUIController.ButtonClicked(4);
            }

            if (GUILayout.Button("Button 5"))
            {
                t.battleUIController.ButtonClicked(5);
            }

            if (GUILayout.Button("Button 6"))
            {
                t.battleUIController.ButtonClicked(6);
            }

            if (GUILayout.Button("Back"))
            {
                t.battleUIController.BackButtonClicked();
            }

            GUILayout.Space(10.0f);

            if (GUILayout.Button("Other selects random attack"))
            {
                uint selectionIndex = (uint)Random.Range(0,
                    t.battleHandler.OtherKeeper.monsters[t.battleHandler.OtherCretinIndex].GetAvailableAttacks
                        .Count);

                t.battleHandler.SendBattleOption(false, GameState.Player.EnumBattleSelection.ATTACK, selectionIndex);
            }

            if (GUILayout.Button("Other sends out random cretin"))
            {
                int whileCount = 0;
                while (whileCount < 20)
                {
                    uint selectionIndex = (uint)Random.Range(0, t.battleHandler.OtherKeeper.monsters.Count);

                    if (t.battleHandler.OtherKeeper.monsters[(int)selectionIndex].CurrentHP > 0)
                    {
                        if (selectionIndex != t.battleHandler.OtherCretinIndex)
                        {
                            t.battleHandler.SendBattleOption(false, GameState.Player.EnumBattleSelection.SWAP, selectionIndex);
                            break;
                        }
                    }
            
                    whileCount++;
                }

                if(whileCount >= 20)
                    Debug.Log("No swap made");
            }
        }
    }
}
