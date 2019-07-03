import QtQuick.Controls 2.0
import QtQuick 2.0
import QtQuick.Controls.Material 2.0

SwipeDelegate {
	property bool expanded: false
	property var subTasks
	id: infoRow
	z: 0
	height: 40
	width: parent.width

	text: "<%1>%2</%1>".arg(model.modelData.done ? "s" : "b").arg(
			  model.modelData.name)
	swipe.left: Rectangle {
		width: parent.width
		height: parent.height
		clip: true
		color: !model.modelData.done ? "#722" : "#277"
		Behavior on color {
			ColorAnimation {
				easing.type: Easing.InOutCirc
				duration: 300
			}
		}
		Label {
			text: qsTr("Release to Toggle")
			anchors.fill: parent
			anchors.rightMargin: 10
			horizontalAlignment: Qt.AlignRight
			verticalAlignment: Qt.AlignVCenter
			opacity: 2 * infoRow.swipe.position
			color: "#aaa"
			Behavior on color {
				ColorAnimation {
					easing.type: Easing.InCirc
				}
			}
		}
		Label {
			text: !model.modelData.done ? qsTr("To-Do") : qsTr(
											  "Done")
			color: "white"
			padding: 20
			anchors.fill: parent
			horizontalAlignment: Qt.AlignLeft
			verticalAlignment: Qt.AlignVCenter
			opacity: !infoRow.swipe.complete
			Behavior on opacity {
				NumberAnimation {
					property: "opacity"
					easing.type: Easing.InOutQuad
				}
			}
		}
	}
	swipe.onCompleted: {
		if (swipe.position == 1) {
			model.modelData.toggle()
			infoRow.swipe.close()
		}
	}
	onClicked: {
		expanded = !expanded
		if (model.modelData.hasChildren) {
			subTasks = model.modelData.subModel
		} else {
			subTasks = []
		}
		focus = true
	}
	swipe.right: Row {
		id: rightSwipe
		anchors.right: parent.right
		height: parent.height
		Label {
			id: moveLabel
			text: qsTr("Edit")
			color: "white"
			verticalAlignment: Label.AlignVCenter
			padding: 12
			height: parent.height
			SwipeDelegate.onClicked: {
				editDialog.visible = true
				infoRow.swipe.close()
			}
			background: Rectangle {
				color: moveLabel.SwipeDelegate.pressed ? Qt.darker(
															 "#ffbf47",
															 1.1) : "#ffbf47"
			}
		}
		Label {
			text: qsTr("Delete")
			id: deleteLabel
			color: "white"
			verticalAlignment: Label.AlignVCenter
			padding: 12
			height: parent.height
			SwipeDelegate.onClicked:  {
				model.modelData.goAway()
				infoRow.swipe.close()
			}
			background: Rectangle {
				color: deleteLabel.SwipeDelegate.pressed ? Qt.darker(
															   "tomato",
															   1.1) : "tomato"
			}
		}

	}

	Text {
		id: dueDate
		anchors.right: parent.right
		anchors.rightMargin: 25
		opacity: infoRow.swipe.position == 0 ? 1 : 0
		Behavior on opacity {
			NumberAnimation {
				duration: 310
			}
		}
		color: (model.modelData.overDue ? "red" : "grey")
		Behavior on color {
			ColorAnimation {
				from: "red"
				to: "grey"
				duration: 200
			}
		}
		anchors.verticalCenter: parent.verticalCenter
		text: model.modelData.due.toLocaleDateString(
				  locale, Locale.ShortFormat)
		visible: (infoRow.text.length * infoRow.font.pointSize * 0.6 < parent.width)
	}

	Text{
		id: dragHandle
		anchors.right: parent.right
		anchors.rightMargin: 8
		anchors.verticalCenter: parent.verticalCenter
		visible: !infoRow.swipe.complete
		font.family: "Fontello"
		text:"\ue80e"
		opacity: infoRow.swipe.position == 0 ? 1 : 0
		Behavior on opacity {
			NumberAnimation {
				easing.type: Easing.InQuad
				duration: 310
			}
		}
		width: 16
		height: 16
		//                    fillMode: Image.TileVertically
		// Credit <div>Icons made by <a href="https://www.freepik.com/" title="Freepik">Freepik</a> from <a href="https://www.flaticon.com/" 			    title="Flaticon">www.flaticon.com</a> is licensed by <a href="http://creativecommons.org/licenses/by/3.0/" 			    title="Creative Commons BY 3.0" target="_blank">CC 3.0 BY</a></div>
		MouseArea{
			anchors.fill: parent
			property bool dragEnabled: false
			property int startX: 0
			property int startY: 0
			drag{
				target: infoRow
				axis: Drag.XAndYAxis
				maximumX: 50
				minimumX: -50
				maximumY: infoRow.height
				minimumY: -infoRow.height
			}
			onPressed:   {
				startX = infoRow.x
				startY = infoRow.y
				infoRow.z =200
				infoRow.opacity = 0.5
			}
			onReleased: {
				infoRow.z=0
				if(infoRow.y - startY > 10){
					infoRow.y = startY
					model.modelData.moveDown()
				}else if (infoRow.y - startY < -10) {
					model.modelData.moveUp()
					infoRow.y = startY
				} else {
					infoRow.y = startY
				}
				if(infoRow.x - startX > 20){
					infoRow.x = startX
					model.modelData.demote()
				}else if(infoRow.x - startX < -10){
					infoRow.x = startX
					model.modelData.promote()
				} else {
					infoRow.x = startX
				}
				infoRow.opacity = 1
				infoRow.z=0
			}
		}
	}
	ToolTip {
		text: model.modelData.scheduled
		delay: 1000
		visible: infoRow.pressed
	}
}
