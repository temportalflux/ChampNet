using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[Serializable]
[CreateAssetMenu(menuName = "Asset/ Create new Monster", order = 100)]
public class MonsterStats : ScriptableObject
{
    [Header("Monster Stats")]
    public string monsterName;
    public int maxHp;
    public int attack;
    public int defense;
    public int specialAttack;
    public int specialDefense;
    public int speed;

    public AttackObject[] availableAttacks;
}
