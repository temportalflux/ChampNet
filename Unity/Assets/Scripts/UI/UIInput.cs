using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;

public class UIInput : MonoBehaviour {

    public UnityEngine.UI.Button[] fields;
    public RectTransform selector;

    private int selected;
    
	void Start ()
    {

        this.selected = 0;
        
    }
	
	public void down(InputDevice device, InputResponse.UpdateEvent type, MappedButton action)
    {
        switch (action)
        {
            case MappedButton.CONFIRM:
                this.fields[this.selected].onClick.Invoke();
                break;
            default:
                break;
        }
    }

    public void down(InputDevice device, InputResponse.UpdateEvent type, MappedAxis action, AxisDirection value)
    {
        if (value != AxisDirection.Centered)
        {

            switch (action)
            {
                case MappedAxis.Vertical:
                    
                    // Add value to current selection
                    this.selected = (this.selected - (int)value);
                    // Ensure yield is negative
                    if (this.selected < 0) this.selected += this.fields.Length;
                    // Find remainder
                    this.selected %= this.fields.Length;

                    this.selector.position = new Vector3(this.selector.position.x, this.fields[this.selected].gameObject.transform.position.y, 0);
                    
                    break;
                default:
                    break;
            }

        }
    }

}
