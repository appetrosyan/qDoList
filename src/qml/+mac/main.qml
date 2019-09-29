import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.2
import QSettings 1.0
import ac.uk.cam.ap886 1.0
import core 1.0

ApplicationWindow {
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
	signal updateDue(int x, string newDue)
	signal writeToFile(string file)
	signal loadFromFile(string file)
	signal saveAllFiles()
	signal requestTaskList()
	signal moveFocusedTaskUp()
	signal moveFocusedTaskDown()
	signal demoteFocusedTask()
	signal promoteFocusedTask()
	signal toggleFocusedTask()
	SystemPalette{
		id: sysPallete
	}


	Material.accent: sysPallete.highlight
	Material.theme: settings.darkMode?Material.Dark:Material.light
	color: Qt.darker(Material.background, 1.2)
	header:ToolBar{
		RowLayout{
			anchors.fill: parent
			Button{
				text: "settings"
				onClicked: globalDrawer.visible=!globalDrawer.visible
			}
			Button{
				text: "actions"
				Layout.alignment: Qt.AlignRight
				onClicked: {
					contextMenu.visible=!contextMenu.visible
					contextMenu.x = this.x
				}
			}
		}
	}

	Menu{
		id: contextMenu
		visible: false
		Action {
			id: saveAll
			shortcut: StandardKey.Save
			text: "Save files"
			onTriggered: {
				rootWindow.saveAllFiles()
			}
			icon.name: "file-save"
		}
	}


	Drawer{
		id: globalDrawer
		height: rootWindow.height
		width: 250
		ColumnLayout{
			Rectangle{
				width:globalDrawer.width-10
				height: childrenRect.height
				color: "#00ffffff"
				border.color: sysPallete.text
				radius: 12
				FilePicker{
					id: filePicker
					width: globalDrawer.width-12
					height: 160
					Layout.preferredWidth: 250
					Layout.fillHeight: false
					Layout.fillWidth: false
				}
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
			Switch{
				text: qsTr("Dark Mode")
				checked: settings.darkMode
				onCheckedChanged: {
					settings.darkMode = checked
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
		color: Material.background
		width: rootWindow.width
		height: addTask.height - 5
		radius: 15
		border.color: Qt.tint(sysPallete.base, sysPallete.highlight)
		border.width: 1
		TaskAdd {
			id: addTask
			onCreateNewTask: myModel.createNewTask(msg)
		}
	}

	FileDialog {
		id: loadTodoListDialog
		title: qsTr("Choose a local To-Do list file %1")
		.arg(write?qsTr("to write"):qsTr("to read"))
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