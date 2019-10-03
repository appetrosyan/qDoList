import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.3
import QtQuick.Controls 2.12

Item {
	ListView{
		id: fileList
		anchors.fill: parent
		model: myFileList
		clip: true
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
						text: modelData.isBinary? "101":"json"
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
}
