using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class CameraBlit : MonoBehaviour {

    private static CameraBlit _instance;

    public static void setMaterial(Material material)
    {
        CameraBlit._instance.materialBlit = material;
    }

    private Material materialBlit;

    private void Awake()
    {
        CameraBlit._instance = this;
    }

    private void Start()
    {
        this.materialBlit = null;
    }
    
    void OnRenderImage(RenderTexture source, RenderTexture destination)
    {
        if (this.materialBlit != null)
        {
            Graphics.Blit(source, destination, this.materialBlit);
        }
        else
        {
            Graphics.Blit(source, destination);
        }
    }

}
