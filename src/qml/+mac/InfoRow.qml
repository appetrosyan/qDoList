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
	Text{
		id:dueIndicator
		text: modelData.overDue?modelData.due.toLocaleString(Qt.Locale):modelData.prettyDueDate
		color: modelData.overDue?"red":Material.foreground
		Layout.alignment: Qt.AlignRight
		Layout.rightMargin: 5
		MouseArea{
			acceptedButtons: Qt.LeftButton | Qt.RightButton
			anchors.fill:parent
			onWheel: {
				// I admire the sheer stupidity. Why The FUCK, do you provide a dateTime class that extends a JavaScript DateTime class, which isn't a fucking javascript class.
				// I seriously wonder if there were any non-morons designing the framework.
				if(wheel.pixelDelta.x >=50){
					modelData.due = new Date(modelData.due.setTime(modelData.due.getTime()+60*1000))
				}
				if(wheel.pixelDelta.x <= -50){
					modelData.due = new Date(modelData.due.setTime(modelData.due.getTime()-60*1000))
				}
			}
		}
	}
}
