import QtQuick 2.11
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.3

Row{
	property bool expanded: true
	CheckBox{
		id: doneCheckbox
		checked: modelData.done
		onClicked:{
			modelData.toggle()
			checked = modelData.done
		}
		Layout.alignment: Qt.AlignLeft
	}
	TextEdit{
		id: nameRow
		width: parent.width - doneCheckbox.width - dueDatePicker.width -7
		text: modelData.name
		anchors.verticalCenter: parent.verticalCenter
		color: Material.foreground
		Layout.alignment: Qt.AlignLeft
		Keys.onUpPressed: {
			rootWindow.moveFocusedTaskUp()
		}
		Keys.onDownPressed: {
			rootWindow.moveFocusedTaskDown()
		}
		Keys.onTabPressed: {
			rootWindow.demoteFocusedTask()
		}
		Keys.onBacktabPressed: {
			rootWindow.promoteFocusedTask()
		}
		Keys.onReturnPressed: {
			text=text.trim()
			cursorVisible=false
			editingFinished()
			focused=false
		}
		onEditingFinished: {
			modelData.name = text
			modelData.requestFocus()
		}
	}
	Label{
		id: dueDatePicker
		text:  modelData.prettyDueTime
		font.bold: modelData.overDue?true:false
		font.capitalization: Font.Capitalize
		anchors.verticalCenter: parent.verticalCenter
		MouseArea{
			anchors.fill: parent
			onClicked: loader_pickerDialog.item.visible=!loader_pickerDialog.item.visible
		}
		// TODO: CREATE FUCKING REFACTORING TOOLS. IF YOU CAN WRITE COMMENTS THAT TELL ME WHAT TO DO
		// THEN YOU CAN WRITE AN AWK SCRIPT.
		Component {
			id: component_pickerDialog
			Popup{
				property alias picker: inner_picker
				id: pickerDialog
				height: picker.height +20
				width: picker.width +20
				rightMargin: 5
				DatePicker {
					id: inner_picker
					date: modelData.due
					onNewDate: {
						modelData.due = msg
					}
				}
			}
		}
		Loader {
			id: loader_pickerDialog
			sourceComponent: component_pickerDialog
		}
	}
}
