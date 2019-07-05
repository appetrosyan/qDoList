import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

Frame {
	wheelEnabled: true
	hoverEnabled: true
	focusPolicy: Qt.ClickFocus
	height:150
	Tumbler {
		z: 1
		height: 100
		anchors.fill: parent
		model: myFileList
		wrap: true
		delegate: Label {
			id: fileFromTumbler
			text: model.modelData.fileName + (model.modelData.isModified?"*":"")
			opacity: 1.0 - Math.abs(Tumbler.displacement) / (Tumbler.tumbler.visibleItemCount / 4)
			horizontalAlignment: Text.AlignHCenter
			verticalAlignment: Text.AlignVCenter
		}
		onCurrentIndexChanged: {
//			myFileList.get(currentIndex).requestAttention()
			// This is semantically correct, but horrendously inefficient.
			// Sure this only changes the model once per each change, however,
			// The change itself is ... it occurs far too often and makes the
			// experience janky.
		}
		onMovingChanged: {
			myFileList.get(currentIndex).requestAttention()
		}

		onCountChanged: {
			currentIndex = myFileList.activeTrackedFileIndex()
		}
	}
}
