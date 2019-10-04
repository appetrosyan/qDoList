function listModelSort(listModel, compareFunction) {
	let indexes = [ ...Array(listModel.count).keys() ]
	indexes.sort( (a, b) => compareFunction( listModel.get(a), listModel.get(b) ) )
	let sorted = 0
	while ( sorted < indexes.length && sorted === indexes[sorted] ) sorted++
	if ( sorted === indexes.length ) return
	for ( let i = sorted; i < indexes.length; i++ ) {
		listModel.move( indexes[i], listModel.count - 1, 1 )
		listModel.insert( indexes[i], { } )
	}
	listModel.remove( sorted, indexes.length - sorted )
}

function similarity(s1, s2) {
	var longer = s1;
	var shorter = s2;
	if (s1.length < s2.length) {
		longer = s2;
		shorter = s1;
	}
	var longerLength = longer.length;
	if (longerLength === 0) {
		return 1.0;
	}
	return (longerLength - editDistance(longer, shorter)) / parseFloat(longerLength);
}

function editDistance(s1, s2) {
	s1 = s1.toLowerCase();
	s2 = s2.toLowerCase();

	var costs = new Array();
	for (var i = 0; i <= s1.length; i++) {
		var lastValue = i;
		for (var j = 0; j <= s2.length; j++) {
			if (i === 0)
				costs[j] = j;
			else {
				if (j > 0) {
					var newValue = costs[j - 1];
					if (s1.charAt(i - 1) !== s2.charAt(j - 1))
						newValue = Math.min(Math.min(newValue, lastValue),
											costs[j]) + 1;
					costs[j - 1] = lastValue;
					lastValue = newValue;
				}
			}
		}
		if (i > 0)
			costs[s2.length] = lastValue;
	}
	return costs[s2.length];
}
