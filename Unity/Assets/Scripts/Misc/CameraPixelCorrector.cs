using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraPixelCorrector : MonoBehaviour
{

    public Vector2 targetViewportSizeInPixels = new Vector2(480.0f, 320.0f);
    public float pixelsPerUnit = 32.0f;
    private Camera _cam;
    private int _currentScreenWidth;
    private int _currentScreenHeight;
    private float _pixelLockPPU = 32.0f;
    private Vector2 _winSize;

    void Start()
    {
        _cam = Camera.main;
        _cam.orthographic = true;

        resizeCamToTargetSize();
    }

    private void resizeCamToTargetSize()
    {
        if (_currentScreenWidth != Screen.width || _currentScreenHeight != Screen.height)
        {
            Vector2 percentage = new Vector2(Screen.width / targetViewportSizeInPixels.x, Screen.height / targetViewportSizeInPixels.y);
            float targetSize = 0;
            targetSize = percentage.x > percentage.y ? percentage.y : percentage.x;

            int targetSizeFloor = Mathf.FloorToInt(targetSize);

            if (targetSizeFloor < 1)
                targetSizeFloor = 1;

            float camSize = ((Screen.height / 2) / targetSizeFloor / pixelsPerUnit);
            _cam.orthographicSize = camSize;
            _pixelLockPPU = targetSizeFloor * pixelsPerUnit;
        }
        _winSize = new Vector2(Screen.width, Screen.height);
    }

    void Update()
    {
        if (_winSize.x != Screen.width || _winSize.y != Screen.height)
            resizeCamToTargetSize();
    }
}
