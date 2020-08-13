# ListView-EditSubLabel
An extension function for a Win32 List-View Control which allows editing its subitems. 

## Table of contents
* [Description](#description)
* [Parameters](#parameters)
* [Return value](#return-value)
* [Suggested usage](#suggested-usage)

## Description
**ListView-EditSubLabel** creates a dynamic in-place WC_EDIT control on top of the selected subitem for a text input, then updates subitem's text accordingly.

During the edit:
- If ENTER or TAB is pressed, or WC_EDIT control loses focus (i.e. user clicks somewhere outside), then the text in the List-View's subitem is updated
- If ESC is pressed, the changes are discarded

## Parameters
| Parameter | Description |
| ------ | ------ |
| HWND hwnd | A window handle to the List-View control|
| int id | An identifier of the control |
| int iItem | A zero-based index of the List-View item |
| int iSubItem | An one-based index of the List-View subitem |

## Return value
**ListView-EditSubLabel** returns FALSE if it fails to create a WC_EDIT control, otherwise it returns TRUE

## Suggested usage
It's reasonable and expectable for user to edit a subitem by double-clicking on it. Therefore **ListView-EditSubLabel** should be called when handling double-click notification for a List-View control
```c++
case NM_DBLCLK:
		{
			LPNMITEMACTIVATE pia = (LPNMITEMACTIVATE)pnm;
			if (pia->iItem < 0)
				break;
			if (ListView_GetItemState(pnm->hwndFrom, pia->iItem, LVIS_FOCUSED) & LVIS_FOCUSED) {
				ListView_EditSubLabel(hwnd, pnm->idFrom, pia->iItem, pia->iSubItem);
			}
			else {
				ListView_SetItemState(pnm->hwndFrom, pia->iItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			}
			break;
		}
```

Full usage example is provided in **/example** directory
