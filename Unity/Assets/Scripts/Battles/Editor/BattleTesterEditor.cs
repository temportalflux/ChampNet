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

        if (!t.isBattleSetup)
        {
            if (GUILayout.Button("Setup Battle Handler"))
            {
                if(t.localPlayerTest != null && t.otherPlayerTest != null)
                {
                    t.battleHandler.SetUpBattle(t.localPlayerTest, t.otherPlayerTest);
                    t.isBattleSetup = true;
                }
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

            if (GUILayout.Button("Other selects random attack"))
            {
                
            }
        }
    }
}
