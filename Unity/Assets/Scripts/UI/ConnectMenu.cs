using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.Events;
using System;

public class ConnectMenu : MonoBehaviour {

    [System.Serializable]
    public class EventConnect : UnityEvent<string, int> { }
    public EventConnect onConnect;

    private string txtAddress = "127.0.0.1", txtPort = "425";
    private string errorPort = null, errorConnect = null;
    private Color color;
    private bool connecting = false, showGui = true;
    
    void OnGUI()
    {
        if (!this.showGui) return;

        int width = 200;
        GUI.Box(new Rect(0, 0, width, Screen.height), "");

        GUILayout.BeginVertical(GUILayout.Width(width));
        {
            GUILayout.Label("Address");

            this.txtAddress = GUILayout.TextField(this.txtAddress);

            GUILayout.Label("Port");

            string port = GUILayout.TextField(this.txtPort);
            if (port != null && port.Length > 0)
            {
                try
                {
                    int.Parse(port);
                    this.txtPort = port;
                    this.errorPort = null;
                }
                catch (FormatException e)
                {
                    this.errorPort = "Only numbers allowed";
                }
                catch (Exception e)
                {
                    this.errorPort = "parse error";
                    Debug.LogError(e);
                }
            }

            if (this.errorPort != null)
            {
                GUILayout.Label(this.errorPort);
            }

            // Color picker
            GUILayout.BeginVertical(GUILayout.Width(width));
            {
                GUILayout.Label("Red " + (int)(this.color.r * 255));
                this.color.r = GUILayout.HorizontalSlider(this.color.r * 255, 0, 255) / 255F;
                GUILayout.Label("Green " + (int)(this.color.g * 255));
                this.color.g = GUILayout.HorizontalSlider(this.color.g * 255, 0, 255) / 255F;
                GUILayout.Label("Blue " + (int)(this.color.b * 255));
                this.color.b = GUILayout.HorizontalSlider(this.color.b * 255, 0, 255) / 255F;
            }
            GUILayout.EndVertical();

            if (!this.connecting)
            {
                if (GUILayout.Button("Connect"))
                {
                    this.errorConnect = null;
                    this.connecting = true;
                    this.onConnect.Invoke(this.txtAddress, int.Parse(this.txtPort));
                }
            }

            if (this.errorConnect != null)
            {
                GUILayout.Label(this.errorConnect);
            }

        }
        GUILayout.EndVertical();
    }

    public void onConnectionHandled(bool success)
    {
        this.connecting = false;
        this.showGui = !success;
    }

    public void setConnectionError(string error)
    {
        this.errorConnect = error;
    }

    /*
     EventSystem.current.SetSelectedGameObject(myInputField.gameObject, null);
 myInputField.OnPointerClick(new PointerEventData(EventSystem.current));
     */

}
