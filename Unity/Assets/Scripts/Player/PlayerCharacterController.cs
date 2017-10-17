using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerCharacterController : MonoBehaviour
{
    [Header("Transform Dependencies")]
    public Transform spriteTransform;

    [Header("Player Character Controller values")]
    public float speed;

	// Use this for initialization
	void Start ()
    {
		
	}


    public void Move(Vector3 input)
    {
        transform.position += input * speed * Time.deltaTime;

        if (input.sqrMagnitude > 0)
        {
            float alpha = Mathf.Atan2(input.y, input.x) * Mathf.Rad2Deg;

            spriteTransform.rotation = Quaternion.Euler(0, 0, alpha);
        }
    }
}
