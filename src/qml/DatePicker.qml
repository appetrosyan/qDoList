import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Button {
	id: picker
	property int currentYear: 2019
	property var date: Date.now()
	property var editable: true
	signal newDate(string msg)
	property var color: "cyan"
	text: date.toLocaleDateString(locale, Locale.ShortFormat)
	checkable: true
	enabled: editable
	Dialog{
		x: -30
		id: datePickerDialog
		modal: true
		z: 50
		Label{
			text: date.toLocaleDateString(locale, Locale.ShortFormat)
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
