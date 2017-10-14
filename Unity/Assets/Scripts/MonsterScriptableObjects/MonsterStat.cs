/* Author: Jake Ruth
 */

using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[Serializable]
[CreateAssetMenu(menuName = "Asset/ Create new Monster", order = 100)]
public class MonsterStat : ScriptableObject
{
    [Header("Monster Stats")]
    public string monsterName;
    public MonsterType[] types;
    [Space]
    //[Header("")] // used to create a space in the inspector to space things apart
    public int maxHp;
    public int attack;
    public int defense;
    public int specialAttack;
    public int specialDefense;
    public int speed;
    [Space]
    //[Header("")] // used to create a space in the inspector to space things apart
    public AttackObject[] availableAttacks;
}
