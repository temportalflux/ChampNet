using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

[CustomEditor(typeof(GameState))]
public class GameStateEditor : Editor
{

    public override void OnInspectorGUI()
    {
        //base.OnInspectorGUI();

        GameState gameState = this.target as GameState;

        // Draw script line
        GUI.enabled = false;
        EditorGUILayout.ObjectField(
            "Script",
            MonoScript.FromScriptableObject(gameState),
            typeof(MonoScript), false
        );
        GUI.enabled = true;

        EditorGUILayout.PropertyField(this.serializedObject.FindProperty("starters"), true);

        gameState.editorFoldoutPlayers = EditorGUILayout.Foldout(gameState.editorFoldoutPlayers, "Players");
        if (gameState.editorFoldoutPlayers)
        {
            EditorGUI.indentLevel++;

            // Draw players list
            foreach (uint id in gameState.players.Keys)
            {
                GameState.Player player = gameState.players[id];
                if (!gameState.editorFoldouts.ContainsKey(id))
                    gameState.editorFoldouts[id] = false;

                gameState.editorFoldouts[id] = EditorGUILayout.Foldout(gameState.editorFoldouts[id], "Player " + id);
                if (gameState.editorFoldouts[id])
                {
                    EditorGUI.indentLevel++;
                    GUI.enabled = false;

                    EditorGUILayout.IntField("ID", (int)player.playerID);

                    EditorGUILayout.TextField("Name", player.name);

                    EditorGUILayout.ColorField("Color", player.color);

                    EditorGUILayout.Vector3Field("Position", player.position);

                    EditorGUILayout.Vector3Field("Velocity", player.velocity);

                    EditorGUILayout.Vector3Field("Accelleration", player.accelleration);

                    EditorGUILayout.Toggle("In Battle", player.inBattle);

                    EditorGUILayout.LabelField("Cretins #" + player.monsters.Count);
                    foreach (MonsterDataObject mdo in player.monsters)
                    {
                        //EditorGUILayout.ObjectField("Cretin", mdo, typeof(MonsterDataObject), true);
                        EditorGUILayout.LabelField("Cretin " + mdo.GetMonsterName);
                    }

                    GUI.enabled = true;
                    EditorGUI.indentLevel--;
                }

            }

            EditorGUI.indentLevel--;
        }

        // Save changed properties
        this.serializedObject.ApplyModifiedProperties();

    }

}
