import QtQuick 2.11
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12

RowLayout{
	CheckBox{
		checked: modelData.done
		onClicked:{
			modelData.toggle()
			checked = modelData.done
		}
	}
	Text{
		text: modelData.name
	}
}
