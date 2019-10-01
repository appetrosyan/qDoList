import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.0
import ac.uk.cam.ap886 1.0
import core 1.0

ListView {
	id: listView
	displayMarginBeginning: 60
	property var filterFunction: (a) => true
	property var filtered: false
	displayMarginEnd: 60
	add: slideIn
	remove: dropOut
	removeDisplaced: smoothShuffle
	moveDisplaced:smoothShuffle
	move: smoothShuffle
	Transition {
		id: slideIn
		NumberAnimation {
			easing.amplitude: 1.05
			properties: "x"
			from: 100
			duration: 400
			easing.type: Easing.OutExpo
		}
	}
	Transition {
		id: dropOut
		NumberAnimation {
			easing.amplitude: 1.05
			properties: "x"
			to: 10
			duration: 200
			easing.type: Easing.OutExpo
		}
	}

	Transition{
		id:smoothShuffle
		NumberAnimation{
			properties: "y"
			to: accordion.height
			easing.type: Easing.InOutCirc
		}
	}
	delegate: Component{
		id: accordion
		Rectangle{
			visible: filterFunction(modelData)
			height: visible?childrenRect.height:0
			Behavior on height{
				NumberAnimation{
					properties: "height"
					duration: 150
				}
			}
			width: parent.width
			border.width: 1
			border.color: Material.background
			color: Qt.darker(Material.background, 1+0.01*modelData.subtaskCount)
			radius: 5
			InfoRow{
				id:infoRow
				width: parent.width
			}
			ListView{
				x: 10
				visible: infoRow.expanded
				height: visible?childrenRect.height:0
				opacity: visible?1:0
				width: parent.width - x
				anchors.top: infoRow.bottom
				model: modelData.subModel
				delegate: accordion
				interactive: false
				Behavior on height {
					NumberAnimation {
						easing.type: Easing.OutQuad
						properties: "height"
						duration: 200

					}
				}
				Behavior on opacity {
					NumberAnimation {
						properties: "opacity"
						duration: 200
					}
				}
			}
		}
	}
	section.property: "modelData.prettyDueDate"
	section.criteria: ViewSection.FullString
	section.delegate: sectionHeading
	Component {
		id: sectionHeading
		Rectangle {
			width: accordion.width
			height: childrenRect.height
			color: Material.accent
			Text {
				text: section
				font.bold: true
				color: Material.foreground
				font.capitalization:Font.Capitalize
			}

		}
	}

}

