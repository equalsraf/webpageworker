"strict";
/**
 * Prevent video/audio from playing
 *
 */

(function() {
	console.log("loading video.js in ", document.location.href.toString());
	HTMLVideoElement.prototype.play = function() { console.log('Attempting to play video ... redirecting.', this.src )}
	HTMLVideoElement.prototype.load = function() { console.log('Attempting to load video ... redirecting.', this.src )}
	HTMLAudioElement.prototype.play = function() { console.log('Attempting to play audio ... redirecting.', this.src )}
	HTMLAudioElement.prototype.load = function() { console.log('Attempting to load audio ... redirecting.', this.src )}
}());
