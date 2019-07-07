import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import Qt.labs.calendar 1.0
import QtQuick.Layouts 1.0
import "dateFunctions.js" as DF

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
			checked: model.modelData.done
			checkable: model.modelData.doneSubtaskCount === model.modelData.subtaskCount
			enabled: checkable
		}
		TextField {
			id: newName
			text: model.modelData.name
			placeholderText: "Task definition"
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
			placeholderText: "Task comment..."
			Layout.fillWidth: true
			onAccepted: {
				model.modelData.comment = text
				editsFinished()
			}
		}
	}
	ButtonGroup{
		id: btnGrp
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
			currentYear: DF.currentYear()
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
			Layout.alignment: Qt.AlignLeft
			padding: 12
			color: "cyan"

		}
		DatePicker {
			id: scheduledButton
			currentYear: DF.currentYear()
			date: model.modelData.scheduled
			Layout.alignment: Qt.AlignRight
			ButtonGroup.group: btnGrp
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
			color: "red"
		}
		DatePicker {
			id: dueButton
			date: model.modelData.due
			currentYear: DF.currentYear()
			padding: 12
			ButtonGroup.group: btnGrp
			Layout.alignment: Qt.AlignRight
			onNewDate: {
				reDue(new Date(msg))
			}
		}
	}

	property int yearOffset:0
	property int monthOffset:0
	ColumnLayout {
		RowLayout{
			Layout.fillWidth: true
			Button{
				Layout.preferredWidth: 40
				text: "<"
				onClicked: {
					if((DF.currentMonth() + monthOffset)==0){
						monthOffset=12 - DF.currentMonth()
						yearOffset--
					}else{
						monthOffset--
					}


				}
			}
			Label{
				Layout.fillWidth: true
				Layout.alignment: Qt.AlignHCenter
				text: (Qt.locale().monthName(DF.currentMonth()+monthOffset)) +(yearOffset===0?"":DF.currentYear()+yearOffset)
			}

			Button{
				text: ">"
				Layout.preferredWidth: 40
				onClicked: {
					if((DF.currentMonth() + monthOffset)==11){
						monthOffset=0 - DF.currentMonth()
						yearOffset++
					}else {
						monthOffset++
					}
				}
			}
		}

		DayOfWeekRow {
			locale: grid.locale
			Layout.fillWidth: true
			delegate: Text{
				horizontalAlignment: Text.AlignHCenter
				verticalAlignment: Text.AlignVCenter
				text: model.narrowName
				font.pixelSize: 10
				color: Material.foreground
			}
		}
		MonthGrid {
			id: grid
			year: DF.currentYear() + yearOffset
			month: (DF.currentMonth()+monthOffset)
			Layout.fillWidth: true
			property bool updateDue: false
			onClicked: {
				if(dueButton.checked){
					reDue(date)
				}else {
					reSchedule(date)
				}
			}
			delegate: Text{
				horizontalAlignment: Text.AlignHCenter
				verticalAlignment: Text.AlignVCenter
				opacity: (model.month === (DF.currentMonth()+monthOffset) && model.year ===(DF.currentYear()+yearOffset)) ? 1 : 0.1
				text: model.day
				font.pointSize: 14
				color:
					(model.day===Number(Qt.formatDate(due,"dd")) && model.month ===DF.currentMonth()) ?
						"red"
					  :(model.day===Number(Qt.formatDate(scheduled, "dd")) && model.month ===DF.currentMonth()?
							"cyan":
							Material.foreground)
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
