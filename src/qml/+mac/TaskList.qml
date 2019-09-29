import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.0
import ac.uk.cam.ap886 1.0
import core 1.0

ListView {
	displayMarginBeginning: 60
	displayMarginEnd: 60
	delegate: Component{
		id: accordion
		Rectangle{
			height: childrenRect.height
			width: parent.width
			InfoRow{
				id:infoRow
			}

			ListView{
				x: 10
				height: childrenRect.height
				width: parent.width - x
				anchors.top: infoRow.bottom
				model: modelData.subModel
				delegate: accordion
				interactive: false
			}
		}
	}
}

