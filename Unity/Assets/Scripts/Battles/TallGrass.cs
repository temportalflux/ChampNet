using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TallGrass : MonoBehaviour
{

    public MonsterDataObject[] randoms;

    private void OnTriggerStay2D(Collider2D other)
    {
        //Debug.Log(other.name);

        if (UnityEngine.Random.Range(0, 100) < 25)
        {
            MonsterDataObject opponent = this.getRandomOpponent();
            if (opponent != null)
            {
                PlayerLocal player = other.GetComponent<PlayerLocal>();
                if (player != null)
                {
                    player.onChallengeBy(opponent);
                }
            }
        }
        
    }

    private MonsterDataObject getRandomOpponent()
    {
        return randoms.Length > 0 ? this.randoms[UnityEngine.Random.Range(0, this.randoms.Length)] : null;
    }

}
