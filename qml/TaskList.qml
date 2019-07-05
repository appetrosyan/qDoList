import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.0
import ac.uk.cam.ap886 1.0
import core 1.0

ListView {
	id: listView
	property var thisYear
	property var thisMonth
	property var thisDay
	move: slideIn
	add: slideIn
	Transition {
		id: slideIn
		NumberAnimation {
			easing.amplitude: 1.05
			properties: "x"
			from: 100
			duration: 400
			easing.type: Easing.OutBounce
		}
	}
	remove: dropOut
	Transition {
		id: dropOut
		NumberAnimation {
			properties: "y"
			to: 1000
			duration: 1000
			easing.type: Easing.InOutQuad
		}
	}
	delegate: accordion
	Component {
		id: accordion
		Rectangle {
			width: parent.width
			height: childrenRect.height
			color: Qt.darker(sysPallete.window, 1+modelData.doneSubtaskCount/10)
			radius: 15
			InfoRow {
				id: infoRow
			}
			Text {
				anchors.top: infoRow.bottom
				id: commentStrip
				text: ("[%1/%2] %3")
				.arg(modelData.doneSubtaskCount)
				.arg(modelData.subtaskCount)
				.arg(modelData.comment)
				style: Text.Outline
				x: 10
				width: parent.width - x
				visible: infoRow.expanded
				color: Material.foreground
				wrapMode: Text.Wrap
			}

			ListView {
				x: 10
				anchors.top: commentStrip.bottom
				width: parent.width - x
				height: childrenRect.height * infoRow.expanded
				visible: infoRow.expanded ? 1 : 0
				opacity: infoRow.expanded ? 1 : 0
				Behavior on opacity {
					NumberAnimation {
						easing.type: Easing.OutBounce
						duration: 300
					}
				}
				delegate: accordion
				// Now this is why QML is such a dumb idea. In normal assignment
				// that the thing on the right wouldn't be the same as on the left:
				// but in QML it's a binding not an assignment and so what I'm doing
				// is recursive binding.
				model: infoRow.subTasks
				interactive: false
				add: slideIn
				remove: dropOut
			}
			Drawer {
				modal: true
				height: rootWindow.height
				width: 250
				id: editDialog
				TaskEdit {
					anchors.fill: parent
					onEditsFinished: {
						editDialog.visible = false
					}
				}
			}
		}
	}

	// If you need to pass a string to a property in an OOP supporting Language
	// This is a sign that your design is probably monumentally stupid. What if I
	// (quite resonably) want to have a custom comparator? What if I track the due
	// dates with up to millisecond precision and I don't want for things that are
	// a miunte apart to show up as different things? If anyone doing QQC2 sees this
	// comment, please fix!
	section.property: "modelData.prettyDueDate"
	section.criteria: ViewSection.FullString
	section.delegate: Component {
		Rectangle {
			width: listView.width
			height: childrenRect.height
			color: Material.accent
			Text {
				text: section
				color: sysPallete.highlightedText
			}
		}
	}

	Label {
		id: placeholder
		text: qsTr("Empty")
		anchors.margins: 60
		anchors.fill: parent
		opacity: 0.5
		visible: listView.count === 0
		horizontalAlignment: Qt.AlignHCenter
		verticalAlignment: Qt.AlignVCenter
		wrapMode: Label.WordWrap
		font.pixelSize: 18
	}
}
