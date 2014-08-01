/*
 * Lets say you want to scrape video urls from youtube. This script waits for the video to start loading
 * and then exits printing the URL. Notice the timeout bellow, it this possible that this script never
 * exits - e.g. if you call it on page with no videos.
 *
 * webpageworker --timeout 15 --script ytvideo.js http://www.youtube.com/watch?v=0Ef9GudbxXY&html5=1
 */

HTMLVideoElement.prototype.load = function() {
	if (this.src) {
		WebPage.print(this.src);
		WebPage.done(0);
	}
}
