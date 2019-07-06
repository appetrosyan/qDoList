import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.2
import QSettings 1.0
import org.kde.kirigami 2.6 as Kirigami
import ac.uk.cam.ap886 1.0
import core 1.0

Kirigami.ApplicationWindow {
	id: rootWindow
	width: 300
	minimumWidth: 300
	height: 500
	minimumHeight: 400
	title: qsTr("qDolist") + " — [%1/%2]: %3%4".arg(myModel.completeTasks)
	.arg(myModel.size)
	.arg((myFileList.activeTrackedFile!=null)?myFileList.activeTrackedFile.fileName:"")
	.arg((myFileList.activeTrackedFile!=null)?(myFileList.activeTrackedFile.isModified?"*":""):"")
	property int mm: Screen.pixelDensity
	visible: true
	signal addTask
	signal sendMessage(string msg)
	signal sendIndex(int x)
	signal deleteAt(int x)
	signal updateDue(int x, string newDue)
	signal writeToFile(string file)
	signal loadFromFile(string file)
	signal saveAllFiles()
	signal requestTaskList()
	signal moveFocusedTaskUp()
	signal moveFocusedTaskDown()
	signal demoteFocusedTask()
	signal promoteFocusedTask()
	SystemPalette{
		id: sysPallete
	}

	Material.accent: sysPallete.highlight
	Material.theme: settings.darkMode?Material.Dark:Material.light
	globalDrawer: Kirigami.GlobalDrawer{
		id:globalDrawer
		showContentWhenCollapsed: true
		title: i18n("QDoList")
		visible: !handleVisible
		handle.anchors.top: rootWindow.top
		handleVisible: rootWindow.width<600
		//		collapsible: !handleVisible
		//		collapsed: !handleVisible\
		// The file Picker doens't look particularly good.
		modal: handleVisible
		actions: [
			Kirigami.Action{
				id: loadfromFileAction
				text: qsTr("&Open file")
				iconName: "document-open"
				shortcut: StandardKey.Open
				onTriggered: {
					loadTodoListDialog.write = false
					loadTodoListDialog.selectExisting = true
					loadTodoListDialog.visible = true
				}
			},
			Kirigami.Action{
				text: qsTr("&Save as")
				iconName: "document-save-as"
				shortcut: StandardKey.SaveAs
				onTriggered: {
					loadTodoListDialog.write = true
					loadTodoListDialog.selectExisting = false
					loadTodoListDialog.visible = true
				}
			},
			Kirigami.Action{
				text: qsTr("S&ync all files")
				iconName: "document-save"
				shortcut: StandardKey.Save
				onTriggered: {
					saveAllFiles()
				}
			}
		]
		FilePicker{
			id: filePicker
			Layout.preferredWidth: 250
			Layout.fillHeight: false
			Layout.fillWidth: false
			width: 120
			height: 120
		}
		Switch{
			text: qsTr("Auto Sync Files")
			checked: settings.autoSync
			onCheckedChanged: {
				settings.autoSync = checked
			}
			transitions: [Transition {
					NumberAnimation{
						properties: x
						easing.type: Easing.InOutQuad
						duration: 200
					}
				}]
		}

	}


	Timer{
		id: saveTimer
		interval: 1000
		running: settings.autoSync
		repeat: settings.autoSync
		onTriggered: {
			saveAllFiles()
		}
	}

	ColumnLayout {
		anchors.fill: parent
		RowLayout {
			id: centerpiece
			Layout.fillHeight: true
			Layout.fillWidth: true

			ColumnLayout {
				Layout.alignment: Qt.RightButton
				TaskList {
					id: taskList
					z: -2
					Layout.fillWidth: true
					Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
					Layout.minimumWidth: 300
					Layout.fillHeight: true
					highlightRangeMode: ListView.ApplyRange
					snapMode: ListView.SnapToItem
					contentHeight: 2
					pixelAligned: false
					transformOrigin: Item.Center
					model: myModel
				}
			}
		}
	}
	footer:Rectangle{
		z:0
		color: sysPallete.window
		width: rootWindow.width
		height: addTask.height - 5
		radius: 15
		border.color: sysPallete.highlight
		border.width: 1
		TaskAdd {
			id: addTask
			onCreateNewTask: myModel.createNewTask(msg)
		}
	}

	FileDialog {
		id: loadTodoListDialog
		title: "Choose your to-do list"
		folder: shortcuts.home
		property bool write: false
		onAccepted: {
			console.log(loadTodoListDialog.fileUrl)
			if (write){
				writeToFile(loadTodoListDialog.fileUrl)
			}
			else{
				loadFromFile(loadTodoListDialog.fileUrl)
			}
		}
	}
}
