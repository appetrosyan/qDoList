import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.3
import QtQuick.Controls 2.12

Item {
	ListView{
		id: fileList
		anchors.fill: parent
		model: myFileList
		delegate : Component{
				RowLayout{
					width: fileList.width
					ToolButton {
						Layout.fillWidth: true
						text: modelData.fileName
						height: 32
						font.capitalization:  Font.Normal
						onClicked: {
							modelData.requestAttention()
						}
					}
					ToolSeparator{}
					ToolButton{
						text: modelData.isBinary? "101":"txt"
						font.pixelSize: 8
						onClicked:{
							modelData.isBinary = !modelData.isBinary
						}
					}
					ToolButton{
						visible: modelData.isModified
						text: "save"
						font.pixelSize: 10
						onClicked: {
							modelData.saveToFile()
						}
					}
				}
		}
	}

//	Tumbler {
//		property int wheelTicks: 0
//		z: 1
//		height: 100
//		wheelEnabled: true
//		anchors.fill: parent
//		model: myFileList
//		wrap: true
//		smooth: true
//		delegate: Component {
//			Label {
//				id: fileFromTumbler
//				text: model.modelData.fileName + (model.modelData.isModified?"*":"")
//				opacity: 1.0 - Math.abs(Tumbler.displacement) / (Tumbler.tumbler.visibleItemCount / 2)
//				horizontalAlignment: Text.AlignHCenter
//				verticalAlignment: Text.AlignVCenter
//			}
//		}

//		onCurrentIndexChanged: {
////			myFileList.get(currentIndex).requestAttention()
//			// This is semantically correct, but horrendously inefficient.
//			// Sure this only changes the model once per each change, however,
//			// The change itself is ... it occurs far too often and makes the
//			// experience janky.
//		}
//		onMovingChanged: {
//			myFileList.get(currentIndex).requestAttention()
//		}

//		onCountChanged: {
//			currentIndex = myFileList.activeTrackedFileIndex()
//		}
//		MouseArea{
//			anchors.fill: parent
//			onWheel:{
//				if(wheel.angleDelta.y <0){
//					parent.wheelTicks++
//					if(parent.wheelTicks>=25 && !parent.moving){
//						parent.currentIndex = (parent.currentIndex+1)%parent.count
//						parent.wheelTicks =0
//					}
//				}else{
//					parent.wheelTicks--
//					if(parent.wheelTicks <=-25 && !parent.moving){
//						parent.currentIndex = (parent.currentIndex - 1)>=0?parent.currentIndex-1:parent.count + (parent.currentIndex-1)
//						parent.wheelTicks=0
//					}
//				}
//			}
//		}
//	}
}
