import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import Qt.labs.calendar 1.0
import QtQuick.Layouts 1.0

Column {
	property var scheduled: model.modelData.scheduled
	property var due: model.modelData.due
	signal editsFinished()
	signal reSchedule(date dt)
	signal reDue(date dt)
	padding: 12
	onReSchedule: {
		model.modelData.scheduled = (dt)
	}
	onReDue: {
		model.modelData.due = (dt)
	}

	RowLayout {
		anchors.right: parent.right
		anchors.left: parent.left
		visible: false
		Label {
			text: "<b>" + qsTr("Edit") + "</b>"
			horizontalAlignment: Label.AlignHCenter
			padding: 12
			font.pixelSize: 24
		}
	}
	RowLayout {
		anchors.right: parent.right
		anchors.rightMargin: 7
		anchors.left: parent.left
		CheckBox {
			id: check
		}
		TextField {
			id: newName
			text: model.modelData.name
			Layout.fillWidth: true
			onAccepted: {
				model.modelData.name = text
				editsFinished()

			}
		}
	}
	RowLayout{
		anchors{
			right: parent.right
			rightMargin: 7
			left: parent.left
			leftMargin: 10
		}
		TextField{
			id: newComment
			text: model.modelData.comment
			Layout.fillWidth: true
			onAccepted: {
				model.modelData.comment = text
				editsFinished()
			}
		}
	}

	RowLayout {
		anchors.right: parent.right
		anchors.left: parent.left
		Label {
			text: qsTr("Added on")
			padding: 12
		}
		DatePicker {
			date: model.modelData.added
			currentYear: thisYear
			Layout.alignment: Qt.AlignRight
			padding: 12
			editable: false
		}
	}
	RowLayout {
		anchors.right: parent.right
		anchors.left: parent.left
		Label {
			text: qsTr("Scheduled")
			padding: 12
		}
		DatePicker {
			currentYear: thisYear
			date: model.modelData.scheduled
			color: "cyan"
			Layout.alignment: Qt.AlignRight
			padding: 12
			onNewDate: {
				reSchedule(new Date(msg))
			}
		}
	}
	RowLayout {
		anchors.right: parent.right
		anchors.left: parent.left
		Label {
			text: qsTr("Due")
			padding: 12
		}
		DatePicker {
			date: model.modelData.due
			currentYear: thisYear
			padding: 12
			color: "red"
			Layout.alignment: Qt.AlignRight
			onNewDate: {
				reDue(new Date(msg))
			}
		}
	}

	ColumnLayout {
		DayOfWeekRow {
			locale: grid.locale
			Layout.fillWidth: true
			delegate: Text{
				horizontalAlignment: Text.AlignHCenter
				verticalAlignment: Text.AlignVCenter
				//                opacity: model.month === Date.now().month ? 1 : 0.5
				text: model.narrowName
				color: Material.foreground
			}
		}
		MonthGrid {
			id: grid
			year: thisYear
			month: thisMonth
			property bool updateDue: false
			onClicked: {
				if(updateDue){
					// Long press and release
					reSchedule(date)
					updateDue = false
				} else {
					reDue(date)

				}
			}
			onPressAndHold:  {
				updateDue=true
			}

			delegate: Text{
				horizontalAlignment: Text.AlignHCenter
				verticalAlignment: Text.AlignVCenter
				opacity: (model.month === thisMonth) ? (model.day === thisDay?1:0.5) : 0.1
				text: model.day
				font.pointSize: 14
				color: (model.day===Number(Qt.formatDate(due,"dd")) ) ?"red":(model.day===Number(Qt.formatDate(scheduled, "dd"))?"cyan":Material.foreground)
			}
		}
	}
	RowLayout {
		anchors.right: parent.right
		anchors.left: parent.left
		Button {
			id: confirmButton
			text: qsTr("Confirm")
			Layout.alignment: Qt.AlignRight
			Layout.rightMargin: 10
			Layout.leftMargin: 10
			Layout.fillWidth: true
			onClicked: {
				model.modelData.done = check.checkState == 2
				model.modelData.name = newName.text
				editsFinished()
			}
		}
	}

}
