import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Label {
	id: picker
	property int currentYear: 2019
	property var date: Date.now()
	property var editable: true
	signal newDate(string msg)
	text: date.toLocaleDateString(locale, Locale.ShortFormat)
	MouseArea {
		onClicked: datePickerDialog.visible = true
		anchors.fill: parent
		visible: editable
	}

	Drawer {
		id: datePickerDialog
		modal: true
		height: rootWindow.height

		ColumnLayout {
			RowLayout{
				Layout.leftMargin: 7
				Layout.rightMargin: 7
				Button{
					text: qsTr("Today")
					onClicked: {
						picker.newDate(Qt.formatDateTime(new Date(), "yyyy-MM-dd hh:mm"))
					}
				}
				Button{
					text: qsTr("Tomorrow")
					onClicked: {
						var dateTime = new Date()
						dateTime.setDate(Qt.formatDateTime (dateTime,"dd") -(-1) );
						// This, kids, is why implicit conversions are a bad idea.
						// Sure it might be hard to write toInt(), but not having +1
						// do what you expect it to do is worse.
						// #FuckJavaScript
						picker.newDate(Qt.formatDateTime(dateTime, "yyyy-MM-dd hh:mm"))
						// On that note, why not have quick conversions like (int)
						// Like we did in C? Better yet, why can't reinterpret cast
						// be like () and static_cast have a <<Type>> syntax?
					}
				}
				Button{
					text: qsTr("Next Week")
					onClicked: {
						var dateTime = new Date()
						dateTime.setDate(Qt.formatDateTime (dateTime,"dd") -(-7) );
						// This kids is why implicit conversions are a bad idea.
						// Sure it might be hard to write toInt(), but not having +7
						// do what you expect it to do is worse.
						// #FuckJavaScript
						picker.newDate(Qt.formatDateTime(dateTime, "yyyy-MM-dd hh:mm"))
					}
				}
			}

			RowLayout {
				id: customDate
				Tumbler {
					id: day
					model: daysInMonth(month.currentIndex+1, leapyear(year.currentIndex+ currentYear))
					Layout.preferredHeight: 100
					Layout.preferredWidth: 30
				}
				Tumbler {
					id: month
					model: localisedMonths()
					Layout.preferredHeight: 100
					Layout.preferredWidth: 40
				}
				Tumbler {
					id: year
					model: range(20, currentYear)
					Layout.preferredHeight: 100
					Layout.preferredWidth: 50
				}
				ToolSeparator {}
				Tumbler{
					id: hour
					model: 24
					Layout.preferredHeight: 80
					Layout.preferredWidth: 18
				}
				Label{
					text: " : "
				}

				Tumbler{
					id: minute
					model: 60
					Layout.preferredHeight: 80
					Layout.preferredWidth: 18

				}
			}


			RowLayout {
				Button {
					text: qsTr("Set Date and Time")
					// CrapScript at its best;) To anyone that thinks that Javascript is the first language to learn, think again.
					onClicked: {
						var dateTime = new Date((year.currentIndex+currentYear), month.currentIndex, day.currentIndex+1, hour.currentIndex, minute.currentIndex)
						picker.newDate( Qt.formatDateTime(dateTime, "yyyy-MM-dd hh:mm"))
					}
					Layout.fillWidth: true
					Layout.leftMargin: 7
					Layout.rightMargin: 7
				}
			}

		}
	}
	// TODO reimplement in C++
	function range (n, m=1) {
		return Array.from({length: n},
						  (value, key) => key+m)
	}
	function daysInMonth( x , leap=false) {

		if(x===2){
			return leap? range(29): range(28)
		}
		else if(x===1 || x===12 || x===7 || x===5 || x===3 || x===10){
			return range(31)
		}
		else
			return range(30)

	}
	function leapyear(year) {
		return (year % 100 === 0) ? (year % 400 === 0) : (year % 4 === 0);
	}
	function localisedMonths(){
		return Array.apply(0, new Array(12)).map(function(_,i){return i+1})
	}
}
