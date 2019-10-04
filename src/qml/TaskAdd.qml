import QtQuick 2.12
import QtQuick.Window 2.12
import QtQml 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.12
import ac.uk.cam.ap886 1.0
import core 1.0

RowLayout {
	id: taskAdd
	Layout.alignment: Qt.BottomDockWidgetArea
	signal createNewTask(string msg)
	signal suggestionsRequested(string text)
	signal mostLikelySuggestionRequested(int x)
	property string text: ""
	property alias edited: newTask.edited
	TextEdit {
		id: newTask
		objectName: "textEditor"
		text: placeholder
		horizontalAlignment: Text.AlignHCenter
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
			if(newTask.focus===true)
				newTask.text=newTask.focus?"":placeholder
		}
		Keys.onReturnPressed: {
			createNewTask(taskAdd.text)
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
				mostLikelySuggestionRequested(0)
		}

		//			Keys.forwardTo: [rootWindow]
		// Unfortunately this doesn't work as expected.
		// QML documentation is like Trump's speech. Sometimes you're not sure
		// that the devs know what words mean.
		onEditingFinished: {
			newTask.text = placeholder
		}
		onTextChanged:{
			if(newTask.text !=="" && newTask.text!== placeholder  ){
				if(newTask.text.startsWith(":") && newTask.text.length > 1){
					suggestionsRequested(newTask.text)
				}
				taskAdd.text = newTask.text
			}
		}
	}
	ToolButton{
		id: submitButton
		text: "±"
		Layout.alignment: Qt.AlignRight
		onClicked: createNewTask(taskAdd.text)
	}
}
