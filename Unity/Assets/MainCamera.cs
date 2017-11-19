using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MainCamera : MonoBehaviour
{

    public Material spliceTextures;
    public RenderTexture textureA;
    public RenderTexture textureB;
    public Vector3 positionA, positionB;
    private float dist;
    public float distTarget = 5f;

    private void Start()
    {
        GameManager.INSTANCE.mainCamera = this;
    }

    public void SetTexture(int index, RenderTexture texture)
    {
        switch (index % 2)
        {
            case 0: this.textureA = texture; break;
            case 1: this.textureB = texture; break;
            default: break;
        }
        this.spliceTextures.SetTexture("_Tex1", this.textureA);
        this.spliceTextures.SetTexture("_Tex2", this.textureB);
    }

    public Vector3 SetPosition(int index, Vector3 position, out bool doUseIndividualCamera)
    {
        doUseIndividualCamera = this.textureA != null && this.textureB != null;

        switch (index % 2)
        {
            case 0: this.positionA = position; break;
            case 1: this.positionB = position; break;
            default: break;
        }

        this.spliceTextures.SetVector("_PosA", this.positionA);
        this.spliceTextures.SetVector("_PosB", this.positionB);

        /*
        if (this.positionA != null)
        {
            Vector3 b = this.positionB == null ? Vector3.zero : this.positionB;
            this.transform.position = (positionA - b) * 0.5f + b;
        }
        //*/
        float camZ = this.transform.position.z;
        Vector3 posOut = position;
        if (this.textureA != null && this.textureB != null)
        {
            Vector3 diff = this.positionA - this.positionB;
            this.transform.position = (diff * 0.5f) + this.positionB;
            this.dist = diff.sqrMagnitude;
            posOut = (this.transform.position - position);
        }
        else
        {
            this.transform.position = this.positionA;
        }
        this.transform.position += Vector3.forward * camZ;

        return posOut;
    }

    void OnRenderImage(RenderTexture source, RenderTexture destination)
    {
        if (this.textureA != null && this.textureB != null && this.dist >= this.distTarget * this.distTarget)
        {
            Graphics.Blit(source, destination, this.spliceTextures);
        }
        else
        {
            Graphics.Blit(source, destination);
        }
    }

}
