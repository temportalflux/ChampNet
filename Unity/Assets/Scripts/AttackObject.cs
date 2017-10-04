using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public enum AttackType
{
    NORMAL,
    FIRE,
    WATER,
    GRASS
}

[Serializable]
[CreateAssetMenu(menuName = "Asset/ Create New Attack", order = 100)]
public class AttackObject : ScriptableObject
{
    public AttackType type;
    public bool doesDamage;
    public int power;

    public bool doesModifiesStats;
    public bool doesModifyOpponentStats;
    public int modifyAttackValue;
    public int modifyDefenseValue;
    public int modifySpecialAttackValue;
    public int modifySpecialDefenseValue;
    public int modifyAccuracyValue;
    public int modifySpeedValue;

    public int accuracy;
    public int powerPoints;

    public void UseAttack()
    {
        
    }
}
