import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.2
import QSettings 1.0
import org.kde.kirigami 2.0 as Kirigami
import ac.uk.cam.ap886 1.0
import core 1.0

Kirigami.ApplicationWindow {
	id: rootWindow
	width: 300
	minimumWidth: 300
	height: 400
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
	SystemPalette{
		id: sysPallete
	}
	Material.accent: sysPallete.highlight
	Material.theme: settings.darkMode?Material.Dark:Material.light
	Kirigami.GlobalDrawer{
		id:globalDrawer
		showContentWhenCollapsed: true
		title: i18n("QDoList")
		visible: true
		//		modal: true
		actions: [
			Kirigami.Action{
				id: loadfromFileAction
				text: qsTr("&Open file")
				shortcut: StandardKey.Open
				onTriggered: {
					loadTodoListDialog.write = false
					loadTodoListDialog.selectExisting = true
					loadTodoListDialog.visible = true
				}
			},
			Kirigami.Action{
				text: qsTr("&Save as")
				iconName: "file-save-as"
				shortcut: StandardKey.SaveAs
				onTriggered: {
					loadTodoListDialog.write = true
					loadTodoListDialog.selectExisting = false
					loadTodoListDialog.visible = true
				}
			},
			Kirigami.Action{
				text: qsTr("S&ync all files")
				iconName: "file-save"
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
					thisYear: currentYear()
					thisMonth: currentMonth() - 1
					thisDay: currentDay
				}
			}
		}

	}
	footer:Rectangle{
		z:0
		color: sysPallete.window
		width: parent.width
		height: addTask.height - 5
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

	function currentYear() {
		var date = new Date()
		return Number(Qt.formatDate(date, "yyyy"))
	}
	function currentMonth() {
		var date = new Date()
		return Number(Qt.formatDate(date, "MM"))
	}
	function currentDay() {
		var date = new Date()
		return Number(Qt.formatDate(date, "dd"))
	}
}
