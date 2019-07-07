import QtQuick 2.12
import QtQuick.Window 2.12
import QtQml 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.12
import ac.uk.cam.ap886 1.0
import core 1.0

RowLayout {
	Layout.alignment: Qt.BottomDockWidgetArea
	signal createNewTask(string msg)
		TextEdit {
			id: newTask
			objectName: "textEditor"
			text: placeholder
			font.pointSize: 14
			property bool edited: text != placeholder
			property string placeholder: "new task text goes here..."
			opacity: edited+0.5
			color: Material.foreground
			Layout.alignment: Qt.RightToolBarArea
			Layout.fillWidth: true
			Layout.rightMargin: 7
			padding: 14
			onFocusChanged: {
				newTask.text=newTask.focus?"":placeholder
			}
			Keys.onReturnPressed: {
				if(text!="")
					editingFinished()
				else{
					rootWindow.toggleFocusedTask()
				}
			}
			Keys.onUpPressed: {
				if(!placeholder.visible)
					rootWindow.moveFocusedTaskUp()
			}
			Keys.onDownPressed: {
				if(!placeholder.visible)
					rootWindow.moveFocusedTaskDown()
			}
			Keys.onTabPressed: {
				if(!placeholder.visible)
					rootWindow.demoteFocusedTask()
			}
			Keys.onBacktabPressed: {
				if(!placeholder.visible)
					rootWindow.promoteFocusedTask()
			}

//			Keys.forwardTo: [rootWindow]
			// Unfortunately this doesn't work as expected.
			// QML documentation is like Trump's speech. Sometimes you're not sure
			// that the devs know what words mean.
			onEditingFinished: {
				if(newTask.text != placeholder){
					createNewTask(newTask.text)
					newTask.text=""
				}
			}
		}

}
