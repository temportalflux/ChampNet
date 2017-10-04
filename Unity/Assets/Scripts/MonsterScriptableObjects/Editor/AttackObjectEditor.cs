/* Author: Jake Ruth
 */

using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

[CustomEditor(typeof(AttackObject))] // used to create a custom inspector editor of type AttackObject
public class AttackObjectEditor : Editor
{
    public override void OnInspectorGUI()
    {
        // Get the target
        AttackObject ao = target as AttackObject;

        // If the target is null for some reason, return
        if (ao == null)
            return;
        
        // Display each of the 
        ao.attackName = EditorGUILayout.TextField("Attack Name:", ao.attackName);
        ao.type = (MonsterType)EditorGUILayout.EnumPopup("Attack Type", ao.type);

        EditorGUILayout.Space(); // used to create spacing in the inspector

        ao.doesDamage = EditorGUILayout.Toggle("Does Damage?", ao.doesDamage);

        if (ao.doesDamage) // only display the following if the attack does damage
        {
            ao.power = EditorGUILayout.IntField("   Power", ao.power);
        }

        EditorGUILayout.Space(); // used to create spacing in the inspector

        ao.doesModifiesStats = EditorGUILayout.Toggle("Does Modify Stats?", ao.doesModifiesStats);

        if (ao.doesModifiesStats) // only display the following if the attack modifies a stat
        {
            ao.doesModifyOpponentStats = EditorGUILayout.Toggle("   Does Modify Opponent Stats?", ao.doesModifyOpponentStats); // if true, affect opponent, else affect user
            ao.modifyAttackValue = EditorGUILayout.IntField("   Attack Modifier", ao.modifyAttackValue);
            ao.modifyDefenseValue = EditorGUILayout.IntField("   Defense Modifier", ao.modifyDefenseValue);
            ao.modifySpecialAttackValue = EditorGUILayout.IntField("   Special Attack Modifier", ao.modifySpecialAttackValue);
            ao.modifySpecialDefenseValue = EditorGUILayout.IntField("   Defense Modifier", ao.modifySpecialDefenseValue);
            ao.modifySpeedValue = EditorGUILayout.IntField("   Speed Modifier", ao.modifySpeedValue);
            ao.modifyAccuracyValue = EditorGUILayout.IntField("   Accuracy Modifier", ao.modifyAccuracyValue);
        }

        EditorGUILayout.Space(); // used to create spacing in the inspector

        ao.accuracy = EditorGUILayout.IntField("Accuracy", ao.accuracy);
        ao.powerPoints = EditorGUILayout.IntField("Power Points", ao.powerPoints);
    }
}