using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

[CustomEditor(typeof(AttackObject))]
public class AttackObjectEditor : Editor
{
    public override void OnInspectorGUI()
    {
        AttackObject ao = target as AttackObject;

        if (ao == null)
            return;

        ao.doesDamage = GUILayout.Toggle(ao.doesDamage, "Does Damage?");

        if (ao.doesDamage)
        {
            ao.power = EditorGUILayout.IntField("   Power", ao.power);
        }

        EditorGUILayout.Space();

        ao.doesModifiesStats = GUILayout.Toggle(ao.doesModifiesStats, "Does Modify Stats?");

        if (ao.doesModifiesStats)
        {
            ao.doesModifyOpponentStats = GUILayout.Toggle(ao.doesModifyOpponentStats, "   Does Modify Opponent Stats?");
            ao.modifyAttackValue = EditorGUILayout.IntField("   Attack Modifier", ao.modifyAttackValue);
            ao.modifyDefenseValue = EditorGUILayout.IntField("   Defense Modifier", ao.modifyDefenseValue);
            ao.modifySpecialAttackValue = EditorGUILayout.IntField("   Special Attack Modifier", ao.modifySpecialAttackValue);
            ao.modifySpecialDefenseValue = EditorGUILayout.IntField("   Defense Modifier", ao.modifySpecialDefenseValue);
            ao.modifySpeedValue = EditorGUILayout.IntField("   Speed Modifier", ao.modifySpeedValue);
            ao.modifyAccuracyValue = EditorGUILayout.IntField("   Accuracy Modifier", ao.modifyAccuracyValue);
        }

        EditorGUILayout.Space();

        ao.accuracy = EditorGUILayout.IntField("Accuracy", ao.accuracy);
        ao.powerPoints = EditorGUILayout.IntField("Power Points", ao.powerPoints);
    }
}