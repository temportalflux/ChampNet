using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(PlayerCharacterController))]
public class PlayerInputController : MonoBehaviour
{
    private PlayerCharacterController _pcc;


    // Use this for initialization
	void Start ()
	{
	    _pcc = GetComponent<PlayerCharacterController>();
	}
	
	// Update is called once per frame
	void Update ()
    {
		Vector3 input = Vector2.zero;

        if (Input.GetKey(KeyCode.A))
            input.x = -1;
        else if (Input.GetKey(KeyCode.D))
            input.x = 1;
        else if (Input.GetKey(KeyCode.S))
            input.y = -1;
        else if (Input.GetKey(KeyCode.W))
            input.y = 1;

        input.Normalize();

        _pcc.Move(input);

    }
}
