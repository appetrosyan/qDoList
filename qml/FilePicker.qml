import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

Item {
	Tumbler {
		z: 1
		height: 100
		wheelEnabled: true
		anchors.fill: parent
		model: myFileList
		wrap: true
		smooth: true
		delegate: Component {
			Label {
				id: fileFromTumbler
				text: model.modelData.fileName + (model.modelData.isModified?"*":"")
				color: model.modelData.isBinary?"cyan":sysPallete.text
				opacity: 1.0 - Math.abs(Tumbler.displacement) / (Tumbler.tumbler.visibleItemCount / 2)
				horizontalAlignment: Text.AlignHCenter
				verticalAlignment: Text.AlignVCenter
			}
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
