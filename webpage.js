"strict";
/**
 * Javascript wrappings around Webpage object
 *
 *
 */

(function() {
	function _addResult(name, arg) {
		var val = arg;
		if (!(arg instanceof Array)) {
			val = [arg];
		}
		return WebPage.addResult(name, JSON.stringify(val));
	}

	WebPage.result = _addResult;

	WebPage.print = function(arg) {
		return _addResult('print', arg);
	}

	WebPage.error = function(arg) {
		return _addResult('error', arg);
	}
}());


