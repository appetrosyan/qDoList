import QtQuick 2.11
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.3

RowLayout{
	property bool expanded: true
	CheckBox{
		checked: modelData.done
		onClicked:{
			modelData.toggle()
			checked = modelData.done
		}
	}
	TextEdit{
		text: modelData.name
		color: Material.foreground
		Layout.alignment: Qt.AlignLeft
		onEditingFinished: {
			modelData.name = text
		}
	}
	Label{
		text: modelData.overDue?modelData.due.toLocaleString(Qt.Locale):modelData.prettyDueDate
		Layout.alignment: Qt.AlignRight
		Layout.rightMargin: 7
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
