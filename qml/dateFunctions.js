function currentYear() {
    var date = new Date()
    return Number(Qt.formatDate(date, "yyyy"))
}
function currentMonth() {
    var date = new Date()
    return Number(Qt.formatDate(date, "MM"))
}
function currentDay() {
    var date = new Date()
    return Number(Qt.formatDate(date, "dd"))
}
