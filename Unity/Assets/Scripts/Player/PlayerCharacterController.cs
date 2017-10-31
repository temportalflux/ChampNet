using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[SelectionBase]
[RequireComponent(typeof(BoxCollider2D))]
public class PlayerCharacterController : MonoBehaviour
{
    [Header("Transform Dependencies")]
    public Transform spriteTransform;
    private Animator _anim;

    [Header("Player Character Controller values")]
    public float speed;
    public float sight;

    [HideInInspector]
    public Vector3 deltaMove;
    private float _skinWidth;
    private float _size;
    private float _halfSize;

    // Use this for initialization
    void Start()
    {
        _anim = GetComponentInChildren<Animator>();
        _skinWidth = 0.2f;
        _size = 1.0f - _skinWidth;
        _halfSize = _size / 2;
    }

    public void Move(Vector3 input)
    {
        float step = speed * Time.deltaTime;
        if (input.sqrMagnitude > 0)
        {
            int numOfRays = 3;
            float raySeperation = _size / (numOfRays - 1);

            for (int i = 0; i < numOfRays; i++)
            {
                Vector3 inputRotate = new Vector3(-input.y, input.x);
                Vector3 point = spriteTransform.position +
                                input.normalized * (_halfSize + _skinWidth / 2) -
                                inputRotate * _halfSize +
                                inputRotate * i * raySeperation;
                
                //Debug.DrawRay(point, input * 5);

                RaycastHit2D hit = Physics2D.Raycast(point, input, sight);
                if (hit)
                {
                    if (hit.transform == transform)
                        continue;

                    if (hit.transform.tag == "Wall")
                    {
                        step = Mathf.Clamp(step, 0, hit.distance);
                    }
                    else if (hit.transform.tag == "Water" || false) // maybe replace false with swimming?
                    {
                        step = Mathf.Clamp(step, 0, hit.distance);
                    }
                    else if (hit.transform.tag == "Player")
                    {
                        // looking at a player
                    }
                }
            }
        }

        deltaMove = input * step;
        transform.position += deltaMove;

        _anim.SetFloat("velX", deltaMove.x);
        _anim.SetFloat("velY", deltaMove.y);
        _anim.SetBool("walking", deltaMove.sqrMagnitude > 0.001f);
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        Debug.Log(collision.gameObject.name);
    }
}
